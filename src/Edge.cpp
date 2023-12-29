#include "Point.hpp"
#include "Edge.hpp"
#include "Vertex.hpp"
#include "GeometryUtils.hpp"

// Constructor with Vertex
Edge::Edge(Vertex& org, Vertex& dest) : org(org.point()), dest(dest.point()) {}
// Constructor with Vertex pointer
Edge::Edge(Vertex* org, Vertex* dest) : org(org->point()), dest(dest->point()) {}

Point Edge::midpoint(void)
{
	return  (org + dest) * 0.5;
}
Edge& Edge::rot90(void)
{ //90 degree rotation around midpoint
	Point p = dest - org;
	Point mid = midpoint();
	Point n(p.y, -p.x);
	org = mid - n * 0.5;
	dest = mid + n * 0.5;
	return *this;
}

Edge& Edge::flip(void)
{
	return rot90().rot90();
}

Point Edge::point(const double& t)
{
	return Point(org + (dest - org) * t);
}

EDGE_CLASSIFICATION Edge::intersect(const Edge& edge, double& t) const
{
	Point a = org;
	Point b = dest;
	Point c = edge.org;
	Point d = edge.dest;
	Point n = Point(d.y-c.y,c.x-d.x);
	double denom = n.dot( (b - a));
	double num = n.dot(a - c);
	t = -num / denom;
	if (denom == 0.0) {
		// parallel
		POINT_CLASSIFICATION_LINE aclass = org.classify(edge);
		if (aclass == POINT_CLASSIFICATION_LINE::LEFT || aclass == POINT_CLASSIFICATION_LINE::RIGHT) return EDGE_CLASSIFICATION::PARALLEL;
		else
			return EDGE_CLASSIFICATION::EDGE_COLLINEAR;
	}
	
	// if infinite lines ab and cd intersect into one point, then
	return EDGE_CLASSIFICATION::SKEW;
}

/* Edge :: cross returns SKEW - CROSS if and only if this line segment intersects line segment e.If the line segments do intersect, the parametric value along this line segment corresponding to the point of intersection is returned through reference parameter t.Otherwise the function returns COLLINEAR, PARALLEL, or SKEW - NO - CROSS, as appropriate :*/

EDGE_CLASSIFICATION Edge::cross(const Edge& edge, double& t) const
{
	// this is not a cross product it gives information if two edges cross or not
	// when edges are skew and cross then they intersect
	double s;
	EDGE_CLASSIFICATION crossType = edge.intersect(*this, s);
	if (crossType==EDGE_CLASSIFICATION::EDGE_COLLINEAR || crossType == EDGE_CLASSIFICATION::PARALLEL) return crossType;
	if (s < 0.0 || s > 1.0) return EDGE_CLASSIFICATION::SKEW_NO_CROSS;
	intersect(edge,t);
	if (t >= 0.0 && t <= 1.0) return EDGE_CLASSIFICATION::SKEW_CROSS;
	return EDGE_CLASSIFICATION::SKEW_NO_CROSS;
}

bool Edge::isVertical(void)
{
	return org.x == dest.x;
}

bool Edge::isHorizontal(void)
{
	return org.y == dest.y;
}

double Edge::slope(void) const
{
	if (org.x == dest.x) return DBL_MAX;
	return (dest.y - org.y) / (dest.x - org.x);
}

double Edge::yIntercept(void) const
{
	return org.y - slope() * org.x;
}

bool Edge::operator==(const Edge& edge) const
{
	return org == edge.org && dest == edge.dest;
}

bool Edge::operator!=(const Edge& edge) const
{
	return !(*this == edge);
}

bool Edge::isParallel(const Edge& edge) const
{
	if ((*this).slope()==edge.slope()) return true;
}

bool Edge::isPerpendicular(const Edge& edge) const

{
	if ((*this).slope() * edge.slope() == -1) return true;
}

bool Edge::isSame(const Edge& edge) const
{
	if ((*this).slope() == edge.slope() && (*this).yIntercept() == edge.yIntercept()) return true;
}

double Edge::y(const double& x)
{
	return slope() * x + yIntercept();
}

Point Edge::intersection(const Edge& edge)
{
	double t;
	EDGE_CLASSIFICATION edgetype= cross(edge, t);
	// if two edges are skew and cross , then they intersect
	if (edgetype == EDGE_CLASSIFICATION::SKEW_CROSS) return org + (dest - org) * t;
	return Point::null();
}

Point Edge::normal(void) const
{
	Point p = dest - org;
	return Point(p.y, -p.x);
}

double Edge::length(void)
{
	return Point::distance(org,dest);
}




// distance of a point to the edge
// signed distance is positive if p lies to the right of e
double Edge::signedDistance(const Edge& e2) const {
	Point edgeDirection = dest - org;
	Point pointVector = e2.org - org;

	// Cross product to find the signed area of the parallelogram
	double crossProd = pointVector.cross(edgeDirection);

	// Divide by the length of the edge to get signed distance
	double edgeLength = edgeDirection.length();
	return -crossProd / edgeLength;
}

// Calculate the signed distance from a point to this edge
double Edge::signedDistance(const Point& p) const {
	Point edgeDirection = dest - org;
	Point pointVector = p - org;

	// Cross product to find the signed area of the parallelogram
	double crossProd = pointVector.cross(edgeDirection);

	// Divide by the length of the edge to get signed distance
	double edgeLength = edgeDirection.length();
	return -crossProd / edgeLength;
}

// Rotate this edge around a point by theta radians
void Edge::rotateAround(const Point& p,const double& theta) {
	org.rotateAround(p, theta);
	dest.rotateAround(p, theta);
}

// edge scaling
Edge& Edge::scale(const double& s) {
	Point mid = midpoint();
	org = mid + (org - mid) * s;
	dest = mid + (dest - mid) * s;
	return *this;
}

// edge translation
Edge& Edge::trans(const Point& point) 
{
	org = org + point;
	dest = dest + point;
	return *this;
}

Point Edge::toVector() const {
	return Point(dest.x - org.x, dest.y - org.y);
}