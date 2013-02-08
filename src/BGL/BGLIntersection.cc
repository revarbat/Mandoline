#include "config.h"
#include "BGLIntersection.hh"
#include "BGLLine.hh"

namespace BGL {

Line Intersection::line() const
{
    return Line(p1, p2);
}



void Intersection::quantize(double quanta) {
    if (type != NONE) {
        p1.quantize(quanta);
        if (type != POINT) {
            p2.quantize(quanta);
        }
    }
}



void Intersection::quantize() {
    if (type != NONE) {
        p1.quantize();
        if (type != POINT) {
            p2.quantize();
        }
    }
}

}
// vim: set ts=4 sw=4 nowrap expandtab: settings

