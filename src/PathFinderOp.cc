//
//  PathFinderOp.m
//  Mandoline
//
//  Created by GM on 11/24/10.
//  Copyright 2010 Belfry DevWorks. All rights reserved.
//

#include "PathFinderOp.h"
#include "BGL/BGL.h"
#include "SlicingContext.h"
#include "CarvedSlice.h"



PathFinderOp::~PathFinderOp()
{
}



void PathFinderOp::main()
{
    if ( isCancelled ) return;
    if ( NULL == context ) return;

    // TODO: join paths and optimize them

    if ( isCancelled ) return;
}



