#include <gtest/gtest.h>
#include "Vertex.hpp"
#include "Edge.hpp"
#include "Polygon.hpp"
#include "GeometryUtils.hpp"

// Test the Polygon class
class PolygonTest : public ::testing::Test {
protected:
    Polygon* poly;
    Vertex* v1, * v2, * v3;

    void SetUp() override {
        v1 = new Vertex(0, 0);
        v2 = new Vertex(1, 0);
        v3 = new Vertex(0, 1);
        v1->insert(v2)->insert(v3); // Creating a triangular polygon
        poly = new Polygon(v1);
    }

    void TearDown() override {
        delete poly;
    }
};

// Test the constructor
TEST_F(PolygonTest, Constructors) {
    poly;
    Polygon defaultPoly;
    EXPECT_EQ(defaultPoly.size(), 0);
    Vertex* v_1 = new Vertex(0, 0);
    Vertex* v_2 = new Vertex(1, 0);
    Vertex* v_3 = new Vertex(0, 1);
    v_1->insert(v_2)->insert(v_3); // Creating a triangular polygon
    Polygon trianglePoly(v_1);
    EXPECT_EQ(trianglePoly.size(), 3);
}

// Test the copy constructor
TEST_F(PolygonTest, CopyConstructor) {
    Polygon polyCopy(*poly);
    EXPECT_EQ(polyCopy.size(), poly->size());
    EXPECT_EQ(polyCopy.point(), poly->point());
}

// Test the destructor
TEST_F(PolygonTest, Destructor) {
    Polygon* polyToDelete = new Polygon(*poly);
    delete polyToDelete; 
    // Expect no memory leaks or crashes
}

// Test the resize method
TEST_F(PolygonTest, Resize) {
    poly->insert(Point(2, 2));
    EXPECT_EQ(poly->size(), 4);
    poly->remove();
    EXPECT_EQ(poly->size(), 3);
}

// Test the insert and remove method
TEST_F(PolygonTest, InsertAndRemove) {
    Vertex* v4 = new Vertex(1, 1);
    poly->insert(v4->point());
    EXPECT_EQ(poly->size(), 4);

    poly->remove(); // Assuming it removes the last added vertex
    EXPECT_EQ(poly->size(), 3);
}


// Test the point method
TEST_F(PolygonTest, Point) {
	EXPECT_EQ(poly->point(), v1->point());
}

// Test the edge method
TEST_F(PolygonTest, Edge) {
	EXPECT_EQ(poly->edge(), Edge(v1->point(), v2->point()));
}

// Test the advance method
TEST_F(PolygonTest, Advance) {
	EXPECT_EQ(poly->advance(ORIENTATION::CLOCKWISE)->point(), v2->point());
	EXPECT_EQ(poly->advance(ORIENTATION::COUNTERCLOCKWISE)->point(), v1->point());
}

// Test the getters
TEST_F(PolygonTest, Getters) {
    EXPECT_EQ(poly->v()->point(), v1->point());
    EXPECT_EQ(poly->size(), 3);
    EXPECT_EQ(poly->point(), v1->point());
    EXPECT_EQ(poly->edge(), Edge(v1->point(), v1->cw()->point()));
    EXPECT_EQ(poly->cw()->point(), v1->cw()->point());
    EXPECT_EQ(poly->ccw()->point(), v1->ccw()->point());
    EXPECT_EQ(poly->neighbor(ORIENTATION::CLOCKWISE)->point(), v1->cw()->point());
}

// Test the setV and advance method
TEST_F(PolygonTest, AdvanceAndSetV) {
    Vertex* nextVertex = poly->advance(ORIENTATION::CLOCKWISE);
    EXPECT_EQ(nextVertex->point(), v2->point());
    EXPECT_EQ(poly->v()->point(), v2->point());
    poly->setV(v3);
    EXPECT_EQ(poly->v()->point(), v3->point());
}

// Test the split method
TEST_F(PolygonTest, Split) {
	Polygon* splitPoly = poly->split(v2);
	EXPECT_EQ(splitPoly->size(), 2);
	EXPECT_EQ(poly->size(), 3);
	EXPECT_EQ(splitPoly->point(), v2->point());
	EXPECT_EQ(poly->point(), v1->point());
}
