#include <fstream>
#include "../BGL.h"

ostream &svgHeader(ostream &os, float width, float height)
{
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

    return os;
} 



ostream &svgFooter(ostream& os)
{
    os << "</svg>" << endl;
    return os;
}



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

    fstream fout;

    fout.open("output/test-002a-simpreg-orig.svg", fstream::out | fstream::trunc);
    if (fout.good()) {
	svgHeader(fout, 100, 100);

	fout << "<g stroke=\"#77f\" stroke-width=\"3\">" << endl;
	regA.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	fout << "<g stroke=\"#0c0\">" << endl;
	regB.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	svgFooter(fout);
	fout.sync();
	fout.close();
    }

    fout.open("output/test-002b-simpreg-union.svg", fstream::out | fstream::trunc);
    if (fout.good()) {
	svgHeader(fout, 100, 100);

	BGL::SimpleRegions outRegs;
	BGL::SimpleRegions::iterator rit;
	BGL::SimpleRegion::unionOf(regA, regB, outRegs);
	for (rit = outRegs.begin(); rit != outRegs.end(); rit++) {
	    rit->svgPathWithOffset(fout, 10, 10);
	}

	svgFooter(fout);
	fout.sync();
	fout.close();
    }

    fout.open("output/test-002c-simpreg-diff.svg", fstream::out | fstream::trunc);
    if (fout.good()) {
	svgHeader(fout, 100, 100);

	BGL::SimpleRegions outRegs;
	BGL::SimpleRegions::iterator rit;
	BGL::SimpleRegion::differenceOf(regA, regB, outRegs);
	for (rit = outRegs.begin(); rit != outRegs.end(); rit++) {
	    rit->svgPathWithOffset(fout, 10, 10);
	}

	svgFooter(fout);
	fout.sync();
	fout.close();
    }

    fout.open("output/test-002d-simpreg-intsect.svg", fstream::out | fstream::trunc);
    if (fout.good()) {
	svgHeader(fout, 100, 100);

	BGL::SimpleRegions outRegs;
	BGL::SimpleRegions::iterator rit;
	BGL::SimpleRegion::intersectionOf(regA, regB, outRegs);
	for (rit = outRegs.begin(); rit != outRegs.end(); rit++) {
	    rit->svgPathWithOffset(fout, 10, 10);
	}

	svgFooter(fout);
	fout.sync();
	fout.close();
    }

    return 0;
}


