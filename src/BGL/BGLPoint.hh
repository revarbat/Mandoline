//
//  BGLPoint.hh
//  Part of the Belfry Geometry Library
//
//  Created by GM on 10/13/10.
//  Copyright 2010 Belfry Software. All rights reserved.
//

#ifndef BGL_POINT_H
#define BGL_POINT_H

#include <math.h>
#include <iostream>
#include <list>
#include "config.h"
#include "BGLCommon.hh"
#include "BGLAffine.hh"
#include "BGLPoint3d.hh"

using namespace std;

namespace BGL {


class Point {
public:
    // Member variables
    double x, y;

    // Constructors
    Point() : x(0.0), y(0.0) {}
    Point(double nux, double nuy) : x(nux), y(nuy) {}
    Point(const Point &pt) : x(pt.x), y(pt.y) {}
    Point(const Point3d &pt) : x(pt.x), y(pt.y) {}

    // Assignment operator
    Point& operator=(const Point &rhs);

    // Compound assignment operators
    Point& operator+=(const Point &rhs);
    Point& operator-=(const Point &rhs);
    Point& operator*=(double rhs);
    Point& operator*=(const Point &rhs);
    Point& operator/=(double rhs);
    Point& operator/=(const Point &rhs);

    // Binary arithmetic operators
    const Point operator+(const Point &rhs) const;
    const Point operator-(const Point &rhs) const;
    const Point operator*(double rhs) const;
    const Point operator*(const Point &rhs) const;
    const Point operator/(double rhs) const;
    const Point operator/(const Point &rhs) const;

    // Comparison operators
    bool operator==(const Point &rhs) const;
    bool operator>=(const Point &rhs) const;
    bool operator<=(const Point &rhs) const;
    bool operator>(const Point &rhs) const;
    bool operator<(const Point &rhs) const;
    bool operator!=(const Point &rhs) const;

    // Transformations
    Point& scale(double scale);
    Point& scale(const Point& vect);
    Point& scaleAroundPoint(const Point& center, double scale);
    Point& scaleAroundPoint(const Point& center, const Point& vect);

    Point& rotate(double angle);

    void quantize(double quanta);
    void quantize();

    // Calculations
    double distanceFrom(const Point& pt) const;
    double angleToPoint(const Point& pt) const;

    Point &polarOffset(double ang, double rad);

    // Friend functions
    friend ostream& operator <<(ostream &os,const Point &pt);
};

typedef list<Point> Points;



}

#endif

