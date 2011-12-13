#include <fstream>
#include <BGL.hh>
#include "testbed.h"



// Rectangle, mostly
BGL::Point pointSet[] = {
    BGL::Point( 0.0, 15.0),
    BGL::Point(20.0, 27.5),
    BGL::Point(40.0, 15.0),
    BGL::Point(20.0,  2.5),
    BGL::Point( 0.0, 15.0)
};




int main(int argc, char**argv)
{
    char fname[128];

    BGL::Path origPath(sizeof(pointSet)/sizeof(BGL::Point), pointSet);
    origPath *= 3.0;

    BGL::Path offPath1(origPath);
    BGL::Path offPath2(origPath);
    BGL::Path offPath3(origPath);
    BGL::Path offPath4(origPath);

    offPath1 += BGL::Point(3.0*cos(0.0f),             3.0*sin(0.0f));
    offPath2 += BGL::Point(3.0*cos(M_PI/8.0f),        3.0*sin(M_PI/8.0f));
    offPath3 += BGL::Point(3.0*cos(M_PI/4.0f),        3.0*sin(M_PI/4.0f));
    offPath4 += BGL::Point(3.0*cos(M_PI/(8.0f/3.0f)), 3.0*sin(M_PI/(8.0f/3.0f)));

    BGL::Paths outPaths1;
    BGL::Path::differenceOf(origPath, offPath1, outPaths1);
    BGL::CompoundRegion compRegA(outPaths1);

    BGL::Paths outPaths2;
    BGL::Path::differenceOf(origPath, offPath2, outPaths2);
    BGL::CompoundRegion compRegB(outPaths2);

    BGL::Paths outPaths3;
    BGL::Path::differenceOf(origPath, offPath3, outPaths3);
    BGL::CompoundRegion compRegC(outPaths3);

    BGL::Paths outPaths4;
    BGL::Path::differenceOf(origPath, offPath4, outPaths4);
    BGL::CompoundRegion compRegD(outPaths4);

    BGL::CompoundRegion outReg;
    BGL::CompoundRegion outReg2;
    BGL::CompoundRegion outReg3;
    BGL::CompoundRegion::unionOf(compRegA, compRegB, outReg);
    BGL::CompoundRegion::unionOf(outReg, compRegC, outReg2);
    BGL::CompoundRegion::unionOf(outReg2, compRegD, outReg3);

    fstream fout;
    BGL::SVG svg(250, 250);

    synthesize_testfile_name(fname, sizeof(fname), argv[0], "origAB", "svg");
    fout.open(fname, fstream::out | fstream::trunc);
    if (fout.good()) {
	svg.header(fout);

	fout << "<g stroke=\"#c00\">" << endl;
	compRegA.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	fout << "<g stroke=\"#0c0\">" << endl;
	compRegB.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;
	
	svg.footer(fout);
	fout.sync();
	fout.close();
    }

    synthesize_testfile_name(fname, sizeof(fname), argv[0], "unionAB", "svg");
    fout.open(fname, fstream::out | fstream::trunc);
    if (fout.good()) {
	svg.header(fout);

	outReg.svgPathWithOffset(fout, 10, 10);
	
	svg.footer(fout);
	fout.sync();
	fout.close();
    }

    synthesize_testfile_name(fname, sizeof(fname), argv[0], "origABC", "svg");
    fout.open(fname, fstream::out | fstream::trunc);
    if (fout.good()) {
	svg.header(fout);

	fout << "<g stroke=\"#c00\">" << endl;
	outReg.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	fout << "<g stroke=\"#00c\">" << endl;
	compRegC.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;
	
	svg.footer(fout);
	fout.sync();
	fout.close();
    }

    synthesize_testfile_name(fname, sizeof(fname), argv[0], "unionABC", "svg");
    fout.open(fname, fstream::out | fstream::trunc);
    if (fout.good()) {
	svg.header(fout);

	outReg2.svgPathWithOffset(fout, 10, 10);
	
	svg.footer(fout);
	fout.sync();
	fout.close();
    }

    synthesize_testfile_name(fname, sizeof(fname), argv[0], "origABCD", "svg");
    fout.open(fname, fstream::out | fstream::trunc);
    if (fout.good()) {
	svg.header(fout);

	fout << "<g stroke=\"#c00\">" << endl;
	outReg2.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	fout << "<g stroke=\"#0cc\">" << endl;
	compRegD.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;
	
	svg.footer(fout);
	fout.sync();
	fout.close();
    }

    synthesize_testfile_name(fname, sizeof(fname), argv[0], "unionABCD", "svg");
    fout.open(fname, fstream::out | fstream::trunc);
    if (fout.good()) {
	svg.header(fout);

	outReg3.svgPathWithOffset(fout, 10, 10);

	svg.footer(fout);
	fout.sync();
	fout.close();
    }

    return 0;
}


