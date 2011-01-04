//
//  InfillOp.h
//  Mandoline
//
//  Created by GM on 11/24/10.
//  Copyright 2010 Belfry DevWorks. All rights reserved.
//

#ifndef INFILLOP_H
#define INFILLOP_H

#include "CarvedSlice.h"
#include "SlicingContext.h"
#include "Operation.h"

class InfillOp : public Operation {
public:
    float zLayer;
    SlicingContext* context;
    CarvedSlice* slice;

    InfillOp(SlicingContext* ctx, CarvedSlice* slc, float Z)
        : Operation(), zLayer(Z), context(ctx), slice(slc)
    {
    }
    virtual ~InfillOp();
    virtual void main();
};

#endif

