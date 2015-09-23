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

Task Add::getNewTask() {
	return newTask;
}

int Delete::getTaskToDelete() {
	return taskToDelete;
}

std::vector<FieldType> Modify::getFieldsToModify() {
	return fieldsToModify;
}

Task Modify::getTempTask() {
	return tempTask;
}

std::string Search::getSearchPhrase() {
	return searchPhrase;
}
