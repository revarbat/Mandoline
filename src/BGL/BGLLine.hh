//
//  BGLLine.hh
//  Part of the Belfry Geometry Library
//
//  Created by GM on 10/13/10.
//  Copyright 2010 Belfry Software. All rights reserved.
//


#ifndef BGL_LINE_H
#define BGL_LINE_H

#include <math.h>
#include <iostream>
#include <list>
#include "config.h"
#include "BGLCommon.hh"
#include "BGLAffine.hh"
#include "BGLIntersection.hh"
#include "BGLPoint.hh"

using namespace std;

namespace BGL {


class Line {
public:
    // Member variables
    Point startPt;
    Point endPt;
    int flags;
    double temperature;
    double extrusionWidth;

    // Constructors
    Line() :
        startPt(),
        endPt(),
        flags(0),
        temperature(0),
        extrusionWidth(0)
    {
    }

    Line(const Point& p1, const Point& p2) :
        startPt(p1),
        endPt(p2),
        flags(0),
        temperature(0),
        extrusionWidth(0)
    {
    }

    Line(const Line& ln) :
        startPt(ln.startPt),
        endPt(ln.endPt),
        flags(ln.flags),
        temperature(ln.temperature),
        extrusionWidth(ln.extrusionWidth)
    {
    }

    // Assignment operator
    Line& operator=(const Line &rhs);

    // Compound assignment operators
    Line& operator+=(const Point &rhs);
    Line& operator-=(const Point &rhs);
    Line& operator*=(double rhs);
    Line& operator*=(const Point &rhs);
    Line& operator/=(double rhs);
    Line& operator/=(const Point &rhs);

    // Binary arithmetic operators
    const Line operator+(const Point &rhs) const;
    const Line operator-(const Point &rhs) const;
    const Line operator*(double rhs) const;
    const Line operator*(const Point &rhs) const;
    const Line operator/(double rhs) const;
    const Line operator/(const Point &rhs) const;

    // Comparison operators
    bool operator==(const Line &rhs) const;
    bool operator!=(const Line &rhs) const;

    bool hasEndPoint(const Point& pt) const;

    // Transformations
    Line& scale(double scale);
    Line& scale(const Point& vect);
    Line& scaleAroundPoint(const Point& center, double scale);
    Line& scaleAroundPoint(const Point& center, const Point& vect);

    Line& rotate(double angle);

    void quantize(double quanta);
    void quantize();

    // Calculations
    double length() const;
    double angle() const;
    double angleDelta(const Line& ln) const;
    bool isLeftOf(const Point &pt) const;
    bool isLeftOfNormal(const Point &pt) const;

    // Misc
    Line& reverse();
    Line& reverseIfRightOfNormal(const Point &pt);

    bool isLinearWith(const Point& pt) const;
    bool hasInBounds(const Point &pt) const;
    bool contains(const Point &pt) const;
    Point closestSegmentPointTo(const Point &pt) const;
    Point closestExtendedLinePointTo(const Point &pt) const;
    double minimumSegmentDistanceFromPoint(const Point &pt) const;
    double minimumExtendedLineDistanceFromPoint(const Point &pt) const;
    Intersection intersectionWithSegment(const Line &ln) const;
    Intersection intersectionWithExtendedLine(const Line &ln) const;

    Line& leftOffset(double offsetby);

    // Friend functions
    friend ostream& operator <<(ostream &os,const Line &pt);
};

typedef list<Line> Lines;



}


#endif
// vim: set ts=4 sw=4 nowrap expandtab: settings

