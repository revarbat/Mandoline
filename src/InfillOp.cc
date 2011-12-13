//
//  InfillOp.cc
//  Mandoline
//
//  Created by GM on 11/24/10.
//  Copyright 2010 Belfry DevWorks. All rights reserved.
//

#include <BGL.hh>
#include "InfillOp.hh"
#include "SlicingContext.hh"
#include "CarvedSlice.hh"



InfillOp::~InfillOp()
{
}



void InfillOp::main()
{
    if ( isCancelled ) return;
    if ( NULL == context ) return;
    if ( NULL == slice ) return;

    double extrusionWidth = context->standardExtrusionWidth();

    slice->infillMask.infillPathsForRegionWithDensity(context->infillDensity, extrusionWidth, slice->infill);
    slice->state = INFILLED;

    if ( isCancelled ) return;
}


