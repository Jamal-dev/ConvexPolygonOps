#pragma once
#include "Node.hpp"


template<class T>
class List;  // Forward declaration of the List template class

template<class T> 
class ListNode : public Node {
public:
	T _val;
	ListNode(T val):
		_val(val)
	{};
	friend class List<T>;
};

template<class T> class List{
private:
	ListNode<T>* header;
	ListNode<T>* win;
	int _length;
public:
	List(void);
	~List(void);
	T insert(T);
	T append(T);
	List* append(List*);
	T prepend(T);
	T remove(void);
	void val(T);
	T val(void);
	T next(void);
	T prev(void);
	T first(void);
	T last(void);
	int length(void);
	bool isFirst(void);
	bool isLast(void);
	bool isHead(void);
	List* array2list(T* a[], int n);
	List leastItem(const List &list, int (*cmp_func)(T, T));
	List greatestItem(const List &list, int (*cmp_func)(T, T));
};


template<class T>
List<T>::List(void) :
	_length(0)
{
	header = new ListNode<T>(NULL);
	win = header;
}

template<class T>
List<T>:: ~List(void)
{
	while (length() > 0) {
		first();
		remove();
		delete header;
	}
}

template<class T> T
List<T>::insert(T val)
{
	ListNode<T>* node = new ListNode<T>(val);
	win->insert(node);
	_length++;
	return val;
}

template<class T> T
List<T>::prepend(T val)
{
	header->insert(new ListNode<T>(val));
	++_length;
	return val;
}

template<class T> T
List<T>::append(T val)
{
	header->prev()->insert(new ListNode<T>(val));
	++_length;
	return val;
}

// appending a list to another list
template<class T>
List<T>* List<T>::append(List<T>* list)
{
	ListNode<T>* a = (ListNode<T>*)header->prev();
	a->splice(list->header);
	_length += list->_length;
	list->header->remove();
	list->_length = 0;
	list->win = header;
	return this;
}

template<class T>
T List<T>::remove(void) {
	// Do nothing and return NULL (or nullptr) if the window is at the head
	if (win == header || _length == 0) {
		return NULL; // or nullptr for C++11 and later
	}

	ListNode<T>* nodeToRemove = win;
	T val = nodeToRemove->_val; // Save the value to return

	// Move window to the previous node before removal
	win = static_cast<ListNode<T>*>(nodeToRemove->prev());

	// Remove the node and adjust pointers
	// Assuming ListNode::remove() properly disconnects the node from the list
	delete nodeToRemove->remove();
	--_length;

	return val;
}


//template<class T> T
//List<T>::remove(void)
//{
//	if (win == header)
//		return NULL;
//	void* val = win->_val;
//	win = (ListNode<T>*)win->prev();
//	delete (ListNode<T>*)win->next()->remove();
//	--_length;
//	return val;
//}

template<class T>
void List<T>::val(T v)
{
	/*function val is called with some item v, it replaces the item currently in the window by v.The function does nothing if the window is in the head position.*/
	if (win != header) win->_val = v;
}

template<class T> T
List<T>::val(void)
{
	/*function val is called with no argument, it returns the item currently in the window.The function returns NULL if the window is in the head position.*/
	return win->val;
}

template<class T> T
List<T>::next(void)
{
	win = (ListNode<T>*)win->next();
	return win->_val;
}
template<class T> T
List<T>::prev(void)
{
	win = (ListNode<T>*)win->prev();
	return win->_val;
}

template<class T> T
List<T>::first(void)
{
	win = (ListNode<T>*)header->next();
	return win->_val;
}
template<class T> T
List<T>::last(void)
{
	win = (ListNode<T>*)header->prev();
	return win->_val;
}

template<class T> int
List<T>::length(void)
{
	return _length;
}

template<class T> bool
List<T>::isFirst(void)
{
	return ((win == header->next()) && (_length > 0));
}

template<class T>
bool List<T>::isLast(void)
{
	return ((win == header->prev()) && (_length > 0));
}
template<class T>
bool List<T>::isHead(void)
{
	return (win == header);
}

template<class T>
List<T>* List<T>::array2list(T* a[], int n)
{
	List<T>* list = new List<T>();
	for (int i = 0; i < n; i++) {
		list->append(a[i]);
	}
	return list;
}
template<class T> List<T>
List<T>::leastItem(const List<T>& list, int(*cmp_func)(T, T))
{
	if (list.length() == 0) return NULL;
	T least_value = list.first();
	for (list.next(); !list.isHead(); list.next())
		if (cmp_func(list.val(), least_value) < 0) least_value = list.val();
	return least_value;
}

template<class T> List<T>
List<T>::greatestItem(const List<T>& list, int(*cmp_func)(T, T))
{
	if (list.length() == 0) return NULL;
	T greatest_value = list.first();
	for (list.next(); !list.isHead(); list.next())
		if (cmp_func(list.val(), greatest_value) > 0) greatest_value = list.val();
	return greatest_value;
}
