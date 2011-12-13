//
//  PathFinderOp.cc
//  Mandoline
//
//  Created by GM on 11/24/10.
//  Copyright 2010 Belfry DevWorks. All rights reserved.
//

#include <BGL.hh>
#include "PathFinderOp.hh"
#include "SlicingContext.hh"
#include "CarvedSlice.hh"



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



