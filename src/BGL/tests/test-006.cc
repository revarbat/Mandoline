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
    os << "<g transform=\"scale(2.0)\" stroke-width=\"0.5pt\">" << endl;

    return os;
} 



ostream &svgFooter(ostream& os)
{
    os << "</g>" << endl;
    os << "</svg>" << endl;
    return os;
}



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
    BGL::Path origPath(sizeof(pointSet)/sizeof(BGL::Point), pointSet);
    BGL::Path offPath1(origPath);
    BGL::Path offPath2(origPath);
    BGL::Path offPath3(origPath);
    BGL::Path offPath4(origPath);

    offPath1 += BGL::Point(cos(0.0f), sin(0.0f));
    offPath2 += BGL::Point(cos(M_PI/8.0f), sin(M_PI/8.0f));
    offPath3 += BGL::Point(cos(M_PI/4.0f), sin(M_PI/4.0f));
    offPath4 += BGL::Point(cos(M_PI/(8.0f/3.0f)), sin(M_PI/(8.0f/3.0f)));

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
    BGL::CompoundRegion::unionOf(compRegA, compRegB, outReg);
    BGL::CompoundRegion outReg2;
    BGL::CompoundRegion::unionOf(outReg, compRegC, outReg2);

    fstream fout;

    fout.open("output/test-006a-path-orig.svg", fstream::out | fstream::trunc);
    if (fout.good()) {
	svgHeader(fout, 100, 100);

	fout << "<g stroke=\"#c00\" stroke-width=\"1.5\">" << endl;
	outReg2.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	fout << "<g stroke=\"#0c0\">" << endl;
	compRegD.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;
	
	svgFooter(fout);
	fout.sync();
	fout.close();
    }

    fout.open("output/test-006b-path-union.svg", fstream::out | fstream::trunc);
    if (fout.good()) {
	svgHeader(fout, 100, 100);

	BGL::CompoundRegion outReg3;
	BGL::CompoundRegion::unionOf(outReg2, compRegD, outReg3);
	outReg3.svgPathWithOffset(fout, 10, 10);

	svgFooter(fout);
	fout.sync();
	fout.close();
    }

    return 0;
}


