#include "FragmentFileManager.h"
#include <io.h>
FragmentFileManager::FragmentFileManager(std::string fileName){
	file = std::fstream(fileName, std::ios::binary | std::ios::in | std::ios::out);
	if (!file)
		throw "Cannot open filesystem file! (" + fileName + ")";

	file.seekg(0, std::ios::end);
	if (file.tellg() > sizeof(FRAGMENT_SIZE) + sizeof(uid) + sizeof(treeStartPos)) { // Assure not entirely new file
		file.seekg(0, std::ios::beg);
		file.read((char*)&FRAGMENT_SIZE, sizeof(FRAGMENT_SIZE));
		file.read((char*)&uid, sizeof(uid));
		file.read((char*)&treeStartPos, sizeof(treeStartPos));
		writingPos = treeStartPos;
	}
	else {
		writingPos = sizeof(FRAGMENT_SIZE) + sizeof(uid) + sizeof(treeStartPos);
		FRAGMENT_SIZE = 100;
		uid = 1;
	}
	if (!file)
		throw "File does not contain valid filesystem data!";

	fragmentFileName = fileName;
}

void FragmentFileManager::saveToDisk(const Folder& rootFolder) {
	file.seekp(0, std::ios::beg);
	file.write((char*)&FRAGMENT_SIZE, sizeof(FRAGMENT_SIZE));
	file.write((char*)&uid, sizeof(uid));
	file.write((char*)&writingPos, sizeof(writingPos)); // tree start pos
	
	file.seekp(writingPos);
	rootFolder.saveToFile(file);
}

std::streampos FragmentFileManager::getEmptyFragment(size_t startPos) {
	file.seekg(startPos); // Assert startPos is pointing at a fragment start position, fails otherwise.
	std::streampos returnValue = writingPos;
	size_t id;
	while (file.good() && file.tellg() < writingPos) {

		file.read((char*)&id, sizeof(id));
		if (id == 0) {
			returnValue = file.tellg() - (std::streampos)sizeof(id);
			break;
		}
	
		file.seekg(file.tellg() + (std::streampos)FRAGMENT_SIZE); // skip the fragment
	}
	file.clear();
	return returnValue;
}

std::pair<size_t, size_t> FragmentFileManager::saveData(const char* data, size_t dataSizeInBytes, size_t fileID, size_t fragmentsExisting) {
	size_t neededFragments = 1 + dataSizeInBytes / (FRAGMENT_SIZE - sizeof(size_t));
	size_t size;
	size_t startPos = writingPos;
	size_t gPos = 32; // first fragment pos;
	for (size_t i = 0; i < neededFragments; i++) {
		if (fragmentsExisting > 0) // If this is not the first fragment of the file, go to the end of the file.
			file.seekp(writingPos, std::ios::beg);
		else {
			gPos = getEmptyFragment(gPos);
			file.seekp(gPos, std::ios::beg);
		}
		file.write((char*)&fileID, sizeof(fileID));
		if (dataSizeInBytes - i * (FRAGMENT_SIZE - sizeof(size_t)) < (FRAGMENT_SIZE - sizeof(size_t))) { // Keep track of how much information we need to write.
			size = dataSizeInBytes - i * (FRAGMENT_SIZE - sizeof(size_t));
		}
		else {
			size = FRAGMENT_SIZE - sizeof(size_t);
		}
		file.write((char*)&size, sizeof(size)); // how full is the fragment
		file.write((char*)(data + i*(FRAGMENT_SIZE - sizeof(size_t))), size);
		
		file.seekp(file.tellp() + (std::streampos)(FRAGMENT_SIZE - size - sizeof(size_t))); // Move the p pointer in valid fragment start position.

		if (gPos >= writingPos)
			writingPos += (std::streampos)(sizeof(fileID) + FRAGMENT_SIZE); // Put writingPos at a valid end-of-file position
	}

	return std::pair<size_t, size_t>(neededFragments, startPos); // To be set to the file in the tree corresponding to the fragments we wrote to the disk.
}

void FragmentFileManager::deleteFragments(size_t fileID, size_t start, size_t fragmentsCount) {
	file.seekp(start, std::ios::beg); // p and g are synchronized;
	//file.seekg(start, std::ios::beg);
	size_t id;
	size_t nullID = 0; // Set each fragment's ID as 0, knowing that our first file is with ID = 1;

	for (size_t i = 0; i < fragmentsCount && file.good(); i++) {
		file.read((char*)&id, sizeof(id));
		if (id == fileID) { // Is it an appropriate fragment to delete?
			file.seekp(file.tellg() - (std::streampos)sizeof(id));
			file.write((char*)&nullID, sizeof(id));
		}
		else {
			--i;
		}
		file.seekg(file.tellg() + (std::streampos)FRAGMENT_SIZE); // Move pointer to valid fragment start position.
	}
	
	file.clear(); // In case we reached eof.
	do { // If we have empty fragments at the end of the file -> mark the spot of the leftmost such fragment.
		file.seekg(writingPos - (std::streampos)(FRAGMENT_SIZE + sizeof(id)));
		file.read((char*)&id, sizeof(id));
		if (id == nullID)
			writingPos -= (std::streampos)(FRAGMENT_SIZE + sizeof(id));
	} while (id == nullID);
	file.clear();
}

size_t FragmentFileManager::getUid() {
	return uid++;
}

Folder FragmentFileManager::loadRoot() {
	// treeStartPos is read in the constructor opening the filesystem file.
	if (treeStartPos < sizeof(FRAGMENT_SIZE) + sizeof(uid) + sizeof(treeStartPos)) {
		throw std::exception("Invalid file structure data!");
	}
	file.seekg(treeStartPos);
	return Folder(file);
}

