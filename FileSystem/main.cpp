#include <iostream>
#include "FileSystem.h"
#include "CommandParser.h"
#include "List.hpp"

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

void printHelp() {
	std::cout << "help\n";
}

void parametersNeeded() {
	std::cout << "More parameters needed!";
}

void invalidCommandMessage() {
	std::cout << "Invalid command!\n";
	std::cout << "Type 'help' for list of commands\n";
}

int main() {
	typedef CommandParser::Command C;
	std::cout << "Enter the relative or absolute path to your filesystem file:\n";
	char input[100];
	
	std::cin.getline(input, 100);

	FileSystem fs(input);
	
	fs.printStructure(std::cout);

	CommandParser::Command cmd;
	List<std::string> params;
	do {
		std::cout << "\nroot> ";
		std::cin.sync();
		std::cin.clear();
		std::cin.get(input, 100);
		parseInput(input, params);
		cmd = CommandParser::getInstance().parse(params.PopFront());
		switch (cmd) {
			case C::INVALID:
				invalidCommandMessage();
				break;
			case C::HELP:
				printHelp();
				break;
			case C::mkfile:
				if (params.IsEmpty())
					parametersNeeded();
				else {
					fs.createFile(params.PopFront());
					std::cout << "File created!";
				}
				break;
			case C::deleteEntry:
				if (params.IsEmpty())
					parametersNeeded();
				else {
					if (fs.deleteEntry(params.PopFront()))
						std::cout << "Deleted successfully!";
					else
						std::cout << "Cannot find entry to delete!";
				}
				break;

			case C::mkdir:
				if (params.IsEmpty()) {
					parametersNeeded();
				}
				else {
					fs.createFolder(params.PopFront());
					std::cout << "Directory created successfully!";
				}
				break;

			case C::rename:
				if (params.Size() < 2) {
					parametersNeeded();
				}
				else {
					try {
						fs.rename(params.PopFront(), params.PopFront());
						std::cout << "Renamed successfully!";
					}
					catch (std::string e) {
						std::cout << e;
					}
				}
				break;

			case C::appendText:
				if (params.Size() < 2) {
					parametersNeeded();
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
			case C::structure:
				fs.printStructure(std::cout);
				break;

		}
		params.Clear();
		
	} while (cmd != C::EXIT);

	return 0;
}