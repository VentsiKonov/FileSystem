#include <iostream>
#include "FileSystem.h"
#include "CommandParser.h"
#include "List.hpp"
#include "BST.hpp"

/// Parse a string with command and parameters as a list of strings
void parseInput(const char command[], List<std::string>& output) {
	size_t length = strlen(command);
	std::string part;
	bool qMark = false;
	for (size_t i = 0; i < length; ++i) {
		if (command[i] == '\'' || command[i] == '"') {
			qMark = !qMark;
			continue;
		}
		if (!qMark && command[i] == ' ') {
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

/// The main working loop of out filesystem
void mainLoop(FileSystem& fs) {
	typedef CommandParser::Command C;
	char input[100]; // We assume excesive names will not be given + we don't support piping commands so 100 should be enough.
	CommandParser::Command cmd;
	CommandParser& cmdParser = CommandParser::getInstance();
	List<std::string> params;
	do {
		std::cout << "\n" << fs.currentFolderName() << "~> ";
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

			case C::info:
				if (params.IsEmpty()) {
					std::cout << cmdParser.getHelp(cmd);
				}
				else {
					try {
						fs.printEntryInfo(params.PopFront(), std::cout);
					}
					catch (std::string ex) {
						std::cout << ex;
					}
				}
				break;
			case C::movef:
				if (params.Size() < 2) {
					std::cout << cmdParser.getHelp(cmd);
				}
				else {
					try {
						fs.moveFile(params.Back(), params.PopFront());
						std::cout << "File moved successfully!";
					}
					catch (std::string ex) {
						std::cout << ex;
					}
				}
				break;
			case C::moved:
				if (params.Size() < 2) {
					std::cout << cmdParser.getHelp(cmd);
				}
				else {
					try {
						fs.moveFolder(params.PopBack(), params.PopFront());
						std::cout << "Folder moved successfully!";
					}
					catch (std::string ex) {
						std::cout << ex;
					}
				}
				break;
			case C::copy:
				if (params.Size() < 2) {
					std::cout << cmdParser.getHelp(cmd);
				}
				else {
					try {
						fs.copyFile(params.PopFront(), params.PopFront());
						std::cout << "File copied successfully!";
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
			mainLoop(fs);
	
		}
		catch (std::string ex) {
			std::cout << ex << "\n";
			activeMode = false;
		}
	}
	
	return 0;
}
