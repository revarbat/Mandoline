//
//  InsetOper.hh
//  Mandoline
//
//  Created by GM on 11/24/10.
//  Copyright 2010 Belfry DevWorks. All rights reserved.
//

#ifndef INSETOP_H
#define INSETOP_H

#include "Operation.hh"
#include "SlicingContext.hh"
#include "CarvedSlice.hh"

class InsetOp : public Operation {
public:
    double zLayer;
    SlicingContext* context;
    CarvedSlice* slice;

    InsetOp(SlicingContext* ctx, CarvedSlice* slc, double Z)
        : Operation(), zLayer(Z), context(ctx), slice(slc)
    {
    }
    virtual ~InsetOp();
    virtual void main();
};

#endif

