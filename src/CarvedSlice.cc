#include <ostream>
#include "CarvedSlice.h"


void CarvedSlice::svgPathWithSizeAndOffset(ostream &os, float width, float height, float dx, float dy, float strokeWidth)
{
    float pwidth  = width * 90.0f / 25.4f;
    float pheight = height * 90.0f / 25.4f;

    os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    os << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
    os << "<svg xmlns=\"http://www.w3.org/2000/svg\"";
    os << " xml:space=\"preserve\"";
    os << " style=\"shape-rendering:geometricPrecision; text-rendering:geometricPrecision; image-rendering:optimizeQuality; fill-rule:evenodd; clip-rule:evenodd\"";
    os << " xmlns:xlink=\"http://www.w3.org/1999/xlink\"";
    os << " width=\"" << width << "mm\"";
    os << " height=\"" << height << "mm\"";
    os << " viewport=\"0 0 " << pwidth << " " << pheight << "\">\n";

    CompoundRegions::iterator rit;
    for (rit = shells.begin(); rit != shells.end(); rit++) {
	os << "<path fill=\"none\" stroke=\"black\"";
	os << " stroke-width=\"" << strokeWidth << "mm\"";
	os << " d=\"" << rit->svgPathWithOffset(dx,dy) << "\" />\n";
    }

    Paths::const_iterator pit;
    for (pit = infill.begin(); pit != infill.end(); pit++) {
	os << "<path fill=\"none\" stroke=\"black\"";
	os << " stroke-width=\"" << strokeWidth << "mm\"";
	os << " d=\"" << pit->svgPathWithOffset(dx,dy) << "\" />\n";
    }

    os << "</svg>\n";
}



