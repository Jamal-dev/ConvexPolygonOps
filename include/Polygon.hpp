// include/Polygon.hpp

#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>
#include <cmath>
#include <unordered_set>
class Vertex;
class Point;
class Edge;
enum ORIENTATION;
//#include "Point.hpp"
//#include "Vertex.hpp"
//#include "GeometryUtils.hpp"

class Polygon {
private: 
    int _size;
    double tolerance;
    Vertex* _v;
    void resize(void);
    double distanceBetweenPoints(const Point& p1, const Point& p2) const;
    
public:

    Polygon() : _size(0),tolerance(1e-5), _v(NULL) {} // Default constructor with default tolerance
    // copy constructor
    Polygon(Polygon& other);
    // constructor to initialize a polygon with a circular doubly linked list of vertices
    Polygon(Vertex* v);
    // destructor: deallocates this polygon's vertices before deleting the Polygon object itsef
    ~Polygon();
    // assignment operator
    Polygon& operator=(const Polygon& other);
    // clear() method
    void clear(void);
    // move copy
    Polygon(Polygon&& other) noexcept;
    // move assignment
    Polygon& operator=(Polygon&& other) noexcept;
    Vertex* cw(void) const;
    Vertex* ccw(void) const;
    Vertex* neighbor(ORIENTATION orient) const;
    Vertex* advance(ORIENTATION orient);
    Point point(void) const;
    Edge edge(void) const;
    Vertex* setV(Vertex*);
    Vertex* v(void) const;
    int size(void) const;
    Vertex* insert(const Point& point);
    void remove(void);
    Polygon *split(Vertex*);

    //setter for tolerance
    void setTolerance(double tol);
    //getter for tolerance
    double getTolerance() const;
    std::vector<Point> getPoints();
    void printPoints();
    Vertex* closestVertexToPoint(const Point& targetPoint) const;

    
};

#endif // POLYGON_HPP
