//
//  InfillOp.m
//  Mandoline
//
//  Created by GM on 11/24/10.
//  Copyright 2010 Belfry DevWorks. All rights reserved.
//

#include "InfillOp.h"
#include "BGL/BGL.h"
#include "SlicingContext.h"
#include "CarvedSlice.h"



InfillOp::~InfillOp()
{
}



void InfillOp::main()
{
    if ( isCancelled ) return;
    if ( NULL == context ) return;
    if ( NULL == slice ) return;

    float extrusionWidth = context->standardExtrusionWidth();

    slice->infillMask.infillPathsForRegionWithDensity(context->infillDensity, extrusionWidth, slice->infill);
    slice->state = INFILLED;

    if ( isCancelled ) return;
}


