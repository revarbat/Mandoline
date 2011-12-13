//
//  CarveOper.hh
//  Mandoline
//
//  Created by GM on 11/24/10.
//  Copyright 2010 Belfry DevWorks. All rights reserved.
//

#ifndef CARVEOP_H
#define CARVEOP_H

#include "Operation.hh"
#include "SlicingContext.hh"
#include "CarvedSlice.hh"

class CarveOp : public Operation {
public:
    double zLayer;
    SlicingContext* context;
    CarvedSlice* slice;

    CarveOp(SlicingContext* ctx, CarvedSlice* slc, double Z)
        : Operation(), zLayer(Z), context(ctx), slice(slc)
    {
    }
    virtual ~CarveOp();
    virtual void main();
};

#endif

