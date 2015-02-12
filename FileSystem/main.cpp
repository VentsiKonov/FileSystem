#include <iostream>
#include <fstream>
#include "File.h"
#include "Folder.h"
#include <vector>
#include "FragmentFileManager.h"
#include "FileSystem.h"
#include "HashTable.hpp"
size_t hash(std::string key) {
	return (size_t)key[0];
}
int main() {

	size_t (*hashFN)(std::string);
	hashFN = hash;

	

	/*
	File a("asd", 10);
	a.saveToFile(f);
	f.close();
	std::ifstream g("file", std::ios::binary);

	File b(g);
	b.getInfo(std::cout);
	*/
{
	//_crtBreakAlloc = 143;

	HashTable<std::string, int> table(hash);
	table.put("asd", 0);
	std::cout << table.get("asd");
	try {
		//FileSystem fs("file");

		//fs.createFolder("root/additional");
		//fs.importFile("file.rar", "root/f1/file");
		//fs.importFile("text.txt", "root/test1");
		//fs.importFile("text.txt", "root/test2");
		//fs.deleteFile("root/f1/file");
		//fs.importFile("text.txt", "root/additional/text");
		//fs.exportFile("root/test1", "test1.txt");
		
		//fs.createFolder("2");
		//fs.createFolder("root/folder");
		//fs.createFile("root/2/file2");
		//fs.appendText("root/2/file2", "some file2 data with unknown size");
		//fs.createFile("root/asd");
		//fs.appendText("root/asd", "some text to be deleted");
		//fs.appendText("root/2/file2", "some data for file 2 to be clustered");
		//fs.appendText("root/asd", "some more awesome text");
		//fs.deleteFile("root/asd");
		//fs.exportFile("root/2/file2", "file2pre.txt");
		
		//fs.copyFile("root/2/file2", "root/file1");
		//fs.exportFile("root/2/file2", "file2post.txt");
		//fs.exportFile("root/file1", "file1.txt");

		//fs.exportFile("root/file1", "asd2.txt");
		//fs.exportFile("root/2/file2", "asd.txt");
		//fs.moveFile("root/2/file2", "root");
		//fs.rename("root/file2", "gyz");
		//fs.importFile("test.txt", "root/testfile");
		//fs.exportFile("root/testfile", "testOut.txt");

		//fs.printStructure(std::cout);
		
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