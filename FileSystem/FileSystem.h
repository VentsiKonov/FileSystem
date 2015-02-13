#pragma once
#include "Folder.h"
#include "FragmentFileManager.h"
#include <string>
class FileSystem {
public:
	FileSystem(const std::string& fileName);
	~FileSystem();

	bool deleteEntry(std::string path);

	void createFolder(std::string path);
	void createFile(std::string path);
	void printStructure(std::ostream& out);
	void appendText(std::string filePath, std::string data);
	void moveFile(std::string filePath, std::string newFilePath);
	
	void rename(std::string path, std::string newName);
	void copyFile(std::string filePath, std::string newFilePath);
	void importFile(std::string realFilePath, std::string filePath);
	bool exportEntry(std::string path, std::string realFSPath);

private:
	void operator=(const FileSystem&);
	FileSystem(const FileSystem&);

	void exportFile(std::string filePath, std::string realFSPath);
	void exportFolder(std::string path, std::string realFSPath);
	void deleteFolder(std::string path);
	void deleteFile(std::string filePath);
	List<std::string> parsePath(std::string path);
	Folder* getFolder(List<std::string> computedPath);
	size_t uid;

	Folder* fileStructure;
	FragmentFileManager* fileManager;
};