#pragma once
//#include "Node.hpp"
//#include "TreeNode.hpp"
#include "BraidedNode.hpp"
#include <cstdlib>  // Include the header for rand and srand
// forward declaration for Dictionary<T> class
template <class T> class Dictionary;




// Randomized Node
template<class T>
class RandomizedNode : public BraidedNode<T>
	{
protected:
	RandomizedNode* _parent;
	double _priority;
	void rotateRight(void);
	void rotateLeft(void);
	void bubbleUp(void);
	void bubbleDown(void);
public:
	RandomizedNode(const T& v, const int& seed = -1);
	RandomizedNode* lchild(void) const;
	RandomizedNode* rchild(void) const;
	RandomizedNode* next(void) const;
	RandomizedNode* prev(void) const;
	RandomizedNode* parent(void) const;
	double priority(void) const;
	friend class Dictionary<T>;
	};

template<class T>
RandomizedNode<T>::RandomizedNode(const T& v, const int& seed) : BraidedNode<T>(v)
{
	if (seed != -1) srand(seed);
	_parent = nullptr;
	_priority = (rand() % 32767) / 32767.0;
	}

template<class T> void RandomizedNode<T>::rotateRight(void)
{
	RandomizedNode<T>* y = this;
	RandomizedNode<T>* x = y->lchild();
	RandomizedNode<T>* p = y->parent();
	y->_lchild = x->rchild();
	if (y->lchild()  != nullptr)
		y->lchild()->_parent = y;
	if (p->rchild() == y)
		p->_rchild = x;
	else
		p->_lchild = x;
	x->_parent = p;
	x->_rchild = y;
	y->_parent = x;
}

template<class T> void RandomizedNode<T>::rotateLeft(void)
{
	RandomizedNode<T>* x = this;
	RandomizedNode<T>* y = x->rchild();
	RandomizedNode<T>* p = x->parent();
	x-> _rchild = y->lchild();
	if (x->rchild() != nullptr)
		x->rchild()-> _parent = x;
	if (p->lchild() == x)
		p-> _lchild = y;
	else
		p-> _rchild = y;
	y-> _parent = p;
	y->_lchild = x;
	x-> _parent = y;
}

template<class T> 
void RandomizedNode<T>::bubbleUp(void)
{
	RandomizedNode<T>* p = parent();
	if (priority() < p->priority())
	{
		if (p->lchild() == this)
			p->rotateRight();
		else
			p->rotateLeft();
		bubbleUp();
	}
}

template<class T> 
void RandomizedNode<T>::bubbleDown(void)
{
	float lcPriority = lchild() ? lchild()->priority() : 2.0;
	float rcPriority = rchild() ? rchild()->priority() : 2.0;
	float minPriority = (lcPriority < rcPriority) ?
		lcPriority : rcPriority;
	if (priority() <= minPriority)
		return;
	if (lcPriority < rcPriority)
		rotateRight();
	else
		rotateLeft();
	bubbleDown();
}

template<class T>
RandomizedNode<T>* RandomizedNode<T>::rchild(void) const
{
	return (RandomizedNode< T>* ) this->BraidedNode<T>::_rchild;
		
}
template<class T>
RandomizedNode<T> *RandomizedNode<T>::lchild(void) const
{
	return (RandomizedNode< T>* ) this->BraidedNode<T>::_lchild;
}

template<class T>
RandomizedNode<T>* RandomizedNode<T>::next(void) const
{
	return (RandomizedNode< T>* ) this->BraidedNode<T>::_next;
}

template<class T>
RandomizedNode<T>* RandomizedNode<T>::prev(void) const
{
	return (RandomizedNode< T>* ) this->BraidedNode<T>::_prev;
}

template<class T>
RandomizedNode<T>* RandomizedNode<T>::parent(void) const
{
	return (RandomizedNode< T>* )_parent;
}

template<class T>
double RandomizedNode<T>::priority(void) const
{
	return _priority;
}