FragmentFileManager::~FragmentFileManager() {
	// Assure when this is called, the writer pointer of the file is at the end of valid data;
	__int64 size = file.tellg();
	file.close();
	FILE* systemFile = fopen(fragmentFileName.c_str(), "r+"); // open file for update
	
	_chsize_s(_fileno(systemFile), size); // os-specific
}

void FragmentFileManager::exportFile(size_t fileID, size_t firstBlockPos, size_t fragmentsCount, std::string realFSPath) {
	std::ofstream output(realFSPath, std::ios::binary); // Open new file for writing
	file.seekg(firstBlockPos, std::ios::beg);
	size_t id, size;
	char* data = new char[FRAGMENT_SIZE]; // temporary 'buffer' for the data
	for (size_t i = 0; i < fragmentsCount && file.good(); i++) {
		file.read((char*)&id, sizeof(id));
		if (id == fileID) { // Otherwise skip the fragment
			file.read((char*)&size, sizeof(size));
			file.read((char*)data, size);
			output.write((char*)data, size);
			file.seekg(file.tellg() + std::streampos(FRAGMENT_SIZE - size - sizeof(size)));
		}
		else {
			file.seekg(file.tellg() + (std::streampos)(FRAGMENT_SIZE));
			--i;
		}
	}
	file.clear();
	delete data;
}

std::pair<size_t, size_t> FragmentFileManager::copyFragments(size_t fileID, size_t firstBlockPos, size_t fragmentsCount, size_t newID) {
	size_t size, id;
	char* data = new char[FRAGMENT_SIZE];
	std::streampos gPos = sizeof(FRAGMENT_SIZE) + sizeof(uid) + sizeof(treeStartPos);
	std::streampos startPos;
	std::streampos lookup = firstBlockPos; // how far in the file we got

	file.seekg(firstBlockPos, std::ios::beg);
	for (size_t i = 0; i < fragmentsCount && file.good(); i++) {
		file.seekg(lookup);
		file.read((char*)&id, sizeof(id));
		if (id == fileID) { // Valuable fragment
			file.read((char*)&size, sizeof(size));
			file.read((char*)data, size);
			file.seekg(file.tellg() + (std::streampos)(FRAGMENT_SIZE - size - sizeof(size))); // In case the fragment is not entirely full
			lookup = file.tellg();

			gPos = getEmptyFragment(gPos + (std::streampos)(FRAGMENT_SIZE + sizeof(id))); // Is it possible we put the data in an empty leftmost fragment
			if (i == 0) // In case this is the first fragment, we need it's position to tell the file in the tree where it starts.
				startPos = gPos;

			if (gPos >= writingPos)
				writingPos += FRAGMENT_SIZE + sizeof(id);
			// write the data
			file.seekp(gPos);
			file.write((char*)&newID, sizeof(newID));
			file.write((char*)&size, sizeof(size));
			file.write((char*)data, size);
			file.seekp(file.tellp() + (std::streampos)(FRAGMENT_SIZE - size - sizeof(size)));
		}
		else {
			file.seekg(file.tellg() + std::streampos(FRAGMENT_SIZE));
			lookup = file.tellg();
			--i; // we didnt need this fragment
		}
	}

	delete data;
	file.clear();
	return std::pair<size_t, size_t>(fragmentsCount, startPos);
}

std::pair<size_t, size_t> FragmentFileManager::importFile(std::string filePath, size_t fileID) {
	std::ifstream input(filePath, std::ios::binary); // open file for reading
	if (!file.good()) {
		throw std::string("Invalid file: " + filePath);
	}
	input.seekg(0, std::ios::end);
	size_t dataSize = input.tellg(); // get the size in bytes of the file
	input.seekg(0, std::ios::beg);
	size_t neededFragments = 1 + dataSize / (FRAGMENT_SIZE - sizeof(size_t)); // sizeof(size_t) is the id of the fragment which is stored right before the fragment
	std::streampos empty = 32; // the initial value of 'writingPos' in case this is the first file in out system
	size_t startPos;
	size_t currentFragmentSize;
	char* data = new char[FRAGMENT_SIZE];
	for (size_t i = 0; i < neededFragments && input.good(); i++) {
		empty = getEmptyFragment(empty + (std::streampos)(FRAGMENT_SIZE + sizeof(size_t))); // leftmost empty fragment starting from given position
		if (empty >= writingPos)
			writingPos += FRAGMENT_SIZE + sizeof(size_t);

		if (i == 0) // we need to tell the file in the tree where it starts from -> this is the first fragment position
			startPos = empty;

		// keep track of where in the file we are
		if (dataSize - i*(FRAGMENT_SIZE - sizeof(size_t)) < (FRAGMENT_SIZE - sizeof(size_t))) {
			currentFragmentSize = dataSize - i*(FRAGMENT_SIZE - sizeof(size_t));
		}
		else {
			currentFragmentSize = FRAGMENT_SIZE - sizeof(size_t);
		}

		input.read((char*)data, currentFragmentSize);

		file.seekp(empty, std::ios::beg);
		file.write((char*)&fileID, sizeof(fileID));
		file.write((char*)&currentFragmentSize, sizeof(currentFragmentSize));
		file.write((char*)data, currentFragmentSize);
		file.seekp(file.tellp() + (std::streampos)(FRAGMENT_SIZE - currentFragmentSize - sizeof(currentFragmentSize)));

	}
	input.close();
	delete data;
	file.clear();
	return std::pair<size_t, size_t>(neededFragments, startPos);
}

size_t FragmentFileManager::getFragmentSize() const {
	return FRAGMENT_SIZE;
}
