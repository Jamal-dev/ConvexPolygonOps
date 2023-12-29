#pragma once
#include "Point.hpp"
//class Edge;
#include "Edge.hpp"
#include <vector>
template <class T> class Dictionary;
template <class T> class List;


namespace PlaneSweep 
{
	class EventPoint {
		public:
			Point p;
			virtual void handleTransition(
										  Dictionary<Edge *> &,
										  Dictionary<EventPoint*>&,
				                          List<EventPoint*>*,
										  double& curx)=0;
	};

	class LeftEndpoint : public EventPoint
	{
	public:
		Edge e;
		LeftEndpoint(Edge*);
		void handleTransition(Dictionary<Edge*>&,
			Dictionary<EventPoint*>&,
			List<EventPoint*>*,
			double& curx);
	};

	class RightEndpoint : public EventPoint
	{
	public:
		Edge e;
		RightEndpoint(Edge*);
		void handleTransition(Dictionary<Edge*>&,
			Dictionary<EventPoint*>&,
			List<EventPoint*>*,
			double& curx);
	};

	class Crossing : public EventPoint
	{
	public:
		Edge e1, e2;
		double EPSILON3 = 1e-10;
		Crossing(Edge*, Edge*, Point&);
		Crossing(const Edge &,const Edge &, Point&);
		void handleTransition(Dictionary<Edge*>&,
						Dictionary<EventPoint*>&,
						List<EventPoint*>*,
						double& curx);
	};

	Dictionary<EventPoint*>& buildSchedule(Edge s[], int n);
	int eventCmp(EventPoint* a, EventPoint* b);
	int edgeCmp2(Edge* const a,Edge* const b, const double & curx,const double & EPSILON3=1e-10);

	/*int edgeCmp2(Edge* a, Edge* b, const double& curx, const double& EPSILON3 = 1e-10);*/
	

	class Algo 
	{
	public:
		static double curx;
		static double EPSILON3;
		// Constructor
		Algo() {};
		List<EventPoint*>* intersectSegments(Edge s[], int n);
		List<EventPoint*>* intersectSegments(std::vector<Edge>& s);
		static int edgeCmp2(Edge* const a, Edge* const b);
	};
};

