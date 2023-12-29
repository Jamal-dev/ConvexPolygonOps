#include <gtest/gtest.h>
#include "Vertex.hpp"
#include "Edge.hpp"
#include "GeometryUtils.hpp"


class EdgeTest : public ::testing::Test {
protected:
    Edge e1, e2, e3, e4;

    void SetUp() override {
        // Initialize edges with different points
        e1 = Edge(Point(0, 0), Point(1, 1)); // Diagonal edge
        e2 = Edge(Point(1, 0), Point(1, 2)); // Vertical edge
        e3 = Edge(Point(0, 1), Point(2, 1)); // Horizontal edge
        e4 = Edge(Point(0, 1), Point(1, 0)); // Another diagonal edge
    }
};

// Test the constructor
TEST_F(EdgeTest, ConstructorAndProperties) {
    EXPECT_EQ(e1.org.x, 0);
    EXPECT_EQ(e1.org.y, 0);
    EXPECT_EQ(e1.dest.x, 1);
    EXPECT_EQ(e1.dest.y, 1);
}

// Test length and midpoint method
TEST_F(EdgeTest, LengthAndMidpoint) {
    EXPECT_DOUBLE_EQ(e1.length(), std::sqrt(2));
    Point mid = e1.midpoint();
    EXPECT_DOUBLE_EQ(mid.x, 0.5);
    EXPECT_DOUBLE_EQ(mid.y, 0.5);
}

// Test translation
TEST_F(EdgeTest, Translation) {
    e1.trans(Point(1, 1)); // Translate by (1,1)
    EXPECT_EQ(e1.org.x, 1);
    EXPECT_EQ(e1.org.y, 1);
    EXPECT_EQ(e1.dest.x, 2);
    EXPECT_EQ(e1.dest.y, 2);
}

// Test scale
TEST_F(EdgeTest, Scale) {
    e1.scale(2); // Scale by a factor of 2
    // Check if the length is doubled
    EXPECT_DOUBLE_EQ(e1.length(), 2 * std::sqrt(2));
}

// Test flip
TEST_F(EdgeTest, Flip) {
    Point originalOrg = e1.org;
    Point originalDest = e1.dest;
    e1.flip();
    EXPECT_EQ(e1.org, originalDest);
    EXPECT_EQ(e1.dest, originalOrg);
}

// Test parallel, perpendicular, and same
TEST_F(EdgeTest, ParallelPerpendicularSame) {
    EXPECT_TRUE(e2.isVertical());
    EXPECT_TRUE(e3.isHorizontal());
    EXPECT_FALSE(e1.isHorizontal());
    EXPECT_FALSE(e1.isVertical());
    EXPECT_TRUE(e2.isParallel(Edge(Point(2, 0), Point(2, 3))));
    EXPECT_TRUE(e2.isPerpendicular(e3));
    EXPECT_TRUE(e1.isSame(Edge(Point(0, 0), Point(1, 1))));
}

// Test intersection
TEST_F(EdgeTest, Intersection) {
    Edge e4(Point(0, 1), Point(1, 0));
    Point intersectPoint = e1.intersection(e4);
    EXPECT_DOUBLE_EQ(intersectPoint.x, 0.5);
    EXPECT_DOUBLE_EQ(intersectPoint.y, 0.5);
}

// Test slope and yIntercept
TEST_F(EdgeTest, SlopeYIntercept) {
    EXPECT_DOUBLE_EQ(e1.slope(), 1);
    EXPECT_DOUBLE_EQ(e1.yIntercept(), 0);
}

// Test rotate around
TEST_F(EdgeTest, RotateAround) {
    Point center(0, 0);
    e1.rotateAround(center, M_PI / 2); // Rotate 90 degrees around origin
    EXPECT_NEAR(e1.org.x, 0, 1e-6);
    EXPECT_NEAR(e1.org.y, 0, 1e-6);
    EXPECT_NEAR(e1.dest.x, -1, 1e-6);
    EXPECT_NEAR(e1.dest.y, 1, 1e-6);
}

// signed distance
TEST_F(EdgeTest, SignedDistanceFromEdgeAndPoint) {
    Edge e4(Point(0, 2), Point(2, 2)); // Parallel edge above e3
    EXPECT_DOUBLE_EQ(e3.signedDistance(e4), 1);

    Point p(0, 2); // Point above e3
    EXPECT_DOUBLE_EQ(e3.signedDistance(p), 1);
}

// Test colinear edges
TEST_F(EdgeTest, CollinearEdges) {
    Edge e5(Point(2, 2), Point(3, 3)); // Collinear with e1
    double t;
    auto result = e1.intersect(e5, t);
    EXPECT_EQ(result, EDGE_CLASSIFICATION::EDGE_COLLINEAR);
}

// Test parallel edges
TEST_F(EdgeTest, ParallelEdges) {
    Edge e5(Point(2, 0), Point(2, 2)); // Parallel to e2
    double t;
    auto result = e2.intersect(e5, t);
    EXPECT_EQ(result, EDGE_CLASSIFICATION::PARALLEL);
}

// Test skew edges
TEST_F(EdgeTest, SkewEdges) {
    double t;
    auto result = e1.intersect(e4, t);
    EXPECT_EQ(result, EDGE_CLASSIFICATION::SKEW);
    // Check the parametric value t if needed
}

// Test skew cross edges
TEST_F(EdgeTest, SkewCrossEdges) {
    double t;
    auto result = e1.cross(e4, t);
    EXPECT_EQ(result, EDGE_CLASSIFICATION::SKEW_CROSS);
    // The intersection should be at (0.5, 0.5)
    EXPECT_DOUBLE_EQ(t, 0.5);
}

// Test skew no cross edges
TEST_F(EdgeTest, SkewNoCrossEdges) {
    Edge e5(Point(2, 0), Point(3, -1)); // Not crossing e1
    double t;
    auto result = e1.cross(e5, t);
    EXPECT_EQ(result, EDGE_CLASSIFICATION::SKEW_NO_CROSS);
}
