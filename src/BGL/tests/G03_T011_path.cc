#include <fstream>
#include <BGL.hh>
#include "testbed.h"



// Squiggle
BGL::Point pointSet[] = {
    BGL::Point(30.0, 15.0),
    BGL::Point(35.0,  5.0),
    BGL::Point( 5.0,  5.0),
    BGL::Point(10.0, 15.0),
    BGL::Point( 5.0, 25.0),
    BGL::Point(35.0, 25.0),
    BGL::Point(30.0, 15.0),
};







int main(int argc, char**argv)
{
    char fname[128];

    BGL::Path path(sizeof(pointSet)/sizeof(BGL::Point), pointSet);
    path *= 3.0;

    fstream fout;
    BGL::SVG svg(250, 250);

    synthesize_testfile_name(fname, sizeof(fname), argv[0], "insets", "svg");
    fout.open(fname, fstream::out | fstream::trunc);
    if (fout.good()) {
	svg.header(fout);

	fout << "<g stroke=\"#77f\">" << endl;
	path.svgPathWithOffset(fout, 30, 30);
	fout << "</g>" << endl;

	for (float i = 3.0; i == 3.0; i += 1.0) {
	    BGL::Paths outPaths;
	    path.inset(i, outPaths);
	    BGL::Paths::const_iterator it;
	    for (it = outPaths.begin(); it != outPaths.end(); it++) {
		it->svgPathWithOffset(fout, 30, 30);
	    }
	}

	svg.footer(fout);
	fout.sync();
	fout.close();
    }
    return 0;

    synthesize_testfile_name(fname, sizeof(fname), argv[0], "outsets", "svg");
    fout.open(fname, fstream::out | fstream::trunc);
    if (fout.good()) {
	svg.header(fout);

	fout << "<g stroke=\"#77f\">" << endl;
	path.svgPathWithOffset(fout, 30, 30);
	fout << "</g>" << endl;

	for (float i = -1.0; i >= -30.0; i -= 1.0) {
	    BGL::Paths outPaths;
	    path.inset(i, outPaths);
	    BGL::Paths::const_iterator it;
	    for (it = outPaths.begin(); it != outPaths.end(); it++) {
		it->svgPathWithOffset(fout, 30, 30);
	    }
	}

	svg.footer(fout);
	fout.sync();
	fout.close();
    }

    return 0;
}


