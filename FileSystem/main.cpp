#include <iostream>
#include <fstream>
#include "File.h"
#include "Folder.h"
#include <vector>
#include "FragmentFileManager.h"
#include "FileSystem.h"
#include "HashTable.hpp"

int main() {
	{
	try {
		FileSystem fs("file");
		//fs.createFolder("folder1");
		//fs.createFolder("folder2");
		//fs.createFolder("folder1/inner1");
		//fs.createFolder("folder1/inner2");
		//fs.createFile("file1");
		//fs.createFile("folder1/file2");
		//fs.importFile("text.txt", "root/folder1/file1.txt");
		//fs.appendText("file1", "some random text");
		fs.exportFolder("root", "r");

		fs.printStructure(std::cout);
		
	}
	catch (std::exception ex) {
		std::cout << ex.what();
	}
	//fs.createFolder("asd");

	//FragmentFileManager fr ("initial");
	//Folder a = fr.loadRoot();
	//a.getInfo(std::cout);
	/*a.addFolder("asd");
	a.addFolder("fgh");
	a.addFile("asd.file");
	fr.saveToDisk(a);*/
	//std::ifstream g("file", std::ios::binary);
	//Folder b(g);
	//b.getInfo(std::cout);

}

	_CrtDumpMemoryLeaks();
	
	return 0;

}