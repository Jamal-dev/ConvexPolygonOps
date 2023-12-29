#include "Intersection.hpp"
#include "Vertex.hpp"
#include "Edge.hpp"
#include "Polygon.hpp"
#include "GeometryUtils.hpp"

/* Function aimsAt returns TRUE if and only if edge a aims at edge b.The parametera class indicates the classification of endpoint a.des t relative to edge b.The parameter crossType equals COLLINEAR if and only if edges a and b are collinear*/
bool Intersection::aimsAt(const Edge& a, const Edge& b, POINT_CLASSIFICATION_LINE aclass, EDGE_CLASSIFICATION crossType)
{
    Point va = a.dest - a.org;
    Point vb = b.dest - b.org;
    if (crossType != EDGE_CLASSIFICATION::EDGE_COLLINEAR) {
        if ((va.x * vb.y) >= (vb.x * va.y))
            return (aclass != POINT_CLASSIFICATION_LINE::RIGHT);
        else
            return (aclass != POINT_CLASSIFICATION_LINE::LEFT);
    }
    else {
        return (aclass != POINT_CLASSIFICATION_LINE::BEYOND);
    }
}



void Intersection::advance(Polygon& A, Polygon& R, int inside)
{
    A.advance(ORIENTATION::CLOCKWISE);
    if (inside && (R.point() != A.point()))
        R.insert(A.point());
}

EDGE_CLASSIFICATION Intersection::crossingPoint(Edge& e, Edge& f, Point& p, double tolerance)
{
    double s, t;
    EDGE_CLASSIFICATION classe = e.intersect(f, s);
    if ((classe == EDGE_CLASSIFICATION::EDGE_COLLINEAR) || (classe == EDGE_CLASSIFICATION::PARALLEL))
        return classe;
    double lene = (e.dest - e.org).length();
    if ((s < -tolerance * lene) || (s > 1.0 + tolerance * lene))
        return EDGE_CLASSIFICATION::SKEW_NO_CROSS;
    f.intersect(e, t);
    double lenf = (f.org - f.dest).length();
    if ((-tolerance * lenf <= t) && (t <= 1.0 + tolerance * lenf))
    {
        if (t <= tolerance * lenf) p = f.org;
        else if (t >= 1.0 - tolerance * lenf) p = f.dest;
        else if (s <= tolerance * lene) p = e.org;
        else if (s >= 1.0 - tolerance * lene) p = e.dest;
        else p = f.point(t);
        return EDGE_CLASSIFICATION::SKEW_CROSS;
    }
    else
        return EDGE_CLASSIFICATION::SKEW_NO_CROSS;
}

Polygon* Intersection::convexPolygonIntersect(Polygon& P, Polygon& Q)
{
    Polygon* R = nullptr;
    Point iPnt, startPnt;
    int inflag = Intersection::UNKNOWN;
    int phase = 1;
    int maxItns = 2 * (P.size() + Q.size());
    for (int i = 1; (i <= maxItns) || (phase == 2); i++)
    { // for
        if (i > maxItns) break;
        Edge p = P.edge();
        Edge q = Q.edge();
        POINT_CLASSIFICATION_LINE pclass = p.dest.classify(q);
        POINT_CLASSIFICATION_LINE qclass = q.dest.classify(p);
        EDGE_CLASSIFICATION crossType = crossingPoint(p, q, iPnt);
        if (crossType == EDGE_CLASSIFICATION::SKEW_CROSS)
        {
            if (phase == 1)
            {
                phase = 2;
                R = new Polygon();
                R->insert(iPnt);
                startPnt = iPnt;
            }
            else if (iPnt != R->point())
            {
                if (iPnt != startPnt)
                    R->insert(iPnt);
                else
                    return R;
            }
            if (pclass == POINT_CLASSIFICATION_LINE::RIGHT) inflag = Intersection::P_IS_INSIDE;
            else if (qclass == POINT_CLASSIFICATION_LINE::RIGHT) inflag = Intersection::Q_IS_INSIDE;
            else inflag = Intersection::UNKNOWN;
        }
        else if ((crossType == ORIENTATION::COLLINEAR) &&
            (pclass != POINT_CLASSIFICATION_LINE::BEHIND) &&
            (qclass != POINT_CLASSIFICATION_LINE::BEHIND))
            inflag = Intersection::UNKNOWN;
        bool pAIMSq = aimsAt(p, q, pclass, crossType);
        bool qAIMSp = aimsAt(q, p, qclass, crossType);
        if (pAIMSq && qAIMSp) {
            if ((inflag == Intersection::Q_IS_INSIDE) ||
                ((inflag == Intersection::UNKNOWN) && (pclass == POINT_CLASSIFICATION_LINE::LEFT)))
                advance(P, *R, false);
            else
                advance(Q, *R, false);
        }
        else if (pAIMSq)
        {
            advance(P, *R, inflag == Intersection::P_IS_INSIDE);
        }
        else if (qAIMSp)
        {
            advance(Q, *R, inflag == Intersection::Q_IS_INSIDE);
        }
        else
        {
            if ((inflag == Intersection::Q_IS_INSIDE) ||
                ((inflag == Intersection::UNKNOWN) && (pclass == POINT_CLASSIFICATION_LINE::LEFT)))
                advance(P, *R, false);
            else
                advance(Q, *R, false);
        }
    } // for
    if (GeometryUtils::isPointInsideConvexPolygon(P.point(), Q))
        return new Polygon(P);
    else if (GeometryUtils::isPointInsideConvexPolygon(Q.point(), P))
        return new Polygon(Q);
    return new Polygon;
}