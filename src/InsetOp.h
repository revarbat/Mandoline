//
//  InsetOper.h
//  Mandoline
//
//  Created by GM on 11/24/10.
//  Copyright 2010 Belfry DevWorks. All rights reserved.
//

#ifndef INSETOP_H
#define INSETOP_H

#include "Operation.h"
#include "SlicingContext.h"
#include "CarvedSlice.h"

class InsetOp : public Operation {
public:
    float zLayer;
    SlicingContext* context;
    CarvedSlice* slice;

    InsetOp(SlicingContext* ctx, CarvedSlice* slc, float Z)
        : Operation(), zLayer(Z), context(ctx), slice(slc)
    {
    }
    virtual ~InsetOp();
    virtual void main();
};

#endif

