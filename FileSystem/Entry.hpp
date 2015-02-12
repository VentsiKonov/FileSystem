#pragma once
#include <string>
#include <fstream>
class Entry {
public:
	Entry(std::string name) : name(name) {}
	Entry(const Entry& other){
		copyFrom(other);
	}
	Entry& operator=(const Entry& other) {
		copyFrom(other);
		return *this;
	}
	Entry(std::istream& file) {
		size_t size;
		file.read((char*)&size, sizeof(size));
		if (size > 50) {
			throw std::exception("Unexpected entry name length!");
		}
		char* nameBuf = new char[size + 1];
		file.read(nameBuf, size);
		nameBuf[size] = '\0';
		name.assign(nameBuf);
		delete nameBuf;
	}
	virtual void copyFrom(const Entry& other) {
		if (this != &other) {
			name = other.name;
		}
	}
	virtual ~Entry() {}

	virtual void rename(std::string name) {
		this->name = name;
	}
	virtual void getInfo(std::ostream& out) {
		out << name;
	}

	virtual void saveToFile(std::ostream& file) const{
		size_t size = name.size();
		file.write((char*)&size, sizeof(size));
		file.write(name.c_str(), size);
	}

	virtual std::string getName() {
		return name;
	}

protected:
	std::string name;
};