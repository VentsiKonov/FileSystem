#pragma once
#include "List.hpp"
template <class T, class G>
class HashTable {
public:
	HashTable(size_t (*hashFN)(T), size_t bucketsCount = 10);
	HashTable(const HashTable<T, G> &other);
	HashTable<T, G>& operator=(const HashTable<T, G> &other);
	~HashTable();

	bool contains(const T& key) const;
	G get(const T& key) const;
	G remove(const T& key);
	G pop(); // Awkward
	void put(const T& key, const G& value);
	size_t size() const;

private:
	struct Entry {
		T key;
		G value;
	};

	size_t (*hashFN)(T);
	size_t bucketsCount;
	List<Entry>* buckets;
	size_t elements;

	void copyFrom(const HashTable<T, G> &other);

};

template <class T, class G>
HashTable<T, G>::HashTable(size_t (*hash)(T), size_t bucketsCount){
	this->bucketsCount = bucketsCount;
	buckets = new List<Entry>[bucketsCount];
	hashFN = hash;
	elements = 0;
}

template <class T, class G>
void HashTable<T, G>::copyFrom(const HashTable<T, G>& other) {
	if (this != &other) {
		bucketsCount = other.bucketsCount;
		this->buckets = new List<Entry>[bucketsCount];
		for (size_t i = 0; i < bucketsCount; i++) {
			buckets[i] = other.buckets[i];
		}
		this->elements = other.elements;
		this->hashFN = other.hashFN;
	}
}

template <class T, class G>
HashTable<T, G>::HashTable(const HashTable<T, G>& other) {
	copyFrom(other);
}

template <class T, class G>
HashTable<T, G>& HashTable<T, G>::operator=(const HashTable<T, G>& other) {
	copyFrom(other);
	return *this;
}

template <class T, class G>
HashTable<T, G>::~HashTable() {
	delete [] buckets;
}

template <class T, class G>
size_t HashTable<T, G>::size() const {
	return elements;
}

template <class T, class G>
G HashTable<T, G>::get(const T& key) const {
	size_t hash = hashFN(key);
	List<Entry> &bucket = buckets[hash % bucketsCount];
	for (List<Entry>::Iterator i = bucket.Begin(); i != bucket.End(); ++i) {
		if ((*i).key == key) {
			return (*i).value;
		}
	}
	
	throw "Element not existing";
}

template <class T, class G>
void HashTable<T, G>::put(const T& key, const G& value) {
	size_t hash = hashFN(key);
	Entry e;
	e.key = key;
	e.value = value;
	buckets[hash % bucketsCount].PushBack(e);
	++elements;
}

template <class T, class G>
bool HashTable<T, G>::contains(const T& key) const {
	if (elements == 0)
		return false;

	try {
		get(key);
		return true;
	}
	catch (const char*) {
		return false;
	}
}

template <class T, class G>
G HashTable<T, G>::remove(const T& key) {
	size_t hash = hashFN(key);
	G result;
	bool found = false;
	size_t c = 0;
	List<Entry> &bucket = buckets[hash % bucketsCount];
	for (List<Entry>::Iterator i = bucket.Begin(); i != bucket.End(); ++i) {
		if ((*i).key == key) {
			result = (*i).value;
			found = true;
			break;
		}
		++c;
	}

	if (found)
		return bucket.PopAt(c).value;

	else
		throw "Element not existing";

}

template <class T, class G>
G HashTable<T, G>::pop() {
	for (size_t i = 0; i < bucketsCount; i++) {
		if (buckets[i].Size() > 0) {
			--elements;
			return buckets[i].PopFront().value;
		}
	}

	throw "Hash table empty!";
}
