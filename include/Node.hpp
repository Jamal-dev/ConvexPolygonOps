#pragma once
#include <iostream>
#include <stdexcept>
class Node {
protected:
	Node* _next; // link to successor node
	Node* _prev; // link to predecessor node
public:
	Node(void);
	virtual ~ Node(void);
	Node* next(void) const;
	Node* prev(void) const;
	Node* insert(Node*);
	Node* remove(void);
	void splice(Node*);
	void visualize() const;
};