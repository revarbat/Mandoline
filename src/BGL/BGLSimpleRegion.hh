//
//  BGLSimpleRegion.hh
//  Part of the Belfry Geometry Library
//
//  Created by GM on 10/14/10.
//  Copyright 2010 Belfry Software. All rights reserved.
//


#ifndef BGL_SIMPLEREGION_H
#define BGL_SIMPLEREGION_H

#include "config.h"
#include "BGLPath.hh"
#include "BGLLine.hh"

namespace BGL {

typedef enum {
    INFILL_NONE,
    INFILL_LINES,
    INFILL_RECTANGULAR,
    INFILL_HEXAGONAL
} InfillStyle;


class SimpleRegion;
typedef list<SimpleRegion> SimpleRegions;

class CompoundRegion;

class SimpleRegion {
public:
    Path outerPath;
    Paths subpaths;
    double zLevel;

    SimpleRegion() : outerPath(), subpaths(), zLevel(0.0f) {}
    SimpleRegion(const Path &x) : outerPath(x), subpaths(), zLevel(0.0f) {}
    SimpleRegion(const SimpleRegion &x) : outerPath(x.outerPath), subpaths(x.subpaths), zLevel(x.zLevel) {}

    // Compound assignment operators
    SimpleRegion& operator+=(const Point &rhs);
    SimpleRegion& operator-=(const Point &rhs);
    SimpleRegion& operator*=(double rhs);
    SimpleRegion& operator*=(const Point &rhs);
    SimpleRegion& operator/=(double rhs);
    SimpleRegion& operator/=(const Point &rhs);

    // Binary arithmetic operators
    const SimpleRegion operator+(const Point &rhs) const {
	return SimpleRegion(*this) += rhs;
    }
    const SimpleRegion operator-(const Point &rhs) const {
	return SimpleRegion(*this) -= rhs;
    }
    const SimpleRegion operator*(double rhs) const {
	return SimpleRegion(*this) *= rhs;
    }
    const SimpleRegion operator*(const Point &rhs) const {
	return SimpleRegion(*this) *= rhs;
    }
    const SimpleRegion operator/(double rhs) const {
	return SimpleRegion(*this) /= rhs;
    }
    const SimpleRegion operator/(const Point &rhs) const {
	return SimpleRegion(*this) /= rhs;
    }

    int size();

    double perimeterLength() const;

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

    static SimpleRegions &assembleSimpleRegionsFrom(Paths &paths, SimpleRegions &outRegs);
    static SimpleRegions &assembleSimpleRegionsFrom(const Paths &outerPaths, const Paths &innerPaths, SimpleRegions &outRegs);

    static SimpleRegions &differenceOf  (SimpleRegion &reg, Path &path, SimpleRegions &outRegs);

    static SimpleRegions& unionOf       (SimpleRegion &r1, SimpleRegion &r2, SimpleRegions &outRegs);
    static SimpleRegions& differenceOf  (SimpleRegion &r1, SimpleRegion &r2, SimpleRegions &outRegs);
    static SimpleRegions& intersectionOf(SimpleRegion &r1, SimpleRegion &r2, SimpleRegions &outRegs);

    SimpleRegions &inset(double offsetby, SimpleRegions& outRegs);

    Path &pathAround(Point p1, Point p2, Path &outPath) const;
    Lines &containedSegmentsOfLine(Line &line, Lines &lnsref);
    Paths &containedSubpathsOfPath(const Path &path, Paths &pathsref);

    Paths &joinSubPathsInside(double maxDist, const Paths &inPaths, Paths &outPaths);
    Paths &infillPathsForRegionWithDensity(double angle, InfillStyle style, double density, double extrusionWidth, CompoundRegion &solidMask, Paths &outPaths);
};


}

#endif

