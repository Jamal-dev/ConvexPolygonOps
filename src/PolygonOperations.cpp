#include "PolygonOperations.hpp"
#include "Point.hpp"
#include "Vertex.hpp"
#include "Edge.hpp"

#include "Intersection.hpp"
#include "ConvexHull.hpp"
#include "Polygon.hpp"
#include "GeometryUtils.hpp"
#include <utility> 
#include <set>
#include <map>

bool isIntersectionPoint(const Point& point, const std::set<Point>& intersectionPoints) {
    return intersectionPoints.find(point) != intersectionPoints.end();
}

Vertex* findPositionToInsert(const Point& point, Polygon& A) {
    Vertex* current = A.v();
    Vertex* bestInsertPosition = nullptr;
    double minDistance = std::numeric_limits<double>::max();

    do {
        Edge currentEdge(current, current->cw());
        POINT_CLASSIFICATION_LINE classification = point.classify(currentEdge);

        if (classification == BETWEEN || classification == ORIGIN || classification == DESTINATION) {
            // The point lies on the current edge
            return current;
        }

        // Calculate the signed distance from the point to the current edge
        double distance = std::abs(currentEdge.signedDistance(point));
        if (distance < minDistance) {
            minDistance = distance;
            bestInsertPosition = current;
        }

        current = current->cw();
    } while (current != A.v());

    return bestInsertPosition;
}

bool vertexExistsInPolygon(const Point& point, const Polygon& polygon) {
    Vertex* start = polygon.v();
    Vertex* current = start;
    do {
        if (current->point() == point) {
            return true;
        }
        current = current->cw();
    } while (current != start);
    return false;
}

bool vertexExistsInPolygon(const Vertex* vertex, const Polygon& polygon) {
    Vertex* start = polygon.v();
    Vertex* current = start;
    do {
        if (current == vertex) {
            return true;
        }
        current = current->cw();
    } while (current != start);
    return false;
}



Polygon intersectionConvexPolygons( Polygon& p,  Polygon& q)
{
	return *Intersection::convexPolygonIntersect(p,q);
}



Polygon unionConvexPolygons(Polygon& A, Polygon& B)
{
    // limited for simple cases
    std::set<Point> intersectionPoints;
    std::vector<std::pair<Vertex*, Point>> insertionsA;
    std::vector<std::pair<Vertex*, Point>> insertionsB;

    // First phase: Identifying intersection points on edges of Polygon A and Polygon B
    Vertex* startA = A.v();
    Vertex* currentA = startA;
    do {
        Vertex* startB = B.v();
        Vertex* currentB = startB;
        do {
            Point intersectionPoint;
            Edge edgeA(currentA, currentA->cw());
            Edge edgeB(currentB, currentB->cw());
            EDGE_CLASSIFICATION result = Intersection::crossingPoint(edgeA, edgeB, intersectionPoint, A.getTolerance());
            if (result == EDGE_CLASSIFICATION::SKEW_CROSS) {
                intersectionPoints.insert(intersectionPoint);
                if (!vertexExistsInPolygon(intersectionPoint, A)) {
                    insertionsA.emplace_back(currentA, intersectionPoint);
                }
                if (!vertexExistsInPolygon(intersectionPoint, B)) {
                    insertionsB.emplace_back(currentB, intersectionPoint);
                }
            }
            currentB = currentB->cw();
        } while (currentB != startB);
        currentA = currentA->cw();
    } while (currentA != startA);

    // Inserting intersection points into Polygon A
    for (const auto& pair : insertionsA) {
        Vertex* position = pair.first;
        const Point& point = pair.second;
        A.setV(position);
        if (!vertexExistsInPolygon(point, A))
            A.insert(point);
    }
    // Inserting intersection points into Polygon A
    for (const auto& pair : insertionsB) {
		Vertex* position = pair.first;
		const Point& point = pair.second;
		B.setV(position);
		if (!vertexExistsInPolygon(point, B))
			B.insert(point);
	}

    


    // loop over all points of A and check if they are inside B
    Vertex* current = A.v();

    do {
        if (GeometryUtils::isPointInPolygon(current->point(), B) == POINT_CLASSIFICATION_POLYGON::INSIDE) {
			A.setV(current);
			current = current->cw();
			A.remove();
		}
        else {
			current = current->cw();
		}
	} while (current != A.v());
	// loop over all points of B and check if they are inside A
	current = B.v();
    do {
        if (GeometryUtils::isPointInPolygon(current->point(), A) == POINT_CLASSIFICATION_POLYGON::INSIDE) {
			B.setV(current);
			current = current->cw();
			B.remove();
		}
        else {
			current = current->cw();
		}
	} while (current != B.v());



    // make dictionary intersectionPointsMap[point]=make_pair(VertexA,VertexB) for each intersection point
    std::map<Point, std::pair<Vertex*, Vertex*>> intersectionPointsMap;
    for (const auto& point : intersectionPoints) {
        Vertex* vertexA = A.closestVertexToPoint(point);
        Vertex* vertexB = B.closestVertexToPoint(point);
        intersectionPointsMap[point] = std::make_pair(vertexA, vertexB);
    }


	// merge points of A and B
    std::set<Point> unionPointsSet;


    // (1) Lambda to check if a point already exists in unionPoints
    auto pointExistsInUnion = [&](const Point& point) {
        return std::find(unionPointsSet.begin(), unionPointsSet.end(), point) != unionPointsSet.end();
    };

    // (2) Lambda to check if a point is an intersection point
    auto isIntersectionPoint = [&](const Point& point) {
        return std::find_if(insertionsA.begin(), insertionsA.end(),
            [&](const auto& pair) { return pair.second == point; }) != insertionsA.end() ||
            std::find_if(insertionsB.begin(), insertionsB.end(),
                [&](const auto& pair) { return pair.second == point; }) != insertionsB.end();
    };
    
    Polygon result;
    Vertex* start = A.v();
    current = start;
    bool inPolygonA = true;
    bool flag = true;
    do {
        Point currentPoint = current->point();

        // (3) Add point if it doesn't already exist in unionPoints
        if (!pointExistsInUnion(currentPoint)) {
            unionPointsSet.insert(currentPoint);
            result.insert(currentPoint);
        }

        // (4) Check if current point is an intersection point
        if (isIntersectionPoint(currentPoint)) {
            auto [vertexA, vertexB] = intersectionPointsMap[currentPoint];
            current = (inPolygonA ? B.setV(vertexB) : A.setV(vertexA));
            inPolygonA = !inPolygonA; // Switch between A and B
            current = current->cw();
        }
        else {
            // (5) Advance the current vertex
            current = current->cw();
        }

        // (6) Exit if we have looped back to the start vertex of A
    } while (current != start );


    return result;

	
}

