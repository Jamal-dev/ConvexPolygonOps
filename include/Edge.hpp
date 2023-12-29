#pragma once
//#include "Point.hpp"
//#include "GeometryUtils.hpp"
class Point;
class Vertex;
enum EDGE_CLASSIFICATION;
enum POINT_CLASSIFICATION_LINE;
class Edge {
public:
	Point org;
	Point dest;
	// Constructor
	Edge(const Point& org, const Point& dest) : org(org), dest(dest) {}
	// Constructor with Vertex
	Edge(Vertex& org, Vertex& dest);
	// Constructor with Vertex pointer
	Edge(Vertex* org, Vertex* dest);
	// void constructor
	Edge(void): org(Point(0,0)),dest(Point(1,0)) {}
	// edge rotation around its midpoint
	Edge &rot90(void);
	// Rotate this edge around a point by theta radians
	void rotateAround(const Point& p,const double& theta);
	// edge scaling
	Edge &scale(const double& s);
	// edge translation
	Edge &trans(const Point& point);
	// edge flip
	Edge &flip(void);
	// edge length
	double length(void);
	// edge midpoint
	Point midpoint(void);
	// point 
	Point point(const double &t);
	// is_vertical
	bool isVertical(void);
	// is_horizontal
	bool isHorizontal(void);
	// is_parallel
	bool isParallel(const Edge& edge) const;
	// is_perpendicular
	bool isPerpendicular(const Edge& edge) const;
	// is_same
	bool isSame(const Edge& edge) const;
	// slope
	double slope(void) const;
	// y_intercept
	double yIntercept(void) const;
	// y value
	double y(const double &x);
	// distance of a point to the edge
	double signedDistance(const Point& point) const;
	// distance of a point to the edge
	double signedDistance(const Edge& edge) const;
	// intersection of two edges
	Point intersection(const Edge& edge);
	// normal unit vector
	Point normal(void) const;
	
	// intersect
	EDGE_CLASSIFICATION intersect(const Edge& edge, double& t)const;
	// cross
	EDGE_CLASSIFICATION cross(const Edge& edge, double& t)const;
	// operator ==
	bool operator==(const Edge& edge) const;
	// operator !=
	bool operator!=(const Edge& edge) const;

	Point toVector() const;
};