// @@author Aaron Chong Jun Hao

#include "stdafx.h"
#include "Command.h"

Command::Command() {}

Command::Command(CommandType newCommand, std::string restOfUserCommand, std::string inputString) {
	cmd = newCommand;
	restOfCommand = restOfUserCommand;
	userInput = inputString;
}

Command::~Command() {}

// ==================================================
//                      METHODS
// ==================================================

CommandType Command::getCommand() {
	return cmd;
}
std::string Command::getRestOfCommand() {
	return restOfCommand;
}

std::string Command::getUserInput() {
	return userInput;
}