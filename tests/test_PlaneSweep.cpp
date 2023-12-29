//#include <gtest/gtest.h>
//
//#include "PlaneSweep.hpp"
//#include "Point.hpp"
//#include <vector>
//
//
//
//class PlaneSweepTest : public ::testing::Test {
//protected:
//    PlaneSweep::Algo algo;
//
//    // Helper function to create edges
//    Edge createEdge(double x1, double y1, double x2, double y2) {
//        Point p1(x1, y1);
//        Point p2(x2, y2);
//        return Edge(p1, p2);
//    }
//
//    void SetUp() override {
//        // Setup code if needed
//
//    }
//
//    void TearDown() override {
//        // Cleanup code if needed
//    }
//};
//
//TEST_F(PlaneSweepTest, BasicFunctionality) {
//    std::vector<Edge> edges = {
//        createEdge(0, 0, 1, 1),
//        createEdge(1, 0, 0, 1)
//    };
//    auto result = algo.intersectSegments(edges);
//    // Expect 1 intersection point
//    //EXPECT_EQ(result.size(), 1);
//    // Additional checks on the intersection point can be added here
//}
//
//TEST_F(PlaneSweepTest, NoIntersections) {
//    std::vector<Edge> edges = {
//        createEdge(0, 0, 1, 1),
//        createEdge(2, 2, 3, 3)
//    };
//    auto result = algo.intersectSegments(edges);
//    //EXPECT_TRUE(result->isEmpty());
//}
//
//TEST_F(PlaneSweepTest, MultipleIntersections) {
//    std::vector<Edge> edges = {
//        // Define edges with multiple intersections
//    };
//    auto result = algo.intersectSegments(edges);
//    // Define your expectations for multiple intersections
//    //EXPECT_EQ(result->size(), expected_number_of_intersections);
//}
//
//TEST_F(PlaneSweepTest, CollinearEdges) {
//    std::vector<Edge> edges = {
//        // Define collinear edges
//    };
//    auto result = algo.intersectSegments(edges);
//    // Define your expectations for collinear edges
//}
//
//TEST_F(PlaneSweepTest, EdgeCases) {
//    std::vector<Edge> edges = {
//        // Define edges for edge cases
//    };
//    auto result = algo.intersectSegments(edges);
//    // Define your expectations for edge cases
//}
//
//TEST_F(PlaneSweepTest, LargeDataSet) {
//    std::vector<Edge> edges = {
//        // Define a large set of edges
//    };
//    //auto start = std::chrono::high_resolution_clock::now();
//    //auto result = algo.intersectSegments(edges.data(), edges.size());
//    //auto end = std::chrono::high_resolution_clock::now();
//    //std::chrono::duration<double> diff = end - start;
//    // Expect performance within certain threshold, e.g., time < 1 second
//    //EXPECT_LT(diff.count(), 1.0);
//}
//
//TEST_F(PlaneSweepTest, RandomizedEdges) {
//    std::vector<Edge> edges = {
//        // Generate random edges
//    };
//    auto result = algo.intersectSegments(edges);
//    // Expectations based on randomized edges
//}
//
//
