//
//  BGLAffine.h
//  Part of the Belfry Geometry Library
//
//  Created by GM on 10/13/10.
//  Copyright 2010 Belfry Software. All rights reserved.
//


#ifndef BGL_MATRIX_H
#define BGL_MATRIX_H

#include <math.h>
#include "config.h"

namespace BGL {


class Affine {
public:
    double a, b, c, d, tx, ty;

    // Constructors
    Affine() : a(1.0), b(0.0), c(0.0), d(1.0), tx(0.0), ty(0.0)  {}
    Affine(double A, double B, double C, double D, double TX, double TY) : a(A), b(B), c(C), d(D), tx(TX), ty(TY) {}
    Affine(const Affine& x) : a(x.a), b(x.b), c(x.c), d(x.d), tx(x.tx), ty(x.ty) {}

    static Affine translationAffine(double dx, double dy);
    static Affine scalingAffine(double sx, double sy);
    static Affine rotationAffine(double radang);

    Affine& transform(const Affine& aff);
    Affine& translate(double dx, double dy);
    Affine& scale(double sx, double sy);
    Affine& scaleAroundPoint(double sx, double sy, double x, double y);
    Affine& rotate(double radang);
    Affine& rotateAroundPoint(double radang, double x, double y);

    void transformPoint(double& x, double &y) const;
};



}

#endif

