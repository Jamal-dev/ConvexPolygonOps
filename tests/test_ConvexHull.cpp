#include <gtest/gtest.h>
#include "ConvexHull.hpp"
#include "Point.hpp"
#include "Polygon.hpp"
#include "GeometryUtils.hpp"
#include <vector>


class ConvexHullTest : public ::testing::Test {
protected:
    // Helper function to check if a point is in the result
    bool isPointInResult(const Point& p,Polygon& poly) {
        POINT_CLASSIFICATION_POLYGON result = GeometryUtils::isPointInPolygon(p, poly);
        if (result == POINT_CLASSIFICATION_POLYGON::INSIDE || result == POINT_CLASSIFICATION_POLYGON::BOUNDARY)
			return true;
        else
            return false;
    }
};

TEST_F(ConvexHullTest, EmptyInput) {
    std::vector<Point> points;
    Polygon result = ConvexHull::grahamscan(points);
    ASSERT_EQ(result.v(), nullptr);  // Assuming Polygon class has isEmpty method
}

TEST_F(ConvexHullTest, SinglePoint) {
    std::vector<Point> points = { {0, 0} };
    Polygon result = ConvexHull::grahamscan(points);
    ASSERT_EQ(result.size(), 1);
}

// ... Additional tests for TwoPoints, CollinearPoints, etc. ...
TEST_F(ConvexHullTest, TwoPoints) {
    std::vector<Point> points = { {0, 0}, {1, 1} };
    Polygon result = ConvexHull::grahamscan(points);
    ASSERT_EQ(result.size(), 2);
    ASSERT_TRUE(isPointInResult(points[0], result));
    ASSERT_TRUE(isPointInResult(points[1], result));
}

TEST_F(ConvexHullTest, CollinearPoints) {
    std::vector<Point> points = { {0, 0}, {1, 1}, {2, 2}, {3, 3} };
    Polygon result = ConvexHull::grahamscan(points);
    ASSERT_EQ(result.size(), 2);
    ASSERT_TRUE(isPointInResult(points[0], result));
    ASSERT_TRUE(isPointInResult(points[3], result));
}

TEST_F(ConvexHullTest, SimplePolygonTriangle) {
    std::vector<Point> points = { {0, 0}, {1, 0}, {0, 1} };
    Polygon result = ConvexHull::grahamscan(points);
    ASSERT_EQ(result.size(), 3);
    for (const auto& p : points) {
        ASSERT_TRUE(isPointInResult(p, result));
    }
}


TEST_F(ConvexHullTest, ComplexPolygon) {
    std::vector<Point> points = { {0, 0}, {1, 0}, {1, 1}, {0, 1}, {0.5, 0.5}, {2, 2} };
    Polygon result = ConvexHull::grahamscan(points);
    std::vector<Point> expectedHull = { {0, 0}, {1, 0}, {2, 2}, {0, 1} };
    for (const auto& p : expectedHull) {
        ASSERT_TRUE(isPointInResult(p, result));
    }
}

TEST_F(ConvexHullTest, PointsFormingCircle) {
    std::vector<Point> points;
    // Generate points on a circle
    for (int i = 0; i < 8; ++i) {
        double angle = 2.0 * M_PI * i / 8.0;
        points.push_back({ cos(angle), sin(angle) });
    }
    Polygon result = ConvexHull::grahamscan(points);
    ASSERT_EQ(result.size(), 8);
    for (const auto& p : points) {
        ASSERT_TRUE(isPointInResult(p, result));
    }
}

TEST_F(ConvexHullTest, DuplicatePoints) {
    std::vector<Point> points = { {0, 0}, {1, 1}, {1, 1}, {0, 1}, {1, 0}, {0, 0} };
    Polygon result = ConvexHull::grahamscan(points);
    std::vector<Point> expectedHull = { {0, 0}, {1, 0}, {1, 1}, {0, 1} };
    ASSERT_EQ(result.size(), 4);
    for (const auto& p : expectedHull) {
        ASSERT_TRUE(isPointInResult(p, result));
    }
}

