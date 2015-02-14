#pragma once
#include "Entry.hpp"
/// Represents a File in a File Structure tree. Contains data about the file's fragments in the filesystem file.
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
	/// After data is appended to a file, the file needs to be updated with the size of the data it now contains
	void addFragments(size_t number, size_t startPos = 0);
protected:

	size_t fragmentsCount;
	size_t id;
	int firstBlockPos;
};