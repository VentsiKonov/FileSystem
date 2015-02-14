#include "CommandParser.h"

/// Hashing function for the HashTable string->command
size_t CommandParser::hashFunction(std::string s) {
	return (size_t)s[0] % 17; // tuned a bit for better distrinution
}

/// Filles the list of associations string->command
CommandParser::CommandParser() : map(hashFunction) {
	map.put("mkfile", Command::mkfile);
	map.put("mkdir", Command::mkdir);
	map.put("del", Command::deleteEntry);
	map.put("movef", Command::movef);
	map.put("moved", Command::moved);
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

/// Singleton class instance
CommandParser& CommandParser::getInstance() {
	static CommandParser instance;
	return instance;
}

/// Tries to give the command associated with given string, returns INVALID otherwise
CommandParser::Command CommandParser::parse(std::string cmd) {
	try {
		return map.get(cmd);
	}
	catch (const char*) {
		return Command::INVALID;
	}
}

/// Gives information on how to use a function given a command from the Commands enum
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
			help = "copy 'path-to-file' 'path-to-folder/name'\nCopies the entry given in the folder under 'name'.\n";
			break;
		case Command::exportToFS:
			help = "exp 'path-to-entry' 'path-in-real-filesystem'\nExports file or directory to the real filesystem. Recursively for directories.\n";
			break;
		case Command::importFile:
			help = "impf 'path-to-real-file' 'path/name'\nImports file from real filesystem to given directory path with the given name.\n";
			break;
		case Command::mkdir:
			help = "mkdir 'path-to-dir/name'\nCreates an empty directory with name 'name' in the 'path-to-dir' directory.\n";
			break;
		case Command::mkfile:
			help = "mkfile 'path-to-dir/name'\nCreates an empty file with name 'name' in the 'path-to-dir' directory.\n";
			break;
		case Command::moved:
			help = "moved 'new-path/name' 'path-to-directory'\nMoves the 'path-to-directory' directory and all files in the 'new-path' with name 'name'.\n";
			break;
		case Command::movef:
			help = "movef 'new-path' 'path-to-file'\nMoves the file to an existing directory.\n";
			break;
		case Command::rename:
			help = "rnm 'path-to-entry' 'new-name'\nRenames the entry to the given name. \n";
			break;
		case Command::structure:
			help = "dir \nPrints the file structure relative to the current directory.\n";
			break;
		case Command::HELP:
			for (int i = (Command)0; i < (Command)14; ++i) { // Specifically tuned!
				help += getHelp((Command)i) + "\n";
			}
			break;
		default:
			break;
	}
	return help;
}