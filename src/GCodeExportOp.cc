//
//  GCodeExportOp.cc
//  Mandoline
//
//  Created by GM on 11/24/10.
//  Copyright 2010 Belfry DevWorks. All rights reserved.
//

#include <BGL.hh>
#include "GCodeExportOp.hh"

#include <iostream>
#include <fstream>



GCodeExportOp::GCodeExportOp(SlicingContext* ctx)
    : Operation(), context(ctx)
{
}



GCodeExportOp::~GCodeExportOp()
{
}



ostream &pathToGcode(Path& path, double zHeight, double speedMult, int tool, SlicingContext &ctx, ostream& out)
{
    if (path.size() == 0) {
        return out;
    }

    Lines::iterator line = path.segments.begin();
    double feed = ctx.feedRateForWidth(tool, line->extrusionWidth) * speedMult;

    out.setf(ios::fixed);
    out.precision(2);

    // Move to the starting position...
    out << "G1 X" << line->startPt.x - ctx.xAxisOffset[tool]
        << " Y" << line->startPt.y - ctx.yAxisOffset[tool]
        << " Z" << zHeight
	<< " F" << feed
        << endl;

    double filamentFeed = ctx.filamentFeedRate[tool] * speedMult;
    double retractionRate = ctx.retractionRate[tool];

    // undo reversal
    out << "M108 R" << retractionRate << " T" << tool << " (extrusion speed)" << endl;
    out << "M101 T" << tool << " (extrusion forward)" << endl;
    out.precision(0);
    out << "G4 P" << ctx.pushBackTime[tool] << " (Pause)" << endl;
    out.precision(2);

    // set filament speed
    out << "M108 R" << filamentFeed << " T" << tool << " (extrusion speed)" << endl;

    // start extruding
    out << "M101 T" << tool << " (extrusion forward)" << endl;

    for ( ; line != path.segments.end(); line++) {
	feed = ctx.feedRateForWidth(tool, line->extrusionWidth) * speedMult;

	// Move to next point in polyline
        out << "G1 X" << line->endPt.x - ctx.xAxisOffset[tool]
            << " Y" << line->endPt.y - ctx.yAxisOffset[tool]
            << " Z" << zHeight
            << " F" << feed
            << endl;
    }

    // reversal
    out << "M102 T" << tool << " (extrusion backward)" << endl;
    out << "M108 R" << retractionRate << " T" << tool << " (extrusion speed)" << endl;
    out.precision(0);
    out << "G4 P" << ctx.retractionTime[tool] << " (Pause)" << endl;
    out.precision(2);

    // stop extruding
    out << "M103 T" << tool << " (extrusion stop)" << endl;

    return out;
}



ostream &pathsToGcode(Paths& paths, double zHeight, double speedMult, int tool, SlicingContext &ctx, ostream& out) {
    // TODO: inner paths?
    Paths::iterator pit;
    for ( pit = paths.begin(); pit != paths.end(); pit++) {
        pathToGcode((*pit), zHeight, speedMult, tool, ctx, out);
    }

    return out;
}



ostream &simpleRegionsToGcode(SimpleRegions& regions, double zHeight, double speedMult, int tool, SlicingContext ctx, ostream& out) {
    SimpleRegions::iterator perimeter;
    for (
        perimeter = regions.begin();
        perimeter != regions.end();
        perimeter++
    ) {
        pathToGcode(perimeter->outerPath, zHeight, speedMult, tool, ctx, out);

        // TODO: inner paths?
        pathsToGcode(perimeter->subpaths, zHeight, speedMult, tool, ctx, out);
    }

    return out;
}



void GCodeExportOp::main()
{
    if ( isCancelled ) return;
    if ( NULL == context ) return;

    int mainTool = 0;
    int supportTool = context->supportTool;

    // For each slice, write out code.
    cout << "Layer count: " << context->slices.size() << endl;

    list<CarvedSlice>::iterator it;

    fstream fout;
    fout.open("out.gcode", fstream::out | fstream::trunc);
    if (!fout.good()) {
        return;
    }

    fout.setf(ios::fixed);
    fout.precision(0);
    fout << "G21 (Metric)" << endl;
    fout << "G90 (Absolute positioning)" << endl;
    fout << "M107 T0 (Fan off)" << endl;
    fout << "M103 T0 (Extruder motor off)" << endl;
    fout << "M104 T0 S" << context->extruderTemp[mainTool]
         << " (Set extruder temp)" << endl;
    if (supportTool != mainTool) {
	fout << "M103 T" << supportTool << " (Extruder motor off)" << endl;
	fout << "M104 T" << supportTool
	     << " S" << context->extruderTemp[supportTool]
	     << " (Set extruder temp)" << endl;
    }
    fout << "M109 T0 S" << context->platformTemp
         << " (Set platform temp)" << endl;

    fout << "G92 X0 Y0 Z0  (Zero our position out.)" << endl;
    fout << "G0 X-10 Y-10 Z5 (move back to make sure homing doesnt fail)" << endl;

    // TODO: Remove this.  It's specific to the belfry fabber machine.
    fout << "G161 Z (Home Z axis to bottom.)" << endl;
    fout << "G92 X0 Y0 Z0  (Zero our position out.)" << endl;
    fout << "G0 Z5 (Lift Z axis 5 mm)" << endl;
    fout << "G162 X Y (Home X and Y axes to XY+ max)" << endl;
    fout << "G92 X85 Y95 Z5  (Recalibrate positions.)" << endl;
    fout << "G0 X0 Y0 (Move to center.)" << endl;
    fout << "G0 Z0.5" << endl;
    fout << "M6 T0 (Wait for tool to heat up)" << endl;

    // For each layer....
    for (it = context->slices.begin(); it != context->slices.end(); it++) {
        fout << "(new layer)" << endl;

        CarvedSlice* slice = &(*it);

        fout << "(perimeter)" << endl;

	int layerTool = mainTool;
	if (slice->zLayer <= 0.005 + context->raftLayers * context->layerThickness) {
	    layerTool = supportTool;
	}

	CompoundRegions::reverse_iterator cit;
	for (cit = slice->shells.rbegin(); cit != slice->shells.rend(); cit++) {
	    simpleRegionsToGcode(cit->subregions, slice->zLayer, slice->speedMult, layerTool, *context, fout);
	}

        fout << "(infill)" << endl;

        pathsToGcode(slice->infill, slice->zLayer, slice->speedMult, layerTool, *context, fout);
        pathsToGcode(slice->supportPaths, slice->zLayer, slice->speedMult, supportTool, *context, fout);
    }

    fout << "M109 T0 S0 (Platform heater off)" << endl;
    fout << "M104 T0 S0 (Extruder heater off)" << endl;
    fout << "M103 T0 (Extruder motor off)" << endl;
    if (supportTool != mainTool) {
	fout << "M104 S0 T" << supportTool << " (Extruder heater off)" << endl;
	fout << "M103 T" << supportTool << " (Extruder motor off)" << endl;
    }
    fout << "G91 (incremental mode)" << endl;
    fout << "G0 Z1.0 (raise head slightly)" << endl;
    fout << "G90 (absolute mode)" << endl;
    fout << "G0 X0.0 Y50.0 (Move platform forward)" << endl;
    fout << "M18 (turn off steppers)" << endl;

    fout.close();

    if ( isCancelled ) return;
}



