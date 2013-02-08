#include "config.h"
#include "BGLPoint.hh"

namespace BGL {

ostream& operator <<(ostream &os,const Point3d &pt)
{
    os.precision(2);
    os.setf(ios::fixed);
    os << "(" << pt.x << ", " << pt.y << ", " << pt.z << ")";
    return os;
}


}
// vim: set ts=4 sw=4 nowrap expandtab: settings

