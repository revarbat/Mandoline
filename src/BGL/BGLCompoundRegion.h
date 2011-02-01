//
//  BGLCompoundRegion.h
//  Part of the Belfry Geometry Library
//
//  Created by GM on 10/14/10.
//  Copyright 2010 Belfry Software. All rights reserved.
//


#ifndef BGL_REGION_H
#define BGL_REGION_H

#include "config.h"
#include "BGLSimpleRegion.h"
#include "BGLPath.h"
#include "BGLLine.h"

namespace BGL {


class CompoundRegion {
public:
    SimpleRegions subregions;
    float zLevel;

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
    CompoundRegion& operator*=(float rhs);
    CompoundRegion& operator*=(const Point &rhs);
    CompoundRegion& operator/=(float rhs);
    CompoundRegion& operator/=(const Point &rhs);

    // Binary arithmetic operators
    const CompoundRegion operator+(const Point &rhs) const {
	return CompoundRegion(*this) += rhs;
    }
    const CompoundRegion operator-(const Point &rhs) const {
	return CompoundRegion(*this) -= rhs;
    }
    const CompoundRegion operator*(float rhs) const {
	return CompoundRegion(*this) *= rhs;
    }
    const CompoundRegion operator*(const Point &rhs) const {
	return CompoundRegion(*this) *= rhs;
    }
    const CompoundRegion operator/(float rhs) const {
	return CompoundRegion(*this) /= rhs;
    }
    const CompoundRegion operator/(const Point &rhs) const {
	return CompoundRegion(*this) /= rhs;
    }

    int32_t size() const;
    bool contains(const Point &pt) const;

    string svgPathWithOffset(float dx, float dy);
    ostream &svgPathDataWithOffset(ostream& os, float dx, float dy) const;
    ostream &svgPathWithOffset(ostream& os, float dx, float dy) const;

    void simplify(float minErr);

    CompoundRegion &unionWith(SimpleRegion &reg);
    CompoundRegion &differenceWith(SimpleRegion &reg);
    CompoundRegion &intersectionWith(SimpleRegion &reg);

    CompoundRegion &unionWith(CompoundRegion &reg);
    CompoundRegion &differenceWith(CompoundRegion &reg);
    CompoundRegion &intersectionWith(CompoundRegion &reg);

    CompoundRegion &insetRegion(float insetBy, CompoundRegion &outReg);

    static CompoundRegion &differenceOf  (CompoundRegion &r1, CompoundRegion &r2, CompoundRegion &outReg);
    static CompoundRegion &unionOf       (CompoundRegion &r1, CompoundRegion &r2, CompoundRegion &outReg);
    static CompoundRegion &intersectionOf(CompoundRegion &r1, CompoundRegion &r2, CompoundRegion &outReg);

    static CompoundRegion &assembleCompoundRegionFrom(Paths &paths, CompoundRegion &outReg);

    Lines &containedSegmentsOfLine(Line &line, Lines &lnsref);
    Paths &containedSubpathsOfPath(Path &path, Paths &pathsref);

    Paths &infillPathsForRegionWithDensity(float density, float extrusionWidth, Paths &outPaths);

};
typedef list<CompoundRegion> CompoundRegions;


}

#endif

