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
    OUTPUT
} CarveSliceStatus;


class CarvedSlice {
public:
    double zLayer;
    CarveSliceStatus state;
    CompoundRegion perimeter;
    CompoundRegion infillMask;
    CompoundRegion solidFillMask;
    CompoundRegions shells;
    Paths infill;

    CarvedSlice() :
    	zLayer(0.0),
	state(INIT),
	perimeter(),
	infillMask(),
	solidFillMask(),
	shells(),
	infill()
    {
    }

    void svgPathWithSizeAndOffset(ostream &os, double width, double height, double dx, double dy, double strokeWidth);
};


#endif

