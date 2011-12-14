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


ostream &pathToGcode(Path& path, double zHeight, ostream& out) {
    Lines::iterator line = path.segments.begin();

    out << "M103" << endl;

    // Move to the starting position...
    out << "G1 X" << line->startPt.x
        << " Y" << line->startPt.y
        << " Z" << zHeight
        << std::endl;
    out << "M101" << endl;

    for (;
         line != path.segments.end();
         line++) {


        out << "G1 X" << line->endPt.x
            << " Y" << line->endPt.y
            << " Z" << zHeight
            << std::endl;
    }

    return out;
}

ostream &pathsToGcode(Paths& paths, double zHeight, ostream& out) {
    // TODO: inner paths?
    Paths::iterator pit;
    for (pit = paths.begin();
         pit != paths.end();
         pit++) {
        pathToGcode((*pit), zHeight, out);
    }

    return out;
}


ostream &simpleRegionsToGcode(SimpleRegions& regions, double zHeight, ostream& out) {
    SimpleRegions::iterator perimeter;
    for (perimeter = regions.begin();
         perimeter != regions.end();
         perimeter++) {

        pathToGcode(perimeter->outerPath, zHeight, out);

        // TODO: inner paths?
        pathsToGcode(perimeter->subpaths, zHeight, out);
    }

    return out;
}

void GCodeExportOp::main()
{
    if ( isCancelled ) return;
    if ( NULL == context ) return;

    // TODO: join paths and optimize them
    std::cout << "Gcode pather starting" << std::endl;
    // For each slice, write out code.
    std::cout << "Layer count: " << context->slices.size() << std::endl;

    map<double,CarvedSlice>::iterator it;

    fstream fout;
    fout.open("out.gcode", fstream::out | fstream::trunc);
    if (!fout.good()) {
        return;
    }

    fout << "M108 R35" << std::endl;
    fout << "M108 R2" << std::endl;

    // For each layer....
    for (it = context->slices.begin(); it != context->slices.end(); it++) {
        fout << "(new layer)" << std::endl;

        CarvedSlice* slice = &(*it).second;

        fout << "(perimeter)" << std::endl;

        simpleRegionsToGcode(slice->perimeter.subregions, (*it).first, fout);

        fout << "(infill)" << std::endl;

        pathsToGcode(slice->infill, (*it).first, fout);
    }

    fout.close();

    if ( isCancelled ) return;
}



