#include <fstream>
#include <BGL.hh>
#include "testbed.h"



BGL::Point pointSetA11[] =
{
    BGL::Point( 5.0,  5.0),
    BGL::Point( 5.0, 25.0),
    BGL::Point(35.0, 25.0),
    BGL::Point(35.0,  5.0),
    BGL::Point( 5.0,  5.0)
};

BGL::Point pointSetA12[] =
{
    BGL::Point(20.0,  7.5),
    BGL::Point(20.0, 22.5),
    BGL::Point(25.0, 22.5),
    BGL::Point(25.0,  7.5),
    BGL::Point(20.0,  7.5)
};

BGL::Point pointSetA13[] =
{
    BGL::Point(32.5,  7.5),
    BGL::Point(32.5, 22.5),
    BGL::Point(27.5, 22.5),
    BGL::Point(27.5,  7.5),
    BGL::Point(32.5,  7.5)
};



// Squiggle
BGL::Point pointSetA21[] = {
    BGL::Point(55.0, 25.0),
    BGL::Point(36.0, 15.0),
    BGL::Point(55.0,  0.0),
    BGL::Point(65.0,  0.0),
    BGL::Point(65.0,  5.0),
    BGL::Point(60.0,  5.0),
    BGL::Point(60.0,  2.5),
    BGL::Point(52.5, 10.0),
    BGL::Point(80.0, 15.0),
    BGL::Point(80.0, 25.0),
    BGL::Point(55.0, 25.0)
};





BGL::Point pointSetB11[] =
{
    BGL::Point( 0.0, 15.0),
    BGL::Point(20.0, 27.5),
    BGL::Point(40.0, 15.0),
    BGL::Point(20.0,  2.5),
    BGL::Point( 0.0, 15.0)
};

BGL::Point pointSetB12[] =
{
    BGL::Point(2.5, 15.0),
    BGL::Point(7.5, 17.5),
    BGL::Point(7.5, 12.5),
    BGL::Point(2.5, 15.0)
};

BGL::Point pointSetB13[] =
{
    BGL::Point(10.0, 17.5),
    BGL::Point(10.0, 12.5),
    BGL::Point(15.0, 12.5),
    BGL::Point(15.0, 17.5),
    BGL::Point(10.0, 17.5)
};

BGL::Point pointSetB14[] =
{
    BGL::Point(31.25, 17.5),
    BGL::Point(37.50, 15.0),
    BGL::Point(31.25, 12.5),
    BGL::Point(31.25, 17.5)
};

BGL::Point pointSetB15[] =
{
    BGL::Point(28.75, 12.5),
    BGL::Point(28.75, 17.5),
    BGL::Point(30.00, 17.5),
    BGL::Point(30.00, 12.5),
    BGL::Point(28.75, 12.5)
};

BGL::Point pointSetB16[] =
{
    BGL::Point(20.000, 13.125),
    BGL::Point(20.000, 16.875),
    BGL::Point(18.125, 15.000),
    BGL::Point(20.000, 13.125)
};

BGL::Point pointSetB17[] =
{
    BGL::Point(25.000, 13.125),
    BGL::Point(25.000, 16.875),
    BGL::Point(23.125, 15.000),
    BGL::Point(25.000, 13.125)
};


// Rectangle, mostly
BGL::Point pointSetB21[] = {
    BGL::Point(45.0,  5.0),
    BGL::Point(45.0, 25.0),
    BGL::Point(60.0, 25.0),
    BGL::Point(62.5, 30.0),
    BGL::Point(65.0, 20.0),
    BGL::Point(67.5, 25.0),
    BGL::Point(75.0, 25.0),
    BGL::Point(75.0,  5.0),
    BGL::Point(45.0,  5.0)
};





