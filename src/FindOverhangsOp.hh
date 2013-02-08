//
//  FindOverhangsOp.hh
//  Mandoline
//
//  Created by GM on 2012-03-02
//  Copyright 2012 Belfry DevWorks. All rights reserved.
//

#ifndef FINDOVERHANGSOP_H
#define FINDOVERHANGSOP_H

#include "CarvedSlice.hh"
#include "SlicingContext.hh"
#include "Operation.hh"

class FindOverhangsOp : public Operation {
public:
    SlicingContext* context;

    FindOverhangsOp(SlicingContext* ctx)
        : Operation(), context(ctx)
    {
    }
    virtual ~FindOverhangsOp();
    virtual void main();
};

#endif
// vim: set ts=4 sw=4 nowrap expandtab: settings

