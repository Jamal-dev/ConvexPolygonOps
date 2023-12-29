#include <gtest/gtest.h>
#include "Point.hpp"
#include "Edge.hpp"
#include "GeometryUtils.hpp"
#include <cmath>

class PointTest : public ::testing::Test {
protected:
    Point p1, p2, p3;

    void SetUp() override {
        p1 = Point(3, 4);
        p2 = Point(1, 2);
        p3 = Point(0, 0); // Origin
    }
};

// Test the constructor
TEST_F(PointTest, Constructor) {
    EXPECT_EQ(p1.x, 3);
    EXPECT_EQ(p1.y, 4);
}

// Test the copy constructor
TEST_F(PointTest, CopyConstructor) {
    Point p4(p1);
    EXPECT_EQ(p4.x, p1.x);
    EXPECT_EQ(p4.y, p1.y);
}

// Test the assignment operator
TEST_F(PointTest, AssignmentOperator) {
    Point p4 = p1;
    EXPECT_EQ(p4, p1);
}

// Test the operator[]
TEST_F(PointTest, AccessOperator) {
    EXPECT_EQ(p1[0], 3);
    EXPECT_EQ(p1[1], 4);
}

// Test the operator+=
TEST_F(PointTest, PlusEqualsOperator) {
    p1 += p2;
    EXPECT_EQ(p1.x, 4);
    EXPECT_EQ(p1.y, 6);
}

// Test the operator-=
TEST_F(PointTest, MinusEqualsOperator) {
    p1 -= p2;
    EXPECT_EQ(p1.x, 2);
    EXPECT_EQ(p1.y, 2);
}

// Test the operator+
TEST_F(PointTest, AdditionOperator) {
    Point p4 = p1 + p2;
    EXPECT_EQ(p4.x, 4);
    EXPECT_EQ(p4.y, 6);
}

// Test the operator-
TEST_F(PointTest, SubtractionOperator) {
    Point p4 = p1 - p2;
    EXPECT_EQ(p4.x, 2);
    EXPECT_EQ(p4.y, 2);
}

// Test the negate operator
TEST_F(PointTest, NegateOperator) {
    Point p4 = -p1;
    EXPECT_EQ(p4.x, -p1.x);
    EXPECT_EQ(p4.y, -p1.y);
}

// Test the operator* (scalar multiplication)
TEST_F(PointTest, ScalarMultiplicationOperator) {
    double scalar = 2.0;
    Point p4 = p1 * scalar;
    EXPECT_EQ(p4.x, p1.x * scalar);
    EXPECT_EQ(p4.y, p1.y * scalar);
}

// Test the operator/ (scalar division)
TEST_F(PointTest, ScalarDivisionOperator) {
    double scalar = 2.0;
    Point p4 = p1 / scalar;
    EXPECT_EQ(p4.x, p1.x / scalar);
    EXPECT_EQ(p4.y, p1.y / scalar);
}

// Test the operator== and operator!=
TEST_F(PointTest, EqualityOperators) {
    EXPECT_TRUE(p1 == Point(3, 4));
    EXPECT_FALSE(p1 != Point(3, 4));
    EXPECT_TRUE(p1 != p2);
    EXPECT_FALSE(p1 == p2);
}

// Test the magnitude and length functions
TEST_F(PointTest, MagnitudeLength) {
    EXPECT_DOUBLE_EQ(p1.magnitude(), 5.0);
    EXPECT_DOUBLE_EQ(p1.length(), 5.0); // Assuming length() and magnitude() are the same
}

// Test the normalize function
TEST_F(PointTest, Normalize) {
    Point p4 = p1.normalize();
    double mag = p1.magnitude();
    EXPECT_DOUBLE_EQ(p4.x, p1.x / mag);
    EXPECT_DOUBLE_EQ(p4.y, p1.y / mag);
}

// Test the dot product
TEST_F(PointTest, DotProduct) {
    double dot = p1.dot(p2);
    EXPECT_DOUBLE_EQ(dot, p1.x * p2.x + p1.y * p2.y);
}

// Test the cross product
TEST_F(PointTest, CrossProduct) {
    double cross = p1.cross(p2);
    EXPECT_DOUBLE_EQ(cross, p1.x * p2.y - p1.y * p2.x);
}

