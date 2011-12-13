#include <fstream>
#include <BGL.hh>
#include "testbed.h"


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
    char fname[128];

    BGL::Path squigglePath(sizeof(squigglePoints)/sizeof(BGL::Point), squigglePoints);
    BGL::Path rectPath(sizeof(rectPoints)/sizeof(BGL::Point), rectPoints);

    squigglePath *= 3.0;
    rectPath *= 3.0;

    fstream fout;
    BGL::SVG svg(250, 250);

    synthesize_testfile_name(fname, sizeof(fname), argv[0], "union", "svg");
    fout.open(fname, fstream::out | fstream::trunc);
    if (fout.good()) {
	svg.header(fout);

	fout << "<g stroke=\"#000\" stroke-width=\"1.2\" fill=\"#ccf\">" << endl;
	BGL::Paths outPaths;
	BGL::Path::unionOf(squigglePath, rectPath, outPaths);
	BGL::SimpleRegions::iterator rit;
	BGL::SimpleRegions outRegs;
	BGL::SimpleRegion::assembleSimpleRegionsFrom(outPaths, outRegs);
	for (rit = outRegs.begin(); rit != outRegs.end(); rit++) {
	    rit->svgPathWithOffset(fout, 10, 10);
	}
	fout << "</g>" << endl;

	fout << "<g stroke=\"c00\" stroke-width=\"0.6\">" << endl;
	squigglePath.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	fout << "<g stroke=\"#0c0\">" << endl;
	rectPath.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	svg.footer(fout);
	fout.sync();
	fout.close();
    }

    synthesize_testfile_name(fname, sizeof(fname), argv[0], "diff", "svg");
    fout.open(fname, fstream::out | fstream::trunc);
    if (fout.good()) {
	svg.header(fout);

	fout << "<g stroke=\"#000\" stroke-width=\"1.2\" fill=\"#ccf\">" << endl;
	BGL::Paths outPaths;
	BGL::Path::differenceOf(squigglePath, rectPath, outPaths);
	BGL::SimpleRegions::iterator rit;
	BGL::SimpleRegions outRegs;
	BGL::SimpleRegion::assembleSimpleRegionsFrom(outPaths, outRegs);
	for (rit = outRegs.begin(); rit != outRegs.end(); rit++) {
	    rit->svgPathWithOffset(fout, 10, 10);
	}
	fout << "</g>" << endl;

	fout << "<g stroke=\"#c00\" stroke-width=\"0.6\">" << endl;
	squigglePath.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	fout << "<g stroke=\"#0c0\">" << endl;
	rectPath.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	svg.footer(fout);
	fout.sync();
	fout.close();
    }

    synthesize_testfile_name(fname, sizeof(fname), argv[0], "intsect", "svg");
    fout.open(fname, fstream::out | fstream::trunc);
    if (fout.good()) {
	svg.header(fout);

	fout << "<g stroke=\"#000\" stroke-width=\"1.2\" fill=\"#ccf\">" << endl;
	BGL::Paths outPaths;
	BGL::Path::intersectionOf(squigglePath, rectPath, outPaths);
	BGL::SimpleRegions::iterator rit;
	BGL::SimpleRegions outRegs;
	BGL::SimpleRegion::assembleSimpleRegionsFrom(outPaths, outRegs);
	for (rit = outRegs.begin(); rit != outRegs.end(); rit++) {
	    rit->svgPathWithOffset(fout, 10, 10);
	}
	fout << "</g>" << endl;

	fout << "<g stroke=\"#c00\" stroke-width=\"0.6\">" << endl;
	squigglePath.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	fout << "<g stroke=\"#0c0\">" << endl;
	rectPath.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	svg.footer(fout);
	fout.sync();
	fout.close();
    }

    return 0;
}


