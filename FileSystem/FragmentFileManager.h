#pragma once
#include <string>
#include <fstream>
#include "Folder.h"
/// Manages the file containing all file data and the file/folder structure of the filesystem.
/// A fragments represents a piece of file's data. 
/// Each fragment contains the ID of the file to whom it belongs, a size_t variable for the size of valuable data in the fragment and the data itself.7
/// The fragments are kept in sorted order in the filesystem file so that data is not shuffled.
class FragmentFileManager {
public:
	FragmentFileManager(std::string fileName);
	~FragmentFileManager();

	/// Loads the file structure found in the file
	Folder loadRoot();
	/// Marks all fragments of given file empty and susceptible to overwrite.
	void deleteFragments(size_t id, size_t startPos, size_t fragmentsCount);
	/// Copies all fragments of one file in order to make a copy of it with different ID.
	std::pair<size_t, size_t> copyFragments(size_t id, size_t startPos, size_t fragmentsCount, size_t newID);
	/// Saves the current file structure to the disk. 
	void saveToDisk(const Folder& rootFolder);
	/// Saves data for specific file on an empty fragment or in the end of the file. Appending data always happens at the end of the file
	std::pair<size_t, size_t> saveData(const char* data, size_t dataSizeInBytes, size_t fileID, size_t fragmentsExisting); // <fragmentsCount, firstFragmentPosition>
	/// Opens a file from the real file system and makes a copy of it in ours finding the leftmost available fragment.
	std::pair<size_t, size_t> importFile(std::string filePath, size_t fileID); // <fragmentsCount, firstFragmentPosition>
	/// Makes a copy of the file in the real filesystem. Starting from given position it skips the fragments of other files and appending to the real file only the ones for that file.
	void exportFile(size_t fileID, size_t firstBlockPos, size_t fragmentsCount, std::string realFSPath);
	/// Gives a unique ID for a new file.
	size_t getUid();
	/// Gives the current size of the fragments so it can be used in printing file's size information.
	size_t getFragmentSize() const;

private:
	/// We will not have more than one filesystem using the same fragment file, so copy-ctor and operator= are disabled.
	FragmentFileManager(const FragmentFileManager&);
	void operator=(const FragmentFileManager&);

	/// Finds the leftmost empty fragment or creates a new one at the end of the file.
	std::streampos getEmptyFragment(size_t startPos);

	std::fstream file;
	std::string fragmentFileName;
	size_t FRAGMENT_SIZE; // const!
	std::streampos treeStartPos;
	std::streampos writingPos;
	size_t uid;
};