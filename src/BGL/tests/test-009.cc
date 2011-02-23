#include <fstream>
#include "../BGL.h"



BGL::Point pointSetA1[] =
{
    BGL::Point( 5.0,  5.0),
    BGL::Point( 5.0, 25.0),
    BGL::Point(35.0, 25.0),
    BGL::Point(35.0,  5.0),
    BGL::Point( 5.0,  5.0)
};

BGL::Point pointSetA2[] =
{
    BGL::Point(20.0,  7.5),
    BGL::Point(20.0, 22.5),
    BGL::Point(25.0, 22.5),
    BGL::Point(25.0,  7.5),
    BGL::Point(20.0,  7.5)
};

BGL::Point pointSetA3[] =
{
    BGL::Point(32.5,  7.5),
    BGL::Point(32.5, 22.5),
    BGL::Point(27.5, 22.5),
    BGL::Point(27.5,  7.5),
    BGL::Point(32.5,  7.5)
};






BGL::Point pointSetB1[] =
{
    BGL::Point( 0.0, 15.0),
    BGL::Point(20.0, 27.5),
    BGL::Point(40.0, 15.0),
    BGL::Point(20.0,  2.5),
    BGL::Point( 0.0, 15.0)
};

BGL::Point pointSetB2[] =
{
    BGL::Point(2.5, 15.0),
    BGL::Point(7.5, 17.5),
    BGL::Point(7.5, 12.5),
    BGL::Point(2.5, 15.0)
};

BGL::Point pointSetB3[] =
{
    BGL::Point(10.0, 17.5),
    BGL::Point(10.0, 12.5),
    BGL::Point(15.0, 12.5),
    BGL::Point(15.0, 17.5),
    BGL::Point(10.0, 17.5)
};

BGL::Point pointSetB4[] =
{
    BGL::Point(31.25, 17.5),
    BGL::Point(37.50, 15.0),
    BGL::Point(31.25, 12.5),
    BGL::Point(31.25, 17.5)
};

BGL::Point pointSetB5[] =
{
    BGL::Point(28.75, 12.5),
    BGL::Point(28.75, 17.5),
    BGL::Point(30.00, 17.5),
    BGL::Point(30.00, 12.5),
    BGL::Point(28.75, 12.5)
};

BGL::Point pointSetB6[] =
{
    BGL::Point(20.000, 13.125),
    BGL::Point(20.000, 16.875),
    BGL::Point(18.125, 15.000),
    BGL::Point(20.000, 13.125)
};

BGL::Point pointSetB7[] =
{
    BGL::Point(25.000, 13.125),
    BGL::Point(25.000, 16.875),
    BGL::Point(23.125, 15.000),
    BGL::Point(25.000, 13.125)
};





int main(int argc, char**argv)
{
    BGL::SimpleRegion regA;
    regA.outerPath = BGL::Path(sizeof(pointSetA1)/sizeof(BGL::Point), pointSetA1);
    regA.subpaths.push_back(BGL::Path(sizeof(pointSetA2)/sizeof(BGL::Point), pointSetA2));
    regA.subpaths.push_back(BGL::Path(sizeof(pointSetA3)/sizeof(BGL::Point), pointSetA3));

    BGL::SimpleRegion regB;
    regB.outerPath = BGL::Path(sizeof(pointSetB1)/sizeof(BGL::Point), pointSetB1);
    regB.subpaths.push_back(BGL::Path(sizeof(pointSetB2)/sizeof(BGL::Point), pointSetB2));
    regB.subpaths.push_back(BGL::Path(sizeof(pointSetB3)/sizeof(BGL::Point), pointSetB3));
    regB.subpaths.push_back(BGL::Path(sizeof(pointSetB4)/sizeof(BGL::Point), pointSetB4));
    regB.subpaths.push_back(BGL::Path(sizeof(pointSetB5)/sizeof(BGL::Point), pointSetB5));
    regB.subpaths.push_back(BGL::Path(sizeof(pointSetB6)/sizeof(BGL::Point), pointSetB6));
    regB.subpaths.push_back(BGL::Path(sizeof(pointSetB7)/sizeof(BGL::Point), pointSetB7));

    regA *= 3.0;
    regB *= 3.0;

    fstream fout;
    BGL::SVG svg(250, 100);
    BGL::SimpleRegions::iterator sit;

    fout.open("output/test-009a-insetA-by05.svg", fstream::out | fstream::trunc);
    if (fout.good()) {
	svg.header(fout);

	fout << "<g stroke=\"#77f\">" << endl;
	regA.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	BGL::SimpleRegions outRegs;
	regA.inset(1.5f, outRegs);
	for (sit = outRegs.begin(); sit != outRegs.end(); sit++) {
	    sit->svgPathWithOffset(fout, 10, 10);
	}

	svg.footer(fout);
	fout.sync();
	fout.close();
    }

    fout.open("output/test-009b-insetA-by1.svg", fstream::out | fstream::trunc);
    if (fout.good()) {
	svg.header(fout);

	fout << "<g stroke=\"#77f\">" << endl;
	regA.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	BGL::SimpleRegions outRegs;
	regA.inset(3.0f, outRegs);
	for (sit = outRegs.begin(); sit != outRegs.end(); sit++) {
	    sit->svgPathWithOffset(fout, 10, 10);
	}

	svg.footer(fout);
	fout.sync();
	fout.close();
    }

    fout.open("output/test-009c-insetB-by05.svg", fstream::out | fstream::trunc);
    if (fout.good()) {
	svg.header(fout);

	fout << "<g stroke=\"#77f\">" << endl;
	regB.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	BGL::SimpleRegions outRegs;
	regB.inset(1.5f, outRegs);
	for (sit = outRegs.begin(); sit != outRegs.end(); sit++) {
	    sit->svgPathWithOffset(fout, 10, 10);
	}

	svg.footer(fout);
	fout.sync();
	fout.close();
    }

    fout.open("output/test-009d-insetB-by1.svg", fstream::out | fstream::trunc);
    if (fout.good()) {
	svg.header(fout);

	fout << "<g stroke=\"#77f\">" << endl;
	regB.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	BGL::SimpleRegions outRegs;
	regB.inset(3.0f, outRegs);
	for (sit = outRegs.begin(); sit != outRegs.end(); sit++) {
	    sit->svgPathWithOffset(fout, 10, 10);
	}

	svg.footer(fout);
	fout.sync();
	fout.close();
    }

    return 0;
}


