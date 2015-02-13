#include "CommandParser.h"

size_t CommandParser::hashFunction(std::string s) {
	return (size_t)s[0] % 17;
}

CommandParser::CommandParser() : map(hashFunction) {
	map.put("mkfile", Command::mkfile);
	map.put("mkdir", Command::mkdir);
	map.put("del", Command::deleteEntry);
	map.put("move", Command::move);
	map.put("copy", Command::copy);
	map.put("info", Command::info);
	map.put("app", Command::appendText);
	map.put("rnm", Command::rename);
	map.put("exp", Command::exportToFS);
	map.put("impf", Command::importFile);
	map.put("impd", Command::importFolder);
	map.put("dir", Command::structure);
	map.put("exit", Command::EXIT);
	map.put("help", Command::HELP);
}

CommandParser& CommandParser::getInstance() {
	static CommandParser instance;
	return instance;
}

CommandParser::Command CommandParser::parse(std::string cmd) {
	try {
		return map.get(cmd);
	}
	catch (const char*) {
		return Command::INVALID;
	}
}

std::string CommandParser::getHelp(const Command& cmd) {
	std::string help;
	switch (cmd) {
		case Command::info:
			help = "info 'path-to-entry'\nGives relevant information about the entry.\n";
			break;
		case Command::appendText:
			help = "app 'path-to-file' 'text'\nAppends 'text' to the end of given file.\n";
			break;
		case Command::EXIT:
			help = "exit \nExits the filesystem session saving all data.\n";
			break;
		case Command::deleteEntry:
			help = "del 'path-to-entry'\nDeletes the entry specified in 'path-to-entry'.\n";
			break;
		case Command::copy:
			help = "copy 'path-to-entry' 'path-to-folder'\nCopies the entry given in the folder. Recursively for directories.\n";
			break;
		case Command::exportToFS:
			help = "exp 'path-to-entry' 'path-in-real-filesystem'\nExports file or directory to the real filesystem. Recursively for directories.\n";
			break;
		case Command::importFile:
			help = "impf 'path-to-real-file' 'path/name'\nImports file from real filesystem to given directory path with the given name.\n";
			break;
		case Command::HELP:
			for (int i = (Command)0; i < (Command)13; ++i) { // Specifically tuned!
				help += getHelp((Command)i) + "\n";
			}
			break;
		default:
			break;
	}
	return help;
}