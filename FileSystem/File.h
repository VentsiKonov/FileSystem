#pragma once
#include "Entry.hpp"
class File : public Entry {
public:
	File(std::string name, size_t id);
	File(const File& other);
	File& operator=(const File& other);
	File(std::istream& file);
	virtual ~File() {}

	virtual void copyFrom(const File& other);

	virtual void getInfo(std::ostream& out, size_t fragmentSize);

	virtual void saveToFile(std::ostream& file) const;

	size_t getId() const;
	size_t getStartPos() const;
	size_t getFragmentsCount()const;
	void addFragments(size_t number, size_t startPos = 0);
protected:

	size_t fragmentsCount;
	size_t id;
	int firstBlockPos;
};