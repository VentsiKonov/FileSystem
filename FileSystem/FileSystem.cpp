#include "FileSystem.h"
#include <fstream>
#include <iostream>
#include <functional>
FileSystem::FileSystem(const std::string& fileName) {
	if (fileName.empty()) {
		throw std::string("File name cannot be empty!");
	}
	std::fstream file(fileName); 
	fileManager = nullptr;
	fileStructure = nullptr;
	if (!file) {
		std::ofstream create(fileName); // Creates the file
		create.close();
		file.close();
		fileManager = new FragmentFileManager(fileName);
		fileStructure = new Folder("root");
	}
	else {
		file.close();
		fileManager = new FragmentFileManager(fileName);
		try {
			fileStructure = new Folder(fileManager->loadRoot());
		}
		catch (std::exception ex) {
			std::cerr << ex.what();
			delete fileManager;
			delete fileStructure;
			throw;
			//fileStructure = new Folder("root");
		}
	}

}

FileSystem::~FileSystem() {
	fileManager->saveToDisk(*fileStructure);
	delete fileStructure;
	delete fileManager;
}

/// Parse a given string path to a list of names 
List<std::string> FileSystem::parsePath(std::string path) {
	std::string word = "";
	List<std::string> computedPath;
	
	// Defend against paths starting with / or \ 
	for (std::string::iterator i = path.begin(); i != path.end(); ++i) {
		if ((*i) == '\\' || (*i) == '/') {
			computedPath.PushBack(word);
			word = "";
		}
		else {
			word += (*i);
		}
	}
	computedPath.PushBack(word);

	return computedPath;
}

Folder* FileSystem::getFolder(List<std::string> cPath) {
	if (cPath.IsEmpty()) // means we need the current directory
		return fileStructure;
	if (cPath.Size() == 1 && cPath.Front() == fileStructure->getName()) // also means we need the current directory
		return fileStructure;
	Folder* folder = fileStructure;

	while (folder != nullptr && !cPath.IsEmpty()) { // proceed into subtrees looking for the one we need

		folder = folder->getFolder(cPath.PopFront());
	}

	return folder;
}

std::string FileSystem::currentFolderName() const {
	return fileStructure->getName();
}

void FileSystem::createFolder(std::string path) {
	List<std::string> cPath(parsePath(path));

	std::string folderName = cPath.PopBack();
	Folder* containingFolder = getFolder(cPath);
	
	if (!containingFolder)
		throw std::string("Cannot find requested folder: " + path);
	try {
		containingFolder->addFolder(folderName);
	}
	catch (const char* ex) {
		throw std::string("Folder with the same name already exists!");
	}

}

void FileSystem::createFile(std::string path) {
	List<std::string> cPath(parsePath(path));

	std::string fileName = cPath.PopBack();
	Folder* containingFolder = getFolder(cPath);
	if (!containingFolder)
		throw std::string("Cannot find requested folder: " + path);
	try {
		containingFolder->addFile(fileName, fileManager->getUid());
	}
	catch (const char* ex) {
		throw std::string(ex);
	}
}

void FileSystem::printStructure(std::ostream& out) {
	fileStructure->printContents(out);
}

void FileSystem::appendText(std::string filePath, std::string data) {
	List<std::string> cPath(parsePath(filePath));
	
	std::string fileName = cPath.PopBack();
	Folder* containingFolder = getFolder(cPath);
	if (!containingFolder)
		throw std::string("Cannot find requested folder: " + filePath);

	File* file = containingFolder->getFile(fileName);
	if (!file) 
		throw std::string("Cannot find requested file: " + filePath);
	
	size_t dataSize = data.length(); // Size in bytes
	const char* dataRaw = data.c_str();
	std::pair<size_t, size_t> info = fileManager->saveData(dataRaw, dataSize, file->getId(), file->getFragmentsCount());
	file->addFragments(info.first, info.second);
}