//std::vector<Point> allPoints = A.getPoints();
//std::vector<Point> pointsP2 = B.getPoints();
//allPoints.insert(allPoints.end(), pointsP2.begin(), pointsP2.end());
//// using ConvexHull::grahamscan; More accurate approach would to use PlaneSweep Algorithm
//return ConvexHull::grahamscan(allPoints);





Polygon differenceConvexPolygons(Polygon& A, Polygon& B) {
    std::set<Point> intersectionPoints;
    std::vector<std::pair<Vertex*, Point>> insertionsA;
    std::vector<std::pair<Vertex*, Point>> insertionsB;

    // First phase: Identifying intersection points on edges of Polygon A
    Vertex* startA = A.v();
    Vertex* currentA = startA;
    do {
        Vertex* startB = B.v();
        Vertex* currentB = startB;
        do {
            Point intersectionPoint;
            Edge edgeA(currentA, currentA->cw());
            Edge edgeB(currentB, currentB->cw());
            EDGE_CLASSIFICATION result = Intersection::crossingPoint(edgeA, edgeB, intersectionPoint, A.getTolerance());
            if (result == EDGE_CLASSIFICATION::SKEW_CROSS) {
                intersectionPoints.insert(intersectionPoint);
                if (!vertexExistsInPolygon(intersectionPoint, A)) {
                    insertionsA.emplace_back(currentA, intersectionPoint);
                }
                if (!vertexExistsInPolygon(intersectionPoint, B)) {
					insertionsB.emplace_back(currentB, intersectionPoint);
				}
            }
            currentB = currentB->cw();
        } while (currentB != startB);
        currentA = currentA->cw();
    } while (currentA != startA);

    // Inserting intersection points into Polygon A
    for (const auto& pair : insertionsA) {
        Vertex* position = pair.first;
        const Point& point = pair.second;
        A.setV(position);
        if (!vertexExistsInPolygon(point, A))
            A.insert(point);
    }

    // Inserting intersection points into Polygon B
    for (const auto& pair : insertionsB) {
		Vertex* position = pair.first;
		const Point& point = pair.second;
		B.setV(position);
		if (!vertexExistsInPolygon(point, B))
			B.insert(point);
	}


    // Adding points of Polygon B that are inside A and not on its edges
    Vertex* currentB = B.v();
    do {
        if (GeometryUtils::isPointInPolygon(currentB->point(), A) != POINT_CLASSIFICATION_POLYGON::OUTSIDE &&
            !vertexExistsInPolygon(currentB->point(), A)) {
            Vertex* insertPosition = findPositionToInsert(currentB->point(), A);
            A.setV(insertPosition);
            if (!vertexExistsInPolygon(currentB->point(), A))
                A.insert(currentB->point());
        }
        currentB = currentB->cw();
    } while (currentB != B.v());

    // remove points of A which are inside of B
    currentA = A.v();
    do {
        if (GeometryUtils::isPointInPolygon(currentA->point(), B) == POINT_CLASSIFICATION_POLYGON::INSIDE) {
			A.setV(currentA);
			currentA = currentA->cw();
			A.remove();
		}
        else {
			currentA = currentA->cw();
		}
	} while (currentA != A.v());

    return A;
}