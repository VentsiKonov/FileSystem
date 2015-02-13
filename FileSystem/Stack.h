#pragma once

// Singly linked list with just one pointer - top and no functionality to add/remove elements to back or middle.

template <class T>
class Stack {
private:
	struct Element {
		T value;
		Element* next;
		
		Element() {
			this->next = nullptr;
		}
		Element(const T& value, Element* next){
			this->value = value;
			this->next = next;
		}
		~Element() {
			// No cascade deletion of elements so no additional work to do
		}
	private:
		Element(const Element& el);
		void operator=(const Element& el);
	};

public:
	Stack();
	Stack(const Stack<T>& st);
	Stack<T>& operator=(const Stack<T>& st);
	~Stack();


	void Push(T value);
	T Pop();
	T Peak() const;
	bool Empty() const;
	void Clean();
	size_t Size() const;

private:
	Element* top;
	size_t count;
	void CopyData(const Stack<T>& st);

};

template <class T>
Stack<T>::Stack() {
	this->count = 0;
	this->top = NULL;
}

template <class T>
Stack<T>::Stack(const Stack<T>& st) {
	CopyData(st);
}

template <class T>
Stack<T>& Stack<T>::operator=(const Stack& st) {
	CopyData(st);

	return *this;
}

template <class T>
Stack<T>::~Stack() {
	if (!this->Empty())
		this->Clean();
}

template <class T>
bool Stack<T>::Empty() const {
	return !this->count;
}

template <class T>
void Stack<T>::Push(T value) {
	this->top = new Element(value, this->top);
	++this->count;
}

template <class T>
T Stack<T>::Peak() const {
	if (this->top)
		return this->top->value;
	else
		throw "Stack is empty!";
}

template <class T>
T Stack<T>::Pop() {
	T value = this->Peak();
	Element* top = this->top;
	this->top = this->top->next;
	delete top;
	--this->count;
	return value;
}

template <class T>
void Stack<T>::Clean() {
	if (!this->Empty()) {
		Element* it;
		while (this->top) {
			it = this->top;
			this->top = it->next;
			delete it;
			--this->count;
		}
	}
	else {
		this->count = 0;
		this->top = NULL;
	}
}

template <class T>
void Stack<T>::CopyData(const Stack<T>& st) {
	if (this != &st) {
		this->Clean();
		Stack<T> temp;
		for (Element* it = st.top; it != NULL; it = it->next) {
			temp.Push(it->value);
		}
		while (!temp.Empty()) {
			this->Push(temp.Pop());
		}
	}
}

template <class T>
size_t Stack<T>::Size() const {
	return this->count;
}
