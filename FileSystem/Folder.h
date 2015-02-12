#pragma once
#include "Entry.hpp"
#include "File.h"
#include "List.hpp"

class Folder : public Entry {
public:
	Folder(std::string name) : Entry(name) {}
	Folder(const Folder& other);
	Folder& operator=(const Folder& other);
	Folder(std::istream& file);
	virtual ~Folder();

	virtual void copyFrom(const Folder& other);

	virtual void getInfo(std::ostream& out);

	virtual void saveToFile(std::ostream& file) const;

	void deleteFile(std::string name);
	void deleteFolder(std::string name);
	void printContents(std::ostream& out, size_t offset = 0) const;
	void addFile(std::string name, size_t fileID);
	void addFile(File* file);
	void addFolder(std::string name);
	Folder* getFolder(std::string name);
	File* getFile(std::string name);
	List<size_t> getFilesIDs() const;

protected:
	List<Folder*> folders;
	List<File*> files;

};