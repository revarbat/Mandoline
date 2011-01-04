//
//  InsetOp.cc
//  Mandoline
//
//  Created by GM on 11/24/10.
//  Copyright 2010 Belfry DevWorks. All rights reserved.
//

#include "InsetOp.h"
#include "BGL/BGL.h"
#include "SlicingContext.h"
#include "CarvedSlice.h"



InsetOp::~InsetOp()
{
}



void InsetOp::main()
{
    if ( isCancelled ) return;
    if ( NULL == context ) return;
    if ( NULL == slice ) return;

    // TODO: perform actual insets to generate perimeter shells and the infill mask
    slice->infillMask = slice->perimeter;
    slice->shells.push_back(slice->perimeter);
    slice->state = INSET;

    if ( isCancelled ) return;
}


