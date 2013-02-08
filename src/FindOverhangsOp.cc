//
//  FindOverhangsOp.cc
//  Mandoline
//
//  Created by GM on 2012-03-02
//  Copyright 2012 Belfry DevWorks. All rights reserved.
//

#include <BGL.hh>
#include "FindOverhangsOp.hh"
#include "SlicingContext.hh"
#include "CarvedSlice.hh"



FindOverhangsOp::~FindOverhangsOp()
{
}



void FindOverhangsOp::main()
{
    if ( isCancelled ) return;
    if ( NULL == context ) return;

    const double criticalAngle = 45.0; // deg
    double insetBy = fabs(context->layerThickness * sin(criticalAngle * M_PI / 180.0));

    double supportGap = 0.5; // mm
    if (context->mainTool != context->supportTool) {
        // Presumably a separate support material is soluble and doesn't need a gap.
        supportGap = 0.0;
    }

    list<CarvedSlice>::reverse_iterator rit;
    list<CarvedSlice>::reverse_iterator prev_rit = context->slices.rend();

    BGL::CompoundRegion supportRgn;
    for (rit = context->slices.rbegin(); rit != context->slices.rend(); rit++) {
        if (prev_rit != context->slices.rend()) {
            BGL::CompoundRegion upperRgn(prev_rit->perimeter);
            BGL::CompoundRegion lowerRgn;
            BGL::CompoundRegion lowerRgn2;

            // Difference shrunken lower perim from full sized upper perim.
            // This leaves the regions that are at more than the critical overhang angle.
            rit->perimeter.inset(insetBy, lowerRgn);
            upperRgn.differenceWith(lowerRgn);

            // Union the overhangs into the cumulative support region.
            supportRgn.unionWith(upperRgn);

            // Difference out the current perimeter to remove supported support regions.
            // We use a slightly expanded perimeter for the diff, so we can separate the
            // support structures from the main model.
            rit->perimeter.inset(-supportGap, lowerRgn2);
            supportRgn.differenceWith(lowerRgn2);

            rit->supportMask = supportRgn;
        }
        context->processedLayers++;
        prev_rit = rit;
    }

    if ( isCancelled ) return;
}


// vim: set ts=4 sw=4 nowrap expandtab: settings

