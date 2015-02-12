#include "File.h"
File::File(std::string name, size_t id) : Entry(name) {
	fragmentsCount = 0;
	this->id = id;
	firstBlockPos = 0;
}

File& File::operator=(const File& other) {
	copyFrom(other);
	return *this;
}

File::File(const File& other) : Entry(other) {
	copyFrom(other);
}

void File::copyFrom(const File& other) {
	// Independent from real files!
	// If copy c-tor is used, it is assumed the old file in the tree will be removed leaving the new one as unique by id!

	if (this != &other) {
		Entry::copyFrom(other);
		fragmentsCount = other.fragmentsCount;
		id = other.id;
		firstBlockPos = other.firstBlockPos;
	}
}

void File::getInfo(std::ostream& out) {
	Entry::getInfo(out);
	out << '\n';
	out << "\tSize: " << fragmentsCount * 32 << " bytes.";
}

void File::saveToFile(std::ostream& file) const{
	Entry::saveToFile(file);
	file.write((char*)&id, sizeof(id));
	file.write((char*)&fragmentsCount, sizeof(fragmentsCount));
	file.write((char*)&firstBlockPos, sizeof(firstBlockPos));
}

File::File(std::istream& file) : Entry(file){

	file.read((char*)&id, sizeof(id));
	file.read((char*)&fragmentsCount, sizeof(fragmentsCount));
	file.read((char*)&firstBlockPos, sizeof(firstBlockPos));
}

size_t File::getId() const {
	return id;
}

size_t File::getFragmentsCount() const {
	return fragmentsCount;
}

size_t File::getStartPos() const {
	return firstBlockPos;
}

void File::addFragments(size_t count, size_t startPosition) {
	if (firstBlockPos == 0) {
		firstBlockPos = startPosition;
	}
	fragmentsCount += count;
}