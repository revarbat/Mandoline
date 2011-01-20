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
    BGL::Path squigglePath(sizeof(squigglePoints)/sizeof(BGL::Point), squigglePoints);
    BGL::Path rectPath(sizeof(rectPoints)/sizeof(BGL::Point), rectPoints);

    fstream fout;

    fout.open("output/test-001a-path-orig.svg", fstream::out | fstream::trunc);
    if (fout.good()) {
	svgHeader(fout, 100, 100);

	fout << "<g stroke=\"#77f\" stroke-width=\"3\">" << endl;
	squigglePath.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	fout << "<g stroke=\"#0c0\">" << endl;
	rectPath.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	svgFooter(fout);
	fout.sync();
	fout.close();
    }

    fout.open("output/test-001b-path-union.svg", fstream::out | fstream::trunc);
    if (fout.good()) {
	svgHeader(fout, 100, 100);

	BGL::Paths outPaths;
	BGL::Paths::iterator pit;
	BGL::Path::unionOf(squigglePath, rectPath, outPaths);
	for (pit = outPaths.begin(); pit != outPaths.end(); pit++) {
	    pit->svgPathWithOffset(fout, 10, 10);
	}

	svgFooter(fout);
	fout.sync();
	fout.close();
    }

    fout.open("output/test-001c-path-diff.svg", fstream::out | fstream::trunc);
    if (fout.good()) {
	svgHeader(fout, 100, 100);

	BGL::Paths outPaths;
	BGL::Paths::iterator pit;
	BGL::Path::differenceOf(squigglePath, rectPath, outPaths);
	for (pit = outPaths.begin(); pit != outPaths.end(); pit++) {
	    pit->svgPathWithOffset(fout, 10, 10);
	}

	svgFooter(fout);
	fout.sync();
	fout.close();
    }

    fout.open("output/test-001d-path-intsect.svg", fstream::out | fstream::trunc);
    if (fout.good()) {
	svgHeader(fout, 100, 100);

	BGL::Paths outPaths;
	BGL::Paths::iterator pit;
	BGL::Path::intersectionOf(squigglePath, rectPath, outPaths);
	for (pit = outPaths.begin(); pit != outPaths.end(); pit++) {
	    pit->svgPathWithOffset(fout, 10, 10);
	}

	svgFooter(fout);
	fout.sync();
	fout.close();
    }

    return 0;
}


