//
//  SlicingContext.h
//  Mandoline
//
//  Created by GM on 11/24/10.
//  Copyright 2010 Belfry DevWorks. All rights reserved.
//

#ifndef SLICINGCONTEXT_H
#define SLICINGCONTEXT_H


#include <map>
#include "BGL/BGL.h"
#include "CarvedSlice.h"


using namespace std;
using namespace BGL;


class SlicingContext {
public:
    float filamentFeedRate;
    float filamentDiameter;
    float layerThickness;
    float widthOverHeightRatio;
    float shrinkageRatio;
    float infillDensity;
    int   perimeterShells;

    float svgWidth;
    float svgHeight;
    float svgXOff;
    float svgYOff;
    string dumpPrefix;
    
    Mesh3d mesh;
    map<float,CarvedSlice> slices;
    
    SlicingContext();

    void loadDefaultsFromFile(const char *fileName);
    void calculateSvgOffsets();
    float standardFeedRate();
    float standardExtrusionWidth();
    float ratioForWidth(float extrusionWidth);
    float feedRateForWidth(float extrusionWidth);

    CarvedSlice* allocSlice(float Z);
    CarvedSlice* getSliceAtZ(float Z);
};

#endif

