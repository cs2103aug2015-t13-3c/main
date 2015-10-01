// @@author Aaron Chong Jun Hao

#include "stdafx.h"

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

void Add::setNewTask(Task aTask) {
	newTask = aTask;
	return;
}

Task Add::getNewTask() {
	return newTask;
}

void Delete::setTaskToDelete(int index) {
	taskToDelete = index;
	return;
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

void Search::setSearchPhrase(std::string phr) {
	searchPhrase = phr;
	return;
}
