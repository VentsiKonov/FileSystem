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
		Entry::copyFrom(other); // TODO: Make clean copy - not like this;
		clear();
		for (List<File*>::Iterator i = other.files.Begin(); i != other.files.End(); ++i) {
			files.PushBack(new File(**i));
		}
		for (List<Folder*>::Iterator i = other.folders.Begin(); i != other.folders.End(); ++i) {
			folders.PushBack(new Folder(**i));
		}
	}

}

void Folder::getInfo(std::ostream& out) {
	out << "<" << name << ">\n";
	out << files.Size() << " files. " << folders.Size() << " folders.";
}

Folder::Folder(std::istream& file) : Entry(file) {
	size_t foldersCount, filesCount;
	file.read((char*)&foldersCount, sizeof(foldersCount));
	file.read((char*)&filesCount, sizeof(filesCount));

	for (size_t i = 0; i < foldersCount; i++) {
		folders.PushBack(new Folder(file));
	}
	for (size_t i = 0; i < filesCount; i++) {
		files.PushBack(new File(file));
	}
}

Folder::~Folder() {
	clear();
}

void Folder::saveToFile(std::ostream& file) const{
	Entry::saveToFile(file);
	size_t foldersCount = folders.Size();
	size_t filesCount = files.Size();
	file.write((char*)&foldersCount, sizeof(foldersCount));
	file.write((char*)&filesCount, sizeof(filesCount));

	for (List<Folder*>::Iterator i = folders.Begin(); i != folders.End(); ++i) {
		(*i)->saveToFile(file);
	}
	for (List<File*>::Iterator j = files.Begin(); j != files.End(); ++j) {
		(*j)->saveToFile(file);
	}
}

void Folder::addFile(std::string name, size_t fileID) {
	files.PushBack(new File(name, fileID));
}

void Folder::addFile(File* file) {
	files.PushBack(file);
}

void Folder::addFolder(std::string name) {
	folders.PushBack(new Folder(name));
}

Folder* Folder::getFolder(std::string name) {
	for (List<Folder*>::Iterator i = folders.Begin(); i != folders.End(); ++i) {
		if ((*i)->name == name) {
			return *i;
		}
	}
	return nullptr;
}

File* Folder::getFile(std::string name) {
	for (List<File*>::Iterator i = files.Begin(); i != files.End(); ++i) {
		if ((*i)->getName() == name) {
			return *i;
		}
	}
	return nullptr;
}

void Folder::printContents(std::ostream& out, size_t offset) const {
	for (size_t i = 0; i < offset; i++) {
		out << "    ";
	}
	out << "<" << name << ">\n";
	for (List<File*>::Iterator j = files.Begin(); j != files.End(); ++j) {
		for (size_t i = 0; i < offset + 1; i++) {
			out << "    ";
		}
		out << (*j)->getName() << "\n";
	}
	for (List<Folder*>::Iterator k = folders.Begin(); k != folders.End(); ++k) {
		(*k)->printContents(out, offset + 1);
	}
}

void Folder::deleteFile(std::string name) { // Optimise when making hashtable of files
	size_t c = 0;
	for (List<File*>::Iterator i = files.Begin(); i != files.End(); ++i) {
		if ((*i)->getName() == name) {
			break;
		}
		++c;
	}
	delete files.PopAt(c);
}

void Folder::deleteFolder(std::string name) {
	size_t c = 0;
	for (List<Folder*>::Iterator i = folders.Begin(); i != folders.End(); ++i) {
		if ((*i)->getName() == name) {
			break;
		}
		++c;
	}
	delete folders.PopAt(c);
}

void Folder::clear() {
	while (!files.IsEmpty()) {
		delete files.PopFront();
	}
	while (!folders.IsEmpty()) {
		delete folders.PopFront();
	}
}

void Folder::deleteAllFiles(std::function<void(size_t, size_t, size_t)> delFunc) {
	for (List<File*>::Iterator i = files.Begin(); i != files.End(); ++i) {
		delFunc((*i)->getId(), (*i)->getStartPos(), (*i)->getFragmentsCount());
	}
	for (List<Folder*>::Iterator i = folders.Begin(); i != folders.End(); ++i) {
		(*i)->deleteAllFiles(delFunc);
	}
}

void Folder::exportToFS(std::string realFSPath, std::function<void(size_t, size_t, size_t, std::string)> fileExportFN) {
	//std::cout << "mkdir " << realFSPath << "\n";
	system(std::string("mkdir " + realFSPath).c_str()); // OS-specific

	for (List<File*>::Iterator i = files.Begin(); i != files.End(); ++i) {
		fileExportFN((*i)->getId(), (*i)->getStartPos(), (*i)->getFragmentsCount(), realFSPath + "\\" + (*i)->getName());
	}
	
	for (List<Folder*>::Iterator i = folders.Begin(); i != folders.End(); ++i) {
		(*i)->exportToFS(realFSPath + "\\" + (*i)->name, fileExportFN);
	}
}