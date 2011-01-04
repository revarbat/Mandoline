//
//  CarveOp.cc
//  Mandoline
//
//  Created by GM on 11/24/10.
//  Copyright 2010 Belfry DevWorks. All rights reserved.
//

#include "CarveOp.h"
#include "BGL/BGL.h"
#include "SlicingContext.h"
#include "CarvedSlice.h"



CarveOp::~CarveOp()
{
}



void CarveOp::main()
{
    if ( isCancelled ) return;
    if ( NULL == context ) return;
    if ( NULL == slice ) return;

    context->mesh.regionForSliceAtZ(zLayer, slice->perimeter);
    slice->state = CARVED;

    if ( isCancelled ) return;
}


