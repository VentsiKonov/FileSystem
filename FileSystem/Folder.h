#pragma once
#include "Entry.hpp"
#include "List.hpp"
#include "File.h"
#include "BST.hpp"
#include <functional>
/// Represents a Folder from the filesystem tree. Folders are NOT included in any way in the filesystem fragments, they exist as containers for files in the tree.
/// Files and folders are separated for improved performance in two different Binary Search Trees + the ability to have a file and a folder in one folder with the same name.
class Folder : public Entry {
public:
	Folder(std::string name) : Entry(name) {}
	Folder(const Folder& other);
	Folder& operator=(const Folder& other);
	/// Restores an object from given file.
	Folder(std::istream& file);
	virtual ~Folder();

	virtual void copyFrom(const Folder& other);

	virtual void getInfo(std::ostream& out);

	/// Provides functionality to save the state of an obeject to a file recursively saving all contained objects.
	virtual void saveToFile(std::ostream& file) const;

	Folder* removeFolder(std::string name);
	void deleteFile(std::string name);
	void deleteFolder(std::string name);
	/// Recursively prints the contents of the current folder and all subfolders
	void printContents(std::ostream& out, size_t offset = 0) const;
	void addFile(std::string name, size_t fileID);
	void addFile(File* file);
	void addFolder(std::string name);
	void addFolder(Folder* folder);
	Folder* getFolder(std::string name);
	File* getFile(std::string name);
	void clear();
	/// Deletes all descendand files from the current directory and all subdirectories. delFunc is a function that handles File deletion properly.
	void deleteAllFiles(std::function<void (size_t, size_t, size_t)> delFunc);
	/// Exports current directory and all subdirectories in the real filesystem. fileExportFN is a function that handles File export properly.
	void exportToFS(std::string realFSPath, std::function<void(size_t, size_t, size_t, std::string)> fileExportFN);

protected:
	BST<std::string, Folder*> folders;
	BST<std::string, File*> files;

};