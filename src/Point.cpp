// src/Point.cpp

#include "Point.hpp"
#include "Edge.hpp"
#include "GeometryUtils.hpp"







// Implementations of any utility functions
double Point::distance(const Point& a, const Point& b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

// assignment operator
Point& Point::operator=(const Point& other) {
	x = other.x;
	y = other.y;
    tolerance = other.tolerance;
	return *this;
}

// += operator
Point& Point::operator+=(const Point& other) {
    x += other.x;
    y += other.y;
    return *this;
}

// -= operator
Point& Point::operator-=(const Point& other) {
	x -= other.x;
	y -= other.y;
    return *this;
}

// + operator
Point Point::operator+(const Point& other) const {
    return Point(x + other.x, y + other.y);
}

// - operator
Point Point::operator-(const Point& other) const {
	return Point(x - other.x, y - other.y);
}

// * operator multiplying by a scalar
Point Point::operator*(double scalar) const {
    return Point(x * scalar, y * scalar);
}

// / operator dividing by a scalar
Point Point::operator/(double scalar) const {
	return Point(x / scalar, y / scalar);
}

// == operator
bool Point::operator==(const Point& other) const {
    return std::abs(x-other.x) <= tolerance && std::abs(y-other.y) <= tolerance;
}

// != operator
bool Point::operator!=(const Point& other) const {
    return !(*this == other);
}
// < operator
bool Point::operator<(const Point& other) const {
    return x < other.x || (x == other.x && y < other.y);
}
// <= operator
bool Point::operator<=(const Point& other) const {
    return *this < other || *this == other;
}
// > operator
bool Point::operator>(const Point& other) const {
    return !(*this <= other);
}
// >= operator
bool Point::operator>=(const Point& other) const {
    return !(*this < other);
}
// negate operator
Point Point::operator-() const {
    return Point(-x, -y);
}
// dot product
double Point::dot(const Point& other) const {
    return x * other.x + y * other.y;
}
// cross product
double Point::cross(const Point& other) const {
    return x * other.y - y * other.x;
}
// magnitude
double Point::magnitude() const {
    return std::sqrt(x * x + y * y);
}
// normalize
Point Point::normalize() const {
    return *this / magnitude();
}
// rotate counter-clockwise by theta radians
Point Point::rotate(const double& theta) const {
    return Point(x * std::cos(theta) - y * std::sin(theta),
        x * std::sin(theta) + y * std::cos(theta));
}
// rotate counter-clockwise by theta degrees
Point Point::rotateDegrees(const double& theta) const {
    return rotate(theta * M_PI / 180);
}

// length
double Point::length() const {
    return std::sqrt(x * x + y * y);
}
// polar angle
double Point::polarAngle() const {
    return std::atan2(y, x);
}
// [] operator
double& Point::operator[](int index) {
	if (index == 0) return x;
	return y;
}
// classify the location of the point
POINT_CLASSIFICATION_LINE Point::classify(const Point& p0, const Point& p1)
const 
    {
    Point p2 = *this;
	Point a = p1 - p0;
	Point b = p2 - p0;
	double crossProd = a.cross(b);
	if (crossProd > 0.0) return LEFT;
	if (crossProd < 0.0) return RIGHT;
	if ((a.x * b.x < 0.0) || (a.y * b.y < 0.0)) return BEHIND;
	if (a.length() < b.length()) return BEYOND;
	if (p0 == p2) return ORIGIN;
	if (p1 == p2) return DESTINATION;
	return BETWEEN;
}

POINT_CLASSIFICATION_LINE Point::classify(const Edge& edge) const
{
	return classify(edge.org, edge.dest);
}

// polar angle in degrees
double Point::polarAngleDegrees() const
{
	return polarAngle() * 180 / M_PI;
}

// return null point
Point Point::null() {
    // max value for double
    return Point(DBL_MAX, DBL_MAX);
}

// is null
bool Point::isNull() const {
	return *this == null();
}

// Rotate this point around another point by theta radians
void Point::rotateAround(const Point& p, double theta) {
    double cosTheta = cos(theta);
    double sinTheta = sin(theta);

    double tx = x - p.x;  // translate point to origin
    double ty = y - p.y;

    x = p.x + tx * cosTheta - ty * sinTheta;
    y = p.y + tx * sinTheta + ty * cosTheta;
}

