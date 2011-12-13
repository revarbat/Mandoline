#include <fstream>
#include <BGL.hh>
#include "testbed.h"


BGL::Point pointSet1[] =
{
    BGL::Point(56.912, 64.926),
    BGL::Point(56.887, 20.077),
    BGL::Point(42.089, 20.000),
    BGL::Point(26.464, 35.746),
    BGL::Point(28.931, 38.583),
    BGL::Point(32.017, 41.454),
    BGL::Point(41.566, 31.539),
    BGL::Point(46.168, 30.014),
    BGL::Point(50.028, 35.043),
    BGL::Point(50.044, 52.040),
    BGL::Point(41.570, 70.627),
    BGL::Point(20.000, 92.006),
    BGL::Point(24.813, 97.095),
    BGL::Point(56.912, 64.926)
};



BGL::Point pointSet2[] =
{
    BGL::Point(56.912, 64.926),
    BGL::Point(56.887, 20.077),
    BGL::Point(56.572, 20.077),
    BGL::Point(56.597, 64.926),
    BGL::Point(56.912, 64.926)
};



BGL::Point pointSet3[] =
{
    BGL::Point(56.887, 20.077),
    BGL::Point(42.089, 20.000),
    BGL::Point(42.087, 20.315),
    BGL::Point(56.885, 20.392),
    BGL::Point(56.887, 20.077)
};




int main(int argc, char**argv)
{
    char fname[128];

    BGL::Path path(sizeof(pointSet1)/sizeof(BGL::Point), pointSet1);
    BGL::Path dpath1(sizeof(pointSet2)/sizeof(BGL::Point), pointSet2);
    BGL::Path dpath2(sizeof(pointSet3)/sizeof(BGL::Point), pointSet3);

    fstream fout;
    BGL::SVG svg(250, 250);
    BGL::Paths::iterator pit;

    /*
    synthesize_testfile_name(fname, sizeof(fname), argv[0], "inset", "svg");
    fout.open(fname, fstream::out | fstream::trunc);
    if (fout.good()) {
	svg.header(fout);

	fout << "<g stroke=\"#77f\">" << endl;
	path.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	BGL::Paths outPaths;
	path.inset(0.63/2.0, outPaths);
	for (pit = outPaths.begin(); pit != outPaths.end(); pit++) {
	    pit->svgPathWithOffset(fout, 10, 10);
	}

	svg.footer(fout);
	fout.sync();
	fout.close();
    }
    */

    synthesize_testfile_name(fname, sizeof(fname), argv[0], "diff1", "svg");
    fout.open(fname, fstream::out | fstream::trunc);
    if (fout.good()) {
	svg.header(fout);

	fout << "<g stroke=\"#77f\">" << endl;
	path.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	fout << "<g stroke=\"#7f7\">" << endl;
	dpath1.svgPathWithOffset(fout, 10, 10);
	fout << "</g>" << endl;

	BGL::Paths outPaths;
	BGL::Path::differenceOf(path, dpath1, outPaths);
	for (pit = outPaths.begin(); pit != outPaths.end(); pit++) {
	    pit->svgPathWithOffset(fout, 10, 10);
	}

	svg.footer(fout);
	fout.sync();
	fout.close();
    }

    return 0;
}


