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
    BGL::Path offPath1(rectPath);
    BGL::Path offPath2(rectPath);

    offPath1 += BGL::Point(cos(M_PI/8.0f), sin(M_PI/8.0f));
    offPath2 += BGL::Point(cos(M_PI/4.0f), sin(M_PI/4.0f));

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

    fout.open("output/test-005a-path-orig.svg", fstream::out | fstream::trunc);
    if (fout.good()) {
	svgHeader(fout, 100, 100);

	fout << "<g stroke=\"#c00\">" << endl;
	compRegA.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	fout << "<g stroke=\"#55f\">" << endl;
	compRegB.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	svgFooter(fout);
	fout.sync();
	fout.close();
    }

    fout.open("output/test-005b-path-union.svg", fstream::out | fstream::trunc);
    if (fout.good()) {
	svgHeader(fout, 100, 100);

	BGL::CompoundRegion outReg;
	BGL::CompoundRegion::unionOf(compRegA, compRegB, outReg);
	outReg.svgPathWithOffset(fout, 10, 10);

	svgFooter(fout);
	fout.sync();
	fout.close();
    }

    return 0;
}


