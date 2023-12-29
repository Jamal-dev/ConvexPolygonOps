#include <gtest/gtest.h>
#include "Vertex.hpp"
#include "GeometryUtils.hpp"

class VertexTest : public ::testing::Test {
protected:
    Vertex* v1, * v2, * v3, * v4;

    void SetUp() override {
        v1 = new Vertex(1, 1);
        v2 = new Vertex(2, 2);
        v3 = new Vertex(3, 3);
        v4 = new Vertex(4, 4);
    }

    void TearDown() override {
        delete v1;
        delete v2;
        delete v3;
        delete v4;
    }
};

// Test the constructor and point inheritance
TEST_F(VertexTest, ConstructorAndPointInheritance) {
    EXPECT_EQ(v1->x, 1);
    EXPECT_EQ(v1->y, 1);
    Point p(5, 5);
    Vertex v(p);
    EXPECT_EQ(v.x, 5);
    EXPECT_EQ(v.y, 5);
}

// Test the insert method and cw and ccw methods
TEST_F(VertexTest, ClockwiseCounterClockwise) {
    v1->insert(v2)->insert(v3);
    EXPECT_EQ(v1->cw(), v2);
    EXPECT_EQ(v2->cw(), v3);
    EXPECT_EQ(v3->cw(), v1); // Assuming circular list
    EXPECT_EQ(v1->ccw(), v3);
    EXPECT_EQ(v2->ccw(), v1);
    EXPECT_EQ(v3->ccw(), v2);
}

// Test neighbor method
TEST_F(VertexTest, Neighbor) {
    v1->insert(v2);
    EXPECT_EQ(v1->neighbor(ORIENTATION::CLOCKWISE), v2);
    EXPECT_EQ(v2->neighbor(ORIENTATION::COUNTERCLOCKWISE), v1);
}

// Test insert and remove methods
TEST_F(VertexTest, InsertAndRemove) {
    v1->insert(v2);
    EXPECT_EQ(v1->cw(), v2);
    EXPECT_EQ(v2->ccw(), v1);

    v2->remove();
    EXPECT_EQ(v1->cw(), v1); // v1 should now point to itself
    EXPECT_EQ(v2->cw(), v2); // v2 should be standalone
}

// Test the splice method
TEST_F(VertexTest, Splice) {
    v1->insert(v2);
    v3->insert(v4);
    v1->splice(v3);
    // Should link v1 to v4 and v3 to v2, making the list v1 <-> v4 <-> v3 <-> v2 (circular)
    EXPECT_EQ(v1->cw(), v4);
    EXPECT_EQ(v4->ccw(), v1);
    EXPECT_EQ(v4->cw(), v3);
    EXPECT_EQ(v3->ccw(), v4);
    EXPECT_EQ(v3->cw(), v2);
    EXPECT_EQ(v2->ccw(), v3);
    EXPECT_EQ(v2->cw(), v1);
    EXPECT_EQ(v1->ccw(), v2);
}

// Test the split method
TEST_F(VertexTest, Split) {
    v1->insert(v2); // Ensure v1 and v2 are linked

    Vertex* bp = v1->split(v2);
    // first it should arrange
    // v1->ap->bp->v2 then will split
    // v1->v2 ; ap->bp
    // copy of v1 ap
    // copy of v2 bp
    // v1->v2
    // ap->bp
    // it will split
    // ap->bp ; v1->v2
    // Check if the vertex after v1 is a new vertex (ap), not v2
    Vertex* ap = dynamic_cast<Vertex *>( bp->ccw());
    EXPECT_NE(ap, v1);
    EXPECT_NE(bp, v2);

    // Check if bp is after ap
    EXPECT_EQ(ap->cw(), bp);

    // Similarly, check if ap is before bp
    EXPECT_EQ(bp->ccw(), ap);

    // now check same for v1 and v2
    // Check if v2 is after v1
    EXPECT_EQ(v1->cw(), v2);
    EXPECT_EQ(v2->ccw(), v1);

    // Clean up the new vertices
    delete ap; // Delete ap
    delete bp; // Delete bp
}




// Test the point method
TEST_F(VertexTest, PointMethod) {
    Point p = v1->point();
    EXPECT_EQ(p.x, v1->x);
    EXPECT_EQ(p.y, v1->y);
}
