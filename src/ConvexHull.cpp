#include "ConvexHull.hpp"
#include "GeometryUtils.hpp"
#include "Polygon.hpp"
#include "Point.hpp"
//#include "Stack.hpp"






//Polygon* ConvexHull::grahamscan(Point pts[], const int& n)
//{
//	// stage 1
//	int m = 0;
//	for (int i = 1; i < n; i++)
//		if ((pts[i].y < pts[m].y) ||
//			((pts[i].y == pts[m].y) && (pts[i].x < pts[m].x)))
//			m = i;
//	swap<Point>(pts[0], pts[m]);
//	originPt = pts[0];
//	// stage 2
//	/*Point **p = new (Point*)[n];
//	for (i = 0; i < n; i++)
//		p[i] = &pts[i];*/
//	std::vector<Point*> p;
//	p.reserve(n);  // Reserve space for n pointers
//	for (int i = 0; i < n; i++) {
//		p.emplace_back(&pts[i]);  // Store the address of each Point in pts
//	}
//	selectionSort(&p[1], n - 1, polarCmp); // or any sorting method
//	// stage 3
//	int i;
//	for (i = 1; p[i + 1]->classify(*p[0], *p[i]) == POINT_CLASSIFICATION::BEYOND; i++);
//	Stack<Point*> s;
//	s.push(p[0]);
//	s.push(p[i]);
//	// stage 4
//	for (i = i + 1; i < n; i++)
//	{
//		while (p[i]->classify(*s.nextToTop(), *s.top()) != POINT_CLASSIFICATION::LEFT)
//			s.pop();
//		s.push(p[i]);
//	}
//	//stage 5
//	Polygon* q = new Polygon;
//	while (!s.empty())
//		q->insert(*s.pop());
//
//	return q;
//}

#include "ConvexHull.hpp"

// compares two points by polar angle with respect to originPt
int ConvexHull::polarCmp(const Point& p, const Point& q, const Point& originPt) {
    Point vp = p - originPt;
    Point vq = q - originPt;
    double pPolar = vp.polarAngle();
    double qPolar = vq.polarAngle();
    if (pPolar < qPolar) return -1;
    if (pPolar > qPolar) return 1;
    if (vp.length() < vq.length()) return -1;
    if (vp.length() > vq.length()) return 1;
    return 0;
}

// compares two points p and q by polar angle with respect to originPt
int ConvexHull::polarCmp(Point* p, Point* q, Point originPt)
{
    Point vp = *p - originPt;
    Point vq = *q - originPt;
    double pPolar = vp.polarAngle();
    double qPolar = vq.polarAngle();
    if (pPolar < qPolar) return -1;
    if (pPolar > qPolar) return 1;
    if (vp.length() < vq.length()) return -1;
    if (vp.length() > vq.length()) return 1;
    return 0;
}



Polygon ConvexHull::grahamscan(const std::vector<Point>& inputPts) {
    if (inputPts.empty()) return Polygon();
    Polygon q;
    if (inputPts.size() == 1)
    {
        q.insert(inputPts[0]);
        return q;
    }

    // Sort points and remove duplicates
    std::vector<Point> pts(inputPts);
    std::sort(pts.begin(), pts.end(), [](const Point& a, const Point& b) {
        return a.x < b.x || (a.x == b.x && a.y < b.y);
        });
    auto last = std::unique(pts.begin(), pts.end());
    pts.erase(last, pts.end());

    if (pts.size() == 1) { q.insert(pts[0]); return q; }
    if (pts.size() == 2)
    {
        q.insert(pts[0]); q.insert(pts[1]); return q;
    }

    // Stage 1: Find point with lowest y-coordinate (and leftmost in case of a tie)
    int m = 0;
    for (int i = 1; i < pts.size(); i++) {
        if ((pts[i].y < pts[m].y) || ((pts[i].y == pts[m].y) && (pts[i].x < pts[m].x))) {
            m = i;
        }
    }

    Point originPt = pts[m];

    // Stage 2: Sort points based on polar angle with respect to originPt
    std::vector<Point> sortedPts(pts);
    ConvexHull::selectionSort<Point>(sortedPts, ConvexHull::polarCmp, originPt);  // Corrected call

    // Stage 3 & 4: Perform Graham Scan
    Stack<Point> s;
    s.push(sortedPts[0]);
    s.push(sortedPts[1]);
    for (int i = 2; i < sortedPts.size(); i++) {
        while (s.size() > 1 && sortedPts[i].classify(s.nextToTop(), s.top()) != POINT_CLASSIFICATION_LINE::LEFT) {
            s.pop();
        }
        s.push(sortedPts[i]);
    }

    // Stage 5: Construct polygon from stack
    
    while (!s.empty()) {
        q.insert(s.pop());
    }

    return q;
}