// Test the static distance function
TEST_F(PointTest, DistanceFunction) {
    double dist = Point::distance(p1, p2);
    EXPECT_DOUBLE_EQ(dist, std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2)));
}

// rotate() rotates a point around the origin (0,0)
TEST_F(PointTest, Rotate) {
    double theta = M_PI / 4; // 45 degrees
    Point p4 = p3.rotate(theta);
}

// rotateDegrees() is similar to rotate() but takes degrees
TEST_F(PointTest, RotateDegrees) {
    double theta = 45; // 45 degrees
    Point p4 = p3.rotateDegrees(theta);
}

// polarAngle() returns the angle of the point in radians
TEST_F(PointTest, PolarAngle) {
	double theta = p1.polarAngle();
	EXPECT_DOUBLE_EQ(theta, std::atan2(p1.y, p1.x));
}

// polarAngleDegrees() returns the angle of the point in degrees
TEST_F(PointTest, PolarAngleDegrees) {
	double theta = p1.polarAngleDegrees();
	EXPECT_DOUBLE_EQ(theta, std::atan2(p1.y, p1.x) * 180 / M_PI);
}

// classify() returns the location of the point relative to a line
TEST_F(PointTest, ClassifyWithTwoPoints) {
    Point p0(0, 0), p1(5, 5), leftPoint(1, 2), rightPoint(2, 1), behindPoint(-1, -1),
        beyondPoint(6, 6), betweenPoint(2, 2), originPoint(0, 0), destinationPoint(5, 5);

    EXPECT_EQ(leftPoint.classify(p0, p1), POINT_CLASSIFICATION_LINE::LEFT);
    EXPECT_EQ(rightPoint.classify(p0, p1), POINT_CLASSIFICATION_LINE::RIGHT);
    EXPECT_EQ(behindPoint.classify(p0, p1), POINT_CLASSIFICATION_LINE::BEHIND);
    EXPECT_EQ(beyondPoint.classify(p0, p1), POINT_CLASSIFICATION_LINE::BEYOND);
    EXPECT_EQ(betweenPoint.classify(p0, p1), POINT_CLASSIFICATION_LINE::BETWEEN);
    EXPECT_EQ(originPoint.classify(p0, p1), POINT_CLASSIFICATION_LINE::ORIGIN);
    EXPECT_EQ(destinationPoint.classify(p0, p1), POINT_CLASSIFICATION_LINE::DESTINATION);
}

// classify() returns the location of the point relative to a line segment
TEST_F(PointTest, ClassifyWithEdge) {
    Point p0(0, 0), p1(5, 5);
    Edge edge(p0, p1);
    Point leftPoint(1, 2);
    Point rightPoint(2, 1);
    Point behindPoint(-1, -1);
    Point beyondPoint(6, 6);
    Point betweenPoint(2, 2);
    Point originPoint(0, 0);
    Point destinationPoint(5, 5);

    EXPECT_EQ(leftPoint.classify(edge), LEFT);
    EXPECT_EQ(rightPoint.classify(edge), RIGHT);
    EXPECT_EQ(behindPoint.classify(edge), BEHIND);
    EXPECT_EQ(beyondPoint.classify(edge), BEYOND);
    EXPECT_EQ(betweenPoint.classify(edge), BETWEEN);
    EXPECT_EQ(originPoint.classify(edge), ORIGIN);
    EXPECT_EQ(destinationPoint.classify(edge), DESTINATION);
}

// test null point
TEST_F(PointTest, NullPoint) {
    Point nullPoint = Point::null();
    EXPECT_DOUBLE_EQ(nullPoint.x, DBL_MAX);
    EXPECT_DOUBLE_EQ(nullPoint.y, DBL_MAX);
}

// test isNull()
TEST_F(PointTest, IsNull) {
	Point nullPoint = Point::null();
	EXPECT_TRUE(nullPoint.isNull());
	EXPECT_FALSE(p1.isNull());
}

// rotate around a point
TEST_F(PointTest, RotateAround) {
    Point center(0, 0), p(1, 0);
    double theta = M_PI / 2; // 90 degrees

    p.rotateAround(center, theta);

    EXPECT_NEAR(p.x, 0, 1e-6);
    EXPECT_NEAR(p.y, 1, 1e-6);
    // Add more cases for different centers and angles
}


