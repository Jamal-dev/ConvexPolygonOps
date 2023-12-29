// src/Polygon.cpp

#include "Polygon.hpp"
#include "Point.hpp"
#include "Vertex.hpp"
#include "Edge.hpp"
#include "GeometryUtils.hpp"

// Polygon constructor if we want by reference
Polygon::Polygon(Vertex* v) : _v(v), tolerance(1e-5) {
	resize();
}
// Polygon constructor if we want copies of the vertices
//Polygon::Polygon(Vertex* v) : tolerance(1e-5) {
//    if (v == NULL) {
//        _v = NULL;
//        _size = 0;
//    }
//    else {
//        // Start by creating a new vertex based on the given vertex
//        _v = new Vertex(*v);
//        
//        Vertex* current = _v;
//        Vertex* originalCurrent = v->cw();
//
//        // Loop until we've returned to the original starting vertex
//        while (originalCurrent != v) {
//            current = current->insert(new Vertex(*originalCurrent));
//            originalCurrent = originalCurrent->cw();
//        }
//        resize();
//
//        // Copy the last vertex (which is the original starting vertex 'v')
//        //current->insert(new Vertex(*v));
//    }
//}

std::vector<Point> Polygon::getPoints()
{
	std::vector<Point> points;
    Vertex* v = _v;
    if (v == NULL) {
		return points;
	}
    points.reserve(_size);
	
    for (int i = 0; i < _size; i++) {
        points.emplace_back(v->point());
		v = v->cw();
	}
	return points;
}

void Polygon::printPoints()
{
	std::vector<Point> points = getPoints();
    for (int i = 0; i < points.size(); i++) {
		std::cout << points[i].x << " , " << points[i].y << std::endl;
	}
}

// Polygon copy constructor
//Polygon::Polygon(const Polygon& other) : _size(other._size), tolerance(other.tolerance) {
//    if (other._size == 0) {
//        _v = NULL;
//    }
//    else {
//        _v = new Vertex(*other._v);
//        Vertex* current = _v;
//        Vertex* otherCurrent = other._v->cw();
//        while (otherCurrent != other._v) {
//            current = current->insert(new Vertex(*otherCurrent));
//            otherCurrent = otherCurrent->cw();
//        }
//    }
//}

// Polygon copy constructor another way
Polygon::Polygon(Polygon& other) : _size(other._size), tolerance(other.tolerance) 
{
    if (other._size == 0) {
        _v = NULL;
    }
    else {
        _v = new Vertex(other.point());
        for (int i = 1; i < _size; i++) {
            other.advance(ORIENTATION::CLOCKWISE);
            _v = _v->insert(new Vertex(other.point()));
        }
        other.advance(ORIENTATION::CLOCKWISE);
        _v = _v->cw();
    }
}

// destructor another way
//Polygon::~Polygon()
//{
//    if (_v)
//    {
//        Vertex* v = _v->cw();
//        while (_v != v)
//        {
//            delete v->remove();
//            v = _v->cw();
//        }
//        delete _v;
//    }
//}

Polygon::~Polygon() {
    *this;
    if (!_v) {
        delete _v; // Delete the last vertex if it exists
        return;
    }
    while (_v && _v->cw() != _v) {
        Vertex* next = _v->cw();
        Vertex* toDelete = _v;
        _v = (_v->cw() == _v->ccw()) ? nullptr : next; // Move to the next vertex, or set to nullptr if only one left
        delete toDelete->remove(); // Now safely delete the removed vertex
    }
    if (!_v) {
        delete _v; // Delete the last vertex if it exists
    }
}

void Polygon::clear() {
    while (_v && _v->cw() != _v) {
        Vertex* next = _v->cw();
        delete _v; // Delete the current vertex
        _v = (_v->cw() == _v->ccw()) ? nullptr : next; // Move to the next vertex, or set to nullptr if only one left
    }
    if (_v) {
        delete _v; // Delete the last vertex if it exists
        _v = nullptr; // Set to nullptr to indicate the polygon is empty
    }
    _size = 0; // Reset the size of the polygon
}


