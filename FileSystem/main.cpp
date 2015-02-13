#include <iostream>
#include "FileSystem.h"
#include "CommandParser.h"
#include "List.hpp"
#include "BST.hpp"

void parseInput(const char command[], List<std::string>& output) {
	size_t length = strlen(command);
	std::string part;
	for (size_t i = 0; i < length; ++i) {
		if (command[i] == ' ') {
			output.PushBack(part);
			part = "";
			continue;
		}
		part += command[i];
	}
	output.PushBack(part);
}

void invalidCommandMessage() {
	std::cout << "Invalid command!\n";
	std::cout << "Type 'help' for list of commands\n";
}

void mainLoop(FileSystem& fs) {
	typedef CommandParser::Command C;
	char input[100];
	CommandParser::Command cmd;
	CommandParser& cmdParser = CommandParser::getInstance();
	List<std::string> params;
	do {
		std::cout << "\n~root> ";
		std::cin.sync();
		std::cin.clear();
		std::cin.get(input, 100);
		parseInput(input, params);
		
		cmd = cmdParser.parse(params.PopFront());
		switch (cmd) {
			case C::INVALID:
				invalidCommandMessage();
				break;
			case C::HELP:
				std::cout << cmdParser.getHelp(cmd);
				break;
			case C::mkfile:
				if (params.IsEmpty())
					cmdParser.getHelp(cmd);
				else {
					try {
						fs.createFile(params.PopFront());
						std::cout << "File created!";
					}
					catch (std::string ex) {
						std::cout << ex;
					}
				}
				break;
			case C::deleteEntry:
				if (params.IsEmpty())
					std::cout << cmdParser.getHelp(cmd);
				else {
					if (fs.deleteEntry(params.PopFront()))
						std::cout << "Deleted successfully!";
					else
						std::cout << "Cannot find entry to delete!";
				}
				break;

			case C::mkdir:
				if (params.IsEmpty()) {
					std::cout << cmdParser.getHelp(cmd);
				}
				else {
					try {
						fs.createFolder(params.PopFront());
						std::cout << "Directory created successfully!";
					}
					catch (std::string ex) {
						std::cout << ex;
					}
				}
				break;

			case C::rename:
				if (params.Size() < 2) {
					std::cout << cmdParser.getHelp(cmd);
				}
				else {
					try {
						fs.rename(params.PopBack(), params.PopFront());
						std::cout << "Renamed successfully!";
					}
					catch (std::string e) {
						std::cout << e;
					}
				}
				break;

			case C::appendText:
				if (params.Size() < 2) {
					std::cout << cmdParser.getHelp(cmd);
				}
				else {
					try {
						std::string filePath = params.PopFront();
						std::string data;
						while (!params.IsEmpty())
							data += params.PopFront();
						fs.appendText(filePath, data);
						std::cout << "Text appended successfully!";
					}
					catch (std::string ex) {
						std::cout << ex;
					}
				}
				break;
			case C::importFile:
				if (params.Size() < 2) {
					std::cout << cmdParser.getHelp(cmd);
				}
				else {
					try {
						fs.importFile(params.PopFront(), params.PopFront());
						std::cout << "Imported successfully!";
					}
					catch (std::string ex) {
						std::cout << ex;
					}
				}
				break;

			case C::exportToFS:
				if (params.Size() < 2) {
					std::cout << cmdParser.getHelp(cmd);
				}
				else {
					if (fs.exportEntry(params.PopFront(), params.PopFront())) {
						std::cout << "Export successfull!";
					}
					else {
						std::cout << "Export unsuccessfull! Check parameters!";
					}
				}
				break;
			case C::structure:
				fs.printStructure(std::cout);
				break;

		}
		params.Clear();

	} while (cmd != C::EXIT);
}

int main() {
	std::cout << "Enter the relative or absolute path to your filesystem file:\n";
	char input[100];
	
	bool activeMode = false;
	while (!activeMode) {
		try {
			std::cin.getline(input, 100);
			FileSystem fs(input);
			activeMode = true;
			//fs.printStructure(std::cout);
			mainLoop(fs);
	
		}
		catch (std::exception ex) {
			std::cout << ex.what() << "\n";
			activeMode = false;
		}
	}
	
	return 0;
}