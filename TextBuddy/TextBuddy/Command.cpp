// @@author Aaron Chong Jun Hao

#include "stdafx.h"

// ==================================================
//                 COMMAND (OUTDATED)
// ==================================================

Command::Command() {}

Command::Command(CommandType newCommand, std::string restOfUserCommand, std::string inputString) {
	cmd = newCommand;
	restOfCommand = restOfUserCommand;
	userInput = inputString;
}

Command::~Command() {}

CommandType Command::getCommand() {
	return cmd;
}

std::string Command::getRestOfCommand() {
	return restOfCommand;
}

std::string Command::getUserInput() {
	return userInput;
}

// ==================================================
//                 COMMAND (ABSTRACT)
// ==================================================

// Used for CLEAR_ALL, DISPLAY_ALL, SORT_ALL
void Command::setCmdType(CommandType cmdType) {
	assert(cmdType==CLEAR_ALL || cmdType==DISPLAY_ALL || cmdType==SORT_ALL);
	cmd = cmdType;
	return;
}

void Command::setNewTask(Task task) {}
void Command::setTaskToDelete(int index) {}

std::vector<FieldType> Command::getFieldsToModify() {
	std::vector<FieldType> dummy;
	return dummy;
}

void Command::setTempTask(Task task) {}

void Command::setSearchPhrase(std::string phr) {}

// ==================================================
//                        ADD
// ==================================================

Add::Add() : Command(ADD) {}
Add::~Add() {}

void Add::setNewTask(Task aTask) {
	newTask = aTask;
	return;
}

Task Add::getNewTask() {
	return newTask;
}

// ==================================================
//                       DELETE
// ==================================================

Delete::Delete() : Command(DELETE) {}
Delete::~Delete() {}

void Delete::setTaskToDelete(int index) {
	taskToDelete = index;
	return;
}

int Delete::getTaskToDelete() {
	return taskToDelete;
}

// ==================================================
//                       MODIFY
// ==================================================

Modify::Modify() : Command(MODIFY) {}
Modify::~Modify() {}

std::vector<FieldType> Modify::getFieldsToModify() {
	return fieldsToModify;
}

Task Modify::getTempTask() {
	return tempTask;
}

void Modify::setTempTask(Task task) {
	tempTask = task;
	return;
}

// ==================================================
//                       SEARCH
// ==================================================

Search::Search() : Command(SEARCH) {}
Search::~Search() {}

std::string Search::getSearchPhrase() {
	return searchPhrase;
}

void Search::setSearchPhrase(std::string phr) {
	searchPhrase = phr;
	return;
}
