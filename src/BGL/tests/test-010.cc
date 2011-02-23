#include <fstream>
#include "../BGL.h"



BGL::Point pointSet[] =
{
    BGL::Point(20.000, 13.125),
    BGL::Point(20.000, 16.875),
    BGL::Point(18.125, 15.000),
    BGL::Point(20.000, 13.125)
};





int main(int argc, char**argv)
{
    BGL::Path path(sizeof(pointSet)/sizeof(BGL::Point), pointSet);

    path *= 3.0;

    fstream fout;
    BGL::SVG svg(250, 100);
    BGL::Paths::iterator pit;

    fout.open("output/test-010a-outset.svg", fstream::out | fstream::trunc);
    if (fout.good()) {
	svg.header(fout);

	fout << "<g stroke=\"#77f\">" << endl;
	path.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	BGL::Paths outPaths;
	path.inset(-3.0f, outPaths);
	for (pit = outPaths.begin(); pit != outPaths.end(); pit++) {
	    pit->svgPathWithOffset(fout, 10, 10);
	}

	svg.footer(fout);
	fout.sync();
	fout.close();
    }

    return 0;
}