int main(int argc, char**argv)
{
    char fname[128];

    BGL::SimpleRegion regA1;
    regA1.outerPath = BGL::Path(sizeof(pointSetA11)/sizeof(BGL::Point), pointSetA11);
    regA1.subpaths.push_back(BGL::Path(sizeof(pointSetA12)/sizeof(BGL::Point), pointSetA12));
    regA1.subpaths.push_back(BGL::Path(sizeof(pointSetA13)/sizeof(BGL::Point), pointSetA13));

    BGL::SimpleRegion regA2;
    regA2.outerPath = BGL::Path(sizeof(pointSetA21)/sizeof(BGL::Point), pointSetA21);

    BGL::SimpleRegion regB1;
    regB1.outerPath = BGL::Path(sizeof(pointSetB11)/sizeof(BGL::Point), pointSetB11);
    regB1.subpaths.push_back(BGL::Path(sizeof(pointSetB12)/sizeof(BGL::Point), pointSetB12));
    regB1.subpaths.push_back(BGL::Path(sizeof(pointSetB13)/sizeof(BGL::Point), pointSetB13));
    regB1.subpaths.push_back(BGL::Path(sizeof(pointSetB14)/sizeof(BGL::Point), pointSetB14));
    regB1.subpaths.push_back(BGL::Path(sizeof(pointSetB15)/sizeof(BGL::Point), pointSetB15));
    regB1.subpaths.push_back(BGL::Path(sizeof(pointSetB16)/sizeof(BGL::Point), pointSetB16));
    regB1.subpaths.push_back(BGL::Path(sizeof(pointSetB17)/sizeof(BGL::Point), pointSetB17));

    BGL::SimpleRegion regB2;
    regB2.outerPath = BGL::Path(sizeof(pointSetB21)/sizeof(BGL::Point), pointSetB21);

    BGL::CompoundRegion compregA;
    compregA.subregions.push_back(regA1);
    compregA.subregions.push_back(regA2);

    BGL::CompoundRegion compregB;
    compregB.subregions.push_back(regB1);
    compregB.subregions.push_back(regB2);

    compregA *= 2.0;
    compregB *= 2.0;

    fstream fout;
    BGL::SVG svg(250, 250);

    synthesize_testfile_name(fname, sizeof(fname), argv[0], "union", "svg");
    fout.open(fname, fstream::out | fstream::trunc);
    if (fout.good()) {
	svg.header(fout);

	fout << "<g stroke=\"#000\" stroke-width=\"1.2\" fill=\"#ccf\">" << endl;
	BGL::CompoundRegion outRegs;
	BGL::CompoundRegion::unionOf(compregA, compregB, outRegs);
	outRegs.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	fout << "<g stroke=\"#77f\" stroke-width=\"0.6\">" << endl;
	compregA.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	fout << "<g stroke=\"#0c0\">" << endl;
	compregB.svgPathWithOffset(fout, 10, 10);
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
	BGL::CompoundRegion outRegs;
	BGL::CompoundRegion::differenceOf(compregA, compregB, outRegs);
	outRegs.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	fout << "<g stroke=\"#77f\" stroke-width=\"0.6\">" << endl;
	compregA.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	fout << "<g stroke=\"#0c0\">" << endl;
	compregB.svgPathWithOffset(fout, 10, 10);
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
	BGL::CompoundRegion outRegs;
	BGL::CompoundRegion::intersectionOf(compregA, compregB, outRegs);
	outRegs.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	fout << "<g stroke=\"#77f\" stroke-width=\"0.6\">" << endl;
	compregA.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	fout << "<g stroke=\"#0c0\">" << endl;
	compregB.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	svg.footer(fout);
	fout.sync();
	fout.close();
    }

    synthesize_testfile_name(fname, sizeof(fname), argv[0], "insetA-by5", "svg");
    fout.open(fname, fstream::out | fstream::trunc);
    if (fout.good()) {
	svg.header(fout);

	fout << "<g stroke=\"#77f\">" << endl;
	compregA.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	BGL::CompoundRegion outRegs;
	compregA.inset(1.0f, outRegs);
	outRegs.svgPathWithOffset(fout, 10, 10);

	svg.footer(fout);
	fout.sync();
	fout.close();
    }

    synthesize_testfile_name(fname, sizeof(fname), argv[0], "insetA-by1", "svg");
    fout.open(fname, fstream::out | fstream::trunc);
    if (fout.good()) {
	svg.header(fout);

	fout << "<g stroke=\"#77f\">" << endl;
	compregA.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	BGL::CompoundRegion outRegs;
	compregA.inset(2.0f, outRegs);
	outRegs.svgPathWithOffset(fout, 10, 10);

	svg.footer(fout);
	fout.sync();
	fout.close();
    }

    synthesize_testfile_name(fname, sizeof(fname), argv[0], "insetB-by5", "svg");
    fout.open(fname, fstream::out | fstream::trunc);
    if (fout.good()) {
	svg.header(fout);

	fout << "<g stroke=\"#77f\">" << endl;
	compregB.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	BGL::CompoundRegion outRegs;
	compregB.inset(1.0f, outRegs);
	outRegs.svgPathWithOffset(fout, 10, 10);

	svg.footer(fout);
	fout.sync();
	fout.close();
    }

    synthesize_testfile_name(fname, sizeof(fname), argv[0], "insetB-by1", "svg");
    fout.open(fname, fstream::out | fstream::trunc);
    if (fout.good()) {
	svg.header(fout);

	fout << "<g stroke=\"#77f\">" << endl;
	compregB.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	BGL::CompoundRegion outRegs;
	compregB.inset(2.0f, outRegs);
	outRegs.svgPathWithOffset(fout, 10, 10);

	svg.footer(fout);
	fout.sync();
	fout.close();
    }

    return 0;
}


