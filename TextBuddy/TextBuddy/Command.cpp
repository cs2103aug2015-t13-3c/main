// @@author Aaron Chong Jun Hao

#include "stdafx.h"

// ==================================================
//                      COMMAND
// ==================================================

// Command::Command() {}

Command::Command(CommandType newCmd, std::string rawInput) {
	cmd = newCmd;
	userInput = rawInput;
}

Command::~Command() {}

CommandType Command::getCommand() {
	return cmd;
}

std::string Command::getUserInput() {
	return userInput;
}

// ==================================================
//                        ADD
// ==================================================

// Add::Add() : Command(ADD) {}

Add::Add(Task task, std::string userInput) : Command(ADD,userInput) {
	newTask = task;	
}

Add::~Add() {}

Task Add::getNewTask() {
	return newTask;
}

void Add::setNewTask(Task task) {
	newTask = task;
	return;
}

// ==================================================
//                       

// ==================================================

// Delete::Delete() : Command(_DELETE) {}

Delete::Delete(int taskID) : Command(_DELETE) {
	deleteID = taskID;
}

Delete::~Delete() {}

int Delete::getDeleteID() {
	return deleteID;
}

// ==================================================
//                       MODIFY
// ==================================================

// Modify::Modify() : Command(MODIFY) {}

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

// ==================================================
//                       SEARCH
// ==================================================

// Search::Search() : Command(SEARCH) {}

Search::Search(std::string phraseString) : Command(SEARCH) {
	searchPhrase = phraseString;
}

Search::~Search() {}

std::string Search::getSearchPhrase() {
	return searchPhrase;
}

// ==================================================
//                      MARKDONE
// ==================================================

Markdone::Markdone(int taskID) : Command(MARKDONE) {
	doneID = taskID;
}

Markdone::~Markdone() {}

int Markdone::getDoneID() {
	return doneID;
}

// ==================================================
//                        UNDO
// ==================================================

Undo::Undo() : Command(UNDO) {}

Undo::~Undo() {}

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
//                        SAVE
// ==================================================

Save::Save(std::string filePath) : Command(SAVE) {
	newFilePath = filePath;
}

Save::~Save() {}

std::string Save::getFilePath() {return newFilePath;}

// ==================================================
//                        EXIT
// ==================================================

Exit::Exit() : Command(EXIT) {}
Exit::~Exit() {}
