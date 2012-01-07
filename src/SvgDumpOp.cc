//
//  SVGDumpOperation.cc
//  Mandoline
//
//  Created by GM on 11/24/10.
//  Copyright 2010 Belfry DevWorks. All rights reserved.
//

#include <fstream>
#include "SvgDumpOp.hh"
#include <BGL.hh>
#include "SlicingContext.hh"
#include "CarvedSlice.hh"

using namespace BGL;


SvgDumpOp::~SvgDumpOp()
{
}



void SvgDumpOp::main()
{
    if ( isCancelled ) return;
    if ( NULL == context ) return;
    if ( NULL == slice ) return;

    // HACK: We should be doing stroke width per line segment, but we only really
    // vary extrusionWidth by layer at this point anyways, so this is good enough.
    double extrusionWidth = slice->infill.front().segments.front().extrusionWidth;

    char dumpFileName[512];
    snprintf(dumpFileName, sizeof(dumpFileName), "%.128s-%06.2f.svg", context->dumpPrefix.c_str(), slice->zLayer);

    fstream fout;
    fout.open(dumpFileName, fstream::out | fstream::trunc);
    if (!fout.good()) {
        return;
    }
    slice->svgPathWithSizeAndOffset(fout, context->svgWidth, context->svgHeight, context->svgXOff, context->svgYOff, extrusionWidth);
    fout.close();

    if ( isCancelled ) return;
}


