#pragma once
#include "Stack.h"
/// Binary Search Tree
template <class KEY, class VALUE>
class BST {
private:
	/// Represents one tree node. Left subtree contains element with smaller key value than the current node's key. Right subtree - with a greater key value. 
	struct Node {
		Node(const KEY& key, const VALUE& value);
		Node& operator=(const Node& other);
		Node(const Node& other);
		~Node();

		void insert(const KEY& key, const VALUE& value);
		VALUE get(const KEY& key) const;
		Node* remove(const KEY& key, Node* parent);
		/// Finds the minimal element in the subtree
		Node* minElement();

		Node* left;
		Node* right;
		KEY key;
		VALUE value;
		
		void copyFrom(const Node& other);
	};
public:
	/// Uses a stack to iterate through the elements with depth-first
	class Iterator {
	public:
		Iterator(Node* current);
		Iterator(const Iterator& other);
		Iterator& operator=(const Iterator&);

		Iterator& operator++();
		bool operator==(const Iterator& other) const;
		bool operator!=(const Iterator& other) const;
		/// Dereference to the current value given by reference
		VALUE& operator*() const;
	private:

		Stack<Node*> nodes;
	};

	BST();
	BST(const BST<KEY, VALUE>& other);
	BST& operator= (const BST<KEY, VALUE>& other);
	~BST();
	/// Inserts a key-value pair in the tree. Throws (const char*) if the key already exists in the tree.
	void insert(const KEY& key, const VALUE& value);
	/// Returns the value associated with given key. Throws (const char*) if the key does not exist.
	VALUE get(const KEY& key) const;
	/// Removes an entry from the tree. No-throw guarantee.
	void remove(const KEY& key);
	/// Number of elements currently in the tree.
	size_t size() const;
	/// Iterator to the root of the tree.
	Iterator begin() const;
	/// Iterator representing a blank tree.
	Iterator end() const;

private:
	void copyFrom(const BST<KEY, VALUE>& other);
	size_t nodes;
	Node* root;	
};

template <class KEY, class VALUE>
BST<KEY, VALUE>::Node::Node(const KEY& key, const VALUE& value) {
	this->key = key;
	this->value = value;
	left = nullptr;
	right = nullptr;
}

template <class KEY, class VALUE>
void BST<KEY, VALUE>::Node::copyFrom(const Node& other) {
	if (this != &other) {
		delete this->left;
		delete this->right;
		key = other.key;
		value = other.value;
		left = new Node(other.left);
		right = new Node(other.right);
	}
}

template <class KEY, class VALUE>
BST<KEY, VALUE>::Node::Node(const Node& other) {
	copyFrom(other);
}

template <class KEY, class VALUE>
typename BST<KEY, VALUE>::Node& BST<KEY, VALUE>::Node::operator=(const Node& other) {
	copyFrom(other);
	return *this;
}

template <class KEY, class VALUE>
BST<KEY, VALUE>::Node::~Node() {
	if (left) delete left;
	if (right) delete right;
}

template <class KEY, class VALUE>
void BST<KEY, VALUE>::Node::insert(const KEY& key, const VALUE& value) {
	if (this->key == key)
		throw "Key already exists!";

	else if (key < this->key) {
		if (left)
			left->insert(key, value);
		else
			left = new Node(key, value);
	}
	else {
		if (right)
			right->insert(key, value);
		else
			right = new Node(key, value);
	}
}

template <class KEY, class VALUE>
VALUE BST<KEY, VALUE>::Node::get(const KEY& key) const {
	if (this->key == key)
		return value;
	
	if (key < this->key) {
		if (left)
			return left->get(key);
		else
			throw "Key not existing!";
	}
	else {
		if (right)
			return right->get(key);
		else {
			throw "Key not existing!";
		}
	}
}

template <class KEY, class VALUE>
typename BST<KEY, VALUE>::Node* BST<KEY, VALUE>::Node::remove(const KEY& key, Node* parent) {
	if (key < this->key) {
		if (left)
			return left->remove(key, this);
		else
			return nullptr;
	}
	else if (key > this->key) {
		if (right)
			return right->remove(key, this);
		else 
			return nullptr;
	}
	else {
		if (left && right) {
			Node* min = right->minElement();
			this->key = min->key;
			this->value = min->value;
			return right->remove(this->key, this);
		}
		else if (parent->left == this) {
			if (left)
				parent->left = left;
			else
				parent->left = right;
			
			return this;
		}
		else {
			if (left)
				parent->right = left;
			else
				parent->right = right;

			return this;
		}
	}
}

