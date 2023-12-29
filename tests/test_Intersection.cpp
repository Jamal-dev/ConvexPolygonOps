#include <gtest/gtest.h>
#include "Vertex.hpp"
#include "Edge.hpp"
#include "Polygon.hpp"
#include "GeometryUtils.hpp"
#include "Intersection.hpp"

class IntersectionTest : public ::testing::Test {
protected:
    Polygon P, Q;
    Vertex* v1,*v2,*v3,*v4,*v5,*v6;
    Edge *e1_cross, *e2_cross, *e3_cross;
    Edge *e1_parallel, *e2_parallel;
    Edge *e1_skew_no_cross, *e2_skew_no_cross;
    Edge *e1_colinear, *e2_colinear;
    Point intersectionPoint;
    Vertex *p11, *p12, *p13, *p14, *q11, *q12, *q13;
    Vertex *p21, *p22, *p23, *p24, *q21, *q22, *q23, *q24;
    Vertex *p31, *p32, *p33, *p34, *q31, *q32, *q33, *q34;
    Vertex *p41, *p42, *p43, *p44, *q41, *q42, *q43, *q44;

    void SetUp() override {
        // Common setup code can go here if needed
        v1 = new Vertex(0, 0);
        v2 = new Vertex(1, 1);
        v3 = new Vertex(1, 0);
        v4 = new Vertex(0, 1);
        v5 = new Vertex(2, 2);
        v6 = new Vertex(0.5, 0.25);

        e1_cross = new Edge(v1, v2);
        e2_cross = new Edge(v4, v3);
        e3_cross = new Edge(v3, v4);
        e1_parallel = new Edge(v1, v3);
        e2_parallel = new Edge(v2, v4);
        e1_skew_no_cross = new Edge(v1, v5); 
        e2_skew_no_cross = new Edge(v6, v3); 
        e1_colinear = new Edge(v1, v2);
        e2_colinear = new Edge(v2, v5);
        intersectionPoint = Point(0.5, 0.5);
    }

    void TearDown() override {
        // Cleanup code can go here if needed
        delete v1;
        delete v2;
        delete v3;
        delete v4,v5,v6;
        delete e1_cross;
        delete e2_cross, e3_cross;
        delete e1_parallel;
        delete e2_parallel;
        delete e1_skew_no_cross;
        delete e2_skew_no_cross;
        delete e1_colinear;
        delete e2_colinear;
        delete p11, p12, p13, p14, q11, q12, q13;
        delete p21, p22, p23, p24, q21, q22, q23, q24;
        delete p31, p32, p33, p34, q31, q32, q33, q34;
        delete p41, p42, p43, p44, q41, q42, q43, q44;
    }

    void createPartiallyOverlappingPolygons() {
        // Rectangle P
        Vertex *p1 = new Vertex(0, 0);
        Vertex *p2 = new Vertex(1, 0);
        Vertex *p3 = new Vertex(1, 1);
        Vertex *p4 = new Vertex(0, 1);
        p1->insert(p4)->insert(p3)->insert(p2);
        P = Polygon(p1);

        // Triangle Q, partially overlapping with P
        Vertex *q1 = new Vertex(0.5, 0);
        Vertex *q2 = new Vertex(1, 0);
        Vertex *q3 = new Vertex(0.5, 2);
        q1->insert(q3)->insert(q2);
        Q = Polygon(q1);
    }

    void createNonOverlappingPolygons() {
        // Rectangle P
        Vertex *p1 = new Vertex(0, 0);
        Vertex *p2 = new Vertex(2, 0);
        Vertex *p3 = new Vertex(2, 2);
        Vertex *p4 = new Vertex(0, 2);
        p1->insert(p4)->insert(p3)->insert(p2);
        P = Polygon(p1);

        // Rectangle Q far from P
        Vertex *q1 = new Vertex(10, 10);
        Vertex *q2 = new Vertex(12, 10);
        Vertex *q3 = new Vertex(12, 12);
        Vertex *q4 = new Vertex(10, 12);
        q1->insert(q4)->insert(q3)->insert(q2);
        Q = Polygon(q1);
    }