void FileSystem::exportFile(std::string filePath, std::string realFSPath) {
	List<std::string> cPath(parsePath(filePath));

	std::string fileName = cPath.PopBack();
	Folder* containingFolder = getFolder(cPath);
	if (!containingFolder)
		throw std::string("Cannot find requested folder: " + filePath);

	File* file = containingFolder->getFile(fileName);
	if (!file)
		throw std::string("Cannot find requested file: " + filePath);


	fileManager->exportFile(file->getId(), file->getStartPos(), file->getFragmentsCount(), realFSPath);
}

void FileSystem::exportFolder(std::string path, std::string realFSPath) {
	List<std::string> cPath(parsePath(path));

	Folder* folder = getFolder(cPath);
	if (!folder)
		throw std::string("Cannot find requested folder: " + path);

	std::function<void(size_t, size_t, size_t, std::string)> fp = 
		std::bind(
		&FragmentFileManager::exportFile, fileManager, 
		std::placeholders::_1, std::placeholders::_2, 
		std::placeholders::_3, std::placeholders::_4
		);

	folder->exportToFS(realFSPath, fp);
}

void FileSystem::moveFile(std::string filePath, std::string newFilePath) {
	List<std::string> cPath(parsePath(filePath));
	List<std::string> cNewPath(parsePath(newFilePath));

	std::string fileName = cPath.PopBack();
	Folder* containingFolder = getFolder(cPath);
	if (!containingFolder)
		throw std::string("Cannot find requested folder: " + filePath);

	Folder* newFolder = getFolder(cNewPath);
	if (!newFolder)
		throw std::string("Cannot find requested folder: " + newFilePath);

	File* file = containingFolder->getFile(fileName);
	if (!file)
		throw std::string("Cannot find requested file: " + filePath);

	newFolder->addFile(new File(*file));
	containingFolder->deleteFile(fileName);
}

void FileSystem::deleteFile(std::string filePath) {
	List<std::string> cPath(parsePath(filePath));

	std::string fileName = cPath.PopBack();
	Folder* containingFolder = getFolder(cPath);
	if (!containingFolder)
		throw std::string("Cannot find requested folder: " + filePath).c_str();

	File* file = containingFolder->getFile(fileName);
	if (!file)
		throw std::string("Cannot find requested file: " + filePath).c_str();

	fileManager->deleteFragments(file->getId(), file->getStartPos(), file->getFragmentsCount());
	containingFolder->deleteFile(fileName);
}

