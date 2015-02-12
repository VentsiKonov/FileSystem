#pragma once
#include "Folder.h"
#include "FragmentFileManager.h"
#include <string>
class FileSystem {
public:
	FileSystem(std::string fileName);
	~FileSystem();

	void createFolder(std::string path);
	void createFile(std::string path);
	void printStructure(std::ostream& out);
	void appendText(std::string filePath, std::string data);
	void exportFile(std::string filePath, std::string realFSPath);
	void moveFile(std::string filePath, std::string newFilePath);
	void deleteFolder(std::string path);
	void deleteFile(std::string filePath);
	void rename(std::string path, std::string newName);
	void copyFile(std::string filePath, std::string newFilePath);
	void importFile(std::string realFilePath, std::string filePath);

private:
	void operator=(const FileSystem&);
	FileSystem(const FileSystem&);

	List<std::string> parsePath(std::string path);
	Folder* getFolder(List<std::string> computedPath);
	size_t uid;

	Folder* fileStructure;
	FragmentFileManager* fileManager;
};