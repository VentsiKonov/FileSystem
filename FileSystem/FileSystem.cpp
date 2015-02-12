#include "FileSystem.h"
#include <fstream>
#include <iostream>
FileSystem::FileSystem(std::string fileName) {
	std::fstream file(fileName); 
	fileManager = nullptr;
	fileStructure = nullptr;
	try {
		if (!file) {
			std::ofstream create(fileName); // Creates the file
			create.close();
			fileManager = new FragmentFileManager(fileName);
			fileStructure = new Folder("root");
		}
		else {
			fileManager = new FragmentFileManager(fileName);
			try {
				fileStructure = new Folder(fileManager->loadRoot());
			}
			catch (std::exception ex) {
				std::cerr << ex.what();
				fileStructure = new Folder("root");
			}
		}
		file.close();
	}
	catch (std::exception ex) {
		delete fileManager;
		delete fileStructure;
		throw;
	}

}

FileSystem::~FileSystem() {
	fileManager->saveToDisk(*fileStructure);
	delete fileStructure;
	delete fileManager;
}

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
	if (cPath.Size() == 1 && cPath.Front() == fileStructure->getName())
		return fileStructure;
	Folder* folder = fileStructure;

	//List<std::string> cPath(parsePath(path));
	while (folder != nullptr && !cPath.IsEmpty()) {

		folder = folder->getFolder(cPath.PopFront());
	}

	return folder;
}

void FileSystem::createFolder(std::string path) {
	List<std::string> cPath(parsePath(path));
	if (cPath.Front() == fileStructure->getName()) {
		cPath.PopFront();
	}
	std::string folderName = cPath.PopBack();
	Folder* containingFolder = getFolder(cPath);
	
	if (!containingFolder)
		throw std::exception(std::string("Cannot find requested folder: " + path).c_str());

	containingFolder->addFolder(folderName);

}

void FileSystem::createFile(std::string path) {
	List<std::string> cPath(parsePath(path));
	if (cPath.Front() == fileStructure->getName()) {
		cPath.PopFront();
	}
	std::string fileName = cPath.PopBack();
	Folder* containingFolder = getFolder(cPath);
	if (!containingFolder)
		throw std::exception(std::string("Cannot find requested folder: " + path).c_str());

	containingFolder->addFile(fileName, fileManager->getUid());
}

void FileSystem::printStructure(std::ostream& out) {
	fileStructure->printContents(out);
}

void FileSystem::appendText(std::string filePath, std::string data) {
	List<std::string> cPath(parsePath(filePath));
	if (cPath.Front() == fileStructure->getName()) {
		cPath.PopFront();
	}
	std::string fileName = cPath.PopBack();
	Folder* containingFolder = getFolder(cPath);
	if (!containingFolder)
		throw std::exception(std::string("Cannot find requested folder: " + filePath).c_str());

	File* file = containingFolder->getFile(fileName);
	if (!file) 
		throw std::exception(std::string("Cannot find requested file: " + filePath).c_str());
	
	size_t dataSize = data.length(); // Size in bytes
	const char* dataRaw = data.c_str();
	std::pair<size_t, size_t> info = fileManager->saveData(dataRaw, dataSize, file->getId());
	file->addFragments(info.first, info.second);
}

void FileSystem::exportFile(std::string filePath, std::string realFSPath) {
	List<std::string> cPath(parsePath(filePath));
	if (cPath.Front() == fileStructure->getName()) {
		cPath.PopFront();
	}
	std::string fileName = cPath.PopBack();
	Folder* containingFolder = getFolder(cPath);
	if (!containingFolder)
		throw std::exception(std::string("Cannot find requested folder: " + filePath).c_str());

	File* file = containingFolder->getFile(fileName);
	if (!file)
		throw std::exception(std::string("Cannot find requested file: " + filePath).c_str());


	fileManager->exportFile(file->getId(), file->getStartPos(), file->getFragmentsCount(), realFSPath);
}

void FileSystem::moveFile(std::string filePath, std::string newFilePath) {
	List<std::string> cPath(parsePath(filePath));
	List<std::string> cNewPath(parsePath(newFilePath));
	if (cPath.Front() == fileStructure->getName()) {
		cPath.PopFront();
	}
	if (cNewPath.Front() == fileStructure->getName()) {
		cNewPath.PopFront();
	}
	std::string fileName = cPath.PopBack();
	Folder* containingFolder = getFolder(cPath);
	if (!containingFolder)
		throw std::exception(std::string("Cannot find requested folder: " + filePath).c_str());

	Folder* newFolder = getFolder(cNewPath);
	if (!newFolder)
		throw std::exception(std::string("Cannot find requested folder: " + newFilePath).c_str());

	File* file = containingFolder->getFile(fileName);
	if (!file)
		throw std::exception(std::string("Cannot find requested file: " + filePath).c_str());

	newFolder->addFile(new File(*file));
	containingFolder->deleteFile(fileName);
}

void FileSystem::deleteFile(std::string filePath) {
	List<std::string> cPath(parsePath(filePath));
	if (cPath.Front() == fileStructure->getName()) {
		cPath.PopFront();
	}
	std::string fileName = cPath.PopBack();
	Folder* containingFolder = getFolder(cPath);
	if (!containingFolder)
		throw std::exception(std::string("Cannot find requested folder: " + filePath).c_str());

	File* file = containingFolder->getFile(fileName);
	if (!file)
		throw std::exception(std::string("Cannot find requested file: " + filePath).c_str());

	fileManager->deleteFragments(file->getId(), file->getStartPos(), file->getFragmentsCount());
	containingFolder->deleteFile(fileName);
}

void FileSystem::deleteFolder(std::string path) {
	// TODO
}

void FileSystem::rename(std::string entryPath, std::string newName) { // TODO: Fix for root
	List<std::string> cPath(parsePath(entryPath));
	if (cPath.Front() == fileStructure->getName()) {
		cPath.PopFront();
	}

	std::string entryName = cPath.PopBack();
	Folder* containingFolder = getFolder(cPath);
	if (!containingFolder)
		throw std::exception(std::string("Cannot find requested folder: " + entryPath).c_str());

	Entry* entry = containingFolder->getFolder(entryName);
	if (!entry) {
		entry = containingFolder->getFile(entryName);
	}
	if (!entry) {
		throw std::exception(std::string("Cannot find requested resource: " + entryPath).c_str());

	}
	entry->rename(newName);
}

void FileSystem::copyFile(std::string filePath, std::string newFilePath) {
	List<std::string> cPath(parsePath(filePath));
	List<std::string> cNewPath(parsePath(newFilePath));
	if (cPath.Front() == fileStructure->getName()) {
		cPath.PopFront();
	}
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
	if (cPath.Front() == fileStructure->getName()) {
		cPath.PopFront();
	}
	std::string fileName = cPath.PopBack();
	Folder* containingFolder = getFolder(cPath);
	if (!containingFolder)
		throw std::exception(std::string("Cannot find requested folder: " + filePath).c_str());

	containingFolder->addFile(fileName, fileManager->getUid());
	File* file = containingFolder->getFile(fileName);
	if (!file)
		throw std::exception(std::string("Cannot find requested file: " + filePath).c_str());

	std::pair<size_t, size_t> fileData = fileManager->importFile(realFilePath, file->getId());
	file->addFragments(fileData.first, fileData.second);
}
