#include <fstream>
#include <BGL.hh>
#include "testbed.h"



BGL::Point pointSet[] =
{
    BGL::Point(20.000, 13.125),
    BGL::Point(20.000, 16.875),
    BGL::Point(18.125, 15.000),
    BGL::Point(20.000, 13.125)
};





int main(int argc, char**argv)
{
    char fname[128];

    BGL::Path path(sizeof(pointSet)/sizeof(BGL::Point), pointSet);

    path *= 3.0;

    fstream fout;
    BGL::SVG svg(250, 250);
    BGL::Paths::iterator pit;

    synthesize_testfile_name(fname, sizeof(fname), argv[0], "outset", "svg");
    fout.open(fname, fstream::out | fstream::trunc);
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


