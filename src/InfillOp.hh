//
//  InfillOp.hh
//  Mandoline
//
//  Created by GM on 11/24/10.
//  Copyright 2010 Belfry DevWorks. All rights reserved.
//

#ifndef INFILLOP_H
#define INFILLOP_H

#include "CarvedSlice.hh"
#include "SlicingContext.hh"
#include "Operation.hh"

class InfillOp : public Operation {
public:
    double zLayer;
    SlicingContext* context;
    CarvedSlice* slice;

    InfillOp(SlicingContext* ctx, CarvedSlice* slc, double Z)
        : Operation(), zLayer(Z), context(ctx), slice(slc)
    {
    }
    virtual ~InfillOp();
    virtual void main();
};

#endif