template <class KEY, class VALUE>
typename BST<KEY, VALUE>::Node* BST<KEY, VALUE>::Node::minElement() {
	if (!left)
		return this;
	else
		return left->minElement();
}

template <class KEY, class VALUE>
BST<KEY, VALUE>::BST() {
	root = nullptr;
	nodes = 0;
}

template <class KEY, class VALUE>
BST<KEY, VALUE>::~BST() {
	delete root;
}

template <class KEY, class VALUE>
BST<KEY, VALUE>::BST(const BST<KEY, VALUE>& other) {
	copyFrom(other);
}

template <class KEY, class VALUE>
BST<KEY, VALUE>& BST<KEY, VALUE>::operator=(const BST<KEY, VALUE>& other) {
	copyFrom(other);
	return *this;
}

template <class KEY, class VALUE>
void BST<KEY, VALUE>::copyFrom(const BST<KEY, VALUE>& other) {
	if (this != &other) {
		if (root)
			delete root;
		this->root = new Node(other.root);
		this->nodes = other.nodes;
	}
}

template <class KEY, class VALUE>
void BST<KEY, VALUE>::insert(const KEY& key, const VALUE& value) {
	if (!root) {
		root = new Node(key, value);
	}
	else {
		root->insert(key, value);
	}
	++nodes;
}

template <class KEY, class VALUE>
size_t BST<KEY, VALUE>::size() const {
	return nodes;
}

template <class KEY, class VALUE>
VALUE BST<KEY, VALUE>::get(const KEY& key) const {
	if (root)
		return root->get(key);
	throw "Tree is empty!";
}

template <class KEY, class VALUE>
void BST<KEY, VALUE>::remove(const KEY& key) {
	if (!root)
		return;

	if (root->key == key) {
		Node* temp = new Node(key, root->value);
		temp->left = root;
		Node* removedNode = root->remove(key, temp);
		root = temp->left;
		if (removedNode) {
			removedNode->left = nullptr;
			removedNode->right = nullptr;
			delete removedNode;
			--nodes;
		}
	}
	else {
		Node* removedNode = root->remove(key, nullptr);
		if (removedNode) {
			removedNode->left = nullptr;
			removedNode->right = nullptr;
			delete removedNode;
			--nodes;
		}
	}
	
}

template <class KEY, class VALUE>
typename BST<KEY, VALUE>::Iterator BST<KEY, VALUE>::begin() const {
	return Iterator(root);
}

template <class KEY, class VALUE>
typename BST<KEY, VALUE>::Iterator BST<KEY, VALUE>::end() const {
	return Iterator(nullptr);
}

template <class KEY, class VALUE>
BST<KEY, VALUE>::Iterator::Iterator(Node* current) {
	if (current)
		nodes.Push(current);
}

template <class KEY, class VALUE>
BST<KEY, VALUE>::Iterator::Iterator(const Iterator& other) {
	if (this != &other) {
		this->nodes = other.nodes;
	}
}

template <class KEY, class VALUE>
typename BST<KEY, VALUE>::Iterator& BST<KEY, VALUE>::Iterator::operator++() {
	Node* t = nodes.Pop();
	if (t->right)
		nodes.Push(t->right);
	if (t->left)
		nodes.Push(t->left);

	return *this;
}

template <class KEY, class VALUE>
typename BST<KEY, VALUE>::Iterator& BST<KEY, VALUE>::Iterator::operator=(const Iterator& other) {
	if (this != &other) {
		this->nodes = other.nodes;
	}
	return *this;
}

template <class KEY, class VALUE>
bool BST<KEY, VALUE>::Iterator::operator==(const Iterator& other) const {
	if (other.nodes.Size() == 0)
		if (this->nodes.Size() == 0)
			return true;
		else
			return false;
	else if (this->nodes.Size() == 0)
			return false;
	else if (this->nodes.Peak() == other.nodes.Peak())
		return true;
	else return false;
}

template <class KEY, class VALUE>
bool BST<KEY, VALUE>::Iterator::operator!=(const Iterator& other) const {
	return !((*this) == other);
}

template <class KEY, class VALUE>
VALUE& BST<KEY, VALUE>::Iterator::operator*() const {
	return this->nodes.Peak()->value;
}
