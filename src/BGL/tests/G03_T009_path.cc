#include <fstream>
#include <BGL.hh>
#include "testbed.h"



// Rectangle
BGL::Point pointSet1[] = {
    BGL::Point(-8.590003524884,  8.590000000000),
    BGL::Point(-8.590046474991, -8.590000000000),
    BGL::Point( 8.590000000000, -8.590000000000),
    BGL::Point( 8.590000000000,  8.590000000000),
    BGL::Point(-8.590003524884,  8.590000000000)
};

// Smaller Rectangle
BGL::Point pointSet2[] = {
    BGL::Point(-9.999997000000, 10.000000000000),
    BGL::Point(-9.849997000000, 10.000000000000),
    BGL::Point(-9.849997000000,  8.590000000000),
    BGL::Point(-9.999997000000,  8.590000000000),
    BGL::Point(-9.999997000000, 10.000000000000)
};



int main(int argc, char**argv)
{
    char fname[128];

    BGL::Path path1(sizeof(pointSet1)/sizeof(BGL::Point), pointSet1);
    BGL::Path path2(sizeof(pointSet2)/sizeof(BGL::Point), pointSet2);

    fstream fout;
    BGL::SVG svg(250, 250);

    synthesize_testfile_name(fname, sizeof(fname), argv[0], "diff", "svg");
    fout.open(fname, fstream::out | fstream::trunc);
    if (fout.good()) {
	svg.header(fout);

	BGL::Paths outPaths;
	BGL::Path::differenceOf(path1, path2, outPaths);
	BGL::Paths::iterator it;

	fout << "<g stroke=\"#77f\">" << endl;
	path1 *= 3.0;
	path1.svgPathWithOffset(fout, 30, 30);
	fout << "</g>" << endl;

	fout << "<g stroke=\"#f77\">" << endl;
	path2 *= 3.0;
	path2.svgPathWithOffset(fout, 30, 30);
	fout << "</g>" << endl;

	for (it = outPaths.begin(); it != outPaths.end(); it++) {
	    *it *= 3.0;
	    it->svgPathWithOffset(fout, 30, 30);
	}

	svg.footer(fout);
	fout.sync();
	fout.close();
    }

    return 0;
}


