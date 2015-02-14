#pragma once
#include <string>
#include "HashTable.hpp"
class CommandParser {
public:
	enum Command {
		exportToFS,
		importFile,
		importFolder,
		appendText,
		copy,
		movef,
		moved,
		info,
		mkfile,
		mkdir,
		deleteEntry,
		rename,
		structure,
		EXIT,
		HELP,
		INVALID
	};

	static CommandParser& getInstance();
	Command parse(std::string cmd);

	std::string getHelp(const Command& cmd);

private:
	CommandParser();
	void operator=(const CommandParser&);
	CommandParser(const CommandParser&);
	static size_t hashFunction(std::string s);

	HashTable<std::string, Command> map;
	
};