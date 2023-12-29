// include/Point.hpp

#ifndef POINT_HPP
#define POINT_HPP

#include <corecrt_math_defines.h>
class Edge;
enum POINT_CLASSIFICATION_LINE;
//#include "GeometryUtils.hpp"
#include <cmath>
#include <iostream>

class Point {
public:
    double x, y,tolerance;
    Point(const double& x = 0, const double& y = 0,const double& tolerance=1e-10) : x(x), y(y),tolerance(tolerance) {}

    static double distance(const Point& a, const Point& b);

    // copy constructor
    Point(const Point& other) : x(other.x), y(other.y),tolerance(other.tolerance) {}

    // [] operator
    double& operator[](int index);
    // assignment operator
    Point& operator=(const Point& other);
    // += operator
    Point& operator+=(const Point& other);
    // -= operator
    Point& operator-=(const Point& other);
    // + operator
    Point operator+(const Point& other) const;
    // - operator
    Point operator-(const Point& other) const;
    // negate operator
    Point operator-() const;
    // * operator multiplying by a scalar
    Point operator*(double scalar) const;
    // / operator dividing by a scalar
    Point operator/(double scalar) const;
    // == operator
    bool operator==(const Point& other) const;
    // != operator
    bool operator!=(const Point& other) const;
    // < operator
    bool operator<(const Point& other) const;
    // <= operator
    bool operator<=(const Point& other) const;
    // > operator
    bool operator>(const Point& other) const;
    // >= operator
    bool operator>=(const Point& other) const;
    // dot product
    double dot(const Point& other) const;
    // cross product
    double cross(const Point& other) const;
    // magnitude
    double magnitude() const;
    // normalize
    Point normalize() const;
    // rotate counter-clockwise by theta radians
    Point rotate(const double& theta) const;
    // rotate counter-clockwise by theta degrees
    Point rotateDegrees(const double& theta) const;

    // length
    double length() const;
    // polar angle in radians
    double polarAngle() const;
    // polar angle in degrees
    double polarAngleDegrees() const;
    // classify the location of the point
    POINT_CLASSIFICATION_LINE classify(const Point& a, const Point& b) const;
    POINT_CLASSIFICATION_LINE classify(const Edge& edge) const;
    // return null point
    static Point null();
    bool isNull() const;
    // Rotate this point around another point by theta radians
    void rotateAround(const Point& p, double theta);
    // destructor
	virtual ~Point() {}


    friend std::ostream& operator<<(std::ostream& os, const Point& pt);
};

#endif // POINT_HPP
