#include "Vertex.hpp"
#include "GeometryUtils.hpp"

Vertex* Vertex::cw(void)
{
	return (Vertex*)Node::next();
}

Vertex* Vertex::ccw(void)
{
	return (Vertex*)Node::prev();
}

Vertex* Vertex::neighbor(ORIENTATION orient)
{
	return orient == ORIENTATION::CLOCKWISE ? cw() : ccw();
}

Point Vertex::point(void)
{
	return *((Point*)this);
}

Vertex* Vertex::insert(Vertex* vertex)
{
	return (Vertex*)Node::insert(vertex);
}

Vertex* Vertex::remove(void)
{
	return (Vertex*)Node::remove();
}

void Vertex::splice(Vertex* vertex)
{
	Node::splice(vertex);
}

Vertex* Vertex::split(Vertex* b)
{ 
	// insert bp before vertex b
	Vertex* bp = b->ccw()->insert(new Vertex(b->point()));
	Vertex::insert(new Vertex(point())); // insert ap after this vertex
	Vertex::splice(bp);
	return bp;
}