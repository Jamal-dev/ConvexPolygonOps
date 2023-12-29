#include "Point.hpp"
#include "Dictionary.hpp"
#include "ListNode.hpp"
#include "Edge.hpp"
#include "GeometryUtils.hpp"
#include "PlaneSweep.hpp"

using namespace PlaneSweep;

LeftEndpoint::LeftEndpoint(Edge* _e) : e(*_e)
{
	p = (e.org <e.dest) ? e.org : e.dest;
}

RightEndpoint::RightEndpoint(Edge* _e) : e(*_e)
{
	p = (e.org > e.dest) ? e.org : e.dest;
}

Crossing::Crossing(Edge* _e1, Edge* _e2, Point& _p) : e1(*_e1), e2(*_e2)
{
	p = _p;
}

Crossing::Crossing(const Edge& _e1, const Edge& _e2, Point& _p) : e1(_e1), e2(_e2)
{
	p = _p;
}

int PlaneSweep::eventCmp(EventPoint* a, EventPoint* b)
{
	if (a->p < b->p) return -1;
	else if (a->p > b->p) return 1;
	return 0;
}

//int PlaneSweep::eventCmp(EventPoint** a, EventPoint** b) {
//	if ((*a)->p < (*b)->p) return -1;
//	else if ((*a)->p > (*b)->p) return 1;
//	return 0;
//}


Dictionary<EventPoint*>& PlaneSweep::buildSchedule(Edge s[], int n)
{
	Dictionary<EventPoint*> *schedule =
		new Dictionary<EventPoint*>(PlaneSweep::eventCmp);
	for (int i = 0; i < n; i++) {
		schedule->insert(new LeftEndpoint(&s [i]));
		schedule->insert(new RightEndpoint(&s [i]));
		}
	return *schedule;
}





int PlaneSweep::edgeCmp2( Edge* const a,  Edge* const b,const double &curx, const double& EPSILON3 ) 
{
	const double ya = a->y(curx - EPSILON3);
	const double yb = b->y(curx - EPSILON3);
	if (ya < yb) return -1;
	else if (ya > yb) return 1;
	const double ma = a->slope();
	const double mb = b->slope();
	if (ma > mb) return -1;
	else if (ma < mb) return 1;
	return 0;
}




void LeftEndpoint::handleTransition(Dictionary<Edge*>& sweepline,
	Dictionary<EventPoint*>& schedule,
	List<EventPoint*> *result,
	double& curx)
{
	Edge* b = sweepline.insert(&e);
	Edge* c = sweepline.next();
	sweepline.prev();
	Edge* a = sweepline.prev();
	double t;
	if (a && c && (a->cross(*c, t) == EDGE_CLASSIFICATION::SKEW_CROSS)) {
		Point p = a->point(t);
		if (curx < p.x) {
			Crossing cev(a, c, p);
			delete schedule.remove(&cev);
		}
	}
	Point cross_t = b->point(t);
	if (c && (b->cross(*c, t) == EDGE_CLASSIFICATION::SKEW_CROSS))
		schedule.insert(new Crossing(b, c, cross_t));
	if (a && (b->cross(*a, t) == EDGE_CLASSIFICATION::SKEW_CROSS))
		schedule.insert(new Crossing(a, b, cross_t));
}

void Crossing::handleTransition(Dictionary<Edge*>& sweepline,
	Dictionary<EventPoint*>& schedule,
	List<EventPoint*>* result,
	double & curx)
{
	Edge* b = sweepline.find(&e1);
	Edge* a = sweepline.prev();
	Edge* c = sweepline.find(&e2);
	Edge* d = sweepline.next();
	double t;
	if (a && (a->cross(*c, t) == SKEW_CROSS)) {
		Point p = a->point(t);
		if (curx < p.x)
			schedule.insert(new Crossing(a, c, p));
	}
	if (d && (d->cross(*b, t) == SKEW_CROSS)) {
		Point p = d->point(t);
		if (curx < p.x)
			schedule.insert(new Crossing(b, d, p));
	}
	if (a && (a->cross(*b, t) == SKEW_CROSS)) {
		Point p = a->point(t);
		if (curx < p.x) {
			Crossing cev(a, b, p);
			delete schedule.remove(&cev);
		}
	}
	if (d && (d->cross(*c, t) == SKEW_CROSS)) {
		Point p = d->point(t);
		if (curx < p.x) {
			Crossing cev(c, d, p);
			delete schedule.remove(&cev);
		}
	}
	sweepline.remove(b);
	curx += 2 * EPSILON3;
	sweepline.insert(b);
	curx -= 2 * EPSILON3;
	result->append(this);
}

void RightEndpoint::handleTransition(Dictionary<Edge*>& sweepline,
	Dictionary<EventPoint*>& schedule,
	List<EventPoint*>* result,
	double& curx)
{
	Edge* b = sweepline.find(&e);
	Edge* c = sweepline.next();
	sweepline.prev();
	Edge* a = sweepline.prev();
	double t;
	if (a && c && (a->cross(*c, t) == SKEW_CROSS)) {
		Point p = a->point(t);
		if (curx < p.x)
			schedule.insert(new Crossing(a, c, p));
	}
}

int PlaneSweep::Algo::edgeCmp2(Edge*  a, Edge*  b)
{
	const double ya = a->y(curx - EPSILON3);
	const double yb = b->y(curx - EPSILON3);
	if (ya < yb) return -1;
	else if (ya > yb) return 1;
	const double ma = a->slope();
	const double mb = b->slope();
	if (ma > mb) return -1;
	else if (ma < mb) return 1;
	return 0;
}

List<EventPoint*>* PlaneSweep::Algo::intersectSegments(Edge s[], int n)
{
	Dictionary<EventPoint*> schedule = buildSchedule(s, n);
	/*std::function<int(Edge*const, Edge*const)> edgeCompare = [&curx](Edge*const a, Edge*const b) {
		return PlaneSweep::edgeCmp2(a, b, curx);
	};*/

	
	Dictionary<Edge*> sweepline(PlaneSweep::Algo::edgeCmp2);
	List<EventPoint*>* result = new List<EventPoint*>;
	while (!schedule.isEmpty()) {
		EventPoint* ev = schedule.removeMin();
		curx = ev->p.x;
		ev->handleTransition(sweepline, schedule, result,curx);
	}
	return result;
}

List<EventPoint*>* PlaneSweep::Algo::intersectSegments(std::vector<Edge>& s) 
{
	if (s.empty()) return nullptr;
	Dictionary<EventPoint*> schedule = buildSchedule(s.data(), s.size());
	Dictionary<Edge*> sweepline(PlaneSweep::Algo::edgeCmp2);
	List<EventPoint*>* result = new List<EventPoint*>;
	while (!schedule.isEmpty()) {
		EventPoint* ev = schedule.removeMin();
		curx = ev->p.x;
		ev->handleTransition(sweepline, schedule, result, curx);
	}
	return result;
}