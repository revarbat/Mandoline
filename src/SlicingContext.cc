//
//  SlicingContext.cc
//  Mandoline
//
//  Created by GM on 11/24/10.
//  Copyright 2010 Belfry DevWorks. All rights reserved.
//

#include "SlicingContext.hh"
#include "Defaults.hh"


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
    flatShells           = DEFAULT_FLAT_SHELLS;
    raftLayers           = DEFAULT_RAFT_LAYERS;
    raftOutset           = DEFAULT_RAFT_OUTSET;
    minLayerTime         = DEFAULT_MIN_LAYER_TIME;
    
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




double SlicingContext::standardFeedRate()
{
    double filamentRadius = 0.5f * filamentDiameter;
    return (4*filamentFeedRate*filamentRadius*filamentRadius) / (widthOverHeightRatio*layerThickness*layerThickness);
}



double SlicingContext::standardExtrusionWidth()
{
    return layerThickness * widthOverHeightRatio;
}



double SlicingContext::ratioForWidth(double extrusionWidth)
{
    return extrusionWidth / layerThickness;
}



double SlicingContext::feedRateForWidth(double extrusionWidth)
{
    double filamentRadius = 0.5f * filamentDiameter;
    return (4*filamentFeedRate*filamentRadius*filamentRadius) / (extrusionWidth*layerThickness);
}



CarvedSlice* SlicingContext::allocSlice(double Z)
{
    CarvedSlice *slice = NULL;
    // TODO: pthread mutex lock
    slices.push_back(CarvedSlice());
    slice = &slices.back();
    slice->zLayer = Z;
    // TODO: pthread mutex unlock
    return slice;
}



