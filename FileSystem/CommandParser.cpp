#include "CommandParser.h"

CommandParser& CommandParser::getInstance() {
	static CommandParser instance;
	return instance;
}

CommandParser::CommandParser() : commands(hashFunction) {
	commands.put("mkfile", Command::createFile);
	commands.put("mkdir", Command::createFolder);
	commands.put("renamefile", Command::renameFile);
	commands.put("renamefolder", Command::renameFolder);
	commands.put("movefile", Command::moveFile);
	commands.put("movefolder", Command::moveFolder);
	commands.put("copyfile", Command::copyFile);
	commands.put("copyfolder", Command::copyFolder);
	commands.put("info", Command::getInfo);
	commands.put("importfile", Command::importFile);
	commands.put("importfolder", Command::importFolder);
	commands.put("exportfile", Command::exportFile);
	commands.put("exportfolder", Command::exportFolder);
}

CommandParser::Command CommandParser::getCommand(std::string cmd) const {
	return commands.get(cmd); // Leave exception here
}

size_t CommandParser::hashFunction(std::string s) {
	return (size_t)s[0];
}