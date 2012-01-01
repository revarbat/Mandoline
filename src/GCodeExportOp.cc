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



static double prevFilamentRate = -1;

ostream &pathToGcode(Path& path, double zHeight, SlicingContext &ctx, ostream& out)
{
    Lines::iterator line = path.segments.begin();
    double feed = ctx.feedRateForWidth(line->extrusionWidth);

    // Move to the starting position...
    out << "G1 X" << line->startPt.x
        << " Y" << line->startPt.y
        << " Z" << zHeight
	<< " F" << feed
        << endl;

    if (ctx.filamentFeedRate != prevFilamentRate) {
	// set extrusion speed
	out << "M108 R" << ctx.filamentFeedRate << endl;
	prevFilamentRate = ctx.filamentFeedRate;
    }

    // start extruding
    out << "M101" << endl;

    for ( ; line != path.segments.end(); line++) {
	feed = ctx.feedRateForWidth(line->extrusionWidth);

	// Move to next point in polyline
        out << "G1 X" << line->endPt.x
            << " Y" << line->endPt.y
            << " Z" << zHeight
            << " F" << feed
            << endl;
    }

    // stop extruding
    out << "M103" << endl;

    return out;
}



ostream &pathsToGcode(Paths& paths, double zHeight, SlicingContext &ctx, ostream& out) {
    // TODO: inner paths?
    Paths::iterator pit;
    for ( pit = paths.begin(); pit != paths.end(); pit++) {
        pathToGcode((*pit), zHeight, ctx, out);
    }

    return out;
}



ostream &simpleRegionsToGcode(SimpleRegions& regions, double zHeight, SlicingContext ctx, ostream& out) {
    SimpleRegions::iterator perimeter;
    for (
        perimeter = regions.begin();
        perimeter != regions.end();
        perimeter++
    ) {
        pathToGcode(perimeter->outerPath, zHeight, ctx, out);

        // TODO: inner paths?
        pathsToGcode(perimeter->subpaths, zHeight, ctx, out);
    }

    return out;
}



void GCodeExportOp::main()
{
    if ( isCancelled ) return;
    if ( NULL == context ) return;

    // TODO: join paths and optimize them
    cout << "Gcode pather starting" << endl;
    // For each slice, write out code.
    cout << "Layer count: " << context->slices.size() << endl;

    list<CarvedSlice>::iterator it;

    fstream fout;
    fout.open("out.gcode", fstream::out | fstream::trunc);
    if (!fout.good()) {
        return;
    }

    // For each layer....
    for (it = context->slices.begin(); it != context->slices.end(); it++) {
        fout << "(new layer)" << endl;

        CarvedSlice* slice = &(*it);

        fout << "(perimeter)" << endl;

        //simpleRegionsToGcode(slice->perimeter.subregions, slice->zLayer, *context, fout);
	for (
	    CompoundRegions::reverse_iterator cit = slice->shells.rbegin();
	    cit != slice->shells.rend();
	    cit--
	) {
	    simpleRegionsToGcode(cit->subregions, slice->zLayer, *context, fout);
	}

        fout << "(infill)" << endl;

        pathsToGcode(slice->infill, slice->zLayer, *context, fout);
    }

    fout.close();

    if ( isCancelled ) return;
}



