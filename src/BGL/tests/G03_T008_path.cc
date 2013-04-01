#include <fstream>
#include <BGL.hh>
#include "testbed.h"



// Rectangle, mostly
BGL::Point pointSet[] = {
    BGL::Point( 55, 95),
    BGL::Point( 95, 95),
    BGL::Point( 95,  5),
    BGL::Point(  5,  5),
    BGL::Point(  5, 95),
    BGL::Point( 55, 95)
};




int main(int argc, char**argv)
{
    char fname[128];
    double infillInset=10;

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

	fout << "<g stroke=\"#0c0\">" << endl;
	for (it = outPaths.begin(); it != outPaths.end(); it++) {
	    it->svgPathWithOffset(fout, 0, 0);
	}
	fout << "</g>" << endl;

	fout << "<g stroke=\"#000\" stroke-width=\"0.6\">" << endl;
	path.svgPathWithOffset(fout, 0, 0);
	fout << "</g>" << endl;

	svg.footer(fout);
	fout.sync();
	fout.close();
    }

    return 0;
}


