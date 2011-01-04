//
//  GCodeExportOp.h
//  Mandoline
//
//  Created by GM on 11/24/10.
//  Copyright 2010 Belfry DevWorks. All rights reserved.
//

#ifndef GCODEEXPORTOP_H
#define GCODEEXPORTOP_H

#include "CarvedSlice.h"
#include "SlicingContext.h"
#include "Operation.h"

class GCodeExportOp : public Operation {
public:
    SlicingContext* context;

    GCodeExportOp(SlicingContext* ctx);
    virtual ~GCodeExportOp();
    virtual void main();
};

#endif

