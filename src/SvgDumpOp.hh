//
//  SVGDumpOp.hh
//  Mandoline
//
//  Created by GM on 11/24/10.
//  Copyright 2010 Belfry DevWorks. All rights reserved.
//

#ifndef SVGDUMPOP_H
#define SVGDUMPOP_H

#include "CarvedSlice.hh"
#include "SlicingContext.hh"
#include "Operation.hh"

class SvgDumpOp : public Operation {
public:
    SlicingContext* context;
    CarvedSlice* slice;

    SvgDumpOp(SlicingContext* ctx, CarvedSlice* slc)
        : Operation(), context(ctx), slice(slc)
    {
    }
    virtual ~SvgDumpOp();
    virtual void main();
};

#endif

