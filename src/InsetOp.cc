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

    slice->perimeter.inset(shells*extWidth, slice->infillMask);
    for (int i = 0; i < shells; i++) {
        BGL::CompoundRegion compReg;
	slice->perimeter.inset((i+0.5)*extWidth, compReg);
	slice->shells.push_back(compReg);
    }
    slice->state = INSET;

    if ( isCancelled ) return;
}


