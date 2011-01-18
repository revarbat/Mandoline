#include <fstream>
#include "BGL.h"

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
    os << " >" << endl;

    return os;
} 



ostream &svgFooter(ostream& os)
{
    os << "</svg>" << endl;
    return os;
}



BGL::Point testPath1[] = {
    BGL::Point( 0.0, 15.0),
    BGL::Point(20.0, 30.0),
    BGL::Point(40.0, 15.0),
    BGL::Point(20.0,  0.0),
    BGL::Point( 0.0, 15.0)
};


BGL::Point testPath2[] = {
    BGL::Point( 5.0, 25.0),
    BGL::Point( 5.0,  5.0),
    BGL::Point(35.0,  5.0),
    BGL::Point(35.0, 25.0),
    BGL::Point( 5.0, 25.0)
};


int main(int argc, char**argv)
{
    BGL::Path path1(sizeof(testPath1)/sizeof(BGL::Point), testPath1);
    BGL::Path path2(sizeof(testPath2)/sizeof(BGL::Point), testPath2);

    fstream fout;

    fout.open("test_orig.svg", fstream::out | fstream::trunc);
    if (fout.good()) {
	svgHeader(fout, 300, 300);
	path1.svgPathWithOffset(fout, 10, 10);
	path2.svgPathWithOffset(fout, 10, 10);
	svgFooter(fout);
	fout.close();
    }

    fout.open("test_union.svg", fstream::out | fstream::trunc);
    if (fout.good()) {
	svgHeader(fout, 300, 300);

	BGL::Paths outPaths;
	BGL::Path::unionOf(path1, path2, outPaths);
	BGL::Paths::iterator pit;
	for (pit = outPaths.begin(); pit != outPaths.end(); pit++) {
	    pit->svgPathWithOffset(fout, 10, 10);
	}

	svgFooter(fout);
	fout.close();
    }

    fout.open("test_diff.svg", fstream::out | fstream::trunc);
    if (fout.good()) {
	svgHeader(fout, 300, 300);

	BGL::Paths outPaths;
	BGL::Path::differenceOf(path1, path2, outPaths);
	BGL::Paths::iterator pit;
	for (pit = outPaths.begin(); pit != outPaths.end(); pit++) {
	    pit->svgPathWithOffset(fout, 10, 10);
	}

	svgFooter(fout);
	fout.close();
    }

    fout.open("test_isect.svg", fstream::out | fstream::trunc);
    if (fout.good()) {
	svgHeader(fout, 300, 300);

	BGL::Paths outPaths;
	BGL::Path::intersectionOf(path1, path2, outPaths);
	BGL::Paths::iterator pit;
	for (pit = outPaths.begin(); pit != outPaths.end(); pit++) {
	    pit->svgPathWithOffset(fout, 10, 10);
	}

	svgFooter(fout);
	fout.close();
    }

    return 1;
}


