#pragma once
// Purpose: Header file for Intersection.cpp, which contains functions for
// It will the intersection between two convex polygons
class Edge;
enum POINT_CLASSIFICATION_LINE;
enum EDGE_CLASSIFICATION;
class Polygon;
class Point;

namespace Intersection 
{
	bool aimsAt(const Edge& a, const Edge& b, POINT_CLASSIFICATION_LINE aclass, EDGE_CLASSIFICATION crossType);
	enum { UNKNOWN, P_IS_INSIDE, Q_IS_INSIDE };
	void advance(Polygon& A, Polygon& R, int inside);
	EDGE_CLASSIFICATION crossingPoint(Edge& e, Edge& f, Point& p, double tolerance = 1e-10);
	Polygon* convexPolygonIntersect(Polygon& P, Polygon& Q);

}