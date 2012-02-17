#include <fstream>
#include <BGL.hh>
#include "testbed.h"



// Rectangle, mostly
BGL::Point pointSet[] = {
    BGL::Point( -9.849997, 10.000000),
    BGL::Point( 10.000000, 10.000000),
    BGL::Point( 10.000000, -9.999999),
    BGL::Point(-10.000000, -9.999997),
    BGL::Point( -9.999997, 10.000000),
    BGL::Point( -9.849997, 10.000000)
};




int main(int argc, char**argv)
{
    char fname[128];
    double infillInset=1.410000;

    BGL::Path path(sizeof(pointSet)/sizeof(BGL::Point), pointSet);

    fstream fout;
    BGL::SVG svg(250, 250);

    synthesize_testfile_name(fname, sizeof(fname), argv[0], "inset", "svg");
    fout.open(fname, fstream::out | fstream::trunc);
    if (fout.good()) {
	svg.header(fout);

	BGL::Paths outPaths;
	path.inset(infillInset, outPaths);
	BGL::Paths::iterator it;

	fout << "<g stroke=\"#77f\">" << endl;
	path *= 3.0;
	path.svgPathWithOffset(fout, 30, 30);
	fout << "</g>" << endl;

	for (it = outPaths.begin(); it != outPaths.end(); it++) {
	    *it *= 3.0;
	    it->svgPathWithOffset(fout, 30, 30);
	}

	svg.footer(fout);
	fout.sync();
	fout.close();
    }

    return 0;
}


