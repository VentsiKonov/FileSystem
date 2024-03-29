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
	if (this != &other) {
		Entry::copyFrom(other);
		fragmentsCount = other.fragmentsCount;
		id = other.id;
		firstBlockPos = other.firstBlockPos;
	}
}

void File::getInfo(std::ostream& out, size_t fragmentSize) {
	/// The size o one fragments is unkown by the file, so it is passed as a parameter.
	Entry::getInfo(out);
	out << '\n';
	out << "\tSize on disk: " << fragmentsCount * fragmentSize << " bytes.";
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