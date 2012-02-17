//
//  InsetOp.cc
//  Mandoline
//
//  Created by GM on 11/24/10.
//  Copyright 2010 Belfry DevWorks. All rights reserved.
//

#include <BGL.hh>
#include "InsetOp.hh"
#include "SlicingContext.hh"
#include "CarvedSlice.hh"



InsetOp::~InsetOp()
{
}



void InsetOp::main()
{
    if ( isCancelled ) return;
    if ( NULL == context ) return;
    if ( NULL == slice ) return;

    double infillPerimeterOverlapRatio = 0.15;
    double extWidth = context->standardExtrusionWidth();
    int shells = context->perimeterShells;
    if (shells < 1) {
        shells = 1;
    }

    for (int shellnum = 0; shellnum < shells; shellnum++) {
        BGL::CompoundRegion compReg;
        BGL::CompoundRegion compReg2;
        slice->perimeter.inset((shellnum+1.0)*extWidth, compReg);
        compReg.inset(-0.5*extWidth, compReg2);
        compReg2.setWidth(context->standardExtrusionWidth());
        slice->shells.push_back(compReg2);
    }

    double infillInset = (shells+0.5-infillPerimeterOverlapRatio)*extWidth;
    slice->perimeter.inset(infillInset, slice->infillMask);
    slice->state = INSET;

    if ( isCancelled ) return;
}

// vim: set ts=4 sw=4 nowrap expandtab: settings

