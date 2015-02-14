#pragma once
#include "Folder.h"
#include "FragmentFileManager.h"
#include <string>

class FileSystem {
public:
	FileSystem(const std::string& fileName);
	~FileSystem();

	/// Deletes file or folder from the filesystem with all file fragments as well.
	bool deleteEntry(std::string path);
	/// Prints info about the file/folder
	void printEntryInfo(std::string path, std::ostream& out);

	/// Creates a folder in the tree with a relative path to root/
	void createFolder(std::string path);
	/// Creates a file with new fragments in a directory relative to root/
	void createFile(std::string path);
	/// Prints the structure from the current directory down.
	void printStructure(std::ostream& out);
	/// Appends text at the end of given file.
	void appendText(std::string filePath, std::string data);
	/// Moves a file to a different EXISTING folder
	void moveFile(std::string filePath, std::string newFilePath);
	/// Moves a folder to different existing folder with a new name
	void moveFolder(std::string folderPath, std::string containingFolder);
	
	/// Renames an entry in the file structure. Throws std::string.
	void rename(std::string path, std::string newName);
	/// Copies a file with it's fragments
	void copyFile(std::string filePath, std::string newFilePath);
	/// Imports a file from the real filesystem.
	void importFile(std::string realFilePath, std::string filePath);
	/// Exports file or folder to the real filesystem.
	bool exportEntry(std::string path, std::string realFSPath);
	/// Current root folder name in case it changes.
	std::string currentFolderName() const;

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