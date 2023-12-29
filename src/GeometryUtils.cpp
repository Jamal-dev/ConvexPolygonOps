#include "GeometryUtils.hpp"
#include "Vertex.hpp"
#include "Point.hpp"
#include "Edge.hpp"
#include "Polygon.hpp"


bool GeometryUtils::lineSegmentIntersection(const Point& p1, const Point& q1, const Point& p2, const Point& q2, Point& intersection) {
    // Find the four orientations needed for the general and special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4) {
        // Line segments intersect. Calculating intersection point using line equations
        // Line p1q1 represented as a1x + b1y = c1
        double a1 = q1.y - p1.y;
        double b1 = p1.x - q1.x;
        double c1 = a1 * p1.x + b1 * p1.y;

        // Line p2q2 represented as a2x + b2y = c2
        double a2 = q2.y - p2.y;
        double b2 = p2.x - q2.x;
        double c2 = a2 * p2.x + b2 * p2.y;

        double determinant = a1 * b2 - a2 * b1;

        if (determinant != 0) {
            intersection.x = (b2 * c1 - b1 * c2) / determinant;
            intersection.y = (a1 * c2 - a2 * c1) / determinant;
            return true;
        }
    }

    // Special Cases
    if (o1 == 0 && onSegment(p1, p2, q1)) { intersection = p2; return true; }
    if (o2 == 0 && onSegment(p1, q2, q1)) { intersection = q2; return true; }
    if (o3 == 0 && onSegment(p2, p1, q2)) { intersection = p1; return true; }
    if (o4 == 0 && onSegment(p2, q1, q2)) { intersection = q1; return true; }

    return false; // Doesn't fall in any of the above cases
}

// Utility function to check if point q lies on line segment 'pr'
bool GeometryUtils::onSegment(const Point& p, const Point& q, const Point& r) {
    if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
        q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y)) {
		return true;
	}
	return false;
}

// Utility function to find the orientation of ordered triplet (p, q, r)
// Returns 0 if collinear, 1 if clockwise, 2 if counterclockwise

ORIENTATION GeometryUtils::orientation(const Point& p, const Point& q, const Point& r) {
    Point p1 = q - p;
    Point p2 = r - q;
    double crossProduct = p1.cross(p2);

	if (crossProduct == 0) return ORIENTATION::COLLINEAR; // colinear

	return (crossProduct > 0) ? ORIENTATION::CLOCKWISE : ORIENTATION::COUNTERCLOCKWISE; // clock or counterclock wise
}



bool GeometryUtils::isPointInsideConvexPolygon(const Point& p, Polygon& poly)
{
    if (poly.size()==0) return false;
    if (poly.size()==1) return p==poly.point();
    if (poly.size() == 2) 
    {
        POINT_CLASSIFICATION_LINE pointClass = p.classify(poly.edge());
        return pointClass == POINT_CLASSIFICATION_LINE::BETWEEN || pointClass == POINT_CLASSIFICATION_LINE::ORIGIN || pointClass == POINT_CLASSIFICATION_LINE::DESTINATION;
    }
    Vertex *org = poly.v();
    for (int i = 0; i < poly.size(); i++,poly.advance(ORIENTATION::CLOCKWISE))
        if (p.classify(poly.edge()) == POINT_CLASSIFICATION_LINE::LEFT) 
        {
            poly.setV(org);
			return false;
        }
    return true;
}

Vertex* GeometryUtils::leastVertex(Polygon& poly, int (*cmp)(Point*, Point*))
{
	Vertex *least = poly.v();
    poly.advance(ORIENTATION::CLOCKWISE);
	for (int i = 1; i < poly.size(); i++, poly.advance(ORIENTATION::CLOCKWISE))
		if ( (*cmp)(poly.v(), least) < 0) least = poly.v();
	poly.setV(least);	
    return least;
}

int GeometryUtils::left2RightCmp(const Point& p1, const Point& p2)
{
	if (p1 < p2) return -1;
	if (p1 > p2) return 1;
	return 0;
}

int GeometryUtils::right2LeftCmp(const Point& p1, const Point& p2)
{
	return left2RightCmp(p2,p1);
}

POINT_CLASSIFICATION_POLYGON GeometryUtils::isPointInPolygon(const Point& a, Polygon& p)
{
    int parity = 0;
    for (int i = 0; i < p.size(); i++, p.advance(ORIENTATION::CLOCKWISE)) {
        Edge e = p.edge();
        switch (edgeType(a, e)) {
        case EDGE_CLASSIFICATION_POLYGON::TOUCHING:
            return POINT_CLASSIFICATION_POLYGON::BOUNDARY;
        case EDGE_CLASSIFICATION_POLYGON::CROSSING:
            parity = 1 - parity;
        }
    }
    return (parity ? POINT_CLASSIFICATION_POLYGON::INSIDE : POINT_CLASSIFICATION_POLYGON::OUTSIDE);
}

