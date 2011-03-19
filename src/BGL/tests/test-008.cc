#include <fstream>
#include "../BGL.h"



// Squiggle
BGL::Point pointSet[] = {
    BGL::Point( 5.0,  5.0),
    BGL::Point( 5.0, 25.0),
    BGL::Point(20.0, 25.0),
    BGL::Point(22.5, 30.0),
    BGL::Point(25.0, 20.0),
    BGL::Point(27.5, 25.0),
    BGL::Point(35.0, 25.0),
    BGL::Point(35.0,  5.0),
    BGL::Point( 5.0,  5.0)
};







int main(int argc, char**argv)
{

    BGL::Path path(sizeof(pointSet)/sizeof(BGL::Point), pointSet);
    path *= 3.0;

    fstream fout;
    BGL::SVG svg(250, 250);

    fout.open("output/test-008a-insets.svg", fstream::out | fstream::trunc);
    if (fout.good()) {
	svg.header(fout);

	fout << "<g stroke=\"#77f\">" << endl;
	path.svgPathWithOffset(fout, 30, 30);
	fout << "</g>" << endl;

	for (float i = 1.0; i <= 50.0; i += 1.0) {
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

    fout.open("output/test-008b-outsets.svg", fstream::out | fstream::trunc);
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