void FileSystem::deleteFolder(std::string path) {
	List<std::string> cPath(parsePath(path));

	std::string folderName = cPath.PopBack();
	Folder* containingFolder = getFolder(cPath);
	if (!containingFolder)
		throw std::string("Cannot find requested folder: " + path).c_str();
	// deleteFragments is not static, so we need to provide a 'this' pointer to it implicitly or give a pointer to the FragmentFileManager object, which means the folder and all files can harm the behaviour.
	std::function<void(size_t, size_t, size_t)> fp = std::bind(&FragmentFileManager::deleteFragments, fileManager, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	containingFolder->getFolder(folderName)->deleteAllFiles(fp);
	containingFolder->deleteFolder(folderName);
}

void FileSystem::rename(std::string entryPath, std::string newName) { // TODO: Fix for root
	List<std::string> cPath(parsePath(entryPath));

	std::string entryName = cPath.PopBack();
	Folder* containingFolder = getFolder(cPath);
	if (!containingFolder)
		throw std::string("Cannot find requested folder: " + entryPath);

	Entry* entry = containingFolder->getFolder(entryName);
	if (!entry) {
		entry = containingFolder->getFile(entryName);
	}
	if (!entry) {
		throw std::string("Cannot find requested resource: " + entryPath);

	}
	entry->rename(newName);
}

void FileSystem::copyFile(std::string filePath, std::string newFilePath) {
	// Move a pointer to a file in different subtree of the filesystem.
	List<std::string> cPath(parsePath(filePath));
	List<std::string> cNewPath(parsePath(newFilePath));

	std::string fileName = cPath.PopBack();
	std::string newFileName = cNewPath.PopBack();

	Folder* containingFolder = getFolder(cPath);
	if (!containingFolder)
		throw std::exception(std::string("Cannot find requested folder: " + filePath).c_str());

	Folder* newFolder = getFolder(cNewPath);
	if (!newFolder)
		throw std::exception(std::string("Cannot find requested folder: " + newFilePath).c_str());

	File* file = containingFolder->getFile(fileName);
	if (!file)
		throw std::exception(std::string("Cannot find requested file: " + filePath).c_str());

	newFolder->addFile(newFileName, fileManager->getUid());
	File* newFile = newFolder->getFile(newFileName);

	std::pair<size_t, size_t> fileData = fileManager->copyFragments(file->getId(), file->getStartPos(), file->getFragmentsCount(), newFile->getId());
	newFile->addFragments(fileData.first, fileData.second);
}

void FileSystem::importFile(std::string realFilePath, std::string filePath) {
	List<std::string> cPath(parsePath(filePath));

	std::string fileName = cPath.PopBack();
	Folder* containingFolder = getFolder(cPath);
	if (!containingFolder)
		throw std::string("Cannot find requested folder: " + filePath);

	containingFolder->addFile(fileName, fileManager->getUid());
	File* file = containingFolder->getFile(fileName);
	if (!file)
		throw std::string("Cannot find requested file: " + filePath);

	std::pair<size_t, size_t> fileData = fileManager->importFile(realFilePath, file->getId());
	file->addFragments(fileData.first, fileData.second);
}

bool FileSystem::deleteEntry(std::string path) { 
	try {
		deleteFile(path);
		return true;
	}
	catch(const char*){
		try {
			deleteFolder(path);
			return true;
		}
		catch (const char*) {
			return false;
		}
	}
	
}

bool FileSystem::exportEntry(std::string path, std::string realFSPath) {
	try {
		exportFile(path, realFSPath);
		return true;
	}
	catch (std::string ex) {
		try {
			exportFolder(path, realFSPath);
			return true;
		}
		catch (std::string ex) {
			return false;
		}
	}
}

void FileSystem::printEntryInfo(std::string path, std::ostream& out) {
	List<std::string> cPath(parsePath(path));

	std::string entryName = cPath.PopBack();
	Folder* containingFolder = getFolder(cPath);
	if (!containingFolder)
		throw std::string("Cannot find requested folder: " + path);

	Entry* entry = containingFolder->getFolder(entryName);
	if (!entry) {
		entry = containingFolder->getFile(entryName);
		if (!entry) {
			throw std::string("Cannot find requested resource: " + path);
		}
		else {
			((File*)entry)->getInfo(out, fileManager->getFragmentSize());
			return;
		}
	}
	
	entry->getInfo(out);
	


}

void FileSystem::moveFolder(std::string folderPath, std::string containingFolderPath) {
	// Just move a pointer to a folder in another folder.

	List<std::string> cPath(parsePath(folderPath));
	List<std::string> cNewPath(parsePath(containingFolderPath));
	
	std::string currentFolderName = cPath.PopBack();
	Folder* oldContainingFolder = getFolder(cPath);
	if (!oldContainingFolder)
		throw std::string("Cannot find requested folder: " + folderPath);
	std::string newFolderName = cNewPath.PopBack();
	Folder* folder;
	try {
		folder = oldContainingFolder->removeFolder(currentFolderName);
	}
	catch (const char* ex) {
		throw std::string("Cannot remove folder: " + currentFolderName);
	}
	if (!folder) 
		throw std::string("Cannot find requested folder: " + folderPath);

	folder->rename(newFolderName);
	Folder* newContainingFolder = getFolder(cNewPath);
	if (!newContainingFolder)
		throw std::string("Cannot find requested folder: " + containingFolderPath);

	try {
		newContainingFolder->addFolder(folder);
	}
	catch (const char*) {
		folder->rename(currentFolderName);
		oldContainingFolder->addFolder(folder);
		throw std::string("Folder already contains a folder with the same name!");
	}

}
