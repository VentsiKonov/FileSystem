#pragma once

template <class T>
class List {
protected:
	class Node {
	public:
		T data;
		Node* next;
		Node();
		Node(const T& data, Node* next);
	private:
		Node(const Node& n);
		void operator=(const Node& n);
	};
public:
	class Iterator {
	private:
		Node* current;

	public:
		Iterator(Node* first) : current(first){}
		Iterator& operator=(const Iterator& other);
		Iterator operator++(int);
		Iterator& operator++();
		Iterator& operator+=(size_t n);
		bool operator==(Iterator& other);
		bool operator!=(Iterator& other);
		T& operator*() const;
	};

public:
	List();
	~List();
	List<T>& operator=(const List<T>& l);
	List(const List<T>& l);

	void PushFront(const T data);
	void PushBack(const T data);
	void PushAt(size_t index, const T data);

	T PopFront();
	T PopBack();
	T PopAt(size_t index);

	T& Front() const;
	T& Back() const;
	T& ElementAt(size_t index) const;

	void Clear();
	bool IsEmpty() const;
	size_t Size() const;

	Iterator Begin() const;
	Iterator End() const;

protected:
	Node* first;
	Node* last;
	size_t size;

	void CopyFrom(const List<T>& l);
	Node* GetNode(size_t index) const;
};

template <class T>
List<T>::Node::Node() {
	next = nullptr;
}

template <class T>
List<T>::Node::Node(const T& data, Node* next) {
	this->data = data;
	this->next = next;
}

template <class T>
List<T>::List() {
	first = last = nullptr;
	size = 0;
}

template <class T>
List<T>::~List() {
	Clear();
}

template <class T>
void List<T>::CopyFrom(const List<T>& l) {
	Node* it = l.first;
	if (!it) return;
	do {
		this->PushBack(it->data);
		it = it->next;
	} while (it);
}

template <class T>
void List<T>::Clear() {
	Node* current;
	while (size > 0) {
		current = first;
		first = first->next;
		--size;
		delete current;
	}
	last = nullptr;
	first = nullptr;
}

template <class T>
List<T>::List(const List<T>& l) {
	if (this != &l) {
		size = 0;
		first = last = nullptr;
		this->CopyFrom(l);
	}
}

template <class T>
List<T>& List<T>::operator=(const List<T>& l) {
	if (this != &l) {
		this->Clear();
		this->CopyFrom(l);
	}

	return *this;
}

template <class T>
void List<T>::PushBack(const T data) {
	if (!first) 
		return PushFront(data);

	last = last->next = new Node(data, nullptr);
	++size;
}

template <class T>
void List<T>::PushFront(const T data) {
	first = new Node(data, first);
	if (!last) last = first;
	++size;
}

template <class T>
void List<T>::PushAt(size_t index, const T data) {
	if (!size) return PushFront(data);
	if (size == index) return PushBack(data);
	if (size < index) throw "Invalid index: Size of list is less then the index!";

	Node* prev = GetNode(index - 1);
	prev->next = new Node(data, prev->next);
	++size;
}

template <class T>
T& List<T>::Front() const {
	if (!first) throw "Empty list!";
	return first->data;
}

template <class T>
T& List<T>::Back() const {
	if (!last) throw "Cannot take last element!";
	return last->data;
}

template <class T>
T& List<T>::ElementAt(size_t index) const {
	return GetNode(index)->data;
}

template <class T>
size_t List<T>::Size() const {
	return size;
}

template <class T>
T List<T>::PopFront() {
	T returnValue = Front();
	Node* current = first;
	first = first->next;
	--size;
	if (size == 0)
		last = first = nullptr;
	delete current;
	return returnValue;
}

template <class T>
T List<T>::PopBack() {
	if (size == 1) 
		return PopFront();
	
	T returnValue = Back();
	Node* it = GetNode(this->size - 2);
	last = it;
	delete last->next;
	last->next = nullptr;
	--size;
	return returnValue;
}

template <class T>
T List<T>::PopAt(size_t index) {
	if (index == 0) return PopFront();
	if (index >= size) throw "Index out of range!";
	Node* prev = GetNode(index - 1);
	Node* current = prev->next;
	T returnValue = current->data;
	prev->next = current->next;
	delete current;
	--size;
	return returnValue;
}

template <class T>
typename List<T>::Node* List<T>::GetNode(size_t index) const {
	Node* it = first;
	while (index--) {
		it = it->next;
	}
	return it;
}

template <class T>
bool List<T>::IsEmpty() const {
	return size == 0;
}

template <class T>
typename List<T>::Iterator& List<T>::Iterator::operator++() {
	if (current != nullptr)
		current = current->next;
	
	return *this;
}

template <class T>
typename List<T>::Iterator List<T>::Iterator::operator++(int) {
	Iterator temp(current);
	++(*this);
	return temp;
}

template <class T>
bool List<T>::Iterator::operator==(Iterator& other) {
	return !((*this) != other);
}

template <class T>
bool List<T>::Iterator::operator!=(Iterator& other) {
	return current != other.current;
}

template <class T>
typename List<T>::Iterator& List<T>::Iterator::operator+=(size_t n) {
	while (n--)
		++(*this);

	return *this;
}

template <class T>
T& List<T>::Iterator::operator*() const{
	return current->data;
}

template <class T>
typename List<T>::Iterator& List<T>::Iterator::operator=(const Iterator& other) {
	this->current = other.current;
	return *this;
}

template <class T>
typename List<T>::Iterator List<T>::Begin() const{
	return Iterator(first);
}

template <class T>
typename List<T>::Iterator List<T>::End() const {
	return Iterator(nullptr);
}

