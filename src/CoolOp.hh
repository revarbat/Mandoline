//
//  CoolOp.hh
//  Mandoline
//
//  Created by GM on 2011-01-05.
//  Copyright 2011 Belfry DevWorks. All rights reserved.
//

#ifndef COOLOP_H
#define COOLOP_H

#include "CarvedSlice.hh"
#include "SlicingContext.hh"
#include "Operation.hh"

class CoolOp : public Operation {
public:
    SlicingContext* context;
    CarvedSlice* slice;

    CoolOp(SlicingContext* ctx, CarvedSlice* slc)
        : Operation(), context(ctx), slice(slc)
    {
    }
    virtual ~CoolOp();
    virtual void main();
};

#endif
// vim: set ts=4 sw=4 nowrap expandtab: settings

