//
//  BGLCompoundRegion.hh
//  Part of the Belfry Geometry Library
//
//  Created by GM on 10/14/10.
//  Copyright 2010 Belfry Software. All rights reserved.
//


#ifndef BGL_REGION_H
#define BGL_REGION_H

#include "config.h"
#include "BGLSimpleRegion.hh"
#include "BGLPath.hh"
#include "BGLLine.hh"

namespace BGL {


class CompoundRegion {
public:
    SimpleRegions subregions;
    double zLevel;

    CompoundRegion() : subregions(), zLevel(0.0f) {}
    CompoundRegion(const SimpleRegions &x) : subregions(x), zLevel(0.0f) {}
    CompoundRegion(const CompoundRegion &x) : subregions(x.subregions), zLevel(x.zLevel) {}
    CompoundRegion(const Paths &x) : subregions(), zLevel(0.0f) {
        Paths::const_iterator it;
	for (it = x.begin(); it != x.end(); it++) {
	    subregions.push_back(SimpleRegion(*it));
	}
    }

    // Compound assignment operators
    CompoundRegion& operator+=(const Point &rhs);
    CompoundRegion& operator-=(const Point &rhs);
    CompoundRegion& operator*=(double rhs);
    CompoundRegion& operator*=(const Point &rhs);
    CompoundRegion& operator/=(double rhs);
    CompoundRegion& operator/=(const Point &rhs);

    // Binary arithmetic operators
    const CompoundRegion operator+(const Point &rhs) const {
	return CompoundRegion(*this) += rhs;
    }
    const CompoundRegion operator-(const Point &rhs) const {
	return CompoundRegion(*this) -= rhs;
    }
    const CompoundRegion operator*(double rhs) const {
	return CompoundRegion(*this) *= rhs;
    }
    const CompoundRegion operator*(const Point &rhs) const {
	return CompoundRegion(*this) *= rhs;
    }
    const CompoundRegion operator/(double rhs) const {
	return CompoundRegion(*this) /= rhs;
    }
    const CompoundRegion operator/(const Point &rhs) const {
	return CompoundRegion(*this) /= rhs;
    }

    int size() const;
    bool contains(const Point &pt) const;

    bool intersectsPerimeter(const Line& ln) const;

    bool intersects(const Line& ln) const;
    bool intersects(const Path& path) const;
    bool intersects(const SimpleRegion& reg) const;
    bool intersects(const CompoundRegion& reg) const;

    string svgPathWithOffset(double dx, double dy);
    ostream &svgPathDataWithOffset(ostream& os, double dx, double dy) const;
    ostream &svgPathWithOffset(ostream& os, double dx, double dy) const;

    void simplify(double minErr);
    void setWidth(double extWidth);

    CompoundRegion &unionWith(SimpleRegion &reg);
    CompoundRegion &differenceWith(SimpleRegion &reg);
    CompoundRegion &intersectionWith(SimpleRegion &reg);

    CompoundRegion &unionWith(CompoundRegion &reg);
    CompoundRegion &differenceWith(CompoundRegion &reg);
    CompoundRegion &intersectionWith(CompoundRegion &reg);

    CompoundRegion &inset(double insetBy, CompoundRegion &outReg);

    static CompoundRegion &differenceOf  (CompoundRegion &r1, CompoundRegion &r2, CompoundRegion &outReg);
    static CompoundRegion &unionOf       (CompoundRegion &r1, CompoundRegion &r2, CompoundRegion &outReg);
    static CompoundRegion &intersectionOf(CompoundRegion &r1, CompoundRegion &r2, CompoundRegion &outReg);

    static CompoundRegion &assembleCompoundRegionFrom(Paths &paths, CompoundRegion &outReg);

    Lines &containedSegmentsOfLine(Line &line, Lines &lnsref);
    Paths &containedSubpathsOfPath(Path &path, Paths &pathsref);

    Paths &joinSubPathsInside(double maxDist, const Paths &inPaths, Paths &outPaths);
    Paths &infillPathsForRegionWithDensity(double angle, InfillStyle style, double density, double extrusionWidth, CompoundRegion &solidMask, Paths &outPaths);

};
typedef list<CompoundRegion> CompoundRegions;


}

#endif

