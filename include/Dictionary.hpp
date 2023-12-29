
#pragma once
#include "RandomizedNode.hpp"
#include <functional>

template <class T>
class Dictionary
{
private:
	RandomizedNode<T>* root; //header node
	RandomizedNode<T>* win; //window 
	int (*cmp)( T, T); //comparison function
	//std::function<int(const T&, const T&)> cmp;
	void _remove(RandomizedNode<T>* node);
public:
	// Constructor
	//Dictionary(int (*cmp)(const T&, const T&), int seed);
	//Dictionary(int (*cmp)(const T&, const T&));
	Dictionary(int (*cmp)(T, T));
	//Dictionary(int (*cmp)(T*, T*));
	//Dictionary(int (*cmp)(T**, T**));
	//Dictionary(std::function<int(const T&, const T&)> cmp);
	//Dictionary(std::function<int(T*, T*)> cmp);

	// Destructor
	~Dictionary();

	T next(void);
	T prev(void);
	void inorder(void (*visit)(T&));
	T val(void);
	bool isFirst(void);
	bool isLast(void);
	bool isHead(void);
	bool isEmpty(void);
	T find(const T& key);
	T findMin(void);
	T locate(const T& key);
	T insert(const T& key);
	void remove(void);
	T remove(const T& key);
	T removeMin(void);

};

//template <class T>
//Dictionary<T>::Dictionary(int (*cmp)(const T&, const T&), int seed) :cmp(cmp)
//{
//	root = new RandomizedNode<T>(nullptr, seed);
//	win = root;
//	root->_priority = -1;
//}

//template <class T>
//Dictionary<T>::Dictionary(int (*cmp)(const T&, const T&)) :cmp(cmp)
//{
//	int seed = -1;
//	root = new RandomizedNode<T>(nullptr, seed);
//	win = root;
//	root->_priority = -1;
//}

//template <class T>
//Dictionary<T>::Dictionary(int (*cmp)(T*, T*)) : cmp(cmp) {
//	int seed = -1;
//	root = new RandomizedNode<T>(nullptr, seed);
//	win = root;
//	root->_priority = -1;
//}

//template <class T>
//Dictionary<T>::Dictionary(int (*cmp)(T**, T**)) : cmp(cmp) {
//	int seed = -1;
//	root = new RandomizedNode<T>(nullptr, seed);
//	win = root;
//	root->_priority = -1;
//}

template <class T>
Dictionary<T>::Dictionary(int (*cmp)(T, T)) : cmp(cmp) {
	int seed = -1;
	root = new RandomizedNode<T>(nullptr, seed);
	win = root;
	root->_priority = -1;
}

//template <class T>
//Dictionary<T>::Dictionary(std::function<int(const T&, const T&)> cmp) : cmp(cmp) {
//	int seed = -1;
//	root = new Dictionary<T>(nullptr, seed);
//	win = root;
//	root->_priority = -1; 
//}
//
//template <class T>
//Dictionary<T>::Dictionary(std::function<int(T*, T*)> cmp) : cmp(cmp) {
//	int seed = -1;
//	root = new Dictionary<T>(nullptr, seed);
//	win = root;
//	root->_priority = -1;
//}

template <class T>
Dictionary<T>::~Dictionary()
{
	delete root;
}

template <class T>
T Dictionary<T>::next(void)
{
	win = win->next();
	return win->val;
}

template <class T>
T Dictionary<T>::prev(void)
{
	win = win->prev();
	return win->val;
}

template <class T>
T Dictionary<T>::val(void)
{
	return win->val;
}

template <class T>
void Dictionary<T>::inorder(void (*visit)(T&))
{
	RandomizedNode<T> *n = root->next();
	while (n != root)
	{
		(*visit)(n->val);
		n = n->next();
	}
}

template <class T>
bool Dictionary<T>::isFirst(void)
{
	return (win == root->next()) && (root != root->next());
}

template <class T>
bool Dictionary<T>::isLast(void)
{
	return (win == root->prev()) && (root != root->next());
}

template <class T>
bool Dictionary<T>::isHead(void)
{
	return (win == root);
}

template <class T>
bool Dictionary<T>::isEmpty(void)
{
	return (root == root->next());
}

template <class T>
T Dictionary<T>::find(const T& key)
{
	RandomizedNode<T>* n = root->rchild();
	while (n )
	{
		int result = (*cmp)(key, n->val);
		if (result < 0)
			n = n->lchild();
		else if (result > 0)
			n = n->rchild();
		else {
			win = n;
			return n->val;
		}
	}
	return nullptr;
}

template <class T>
T Dictionary<T>::findMin(void)
{
	win = root->next();
	return win->val;
}

template <class T>
T Dictionary<T>::locate(const T& key)
{
	RandomizedNode<T>* prev = root;
	RandomizedNode<T>* n = root->rchild();
	while (n)
	{
		int result = (*cmp)(key, n->val);
		if (result < 0)
		{
			n = n->lchild();
		}
		else if (result > 0)
		{
			prev = n;
			n = n->rchild();
		}
		else
		{
			win = n;
			return win->val;
		}
	}
	win = prev;
	return win->val;
}

template <class T>
T Dictionary<T>::insert(const T& key)
{
	// stage 1
	int result = 1;
	RandomizedNode<T>* p = root;
	RandomizedNode<T>* n = root->rchild();
	while (n) {
		p = n;
		result = (*cmp)(key, p->val);
		if (result < 0)
			n = p->lchild();
		else if (result > 0)
			n = p->rchild();
		else
			return NULL;
	}
	win = new RandomizedNode<T>(key);
	win->_parent = p;
	if (result < 0) 
	{
		p->_lchild = win;
		p->prev()->Node::insert(win);
	}
	else 
	{
		p->_rchild = win;
		p->Node:: insert(win);
	}
	// stage 2
	win->bubbleUp();
	return key;
}

template <class T>
void Dictionary<T>::remove(void)
{
	if (win !=root) _remove(win);
}

template <class T>
T Dictionary<T>::removeMin(void)
{
	T val = root->next()->val;
	
	if (root != root->next()) _remove(root->next());
	return val;
}

template <class T>
T Dictionary<T>::remove(const T& key)
{
	T val = find(key);
	if (val) {
		remove();
		return val;
	}
	return nullptr;
}

template <class T>
void Dictionary<T>::_remove(RandomizedNode<T>* node)
{
	node->_priority = 1.5;
	node->bubbleDown();
	RandomizedNode<T>* parent = node->parent();
	if (parent->_lchild==node) 
		parent->_lchild = nullptr;
	else
		parent->_rchild = nullptr;
	if (win ==node) win = node->prev();
	node->Node::remove();
	delete node;
}