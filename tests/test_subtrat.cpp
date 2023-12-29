#include <gtest/gtest.h>
#include "Point.hpp"
#include "Vertex.hpp"
#include "Polygon.hpp" 
#include "GeometryUtils.hpp"
#include "PolygonOperations.hpp"
#include <vector>

class PolygonSubtractionTest : public ::testing::Test {
protected:
    Polygon A, B; // Polygons to use in tests

    void SetUp() override {
        // Set up code for the polygons A and B if necessary
    }

    void TearDown() override {
        // Clean up code if necessary
    }
};

TEST_F(PolygonSubtractionTest, BasicSubtraction) {
    // Set up a basic case where polygon B is completely inside A
    std::vector<Point> a = { Point(0,0),Point(0,1),Point(1,1),Point(1,0) };
    std::vector<Point> b = { Point(0.5,-1),Point(0.5,2),Point(1.5,2),Point(1.5,-1) };
    A = GeometryUtils::createPolygon(a);
    B = GeometryUtils::createPolygon(b);
    Polygon AA = GeometryUtils::createPolygon(a);
    Polygon BB = GeometryUtils::createPolygon(b);

    Polygon aDIFFb = differenceConvexPolygons(A, B);
    Polygon bDIFFa = differenceConvexPolygons(BB, AA);
    
    

    // Expected result: Polygon A without B
    std::vector<Point> expectedVertices_aDIFFb = {Point(0,0),Point(0,1),Point(0.5,1),Point(0.5,0)};
    EXPECT_TRUE(GeometryUtils::comparePolygons(aDIFFb, expectedVertices_aDIFFb));

    // Expected result: Polygon B without A
    std::vector<Point> expectedVertices_bDIFFa = { Point(0.5,-1),Point(0.5,0),Point(1,0),Point(1,1),Point(0.5,1),Point(0.5,2),Point(1.5,2),Point(1.5,-1) };
    EXPECT_TRUE(GeometryUtils::comparePolygons(bDIFFa, expectedVertices_bDIFFa));
}

TEST_F(PolygonSubtractionTest, NoOverlap) {
    // Set up a case where A and B do not overlap
    std::vector<Point> a = { Point(0,0),Point(0,1),Point(1,1),Point(1,0) };
    std::vector<Point> b = { Point(2,0),Point(2,1),Point(3,1),Point(3,0) };
    A = GeometryUtils::createPolygon(a);
    B = GeometryUtils::createPolygon(b);

    Polygon result = differenceConvexPolygons(A, B);

    // Expected result: Polygon A unchanged
    EXPECT_TRUE(GeometryUtils::comparePolygons(result, A.getPoints()));

    // Expected result: Polygon B unchanged
    result = differenceConvexPolygons(B, A);
    EXPECT_TRUE(GeometryUtils::comparePolygons(result, B.getPoints()));
}

TEST_F(PolygonSubtractionTest, SharedEdge) {
    // Set up a case where A and B share an edge
    std::vector<Point> a = { Point(0,0),Point(0,1),Point(1,1),Point(1,0) };
    std::vector<Point> b = { Point(1,0),Point(1,1),Point(2,1),Point(2,0) };
    A = GeometryUtils::createPolygon(a);
    B = GeometryUtils::createPolygon(b);

    Polygon result = differenceConvexPolygons(A, B);

    // Expected result: specific to the test case
    std::vector<Point> expectedResult = { Point(0,0), Point(0,1),Point(1,1),Point(1,0) };
    EXPECT_TRUE(GeometryUtils::comparePolygons(result, expectedResult));
}

//TEST_F(PolygonSubtractionTest, ComplexCase) {
//    // Set up a more complex case with multiple intersections
//    std::vector<Point> a = { Point(0,0),Point(0,1),Point(1,1),Point(1,0) };
//    std::vector<Point> b = { Point(0.5,0),Point(0.5,1),Point(1,0) };
//    A = GeometryUtils::createPolygon(a);
//    B = GeometryUtils::createPolygon(b);
//
//    Polygon result = differenceConvexPolygons(A, B);
//
//    // Expected result: specific to the test case
//    std::vector<Point> expectedVertices = {Point()};
//    EXPECT_TRUE(GeometryUtils::comparePolygons(result, expectedVertices));
//}