    void createContainedPolygons() {
        // Larger Rectangle P
        Vertex *p1 = new Vertex(0, 0);
        Vertex *p2 = new Vertex(5, 0);
        Vertex *p3 = new Vertex(5, 5);
        Vertex *p4 = new Vertex(0, 5);
        p1->insert(p4)->insert(p3)->insert(p2);
        P = Polygon(p1);

        // Smaller Rectangle Q inside P
        Vertex *q1 = new Vertex(1, 1);
        Vertex *q2 = new Vertex(3, 1);
        Vertex *q3 = new Vertex(3, 3);
        Vertex *q4 = new Vertex(1, 3);
        q1->insert(q4)->insert(q3)->insert(q2);
        Q = Polygon(q1);
    }
};

// Test the aimsAt function
TEST_F(IntersectionTest, AimsAt) {
    // Defining edge q as horizontal from the figure
    Edge q(Point(0, 0), Point(10, 0));
    Point p;

    // Corrected thickened edges (should return true for aimsAt)
    Edge thickenedEdge1(Point(-2, 1), Point(1, 0.2)); // Approaching q from the top left
    Edge thickenedEdge2(Point(12, -1), Point(9, -0.2)); // Approaching q from the bottom right

    POINT_CLASSIFICATION_LINE aclass = thickenedEdge1.dest.classify(q);
    EDGE_CLASSIFICATION crossType = Intersection::crossingPoint(thickenedEdge1, q, p);
    EXPECT_TRUE(Intersection::aimsAt(thickenedEdge1, q, aclass, crossType));

    aclass = thickenedEdge2.dest.classify(q);
    crossType = Intersection::crossingPoint(thickenedEdge2, q, p);
    EXPECT_TRUE(Intersection::aimsAt(thickenedEdge2, q, aclass, crossType));

    // Non-thickened edges (should return false for aimsAt)
    Edge nonThickenedEdge1(Point(0, 1), Point(10, 1.0005)); // almost Parallel to q and above
    Edge nonThickenedEdge2(Point(0, -1), Point(10, -1)); // Parallel to q and below
    Edge nonThickenedEdge3(Point(-2, -1), Point(1, 1)); // Already crossed q
    Edge nonThickenedEdge4(Point(12, 1), Point(9, 2)); // going up and left
    Edge nonThickenedEdge5(Point(12, 1), Point(14, 3)); // going up and right

    aclass = nonThickenedEdge1.dest.classify(q);
    crossType = Intersection::crossingPoint(nonThickenedEdge1, q, p);
    EXPECT_FALSE(Intersection::aimsAt(nonThickenedEdge1, q, aclass, crossType));

    aclass = nonThickenedEdge2.dest.classify(q);
    crossType = Intersection::crossingPoint(nonThickenedEdge2, q, p);
    EXPECT_FALSE(Intersection::aimsAt(nonThickenedEdge2, q, aclass, crossType));

    aclass = nonThickenedEdge3.dest.classify(q);
    crossType = Intersection::crossingPoint(nonThickenedEdge3, q, p);
    EXPECT_FALSE(Intersection::aimsAt(nonThickenedEdge3, q, aclass, crossType));

    aclass = nonThickenedEdge4.dest.classify(q);
    crossType = Intersection::crossingPoint(nonThickenedEdge4, q, p);
    EXPECT_FALSE(Intersection::aimsAt(nonThickenedEdge4, q, aclass, crossType));

    aclass = nonThickenedEdge5.dest.classify(q);
    crossType = Intersection::crossingPoint(nonThickenedEdge5, q, p);
    EXPECT_FALSE(Intersection::aimsAt(nonThickenedEdge5, q, aclass, crossType));
}




