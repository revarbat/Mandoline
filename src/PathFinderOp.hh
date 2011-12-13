//
//  PathFinderOp.hh
//  Mandoline
//
//  Created by GM on 11/24/10.
//  Copyright 2010 Belfry DevWorks. All rights reserved.
//

#ifndef PATHFINDEROP_H
#define PATHFINDEROP_H

#include "CarvedSlice.hh"
#include "SlicingContext.hh"
#include "Operation.hh"

class PathFinderOp : public Operation {
public:
    SlicingContext* context;

    PathFinderOp(SlicingContext* ctx)
        : Operation(), context(ctx)
    {
    }
    virtual ~PathFinderOp();
    virtual void main();
};

#endif

