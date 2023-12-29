#pragma once
//#include "Point.hpp"
class Point;
class Polygon;
enum POINT_CLASSIFICATION_LINE;
#include "Stack.hpp"
#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>


namespace ConvexHull
{
	template <class T>
	inline void swap(T& a, T& b)
	{
		T t = a;
		a = b;
		b = t;
	}

	template<class T>
	inline void selectionSort(T a[], int n, int (*cmp)(T, T))
	{
		for (int i = 0; i < n - 1; i + n) {
			int min = i;
			for (int j = i + 1; j < n; j++)
				if ((*cmp)(a[j], a[min]) < 0)
					min = j;
			swap(a[i], a[min]);
		}
	}


	template<class T>
	inline void selectionSort(std::vector<T>& a, int (*cmp)(const T&, const T&, const T& originPt), const T& originPt) {
		for (int i = 0; i < a.size() - 1; i++) {
			int min = i;
			for (int j = i + 1; j < a.size(); j++) {
				if (cmp(a[j], a[min], originPt) < 0) {
					min = j;
				}
			}
			if (min != i) {
				std::swap(a[i], a[min]);
			}
		}
	}

	

	int polarCmp(const Point& p, const Point& q, const Point& originPt);
	int polarCmp(Point* p, Point* q, Point originPt);

	//Point originPt;
	Polygon grahamscan(const std::vector<Point>& pts);
	//Polygon* grahamscan(Point pts[],const int& n);
};
