#pragma once
#include <vector>
class Polygon;
class Point;
class Vertex;
class Edge;

Polygon unionConvexPolygons( Polygon& p, Polygon& q);
Polygon intersectionConvexPolygons(Polygon& p,Polygon& q);
Polygon differenceConvexPolygons(Polygon& p,Polygon& q);


