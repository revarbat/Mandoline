//
//  BGLCompoundRegion.m
//  Part of the Belfry Geometry Library
//
//  Created by GM on 10/14/10.
//  Copyright 2010 Belfry Software. All rights reserved.
//

#include <sstream>
#include <fstream>
#include "BGLCommon.h"
#include "BGLPoint.h"
#include "BGLCompoundRegion.h"



namespace BGL {


// Compound assignment operators
CompoundRegion& CompoundRegion::operator+=(const Point &rhs) {
    SimpleRegions::iterator it;
    for (it = subregions.begin(); it != subregions.end(); it++) {
	*it += rhs;
    }
    return *this;
}



CompoundRegion& CompoundRegion::operator-=(const Point &rhs) {
    SimpleRegions::iterator it;
    for (it = subregions.begin(); it != subregions.end(); it++) {
	*it -= rhs;
    }
    return *this;
}



CompoundRegion& CompoundRegion::operator*=(double rhs) {
    SimpleRegions::iterator it;
    for (it = subregions.begin(); it != subregions.end(); it++) {
	*it *= rhs;
    }
    return *this;
}



CompoundRegion& CompoundRegion::operator*=(const Point &rhs) {
    SimpleRegions::iterator it;
    for (it = subregions.begin(); it != subregions.end(); it++) {
	*it *= rhs;
    }
    return *this;
}



CompoundRegion& CompoundRegion::operator/=(double rhs) {
    SimpleRegions::iterator it;
    for (it = subregions.begin(); it != subregions.end(); it++) {
	*it /= rhs;
    }
    return *this;
}



CompoundRegion& CompoundRegion::operator/=(const Point &rhs) {
    SimpleRegions::iterator it;
    for (it = subregions.begin(); it != subregions.end(); it++) {
	*it /= rhs;
    }
    return *this;
}





int32_t CompoundRegion::size() const
{
    return subregions.size();
}



bool CompoundRegion::contains(const Point &pt) const
{
    SimpleRegions::const_iterator it;
    for (it = subregions.begin(); it != subregions.end(); it++) {
        if (it->contains(pt)) {
	    return true;
	}
    }
    return false;
}



void CompoundRegion::simplify(double minErr)
{
    SimpleRegions::iterator it;
    for (it = subregions.begin(); it != subregions.end(); it++) {
	it->simplify(minErr);
    }
}



CompoundRegion &CompoundRegion::assembleCompoundRegionFrom(Paths &paths, CompoundRegion &outReg)
{
    SimpleRegion::assembleSimpleRegionsFrom(paths, outReg.subregions);
    return outReg;
}



string CompoundRegion::svgPathWithOffset(double dx, double dy)
{
    string out;
    SimpleRegions::iterator rit;
    for (rit = subregions.begin(); rit != subregions.end(); rit++) {
        out.append(rit->svgPathWithOffset(dx, dy));
    }
    return out;
}



ostream &CompoundRegion::svgPathDataWithOffset(ostream& os, double dx, double dy) const
{
    SimpleRegions::const_iterator rit;
    for (rit = subregions.begin(); rit != subregions.end(); rit++) {
        rit->svgPathDataWithOffset(os, dx, dy);
    }
    return os;
}



ostream &CompoundRegion::svgPathWithOffset(ostream& os, double dx, double dy) const
{
    os << "<path fill=\"none\" d=\"";
    svgPathDataWithOffset(os, dx, dy);
    os << "\" />" << endl;
    return os;
}



CompoundRegion &CompoundRegion::unionWith(SimpleRegion &reg)
{
    SimpleRegion currReg(reg);
    SimpleRegions::iterator rit;
    for (rit = subregions.begin(); rit != subregions.end(); ) {
        if (currReg.intersects(*rit)) {
	    SimpleRegions tempRegs;
	    SimpleRegion::unionOf(currReg, *rit, tempRegs);
	    currReg = tempRegs.front();
	    rit = subregions.erase(rit);
	} else {
	    rit++;
	}
    }
    subregions.push_back(currReg);
    return *this;
}



CompoundRegion &CompoundRegion::differenceWith(SimpleRegion &reg)
{
    SimpleRegions::iterator rit;
    SimpleRegions::iterator rit2;
    for (rit = subregions.begin(); rit != subregions.end(); ) {
        if (reg.intersects(*rit)) {
	    SimpleRegions tempRegs;
	    SimpleRegion::differenceOf(*rit, reg, tempRegs);
	    rit = subregions.erase(rit);
	    for (rit2 = tempRegs.begin(); rit2 != tempRegs.end(); rit2++) {
	        subregions.push_front(*rit2);
	    }
	} else {
	    rit++;
	}
    }
    return *this;
}



CompoundRegion &CompoundRegion::intersectionWith(SimpleRegion &reg)
{
    // TODO: implement.
    return *this;
}



CompoundRegion &CompoundRegion::unionWith(CompoundRegion &reg)
{
    SimpleRegions::iterator rit;
    for (rit = reg.subregions.begin(); rit != reg.subregions.end(); rit++) {
        unionWith(*rit);
    }
    return *this;
}



CompoundRegion &CompoundRegion::differenceWith(CompoundRegion &reg)
{
    SimpleRegions::iterator rit;
    for (rit = reg.subregions.begin(); rit != reg.subregions.end(); rit++) {
        differenceWith(*rit);
    }
    return *this;
}



CompoundRegion &CompoundRegion::intersectionWith(CompoundRegion &reg)
{
    // TODO: implement.
    return *this;
}



CompoundRegion &CompoundRegion::unionOf(CompoundRegion &r1, CompoundRegion &r2, CompoundRegion &outReg)
{
    outReg = r1;
    outReg.unionWith(r2);
    return outReg;
}



CompoundRegion &CompoundRegion::differenceOf(CompoundRegion &r1, CompoundRegion &r2, CompoundRegion &outReg)
{
    outReg = r1;
    outReg.differenceWith(r2);
    return outReg;
}



CompoundRegion &CompoundRegion::intersectionOf(CompoundRegion &r1, CompoundRegion &r2, CompoundRegion &outReg)
{
    outReg = r1;
    outReg.intersectionWith(r2);
    return outReg;
}




ostream &ssvgHeader(ostream &os, double width, double height)
{
    width *= 2;
    height *=2;
    double pwidth  = width * 90.0f / 25.4f;
    double pheight = height * 90.0f / 25.4f;

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



ostream &ssvgFooter(ostream& os)
{
    os << "</g>" << endl;
    os << "</svg>" << endl;
    return os;
}



CompoundRegion &CompoundRegion::insetRegion(double insetBy, CompoundRegion &outReg)
{
    // This is a HACK.  But I got frustrated trying to handle proper insets.
    char buf[128];
    const int angles = 16;
    CompoundRegion circumReg;
    for (double ang = 0.0f; ang < M_PI*2.0f; ang += M_PI*2.0f/angles) {
        double dx = insetBy * cos(ang);
        double dy = insetBy * sin(ang);
	CompoundRegion offReg(*this);
	CompoundRegion tempReg(*this);
	offReg += Point(dx,dy);
	tempReg.differenceWith(offReg);
	circumReg.unionWith(tempReg);

	fstream fout;
	snprintf(buf, sizeof(buf), "output/temp-%.2f.svg", ang);
	fout.open(buf, fstream::out | fstream::trunc);
	if (fout.good()) {
	    ssvgHeader(fout, 100, 100);
	    tempReg.svgPathWithOffset(fout, 10, 10);
	    ssvgFooter(fout);
	    fout.sync();
	    fout.close();
	}
	snprintf(buf, sizeof(buf), "output/circum-%.2f.svg", ang);
	fout.open(buf, fstream::out | fstream::trunc);
	if (fout.good()) {
	    ssvgHeader(fout, 100, 100);
	    circumReg.svgPathWithOffset(fout, 10, 10);
	    ssvgFooter(fout);
	    fout.sync();
	    fout.close();
	}
    }
    outReg = *this;
    outReg.differenceWith(circumReg);
    return outReg;
}



Lines &CompoundRegion::containedSegmentsOfLine(Line &line, Lines &outSegs)
{
    SimpleRegions::iterator rit;
    for (rit = subregions.begin(); rit != subregions.end(); rit++) {
        rit->containedSegmentsOfLine(line, outSegs);
    }
    return outSegs;
}



Paths &CompoundRegion::containedSubpathsOfPath(Path &path, Paths &outPaths)
{
    SimpleRegions::iterator rit;
    for (rit = subregions.begin(); rit != subregions.end(); rit++) {
        rit->containedSubpathsOfPath(path, outPaths);
    }
    return outPaths;
}



Paths &CompoundRegion::infillPathsForRegionWithDensity(double density, double extrusionWidth, Paths &outPaths)
{
    SimpleRegions::iterator rit;
    for (rit = subregions.begin(); rit != subregions.end(); rit++) {
        rit->infillPathsForRegionWithDensity(density, extrusionWidth, outPaths);
    }
    return outPaths;
}



}

