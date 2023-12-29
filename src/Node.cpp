#include "Node.hpp"



// void Constructor

Node::Node() :
	_next(this), 
	_prev(this)
{}

// void Destructor
Node:: ~Node()
{}

Node* Node::next() const
{
	return _next;
}
Node* Node::prev() const
{
	return _prev;
}

Node* Node::insert(Node* node)
{
	if (node == nullptr)
		throw std::invalid_argument("Inserted Node cannot be null");
	Node* tempNode = _next;
	node-> _next = tempNode;
	node->_prev = this;
	_next = node;
	tempNode-> _prev = node;
	return node;
}

Node* Node::remove()
{
	if (_next == this)
		return this; // Node is already standalone
	_prev->_next = _next;
	_next->_prev = _prev;
	// Reset the removed node as standalone
	_next = _prev = this;
	return this;
}

void Node::splice(Node* b)
{
	/*if (b == nullptr) {
		throw std::invalid_argument("Splice node cannot be null");
	}*/
	Node* a = this;
	Node* an = a->_next;
	Node* bn = b->_next;
	
	a->_next = bn;
	b->_next = an;
	an->_prev = b;
	bn->_prev = a;
	
}

// Method to visualize the node
void Node::visualize() const {
	std::cout << "Node Address: " << this << std::endl;
	std::cout << "Next Node: " << _next << std::endl;
	std::cout << "Previous Node: " << _prev << std::endl;
}