//
//  PathFinderOp.h
//  Mandoline
//
//  Created by GM on 11/24/10.
//  Copyright 2010 Belfry DevWorks. All rights reserved.
//

#ifndef PATHFINDEROP_H
#define PATHFINDEROP_H

#include "CarvedSlice.h"
#include "SlicingContext.h"
#include "Operation.h"

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

