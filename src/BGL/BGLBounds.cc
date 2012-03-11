#include "config.h"
#include "BGLBounds.hh"

namespace BGL {

const double Bounds::NONE = -9e9;

void Bounds::expand(const Point& pt)
{
    if (minX == NONE) {
        minX = maxX = pt.x;
        minY = maxY = pt.y;
        return;
    }
    if (pt.x < minX) {
        minX = pt.x;
    }
    if (pt.x > maxX) {
        maxX = pt.x;
    }
    if (pt.y < minY) {
        minY = pt.y;
    }
    if (pt.y > maxY) {
        maxY = pt.y;
    }
}


}
// vim: set ts=4 sw=4 nowrap expandtab: settings

