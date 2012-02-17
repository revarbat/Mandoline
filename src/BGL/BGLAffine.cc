//
//  BGLAffine.cc
//  Part of the Belfry Geometry Library
//
//  Created by GM on 10/13/10.
//  Copyright 2010 Belfry Software. All rights reserved.
//


#include "BGLAffine.hh"

namespace BGL {

Affine Affine::translationAffine(double dx, double dy)
{
    return Affine(1.0, 0.0, 0.0, 1.0, dx, dy);
}



Affine Affine::scalingAffine(double sx, double sy)
{
    return Affine(sx, 0.0, 0.0, sy, 0.0, 0.0);
}



Affine Affine::rotationAffine(double radang)
{
    double cosv = cos(radang);
    double sinv = sin(radang);
    return Affine(cosv, -sinv, sinv, cosv, 0.0, 0.0);
}



Affine& Affine::transform(const Affine& aff)
{
    double olda = a;
    double oldb = b;
    double oldc = c;
    double oldd = d;
    a  = aff.a  * olda  +  aff.b  * oldc;
    b  = aff.a  * oldb  +  aff.b  * oldd;
    c  = aff.c  * olda  +  aff.d  * oldc;
    d  = aff.c  * oldb  +  aff.d  * oldd;
    tx += aff.tx * olda  +  aff.ty * oldc;
    ty += aff.tx * oldb  +  aff.ty * oldd;
    return *this;
}



Affine& Affine::translate(double dx, double dy)
{
    return transform(translationAffine(dx,dy));
}



Affine& Affine::scale(double sx, double sy)
{
    return transform(scalingAffine(sx,sy));
}



Affine& Affine::scaleAroundPoint(double sx, double sy, double x, double y)
{
    translate(-x,-y);
    transform(scalingAffine(sx,sy));
    translate(x,y);
    return *this;
}



Affine& Affine::rotate(double radang)
{
    return transform(rotationAffine(radang));
}



Affine& Affine::rotateAroundPoint(double radang, double x, double y)
{
    translate(-x,-y);
    transform(rotationAffine(radang));
    translate(x,y);
    return *this;
}



void Affine::transformPoint(double& x, double &y) const
{
    double nx = a * x + b * y + tx;
    double ny = c * x + d * y + ty;
    x = nx;
    y = ny;
}

}
// vim: set ts=4 sw=4 nowrap expandtab: settings

