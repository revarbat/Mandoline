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
    SlicingContext* context;
    CarvedSlice* slice;

    InfillOp(SlicingContext* ctx, CarvedSlice* slc)
        : Operation(), context(ctx), slice(slc)
    {
    }
    virtual ~InfillOp();
    virtual void main();
};

#endif
// vim: set ts=4 sw=4 nowrap expandtab: settings

