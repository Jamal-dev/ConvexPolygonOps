#pragma once
#include "Node.hpp"

template<class T>
class TreeNode {
protected:
	TreeNode* _lchild;
	TreeNode* _rchild;
	T val;
public:
	TreeNode(T);
	virtual ~TreeNode(void);

};

//constructor for tree node
template <class T>
TreeNode<T>::TreeNode(T val) : val(val), _lchild(nullptr),
_rchild(nullptr) {}

//destructor for tree node
template <class T>
TreeNode<T>::~TreeNode(void)
{
	if (_lchild) delete _lchild;
	if (_rchild) delete _rchild;
}