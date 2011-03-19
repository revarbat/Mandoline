#include <fstream>
#include "../BGL.h"


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
    BGL::Path rectPath(sizeof(rectPoints)/sizeof(BGL::Point), rectPoints);
    rectPath *= 3.0;

    BGL::Path offPath1(rectPath);
    BGL::Path offPath2(rectPath);

    offPath1 += BGL::Point(3.0*cos(M_PI/8.0f), 3.0*sin(M_PI/8.0f));
    offPath2 += BGL::Point(3.0*cos(M_PI/4.0f), 3.0*sin(M_PI/4.0f));

    BGL::Paths::iterator pit;
    BGL::Paths outPaths1;
    BGL::Path::differenceOf(rectPath, offPath1, outPaths1);
    BGL::CompoundRegion compRegA(outPaths1);
    compRegA.subregions.pop_front();
    compRegA.subregions.pop_front();

    BGL::Paths outPaths2;
    BGL::Path::differenceOf(rectPath, offPath2, outPaths2);
    BGL::CompoundRegion compRegB(outPaths2);
    compRegB.subregions.pop_front();
    compRegB.subregions.pop_front();

    fstream fout;
    BGL::SVG svg(250, 250);

    fout.open("output/test-005a-path-orig.svg", fstream::out | fstream::trunc);
    if (fout.good()) {
	svg.header(fout);

	fout << "<g stroke=\"#c00\">" << endl;
	compRegA.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	fout << "<g stroke=\"#55f\">" << endl;
	compRegB.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	svg.footer(fout);
	fout.sync();
	fout.close();
    }

    fout.open("output/test-005b-path-union.svg", fstream::out | fstream::trunc);
    if (fout.good()) {
	svg.header(fout);

	BGL::CompoundRegion outReg;
	BGL::CompoundRegion::unionOf(compRegA, compRegB, outReg);
	outReg.svgPathWithOffset(fout, 10, 10);

	svg.footer(fout);
	fout.sync();
	fout.close();
    }

    return 0;
}


