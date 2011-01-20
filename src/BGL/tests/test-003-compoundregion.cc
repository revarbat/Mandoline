#include <fstream>
#include "../BGL.h"

ostream &svgHeader(ostream &os, float width, float height)
{
    width *= 2;
    height *=2;
    float pwidth  = width * 90.0f / 25.4f;
    float pheight = height * 90.0f / 25.4f;

    os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    os << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
    os << "<svg xmlns=\"http://www.w3.org/2000/svg\"";
    os << " xml:space=\"preserve\"";
    os << " style=\"shape-rendering:geometricPrecision; text-rendering:geometricPrecision; image-rendering:optimizeQuality; fill-rule:evenodd; clip-rule:evenodd\"";
    os << " xmlns:xlink=\"http://www.w3.org/1999/xlink\"";
    os << " width=\"" << width << "mm\"";
    os << " height=\"" << height << "mm\"";
    os << " viewport=\"0 0 " << pwidth << " " << pheight << "\"";
    os << " stroke=\"black\"";
    os << ">" << endl;
    os << "<g transform=\"scale(2.0)\">";

    return os;
} 



ostream &svgFooter(ostream& os)
{
    os << "</g>" << endl;
    os << "</svg>" << endl;
    return os;
}



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

    fstream fout;

    fout.open("output/test-003a-compreg-orig.svg", fstream::out | fstream::trunc);
    if (fout.good()) {
	svgHeader(fout, 100, 100);

	fout << "<g stroke=\"#77f\">" << endl;
	compregA.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	fout << "<g stroke=\"#0c0\">" << endl;
	compregB.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	svgFooter(fout);
	fout.sync();
	fout.close();
    }

    fout.open("output/test-003b-compreg-union.svg", fstream::out | fstream::trunc);
    if (fout.good()) {
	svgHeader(fout, 100, 100);

	BGL::CompoundRegion outRegs;
	BGL::CompoundRegion::unionOf(compregA, compregB, outRegs);
	outRegs.svgPathWithOffset(fout, 10, 10);

	svgFooter(fout);
	fout.sync();
	fout.close();
    }

    fout.open("output/test-003c-compreg-diff.svg", fstream::out | fstream::trunc);
    if (fout.good()) {
	svgHeader(fout, 100, 100);

	BGL::CompoundRegion outRegs;
	BGL::CompoundRegion::differenceOf(compregA, compregB, outRegs);
	outRegs.svgPathWithOffset(fout, 10, 10);

	svgFooter(fout);
	fout.sync();
	fout.close();
    }

    fout.open("output/test-003d-compreg-intsect.svg", fstream::out | fstream::trunc);
    if (fout.good()) {
	svgHeader(fout, 100, 100);

	BGL::CompoundRegion outRegs;
	BGL::CompoundRegion::intersectionOf(compregA, compregB, outRegs);
	outRegs.svgPathWithOffset(fout, 10, 10);

	svgFooter(fout);
	fout.sync();
	fout.close();
    }

    return 0;
}


