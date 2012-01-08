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

#define MAX_TOOLS 2

class SlicingContext {
public:
    double filamentFeedRate[MAX_TOOLS];
    double filamentDiameter[MAX_TOOLS];
    double driveGearDiameter[MAX_TOOLS];
    double nozzleDiameter[MAX_TOOLS];
    double extruderTemp[MAX_TOOLS];
    double retractionRate[MAX_TOOLS];
    double retractionTime[MAX_TOOLS];
    double pushBackTime[MAX_TOOLS];
    double xAxisOffset[MAX_TOOLS];
    double yAxisOffset[MAX_TOOLS];
    double materialFudge[MAX_TOOLS];

    double platformTemp;
    int    supportTool;

    double layerThickness;
    double widthOverHeightRatio;
    double infillDensity;
    int    perimeterShells;
    int    flatShells;
    int    raftLayers;
    double raftOutset;
    double minLayerTime;
    InfillStyle infillStyle;

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
    double standardFeedRate(int tool);
    double standardExtrusionWidth();
    double ratioForWidth(double extrusionWidth);
    double feedRateForWidth(int tool, double extrusionWidth);

    CarvedSlice* allocSlice(double Z);
};

#endif

