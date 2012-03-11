//
//  BGLTriangle3D.hh
//  Part of the Belfry Geometry Library
//
//  Created by GM on 10/14/10.
//  Copyright 2010 Belfry Software. All rights reserved.
//

#ifndef BGL_TRIANGLE3D_H
#define BGL_TRIANGLE3D_H

#include <list>
#include "BGLPoint3d.hh"
#include "BGLLine.hh"

namespace BGL {

class Triangle3d {
public:
    Point3d vertex1, vertex2, vertex3;

    // Constructors
    Triangle3d() : vertex1(), vertex2(), vertex3() {}
    Triangle3d(const Point3d &p1, const Point3d &p2, const Point3d &p3) : vertex1(p1), vertex2(p2), vertex3(p3) {}

    // Assignment operator
    Triangle3d& operator=(const Triangle3d &rhs) {
        if (this != &rhs) {
            vertex1 = rhs.vertex1;
            vertex2 = rhs.vertex2;
            vertex3 = rhs.vertex3;
        }
        return *this;
    }

    // Compound assignment operators
    Triangle3d& operator+=(const Point3d &rhs) {
        vertex1 += rhs;
        vertex2 += rhs;
        vertex3 += rhs;
        return *this;
    }
    Triangle3d& operator-=(const Point3d &rhs) {
        vertex1 -= rhs;
        vertex2 -= rhs;
        vertex3 -= rhs;
        return *this;
    }
    Triangle3d& operator*=(double rhs) {
        vertex1 *= rhs;
        vertex2 *= rhs;
        vertex3 *= rhs;
        return *this;
    }
    Triangle3d& operator*=(const Point3d &rhs) {
        vertex1 *= rhs;
        vertex2 *= rhs;
        vertex3 *= rhs;
        return *this;
    }
    Triangle3d& operator/=(double rhs) {
        vertex1 /= rhs;
        vertex2 /= rhs;
        vertex3 /= rhs;
        return *this;
    }
    Triangle3d& operator/=(const Point3d &rhs) {
        vertex1 /= rhs;
        vertex2 /= rhs;
        vertex3 /= rhs;
        return *this;
    }

    // Binary arithmetic operators
    const Triangle3d operator+(const Point3d &rhs) const {
        return Triangle3d(*this) += rhs;
    }
    const Triangle3d operator-(const Point3d &rhs) const {
        return Triangle3d(*this) -= rhs;
    }
    const Triangle3d operator*(double rhs) const {
        return Triangle3d(*this) *= rhs;
    }
    const Triangle3d operator*(const Point3d &rhs) const {
        return Triangle3d(*this) *= rhs;
    }
    const Triangle3d operator/(double rhs) const {
        return Triangle3d(*this) /= rhs;
    }
    const Triangle3d operator/(const Point3d &rhs) const {
        return Triangle3d(*this) /= rhs;
    }

    // Comparison operators
    bool hasVertex(const Point& pt) const {
        return (pt == vertex1 || pt == vertex2 || pt == vertex3);
    }
    bool operator==(const Triangle3d &rhs) const {
        return (hasVertex(rhs.vertex1) && hasVertex(rhs.vertex2) && hasVertex(rhs.vertex3));
    }
    bool operator!=(const Triangle3d &rhs) const {
        return !(*this == rhs);
    }

    // Comparators for z height double.
    bool operator< (double zcoord) const {
        return (vertex1.z < zcoord && vertex2.z < zcoord && vertex3.z < zcoord);
    }
    bool operator> (double zcoord) const {
        return (vertex1.z > zcoord && vertex2.z > zcoord && vertex3.z > zcoord);
    }
    bool operator<= (double zcoord) const {
        return !(*this > zcoord);
    }
    bool operator>= (double zcoord) const {
        return !(*this < zcoord);
    }

    // Transformations
    Triangle3d& translate(double dx, double dy, double dz) {
        *this += Point3d(dx,dy,dz);
        return *this;
    }
    Triangle3d& scale(double scale) {
        *this *= scale;
        return *this;
    }
    Triangle3d& scale(const Point3d& vect) {
        *this *= vect;
        return *this;
    }
    Triangle3d& scaleAroundPoint(const Point3d& center, double scale) {
        *this -= center;
        *this *= scale;
        *this += center;
        return *this;
    }
    Triangle3d& scaleAroundPoint(const Point3d& center, const Point3d& vect) {
        *this -= center;
        *this *= vect;
        *this += center;
        return *this;
    }
    Triangle3d& rotateX(const Point3d& center, double rad);
    Triangle3d& rotateY(const Point3d& center, double rad);
    Triangle3d& rotateZ(const Point3d& center, double rad);


    // Misc
    bool sliceAtZ(double Z, Line& lnref) const;
};


typedef list<Triangle3d> Triangles3d;

}


#endif
// vim: set ts=4 sw=4 nowrap expandtab: settings