Polygon& Polygon::operator=(const Polygon& other) {
    if (this != &other) { // Protect against self-assignment
        // Clean up the current state
        // Assuming you have a method to properly delete all vertices
        this->clear(); // You need to define this method to delete the vertices and reset _v to nullptr

        // Copy the state from 'other'
        _size = other._size;
        tolerance = other.tolerance;

        if (other._size > 0) {
            _v = new Vertex(*other._v);
            Vertex* current = _v;
            Vertex* otherCurrent = other._v->cw();
            while (otherCurrent != other._v) {
                current = current->insert(new Vertex(*otherCurrent));
                otherCurrent = otherCurrent->cw();
            }
        }
        else {
            _v = nullptr;
        }
    }
    return *this;
}

Polygon::Polygon(Polygon&& other) noexcept
    : _v(other._v), _size(other._size), tolerance(other.tolerance) {
    other._v = nullptr; // We've taken ownership, so null out the other's pointer
    other._size = 0; // Reset other's size
}

Polygon& Polygon::operator=(Polygon&& other) noexcept {
    if (this != &other) { // Protect against self-assignment
        this->clear(); // Clear current state

        // Move the state from 'other'
        _v = other._v;
        _size = other._size;
        tolerance = other.tolerance;

        // Reset 'other' to a destructible state
        other._v = nullptr;
        other._size = 0;
    }
    return *this;
}




void Polygon::resize(void)
{
    if (_v == NULL) {
        _size = 0;
    }
    else {
        Vertex* v = _v->cw();
        _size = 1; // Reset _size and start counting
        for (; v != _v; ++_size, v = v->cw());
    }
}


Vertex* Polygon::v(void) const
{
	return _v;
}

int Polygon::size(void) const
{
	return _size;
}

Point Polygon::point(void) const
{
	return _v->point();
}

Edge Polygon::edge(void) const
{
	return Edge(point(), _v->cw()->point());
}

Vertex* Polygon::cw(void) const
{
	return _v->cw();
}

Vertex* Polygon::ccw(void) const
{
	return _v->ccw();
}

Vertex* Polygon::neighbor(ORIENTATION orient) const
{
	return _v->neighbor(orient);
}

// advance moves over one vertex in the specified direction
Vertex* Polygon::advance(ORIENTATION orient)
{
	return _v = _v->neighbor(orient);
}

// setV sets the current vertex to the specified vertex
Vertex* Polygon::setV(Vertex* v)
{
	// assuming v is a vertex in the polygon
    // it will move to the v
    return _v = v;
}

// insert adds a new vertex to the polygon after the current vertex
Vertex* Polygon::insert(const Point& point)
{
    if (_size++ == 0)
    {
        _v = new Vertex(point.x,point.y);
    }
    else
        _v = _v->insert(new Vertex(point));
    return _v;
}

// remove deletes the current vertex from the polygon
void Polygon::remove(void)
{
	Vertex* v = _v;
	_v = (-- _size == 0)? NULL:_v->ccw();
	delete v->remove();
}

// split splits the polygon into two polygons at the specified vertex
Polygon* Polygon::split(Vertex* v)
{
    Vertex* bp = _v->split(v);
    resize();
    return new Polygon(bp);
}


// setter for tolerance
// Set the tolerance to a positive value
void Polygon::setTolerance(double tol) {
    if (tol > 0) {
		tolerance = tol;
	}
}

// getter for tolerance
double Polygon::getTolerance() const {
	return tolerance;
}

// distanceBetweenPoints returns the distance between two points
double Polygon::distanceBetweenPoints(const Point& p1, const Point& p2) const {
    return std::pow(p2.x - p1.x, 2) + std::pow(p2.y - p1.y, 2);
}

// closestVertexToPoint returns the closest vertex to the specified point
Vertex* Polygon::closestVertexToPoint(const Point& targetPoint) const {
    Vertex* closestVertex = nullptr;
    double minDistanceSquared = std::numeric_limits<double>::max();

    Vertex* current = _v;
    if (current == nullptr) return nullptr;

    do {
        double distanceSquared = distanceBetweenPoints(current->point(), targetPoint);
        if (distanceSquared < minDistanceSquared) {
            minDistanceSquared = distanceSquared;
            closestVertex = current;
        }
        current = current->cw();
    } while (current != _v);

    return closestVertex;
}

