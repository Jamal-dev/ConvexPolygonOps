#pragma once
#include "Node.hpp"
#include "TreeNode.hpp"


// Braided Node class
template<class T>
class BraidedNode : public Node, public TreeNode<T> {
public:
	BraidedNode(const T& val);
	BraidedNode<T>* rchild(void);
	BraidedNode<T>* lchild(void);
	BraidedNode<T>* next(void);
	BraidedNode<T>* prev(void);
};

template<class T>
BraidedNode<T>::BraidedNode(const T& val) : TreeNode<T>(val), Node() {}

template<class T>
BraidedNode<T>* BraidedNode<T>::rchild(void)
{
	return (BraidedNode<T>*) this->TreeNode<T>::_rchild;
}
template<class T>
BraidedNode<T>* BraidedNode<T>::lchild(void)
{
	return (BraidedNode<T>*) this->TreeNode<T>::_lchild;
}
template<class T>
BraidedNode<T>* BraidedNode<T>::next(void)
{
	return (BraidedNode<T>*) this->Node::_next;
}
template<class T>
BraidedNode<T>* BraidedNode<T>::prev(void)
{
	return (BraidedNode<T>*) this->Node::_prev;
}