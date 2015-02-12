#include "Folder.h"

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
		folders = List<Folder*>(other.folders);
		files = List<File*>(other.files);
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
	while (!files.IsEmpty()) {
		delete files.PopFront();
	}
	while (!folders.IsEmpty()) {
		delete folders.PopFront();
	}
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

List<size_t> Folder::getFilesIDs() const {
	List<size_t> ids;
	for (List<File*>::Iterator i = files.Begin(); i != files.End(); ++i) {
		ids.PushBack((*i)->getId());
	}
	return ids;
}