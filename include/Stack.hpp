#pragma once
#include "ListNode.hpp"

template<class T> class Stack {
private:
	List<T>* s;
public:
	Stack(void);
	~Stack(void);
	void push(T v);
	T pop(void);
	bool empty(void);
	int size(void);
	T top(void);
	T nextToTop(void);
	T bottom(void);
};

template<class T> Stack<T>::Stack(void)
	:s(new List<T>)
{}

template<class T> Stack<T>::~Stack(void)
{
	delete s;
}

template<class T> void Stack<T>::push(T v)
{
	s->prepend(v);
}

template<class T> T Stack<T>::pop(void)
{
	s->first();
	return s->remove();
}

template<class T> bool Stack<T>::empty(void)
{
	return s->length() == 0;
}

template<class T> int Stack<T>::size(void)
{
	return s->length();
}

template<class T> T Stack<T>::top(void)
{
	return s->first();
}

template<class T> T Stack<T>::nextToTop(void)
{
	s->first();
	return s->next();
}

template<class T> T Stack<T>::bottom(void)
{
	return s->last();
}