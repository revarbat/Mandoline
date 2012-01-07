//
//  SlicingContext.hh
//  Mandoline
//
//  Created by GM on 11/24/10.
//  Copyright 2010 Belfry DevWorks. All rights reserved.
//

#ifndef SLICINGCONTEXT_H
#define SLICINGCONTEXT_H


#include <map>
#include <BGL.hh>
#include "CarvedSlice.hh"


using namespace std;
using namespace BGL;


class SlicingContext {
public:
    double filamentFeedRate;
    double filamentDiameter;
    double driveGearDiameter;
    double layerThickness;
    double widthOverHeightRatio;
    double shrinkageRatio;
    double infillDensity;
    int    perimeterShells;
    int    flatShells;
    int    raftLayers;
    double raftOutset;
    double minLayerTime;

    double svgWidth;
    double svgHeight;
    double svgXOff;
    double svgYOff;
    string dumpPrefix;
    
    Mesh3d mesh;
    list<CarvedSlice> slices;
    
    SlicingContext();

    void loadSettingsFromFile(const char *fileName);
    void saveSettingsToFile(const char *fileName);
    void calculateSvgOffsets();
    double standardFeedRate();
    double standardExtrusionWidth();
    double ratioForWidth(double extrusionWidth);
    double feedRateForWidth(double extrusionWidth);

    CarvedSlice* allocSlice(double Z);
};

#endif

