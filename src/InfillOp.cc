//
//  InfillOp.cc
//  Mandoline
//
//  Created by GM on 11/24/10.
//  Copyright 2010 Belfry DevWorks. All rights reserved.
//

#include <BGL.hh>
#include "InfillOp.hh"
#include "SlicingContext.hh"
#include "CarvedSlice.hh"



InfillOp::~InfillOp()
{
}



void InfillOp::main()
{
    if ( isCancelled ) return;
    if ( NULL == context ) return;
    if ( NULL == slice ) return;

    // Calculate the regions needing full infill.
    double lowZ  = slice->zLayer - ((context->flatShells+0.5)*context->layerThickness);
    double highZ = slice->zLayer + ((context->flatShells+0.5)*context->layerThickness);
    if (lowZ <= 0.0 || highZ >= context->mesh.maxZ + context->layerThickness) {
        slice->solidFillMask = slice->infillMask;
    } else {
        list<CarvedSlice>::iterator cit;
        for (cit = context->slices.begin(); cit != context->slices.end(); cit++) {
            if (cit->zLayer >= lowZ && cit->zLayer <= highZ) {
                if (fabs(cit->zLayer - slice->zLayer) > 0.001) {
                    BGL::CompoundRegion tempRgn(slice->infillMask);
                    tempRgn.differenceWith(cit->perimeter);
                    slice->solidFillMask.unionWith(tempRgn);
                }
            }
        }
    }

    double extrusionWidth = context->standardExtrusionWidth();
    double angle;
    if (context->infillStyle == INFILL_HEXAGONAL) {
        angle = slice->zLayer / context->layerThickness * M_PI * (2.0/3.0); // Rotate 120deg each layer
    } else {
        angle = slice->zLayer / context->layerThickness * M_PI * 0.5; // Rotate 90deg each layer
    }
    slice->infillMask.infillPathsForRegionWithDensity(angle, context->infillStyle, context->infillDensity, extrusionWidth, slice->solidFillMask, slice->infill);
    slice->state = INFILLED;

    if ( isCancelled ) return;
}

// vim: set ts=4 sw=4 nowrap expandtab: settings