// Test intersting point
TEST_F(IntersectionTest, InterestingPoint) {
    Point p;
    EDGE_CLASSIFICATION edgeType= Intersection::crossingPoint(*e1_cross, *e2_cross, p);
	EXPECT_EQ(intersectionPoint, p);
    EXPECT_EQ(edgeType, EDGE_CLASSIFICATION::SKEW_CROSS);

    edgeType = Intersection::crossingPoint(*e1_parallel, *e2_parallel, p);
    EXPECT_EQ(edgeType, EDGE_CLASSIFICATION::PARALLEL);

    edgeType = Intersection::crossingPoint(*e1_skew_no_cross, *e2_skew_no_cross, p);
    EXPECT_EQ(edgeType, EDGE_CLASSIFICATION::SKEW_NO_CROSS);

    edgeType = Intersection::crossingPoint(*e1_colinear, *e2_colinear, p);
    EXPECT_EQ(edgeType, EDGE_CLASSIFICATION::EDGE_COLLINEAR);

}

// Test isPointInsideConvexPolygon
TEST_F(IntersectionTest, IsPointInsideConvexPolygon) {
	// Rectangle P
	Vertex *p1 = new Vertex(0, 0);
	Vertex *p2 = new Vertex(1, 0);
	Vertex *p3 = new Vertex(1, 1);
	Vertex *p4 = new Vertex(0, 1);
	p1->insert(p4)->insert(p3)->insert(p2);
	Polygon poly = Polygon(p1);

	// Point inside P
	Point p(0.5, 0.5);
	EXPECT_TRUE(GeometryUtils::isPointInsideConvexPolygon(p, poly));

    // Point inside P
    p.x = 0.99; p.y = 0.89;
    EXPECT_TRUE(GeometryUtils::isPointInsideConvexPolygon(p, poly));

	// Point outside P
	Point q(2, 2);
	EXPECT_FALSE(GeometryUtils::isPointInsideConvexPolygon(q, poly));

    q.x=-2; q.y=-2;
    EXPECT_FALSE(GeometryUtils::isPointInsideConvexPolygon(q, poly));

    q.x = -2; q.y=2;
    EXPECT_FALSE(GeometryUtils::isPointInsideConvexPolygon(q, poly));

    q.x = 2; q.y=-2;
    EXPECT_FALSE(GeometryUtils::isPointInsideConvexPolygon(q, poly));
    // Point on the boundary
    // Point on the left edge
    Point r(0, 0.5);
    EXPECT_TRUE(GeometryUtils::isPointInsideConvexPolygon(r, poly));

    // Point on the right edge
    Point s(1, 0.5);
    EXPECT_TRUE(GeometryUtils::isPointInsideConvexPolygon(s, poly));

    // Point on the top edge
    Point t(0.5, 1);
    EXPECT_TRUE(GeometryUtils::isPointInsideConvexPolygon(t, poly));

    // Point on the bottom edge
    Point u(0.5, 0);
    EXPECT_TRUE(GeometryUtils::isPointInsideConvexPolygon(u, poly));
}

// partially overlapping polygon intersection test
TEST_F(IntersectionTest, PartiallyOverlappingPolygons) {
	createPartiallyOverlappingPolygons();
	Polygon* R = Intersection::convexPolygonIntersect(P, Q);
	EXPECT_EQ(R->size(), 4);
    std::vector<Point> points = R->getPoints();
	EXPECT_EQ(points[0], Point(0.5, 0));
    EXPECT_EQ(points[1], Point(0.5, 1));
    EXPECT_EQ(points[2], Point(0.75, 1));
    EXPECT_EQ(points[3], Point(1, 0));
}

// non-overlapping polygon intersection test
TEST_F(IntersectionTest, NonOverlappingPolygons) {
	createNonOverlappingPolygons();
	Polygon* R = Intersection::convexPolygonIntersect(P, Q);
	EXPECT_EQ(R->size(), 0);
    EXPECT_EQ(R->v(), nullptr);
}

// contained polygon intersection test
TEST_F(IntersectionTest, ContainedPolygons) {
	createContainedPolygons();
	Polygon* R = Intersection::convexPolygonIntersect(P, Q);
	EXPECT_EQ(R->size(), 4);
	std::vector<Point> points = R->getPoints();
    std::vector<Point> expectedPoints = Q.getPoints();
	EXPECT_EQ(points[0], expectedPoints[0]);
	EXPECT_EQ(points[1], expectedPoints[1]);
	EXPECT_EQ(points[2], expectedPoints[2]);
	EXPECT_EQ(points[3], expectedPoints[3]);
}
