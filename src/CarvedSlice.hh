#ifndef CARVEDSLICE_H
#define CARVEDSLICE_H

#include <BGL.hh>

using namespace BGL;


typedef enum {
    INIT,
    CARVED,
    INSET,
    INFILLED,
    RAFTED,
    PATHED,
    COOLED,
    OUTPUT
} CarveSliceStatus;


class CarvedSlice {
public:
    double zLayer;
    double layerThickness;
    double speedMult;
    bool   isRaft;
    CarveSliceStatus state;
    CompoundRegion perimeter;
    CompoundRegion infillMask;
    CompoundRegion solidFillMask;
    CompoundRegion supportMask;
    CompoundRegions shells;
    Paths infill;
    Paths supportPaths;

    CarvedSlice(double thickness) :
        zLayer(0.0),
        layerThickness(thickness),
        speedMult(1.0),
        isRaft(false),
        state(INIT),
        perimeter(),
        infillMask(),
        solidFillMask(),
        supportMask(),
        shells(),
        infill(),
        supportPaths()
    {
    }

    void svgPathWithSizeAndOffset(ostream &os, double width, double height, double dx, double dy, double strokeWidth);
};


#endif
// vim: set ts=4 sw=4 nowrap expandtab: settings

