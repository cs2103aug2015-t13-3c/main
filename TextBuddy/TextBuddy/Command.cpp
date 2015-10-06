// @@author Aaron Chong Jun Hao

#include "stdafx.h"

// ==================================================
//                      COMMAND
// ==================================================

// Command::Command() {}

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
//                        ADD
// ==================================================

Add::Add() : Command(ADD) {}
Add::Add(std::string userInput) : Command(ADD,"",userInput) {}
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

int Delete::getDeleteID() {
	return deleteID;
}

void Delete::setDeleteID(int index) {
	deleteID = index;
	return;
}

// ==================================================
//                       MODIFY
// ==================================================

Modify::Modify() : Command(MODIFY) {}

Modify::Modify(int taskID, std::vector<FieldType> fields, Task task) : Command(MODIFY) {
	modifyID = taskID;
	fieldsToModify = fields;
	tempTask = task;
}

Modify::~Modify() {}

int Modify::getModifyID() {
	return modifyID;
}

std::vector<FieldType> Modify::getFieldsToModify() {
	return fieldsToModify;
}

Task Modify::getTempTask() {
	return tempTask;
}

// Obsolete methods, only used for testing

void Modify::setModifyID(int index) {
	modifyID = index;
	return;
}

void Modify::setFieldsToModify(std::vector<FieldType> fields) {
	fieldsToModify = fields;
	return;
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

// ==================================================
//                     CLEAR_ALL
// ==================================================

ClearAll::ClearAll() : Command(CLEAR_ALL) {}
ClearAll::~ClearAll() {}

// ==================================================
//                    DISPLAY_ALL
// ==================================================

DisplayAll::DisplayAll() : Command(DISPLAY_ALL) {}
DisplayAll::~DisplayAll() {}

// ==================================================
//                      SORT_ALL
// ==================================================

SortAll::SortAll() : Command(SORT_ALL) {}
SortAll::~SortAll() {}

// ==================================================
//                        EXIT
// ==================================================

Exit::Exit() : Command(EXIT) {}
Exit::~Exit() {}
