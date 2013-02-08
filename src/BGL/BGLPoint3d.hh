//
//  BGLPoint3d.hh
//  Part of the Belfry Geometry Library
//
//  Created by GM on 10/13/10.
//  Copyright 2010 Belfry Software. All rights reserved.
//

#ifndef BGL_POINT3D_H
#define BGL_POINT3D_H

#include <math.h>
#include <iostream>
#include "BGLCommon.hh"
using namespace std;


namespace BGL {

class Point3d {
public:
    // Member variables
    double x, y, z;

    // Constructors
    Point3d() : x(0.0), y(0.0), z(0.0) {}
    Point3d(double nux, double nuy, double nuz) : x(nux), y(nuy), z(nuz) {}
    Point3d(const Point3d &pt) : x(pt.x), y(pt.y), z(pt.z) {}

    // Assignment operator
    Point3d& operator=(const Point3d &rhs) {
        if (this != &rhs) {
            this->x = rhs.x;
            this->y = rhs.y;
            this->z = rhs.z;
        }
        return *this;
    }

    // Compound assignment operators
    Point3d& operator+=(const Point3d &rhs) {
        this->x += rhs.x;
        this->y += rhs.y;
        this->z += rhs.z;
        return *this;
    }
    Point3d& operator-=(const Point3d &rhs) {
        this->x -= rhs.x;
        this->y -= rhs.y;
        this->z -= rhs.z;
        return *this;
    }
    Point3d& operator*=(double rhs) {
        this->x *= rhs;
        this->y *= rhs;
        this->z *= rhs;
        return *this;
    }
    Point3d& operator*=(const Point3d &rhs) {
        this->x *= rhs.x;
        this->y *= rhs.y;
        this->z *= rhs.z;
        return *this;
    }
    Point3d& operator/=(double rhs) {
        this->x /= rhs;
        this->y /= rhs;
        this->z /= rhs;
        return *this;
    }
    Point3d& operator/=(const Point3d &rhs) {
        this->x /= rhs.x;
        this->y /= rhs.y;
        this->z /= rhs.z;
        return *this;
    }

    // Binary arithmetic operators
    const Point3d operator+(const Point3d &rhs) const {
        return Point3d(*this) += rhs;
    }
    const Point3d operator-(const Point3d &rhs) const {
        return Point3d(*this) -= rhs;
    }
    const Point3d operator*(double rhs) const {
        return Point3d(*this) *= rhs;
    }
    const Point3d operator*(const Point3d &rhs) const {
        return Point3d(*this) *= rhs;
    }
    const Point3d operator/(double rhs) const {
        return Point3d(*this) /= rhs;
    }
    const Point3d operator/(const Point3d &rhs) const {
        return Point3d(*this) /= rhs;
    }

    // Comparison operators
    bool operator==(const Point3d &rhs) const {
        return (fabs(this->x-rhs.x) + fabs(this->y+rhs.y) + fabs(this->z+rhs.z) < EPSILON);
    }
    bool operator!=(const Point3d &rhs) const {
        return !(*this == rhs);
    }

    // Comparators for z height double.
    bool operator== (double zcoord) const {
        return (fabs(z-zcoord) < EPSILON);
    }
    bool operator< (double zcoord) const {
        return (z < zcoord);
    }
    bool operator> (double zcoord) const {
        return (z > zcoord);
    }
    bool operator<= (double zcoord) const {
        return !(*this > zcoord);
    }
    bool operator>= (double zcoord) const {
        return !(*this < zcoord);
    }


    // Transformations
    Point3d& scale(double scale) {
        *this *= scale;
        return *this;
    }
    Point3d& scale(const Point3d& vect) {
        *this *= vect;
        return *this;
    }
    Point3d& scaleAroundPoint3d(const Point3d& center, double scale) {
        *this -= center;
        *this *= scale;
        *this += center;
        return *this;
    }
    Point3d& scaleAroundPoint3d(const Point3d& center, const Point3d& vect) {
        *this -= center;
        *this *= vect;
        *this += center;
        return *this;
    }

    // Calculations
    double distanceFrom(const Point3d& pt) const {
        Point3d vect(*this);
        vect -= pt;
        vect *= vect;
        return sqrt(vect.x+vect.y+vect.z);
    }

    Point3d crossProduct(const Point3d &v) const {
        Point3d result;
        result.x = (y * v.z) - (z * v.y);
        result.y = (z * v.x) - (x * v.z);
        result.z = (x * v.y) - (y * v.x);
        return result;
    }

    Point3d& normalize() {
        double len = sqrt(x*x+y*y+z*z);
        if (len > 0.0) {
            x /= len;
            y /= len;
            z /= len;
        } else {
            x = y = z = 0.0;
        }
        return *this;
    }

    // Friend functions
    friend ostream& operator <<(ostream &os,const Point3d &pt);
};


}

#endif
// vim: set ts=4 sw=4 nowrap expandtab: settings

