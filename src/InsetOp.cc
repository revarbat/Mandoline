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

    double extWidth = context->standardExtrusionWidth();
    int shells = context->perimeterShells;
    if (shells < 1) {
        shells = 1;
    }

    int i;
    for (i = 0; i < shells; i++) {
        BGL::CompoundRegion compReg;
        slice->perimeter.inset((i+0.5)*extWidth, compReg);
	compReg.setWidth(context->standardExtrusionWidth());
        slice->shells.push_back(compReg);
    }
    double infillPerimeterOverlap = 0.15;
    slice->perimeter.inset((i+0.5-infillPerimeterOverlap)*extWidth, slice->infillMask);
    slice->state = INSET;

    if ( isCancelled ) return;
}


