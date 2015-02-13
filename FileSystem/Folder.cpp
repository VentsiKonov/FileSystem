#include "Folder.h"
#include <iostream>

Folder::Folder(const Folder& other) : Entry(other){
	copyFrom(other);
}

Folder& Folder::operator=(const Folder& other) {
	copyFrom(other);
	return *this;
}

void Folder::copyFrom(const Folder& other) {
	if (this != &other) {
		Entry::copyFrom(other); 
		clear();
		for (BST<std::string, File*>::Iterator i = other.files.begin(); i != other.files.end(); ++i) {
			files.insert((*i)->getName(), new File(**i));
		}
		for (BST<std::string, Folder*>::Iterator i = other.folders.begin(); i != other.folders.end(); ++i) {
			folders.insert((*i)->getName(), new Folder(**i));
		}
	}

}

void Folder::getInfo(std::ostream& out) {
	out << "<" << name << ">\n";
	out << files.size() << " files. " << folders.size() << " folders.";
}

Folder::Folder(std::istream& file) : Entry(file) {
	size_t foldersCount, filesCount;
	file.read((char*)&foldersCount, sizeof(foldersCount));
	file.read((char*)&filesCount, sizeof(filesCount));
	Folder* tempD;
	for (size_t i = 0; i < foldersCount; i++) {
		tempD = new Folder(file);
		folders.insert(tempD->getName(), tempD);
	}
	File* tempF;
	for (size_t i = 0; i < filesCount; i++) {
		tempF = new File(file);
		files.insert(tempF->getName(), tempF);
	}
}

Folder::~Folder() {
	clear();
}

void Folder::saveToFile(std::ostream& file) const{
	Entry::saveToFile(file);
	size_t foldersCount = folders.size();
	size_t filesCount = files.size();
	file.write((char*)&foldersCount, sizeof(foldersCount));
	file.write((char*)&filesCount, sizeof(filesCount));

	for (BST<std::string, Folder*>::Iterator i = folders.begin(); i != folders.end(); ++i) {
		(*i)->saveToFile(file);
	}
	for (BST<std::string, File*>::Iterator j = files.begin(); j != files.end(); ++j) {
		(*j)->saveToFile(file);
	}
}

void Folder::addFile(std::string name, size_t fileID) {
	files.insert(name, new File(name, fileID));
}

void Folder::addFile(File* file) {
	files.insert(file->getName(), file);
}

void Folder::addFolder(std::string name) {
	folders.insert(name, new Folder(name));
}

Folder* Folder::getFolder(std::string name) {
	try {
		return folders.get(name);
	}
	catch (const char*) {
		return nullptr;
	}
}

File* Folder::getFile(std::string name) {
	try {
		return files.get(name);
	}
	catch (const char*) {
		return nullptr;
	}
}

void Folder::printContents(std::ostream& out, size_t offset) const {
	for (size_t i = 0; i < offset; i++) {
		out << "    ";
	}
	out << "<" << name << ">\n";
	for (BST<std::string, File*>::Iterator j = files.begin(); j != files.end(); ++j) {
		for (size_t i = 0; i < offset + 1; i++) {
			out << "    ";
		}
		out << (*j)->getName() << "\n";
	}
	for (BST<std::string, Folder*>::Iterator k = folders.begin(); k != folders.end(); ++k) {
		(*k)->printContents(out, offset + 1);
	}
}

void Folder::deleteFile(std::string name) { // Optimise when making hashtable of files
	delete files.get(name);
	files.remove(name);
}

void Folder::deleteFolder(std::string name) {
	delete folders.get(name);
	folders.remove(name);
}

void Folder::clear() {
	for (BST<std::string, File*>::Iterator i = files.begin(); i != files.end(); ++i) {
		delete (*i);
	}
	for (BST<std::string, Folder*>::Iterator i = folders.begin(); i != folders.end(); ++i) {
		delete (*i);
	}
}

void Folder::deleteAllFiles(std::function<void(size_t, size_t, size_t)> delFunc) {
	for (BST<std::string, File*>::Iterator i = files.begin(); i != files.end(); ++i) {
		delFunc((*i)->getId(), (*i)->getStartPos(), (*i)->getFragmentsCount());
	}
	for (BST<std::string, Folder*>::Iterator i = folders.begin(); i != folders.end(); ++i) {
		(*i)->deleteAllFiles(delFunc);
	}
}

void Folder::exportToFS(std::string realFSPath, std::function<void(size_t, size_t, size_t, std::string)> fileExportFN) {
	//std::cout << "mkdir " << realFSPath << "\n";
	system(std::string("mkdir " + realFSPath).c_str()); // OS-specific

	for (BST<std::string, File*>::Iterator i = files.begin(); i != files.end(); ++i) {
		fileExportFN((*i)->getId(), (*i)->getStartPos(), (*i)->getFragmentsCount(), realFSPath + "\\" + (*i)->getName());
	}
	
	for (BST<std::string, Folder*>::Iterator i = folders.begin(); i != folders.end(); ++i) {
		(*i)->exportToFS(realFSPath + "\\" + (*i)->name, fileExportFN);
	}
}