#ifndef GEOMETRYUTILS_HPP
#define GEOMETRYUTILS_HPP
//#include "Vertex.hpp"
//#include "Edge.hpp"
//#include "Polygon.hpp"
class Point;
class Vertex;
class Polygon;
class Edge;

#include <cmath>
#include <algorithm>
#include <vector>
#include <numeric>

// enum for collinear, clockwise and counterclockwise
enum ORIENTATION { COLLINEAR, CLOCKWISE, COUNTERCLOCKWISE };
enum EDGE_CLASSIFICATION { EDGE_COLLINEAR, PARALLEL, SKEW, SKEW_CROSS, SKEW_NO_CROSS };
// this classification classify point accourding to directed edge
enum POINT_CLASSIFICATION_LINE { LEFT, RIGHT, BEYOND, BEHIND, BETWEEN, ORIGIN, DESTINATION };
// this classification classify point accourding to polygon
enum POINT_CLASSIFICATION_POLYGON { INSIDE, OUTSIDE , BOUNDARY };
// this classification classify edge accourding to polygon;TOUCHING means if the ray emanates from edge
// CROSSING means if ray from edge 1 and edge 2 intersect
// INESSENTIAL means if ray edge 1 and edge 2 are not touching or crossing
enum EDGE_CLASSIFICATION_POLYGON { TOUCHING, CROSSING, INESSENTIAL};

namespace GeometryUtils {
    bool lineSegmentIntersection(const Point& p1, const Point& q1, const Point& p2, const Point& q2, Point& intersection);
    // Utility function to check if point q lies on line segment 'pr'
    bool onSegment(const Point& p, const Point& q, const Point& r);
    // Utility function to find the orientation of ordered triplet (p, q, r)
// Returns 0 if collinear, 1 if clockwise, 2 if counterclockwise
    ORIENTATION orientation(const Point& p, const Point& q, const Point& r);

    // more robust applicable for both convex and non-convex polygons
    POINT_CLASSIFICATION_POLYGON isPointInPolygon(const Point& p, Polygon& poly);

    EDGE_CLASSIFICATION_POLYGON edgeType(const Point& a, const Edge& e);

    // check if point is inside convex polygon
    bool isPointInsideConvexPolygon(const Point& p, Polygon& poly);

    // finding least vertex in polygon
    Vertex * leastVertex(Polygon& poly, int (*cmp)(Point*,Point*));

    // comparison function left2RightCmp
    int left2RightCmp(const Point& p1, const Point& p2);

    // comparison function right2LeftCmp
    int right2LeftCmp(const Point& p1, const Point& p2);

    // Helper function to calculate the angle and distance from a reference point
    std::pair<double, double> polarAngleAndDistance(const Point& ref, const Point& p);

    // Comparator for sorting points in clockwise order around the centroid
    bool clockwiseComparator(const Point& a, const Point& b, const Point& centroid); /*{
        auto [angleA, distA] = polarAngleAndDistance(centroid, a);
        auto [angleB, distB] = polarAngleAndDistance(centroid, b);
        return angleA < angleB || (angleA == angleB && distA < distB);
    }*/

    // get centroid of points
    Point getCentroid(const std::vector<Point>& points);

    // create polygon from points
    Polygon createPolygon(const std::vector<Point>& inputPoints);

    // Helper function to compare two polygons with tolerance
    bool comparePolygons(Polygon& result, const std::vector<Point>& expectedVertices, double tolerance = 1e-10);

    // Helper function to check if polygon is convex
    bool isPolygonConvex(const std::vector<Point>& points);

}
#endif // GEOMETRYUTILS_HPP