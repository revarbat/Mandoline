#ifndef CARVEDSLICE_H
#define CARVEDSLICE_H

#include <BGL.hh>

using namespace BGL;


typedef enum {
    INIT,
    CARVED,
    INSET,
    INFILLED,
    PATHED,
    OUTPUT
} CarveSliceStatus;


class CarvedSlice {
public:
    CarveSliceStatus state;
    CompoundRegion perimeter;
    CompoundRegion infillMask;
    CompoundRegions shells;
    Paths infill;

    CarvedSlice() : state(INIT), perimeter(), infill() {}

    void svgPathWithSizeAndOffset(ostream &os, double width, double height, double dx, double dy, double strokeWidth);
};


#endif

