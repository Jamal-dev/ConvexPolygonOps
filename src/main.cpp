// PolygonOpts.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <gtest/gtest.h>
//#include <vld.h>
#include "Vertex.hpp"
#include "Edge.hpp"
#include "Polygon.hpp"
#include "GeometryUtils.hpp"
#include "Intersection.hpp"
#include "ConvexHull.hpp"
#include "PlaneSweep.hpp"
#include "PolygonOperations.hpp"
#include "PolygonIO.hpp"

double PlaneSweep::Algo::curx = 0;  // or any default value you'd like to initialize it with
double PlaneSweep::Algo::EPSILON3 = 1e-10;


// Definition of the << operator for Point
std::ostream& operator<<(std::ostream& os, const Point& pt) {
	os << '(' << pt.x << ", " << pt.y << ')';
	return os;
}


int main(int argc, char* argv[]) {
    std::string mode = "input";  // Default mode
    if (argc > 1) {
        mode = argv[1];
    }

    if (mode == "test") {
        std::cout << "Running in test mode..."<<std::endl;
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    }
    else {
        std::cout << "Running in input mode. (Run with 'test' argument to enter test mode)"<<std::endl;
        runPolygonOperations();
    }

    return 0;
}