EDGE_CLASSIFICATION_POLYGON GeometryUtils::edgeType(const Point& a, const Edge& e) {
    Point v = e.org;
    Point w = e.dest;

    // Handle horizontal edges explicitly
    if (v.y == w.y) {
        if (a.y == v.y && a.x >= std::min(v.x, w.x) && a.x <= std::max(v.x, w.x)) {
            return EDGE_CLASSIFICATION_POLYGON::TOUCHING; // Point is on a horizontal edge
        }
        return EDGE_CLASSIFICATION_POLYGON::INESSENTIAL;
    }

    // Normal case
    switch (a.classify(e)) {
    case POINT_CLASSIFICATION_LINE::LEFT:
        return ((v.y <= a.y) && (a.y < w.y)) ? EDGE_CLASSIFICATION_POLYGON::CROSSING : EDGE_CLASSIFICATION_POLYGON::INESSENTIAL;
    case POINT_CLASSIFICATION_LINE::RIGHT:
        return ((w.y <= a.y) && (a.y < v.y)) ? EDGE_CLASSIFICATION_POLYGON::CROSSING : EDGE_CLASSIFICATION_POLYGON::INESSENTIAL;
    case POINT_CLASSIFICATION_LINE::BETWEEN:
    case POINT_CLASSIFICATION_LINE::ORIGIN:
    case POINT_CLASSIFICATION_LINE::DESTINATION:
        return EDGE_CLASSIFICATION_POLYGON::TOUCHING;
    default:
        return EDGE_CLASSIFICATION_POLYGON::INESSENTIAL;
    }
}



//EDGE_CLASSIFICATION_POLYGON GeometryUtils::edgeType(const Point& a, const Edge& e) {
//    Point v = e.org;
//    Point w = e.dest;
//    switch (a.classify(e)) {
//    case POINT_CLASSIFICATION_LINE::LEFT:
//        return ((v.y < a.y) && (a.y <= w.y)) ? EDGE_CLASSIFICATION_POLYGON::CROSSING : EDGE_CLASSIFICATION_POLYGON::INESSENTIAL;
//    case POINT_CLASSIFICATION_LINE::RIGHT:
//        return ((w.y < a.y) && (a.y <= v.y)) ? EDGE_CLASSIFICATION_POLYGON::CROSSING : EDGE_CLASSIFICATION_POLYGON::INESSENTIAL;
//    case POINT_CLASSIFICATION_LINE::BETWEEN:
//    case POINT_CLASSIFICATION_LINE::ORIGIN:
//    case POINT_CLASSIFICATION_LINE::DESTINATION:
//        return TOUCHING;
//    default:
//        return EDGE_CLASSIFICATION_POLYGON::INESSENTIAL;
//    }
//}


Point GeometryUtils::getCentroid(const std::vector<Point>& points) {
    double x = 0, y = 0;
    for (const auto& p : points) {
        x += p.x;
        y += p.y;
    }
    return { x / points.size(), y / points.size() };
}

Polygon GeometryUtils::createPolygon(const std::vector<Point>& inputPoints) {
    // First, remove duplicates
    std::vector<Point> points = inputPoints; // Copy the input points
    std::sort(points.begin(), points.end()); // Sort the points to remove duplicates
    auto last = std::unique(points.begin(), points.end());
    points.erase(last, points.end());

    // Calculate centroid
    Point centroid = GeometryUtils::getCentroid(points);

    // Sort points in clockwise order
    std::sort(points.begin(), points.end(),
        [&centroid](const Point& a, const Point& b) {
            return GeometryUtils::clockwiseComparator(a, b, centroid);
        });

    // Create polygon with sorted points
    Polygon polygon;
    for (const auto& point : points) {
        polygon.insert(point);
    }

    return polygon;
}

bool GeometryUtils::clockwiseComparator(const Point& a, const Point& b, const Point& centroid) {
    double angleA = std::atan2(a.y - centroid.y, a.x - centroid.x);
    double angleB = std::atan2(b.y - centroid.y, b.x - centroid.x);
    if (angleA != angleB)
        return angleA > angleB;
    double distA = (a.x - centroid.x) * (a.x - centroid.x) + (a.y - centroid.y) * (a.y - centroid.y);
    double distB = (b.x - centroid.x) * (b.x - centroid.x) + (b.y - centroid.y) * (b.y - centroid.y);
    return distA > distB;
}

std::pair<double, double> GeometryUtils::polarAngleAndDistance(const Point& ref, const Point& p) {
    double dx = p.x - ref.x;
    double dy = p.y - ref.y;
    return { std::atan2(dy, dx), std::hypot(dx, dy) };
}

// Helper function to compare two polygons with tolerance
bool GeometryUtils::comparePolygons(Polygon& result, const std::vector<Point>& expectedVertices, double tolerance) {
    std::vector<Point> resultVertices = result.getPoints();
    if (resultVertices.size() != expectedVertices.size()) {
        return false;
    }
    for (const auto& expectedPoint : expectedVertices) {
        auto it = std::find_if(resultVertices.begin(), resultVertices.end(),
            [&expectedPoint, tolerance](const Point& p) {
                return p == expectedPoint;
            });
        if (it == resultVertices.end()) {
            return false;
        }
    }
    return true;
}

bool GeometryUtils::isPolygonConvex(const std::vector<Point>& points) {
    if (points.size() < 3) return false; // A polygon must have at least 3 points

    bool sign = false;
    unsigned int n = points.size();

    for (int i = 0; i < n; i++) {
        Edge edge1(points[i], points[(i + 1) % n]);
        Edge edge2(points[(i + 1) % n], points[(i + 2) % n]);
        const Point vec1 = edge1.toVector();
        const Point vec2 = edge2.toVector();

        const double crossProduct = vec1.cross(vec2);
        if (i == 0) {
            sign = crossProduct > 0;
        }
        else {
            if ((crossProduct > 0) != sign) {
                return false;
            }
        }
    }
    return true;
}