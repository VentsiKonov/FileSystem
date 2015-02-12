#pragma once
#include <string>
#include <fstream>
#include "Folder.h"
class FragmentFileManager {
public:
	FragmentFileManager(std::string fileName);
	~FragmentFileManager();

	Folder loadRoot();
	void deleteFragments(size_t id, size_t startPos, size_t fragmentsCount);
	std::pair<size_t, size_t> copyFragments(size_t id, size_t startPos, size_t fragmentsCount, size_t newID);
	void saveToDisk(const Folder& rootFolder);
	std::pair<size_t, size_t> saveData(const char* data, size_t dataSizeInBytes, size_t fileID, size_t fragmentsExisting); // <fragmentsCount, firstFragmentPosition>
	std::pair<size_t, size_t> importFile(std::string filePath, size_t fileID);
	void exportFile(size_t fileID, size_t firstBlockPos, size_t fragmentsCount, std::string realFSPath);
	size_t getUid();

private:
	FragmentFileManager(const FragmentFileManager&);
	void operator=(const FragmentFileManager&);

	std::streampos getEmptyFragment(size_t startPos);

	std::fstream file;
	std::string fragmentFileName;
	size_t FRAGMENT_SIZE; // const!
	std::streampos treeStartPos;
	std::streampos writingPos;
	size_t uid;
};