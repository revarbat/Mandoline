//
//  SVGDumpOp.h
//  Mandoline
//
//  Created by GM on 11/24/10.
//  Copyright 2010 Belfry DevWorks. All rights reserved.
//

#ifndef SVGDUMPOP_H
#define SVGDUMPOP_H

#include "CarvedSlice.h"
#include "SlicingContext.h"
#include "Operation.h"

class SvgDumpOp : public Operation {
public:
    float zLayer;
    SlicingContext* context;
    CarvedSlice* slice;

    SvgDumpOp(SlicingContext* ctx, CarvedSlice* slc, float Z)
        : Operation(), zLayer(Z), context(ctx), slice(slc)
    {
    }
    virtual ~SvgDumpOp();
    virtual void main();
};

#endif

