// @@author Aaron Chong Jun Hao

#include "stdafx.h"

// ==================================================
//                      COMMAND
// ==================================================

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

Add::Add(Task task) : Command(ADD) {
	newTask = task;	
}

Add::~Add() {}

Task Add::getNewTask() {
	return newTask;
}

// ==================================================
//                       DELETE
// ==================================================

Delete::Delete(int taskID) : Command(DELETE) {
	deleteID = taskID;
}

Delete::~Delete() {}

int Delete::getDeleteID() {
	return deleteID;
}

// ==================================================
//                       MODIFY
// ==================================================

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
//                        VIEW
// ==================================================

View::View(ViewType newView) : Command(VIEW) {
	view = newView;
}

View::~View() {}

ViewType View::getViewType() {
	return view;
}

// ==================================================
//                    DISPLAY_ALL
// ==================================================

DisplayAll::DisplayAll() : Command(DISPLAY_ALL) {}
DisplayAll::~DisplayAll() {}

// ==================================================
//                        LOAD
// ==================================================

Load::Load(std::string newFilePath) : Command(LOAD) {
	filePath = newFilePath;
}

Load::~Load() {}

std::string Load::getFilePath() {
	return filePath;
}

// ==================================================
//                        SAVE
// ==================================================

Save::Save(std::string newFilePath) : Command(SAVE) {
	newFilePath = filePath;
}

Save::~Save() {}

std::string Save::getFilePath() {
	return filePath;
}

// ==================================================
//                        EXIT
// ==================================================

Exit::Exit() : Command(EXIT) {}
Exit::~Exit() {}
