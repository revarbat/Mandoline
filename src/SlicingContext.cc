//
//  SlicingContext.cc
//  Mandoline
//
//  Created by GM on 11/24/10.
//  Copyright 2010 Belfry DevWorks. All rights reserved.
//

#include "SlicingContext.h"
#include "Defaults.h"


// default constructor
SlicingContext::SlicingContext()
{
    filamentFeedRate     = DEFAULT_FILAMENT_FEED_RATE;
    filamentDiameter     = DEFAULT_FILAMENT_DIAMETER;
    layerThickness       = DEFAULT_LAYER_THICKNESS;
    widthOverHeightRatio = DEFAULT_WIDTH_OVER_HEIGHT;
    shrinkageRatio       = DEFAULT_SHRINKAGE_RATIO;
    infillDensity        = DEFAULT_INFILL_DENSITY;
    perimeterShells      = DEFAULT_PERIMETER_SHELLS;
    
    calculateSvgOffsets();
}



void SlicingContext::calculateSvgOffsets()
{
    // Calculate offsets for SVG dumps
    svgWidth  = 40 + mesh.maxX - mesh.minX;
    svgHeight = 40 + mesh.maxY - mesh.minY;
    svgXOff   = 20 - mesh.minX;
    svgYOff   = 20 - mesh.minY;
}



void SlicingContext::loadDefaultsFromFile(const char *fileName)
{
    // TODO: Implement this.
}




float SlicingContext::standardFeedRate()
{
    float filamentRadius = 0.5f * filamentDiameter;
    return (4*filamentFeedRate*filamentRadius*filamentRadius) / (widthOverHeightRatio*layerThickness*layerThickness);
}



float SlicingContext::standardExtrusionWidth()
{
    return layerThickness * widthOverHeightRatio;
}



float SlicingContext::ratioForWidth(float extrusionWidth)
{
    return extrusionWidth / layerThickness;
}



float SlicingContext::feedRateForWidth(float extrusionWidth)
{
    float filamentRadius = 0.5f * filamentDiameter;
    return (4*filamentFeedRate*filamentRadius*filamentRadius) / (extrusionWidth*layerThickness);
}



CarvedSlice* SlicingContext::allocSlice(float Z)
{
    CarvedSlice *slice = NULL;
    // TODO: pthread mutex lock
    slices[Z] = CarvedSlice();
    slice = &slices[Z];
    // TODO: pthread mutex unlock
    return slice;
}


CarvedSlice* SlicingContext::getSliceAtZ(float Z)
{
    CarvedSlice *slice = NULL;
    // TODO: pthread mutex lock
    slice = &slices[Z];
    // TODO: pthread mutex unlock
    return slice;
}



