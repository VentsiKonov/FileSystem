#pragma once
#include <string>
#include "HashTable.hpp"
class CommandParser {
public:
	enum Command {
		createFile,
		createFolder,
		renameFile,
		renameFolder,
		copyFile,
		copyFolder,
		deleteFile,
		deleteFolder,
		moveFile,
		moveFolder,
		importFile,
		importFolder,
		exportFile,
		exportFolder,
		getInfo,
		append
	};
	Command getCommand(std::string cmd) const;
	static CommandParser& getInstance();
private:
	static size_t hashFunction(std::string);
	CommandParser();
	void operator=(const CommandParser&);
	CommandParser(const CommandParser&);

	HashTable<std::string, Command> commands;
};