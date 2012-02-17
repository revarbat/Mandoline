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
    CompoundRegions shells;
    Paths infill;
    Paths supportPaths;

    CarvedSlice(double thickness) :
        zLayer(0.0),
        layerThickness(thickness),
        state(INIT),
        perimeter(),
        infillMask(),
        solidFillMask(),
        shells(),
        infill(),
        supportPaths(),
        isRaft(false),
        speedMult(1.0)
    {
    }

    void svgPathWithSizeAndOffset(ostream &os, double width, double height, double dx, double dy, double strokeWidth);
};


#endif
// vim: set ts=4 sw=4 nowrap expandtab: settings

