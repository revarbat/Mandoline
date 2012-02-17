//
//  RaftOp.hh
//  Mandoline
//
//  Created by GM on 2011-12-30
//  Copyright 2011 Belfry DevWorks. All rights reserved.
//

#ifndef RAFTOP_H
#define RAFTOP_H

#include "Operation.hh"
#include "SlicingContext.hh"
#include "CarvedSlice.hh"

class RaftOp : public Operation {
public:
    SlicingContext* context;

    RaftOp(SlicingContext* ctx)
        : Operation(), context(ctx)
    {
    }
    virtual ~RaftOp();
    virtual void main();
};

#endif
// vim: set ts=4 sw=4 nowrap expandtab: settings

