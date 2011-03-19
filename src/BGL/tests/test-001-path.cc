#include <fstream>
#include "../BGL.h"


// Squiggle
BGL::Point squigglePoints[] = {
    BGL::Point(15.0, 25.0),
    BGL::Point( 0.0, 15.0),
    BGL::Point(15.0,  0.0),
    BGL::Point(25.0,  0.0),
    BGL::Point(25.0,  5.0),
    BGL::Point(20.0,  5.0),
    BGL::Point(20.0,  2.5),
    BGL::Point(12.5, 10.0),
    BGL::Point(40.0, 15.0),
    BGL::Point(40.0, 25.0),
    BGL::Point(15.0, 25.0)
};


// Rectangle, mostly
BGL::Point rectPoints[] = {
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
    BGL::Path squigglePath(sizeof(squigglePoints)/sizeof(BGL::Point), squigglePoints);
    BGL::Path rectPath(sizeof(rectPoints)/sizeof(BGL::Point), rectPoints);

    squigglePath *= 3.0;
    rectPath *= 3.0;

    fstream fout;
    BGL::SVG svg(250, 250);

    fout.open("output/test-001a-path-union.svg", fstream::out | fstream::trunc);
    if (fout.good()) {
	svg.header(fout);

	fout << "<g stroke=\"#77f\" fill=\"#ccf\">" << endl;
	squigglePath.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	fout << "<g stroke=\"#0c0\">" << endl;
	rectPath.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	fout << "<g stroke-width=\"0.6\">" << endl;
	BGL::Paths outPaths;
	BGL::Paths::iterator pit;
	BGL::Path::unionOf(squigglePath, rectPath, outPaths);
	for (pit = outPaths.begin(); pit != outPaths.end(); pit++) {
	    pit->svgPathWithOffset(fout, 10, 10);
	}
	fout << "</g>" << endl;

	svg.footer(fout);
	fout.sync();
	fout.close();
    }

    fout.open("output/test-001b-path-diff.svg", fstream::out | fstream::trunc);
    if (fout.good()) {
	svg.header(fout);

	fout << "<g stroke=\"#77f\" fill=\"#ccf\">" << endl;
	squigglePath.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	fout << "<g stroke=\"#0c0\">" << endl;
	rectPath.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	fout << "<g stroke-width=\"0.6\">" << endl;
	BGL::Paths outPaths;
	BGL::Paths::iterator pit;
	BGL::Path::differenceOf(squigglePath, rectPath, outPaths);
	for (pit = outPaths.begin(); pit != outPaths.end(); pit++) {
	    pit->svgPathWithOffset(fout, 10, 10);
	}
	fout << "</g>" << endl;

	svg.footer(fout);
	fout.sync();
	fout.close();
    }

    fout.open("output/test-001c-path-intsect.svg", fstream::out | fstream::trunc);
    if (fout.good()) {
	svg.header(fout);

	fout << "<g stroke=\"#77f\" fill=\"#ccf\">" << endl;
	squigglePath.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	fout << "<g stroke=\"#0c0\">" << endl;
	rectPath.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	fout << "<g stroke-width=\"0.6\">" << endl;
	BGL::Paths outPaths;
	BGL::Paths::iterator pit;
	BGL::Path::intersectionOf(squigglePath, rectPath, outPaths);
	for (pit = outPaths.begin(); pit != outPaths.end(); pit++) {
	    pit->svgPathWithOffset(fout, 10, 10);
	}
	fout << "</g>" << endl;

	svg.footer(fout);
	fout.sync();
	fout.close();
    }

    return 0;
}


