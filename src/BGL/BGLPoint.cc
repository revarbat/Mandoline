#include "BGLPoint.hh"

namespace BGL {

/// Assignment operator
Point& Point::operator=(const Point &rhs) {
    if (this != &rhs) {
        this->x = rhs.x;
        this->y = rhs.y;
    }
    return *this;
}



/// Compound assignment operator += Point
//  Adds the X and Y coordinates of the given point from this point.
Point& Point::operator+=(const Point &rhs) {
    this->x += rhs.x;
    this->y += rhs.y;
    return *this;
}



/// Compound assignment operator -= Point
//  Subtracts the X and Y coordinates of the given point from this point.
Point& Point::operator-=(const Point &rhs) {
    this->x -= rhs.x;
    this->y -= rhs.y;
    return *this;
}



/// Compound assignment operator *= double
//  Multiplies (scales) the X Y coords of this point by the given factor.
Point& Point::operator*=(double rhs) {
    this->x *= rhs;
    this->y *= rhs;
    return *this;
}



/// Compound assignment operator *= Point
//  Multiplies (scales) the X Y coords of this point by the given X and Y
//  factors that are given as the coordinates of a Point.
Point& Point::operator*=(const Point &rhs) {
    this->x *= rhs.x;
    this->y *= rhs.y;
    return *this;
}



/// Compound assignment operator /= double
//  Divides (scales) the X Y coords of this point by the given divisor.
Point& Point::operator/=(double rhs) {
    this->x /= rhs;
    this->y /= rhs;
    return *this;
}



/// Compound assignment operator /= Point
//  Divides (scales) the X Y coords of this point by the given X and Y
//  divisors that are given as the coordinates of a Point.
Point& Point::operator/=(const Point &rhs) {
    this->x /= rhs.x;
    this->y /= rhs.y;
    return *this;
}



/// Binary arithmetic operator +
//  Adds the X Y coords of the given point to the X Y coords of this point.
//  Returns a new Point with the resultant values.
const Point Point::operator+(const Point &rhs) const {
    return Point(*this) += rhs;
}



/// Binary arithmetic operator -
//  Subtracts the X Y coords of the given point from the X Y coords of this
//  point.  Returns a new Point with the resultant values.
const Point Point::operator-(const Point &rhs) const {
    return Point(*this) -= rhs;
}



const Point Point::operator*(double rhs) const {
    return Point(*this) *= rhs;
}



const Point Point::operator*(const Point &rhs) const {
    return Point(*this) *= rhs;
}



const Point Point::operator/(double rhs) const {
    return Point(*this) /= rhs;
}



const Point Point::operator/(const Point &rhs) const {
    return Point(*this) /= rhs;
}



// Comparison operators
bool Point::operator==(const Point &rhs) const {
    return (fabs(x-rhs.x) < EPSILON && fabs(y-rhs.y) < EPSILON);
}



bool Point::operator>=(const Point &rhs) const {
    if (x < rhs.x + EPSILON) {
        return false;
    }
    if (y < rhs.y + EPSILON) {
        return false;
    }
    return true;
}



bool Point::operator<=(const Point &rhs) const {
    if (x > rhs.x - EPSILON) {
        return false;
    }
    if (y > rhs.y - EPSILON) {
        return false;
    }
    return true;
}



bool Point::operator>(const Point &rhs) const {
    return !(*this <= rhs);
}



bool Point::operator<(const Point &rhs) const {
    return !(*this >= rhs);
}



bool Point::operator!=(const Point &rhs) const {
    return !(*this == rhs);
}



// Transformations
Point& Point::scale(double scale) {
    *this *= scale;
    return *this;
}



Point& Point::scale(const Point& vect) {
    *this *= vect;
    return *this;
}



Point& Point::scaleAroundPoint(const Point& center, double scale) {
    *this -= center;
    *this *= scale;
    *this += center;
    return *this;
}



Point& Point::scaleAroundPoint(const Point& center, const Point& vect) {
    *this -= center;
    *this *= vect;
    *this += center;
    return *this;
}



Point& Point::rotate(double angle) {
    double x2 = x * cos(angle) - y * sin(angle);
    double y2 = x * sin(angle) + y * cos(angle);
    x = x2;
    y = y2;
    return *this;
}



void Point::quantize(double quanta) {
    x = floor(x/quanta+0.5) * quanta;
    y = floor(y/quanta+0.5) * quanta;
}



void Point::quantize() {
    quantize(EPSILON/2.0);
}



// Calculations
double Point::distanceFrom(const Point& pt) const {
    Point delta = *this - pt;
    return hypot(delta.y,delta.x);
}



double Point::angleToPoint(const Point& pt) const {
    Point delta = pt - *this;
    return atan2(delta.y,delta.x);
}



Point &Point::polarOffset(double ang, double rad) {
    x += rad*cos(ang);
    y += rad*sin(ang);
    return *this;
}



ostream& operator <<(ostream &os,const Point &pt)
{
    os.precision(12);
    os.setf(ios::fixed);
    os << "(" << pt.x << ", " << pt.y << ")";
    return os;
}

}

// vim: set ts=4 sw=4 nowrap expandtab: settings

