//
//  CarveOper.h
//  Mandoline
//
//  Created by GM on 11/24/10.
//  Copyright 2010 Belfry DevWorks. All rights reserved.
//

#ifndef CARVEOP_H
#define CARVEOP_H

#include "Operation.h"
#include "SlicingContext.h"
#include "CarvedSlice.h"

class CarveOp : public Operation {
public:
    float zLayer;
    SlicingContext* context;
    CarvedSlice* slice;

    CarveOp(SlicingContext* ctx, CarvedSlice* slc, float Z)
        : Operation(), zLayer(Z), context(ctx), slice(slc)
    {
    }
    virtual ~CarveOp();
    virtual void main();
};

#endif

