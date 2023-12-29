#pragma once
#include "Point.hpp"
#include "Node.hpp"
enum ORIENTATION;
//#include "GeometryUtils.hpp"

class Vertex : public Node, public Point {
public:
	// Constructor
	Vertex(double x = 0, double y = 0) : Point(x, y) {}
	// Constructor with Point
	Vertex(const Point &p) : Point(p) {}
	// Vertex copy constructor newly added
	//Vertex(const Vertex& other) : Point(other.x,other.y) {}

	// get the next vertex by clockwise order
	Vertex* cw(void);
	// get the previous vertex by counter-clockwise order
	Vertex* ccw(void);
	// get the neighbor vertex by clockwise order or counter-clockwise order
	Vertex* neighbor(ORIENTATION orient);
	// get Point
	Point point(void);
	// insert corresponds to base Node class
	Vertex* insert(Vertex* vertex);
	// remove corresponds to base Node class
	Vertex* remove(void);
	// splice corresponds to base Node class
	void splice(Vertex* vertex);
	// split the edge between this and next vertex
	Vertex* split(Vertex *b);
	friend class Polygon;

};