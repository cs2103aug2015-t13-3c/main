// @@author Aaron Chong Jun Hao

#include "Command.h"
#include "stdafx.h"

// ==================================================
//                      COMMAND
// ==================================================

// ========== COMMAND : PUBLIC METHODS ==============

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

std::vector<Task> Command::getTaskStore() {
	return taskStore;
}

std::vector<Task> Command::getCurrentView() {
	return currentView;
}

// virtual function does nothing
void Command::execute() {
}

//virtual function does nothing
void Command::undo() {
	throw ("Action cannot be undone");
}

// ================ COMMAND : PROTECTED METHODS ==================

std::vector<Task> Command::taskStore;
std::vector<Task> Command::currentView;

//sorts in increasing order of dates (except for floating tasks, they are sorted to be at the bottom)
//should use this to sort according to date before display to UI
//since tasks with earliest deadlines/event should be seen first
bool Command::sortDate(std::vector<Task> &taskVector) {

	std::vector<Task>::iterator i;
	std::vector<Task>::iterator j;
	std::vector<Task>::iterator k;
	Task tempTask;
	if (taskVector.size() == 0) {
		return false;
	}
/*	
	for (i = taskVector.begin(); i != taskVector.end(); ++i) {
		for (j = i+1; j != taskVector.end(); ++j) {
			if (j -> getStartTime() < i.getStartTime) {
				swapTaskPosition(i->getID(), j->getID());
			}
		}
	}
*/

	//sorts date after time to ensure date is accurately sorted
	for (i = taskVector.begin(); i != taskVector.end(); ++i) {
		for (j = i+1; j != taskVector.end(); ++j) {
			if (j -> getStartDate() < i -> getStartDate()) {
				std::swap(*i, *j);
			}
		}
	}

	//Bugfix: in-place sorting (Ren Zhi)
	//sorts floating tasks to be at the bottom
	i = taskVector.begin(); //points to start of unsorted part
	k = taskVector.end(); //points to end of unsorted part
	while(i != k) {
		if (i->getType() == FLOATING) {
			tempTask = *i;

			for (j = i+1; j != taskVector.end(); ++j) {
				std::swap(*j, *(j-1)); 
			}
			*(j-1) = tempTask;
			--k;
		} else {
			++i;
		}
	}

	return true;
}

// For now, currentView is set to be the same as taskStore
bool Command::copyView() {
	currentView = taskStore;
	return true;
}

// ==================================================
//                        ADD
// ==================================================


// ============== ADD : PUBLIC METHODS ===============
Add::Add(Task task) : Command(ADD) {
	newTask = task;	
}

Add::~Add() {}

Task Add::getNewTask() {
	return newTask;
}

void Add::execute() {
		addInfo();
		feedback.pushTask(newTask);
		feedback.setAddedMessage();
}

void Add::undo() {
	Delete taskToDelete(newTask.getID());
	taskToDelete.execute();
}

// ============== ADD : PRIVATE METHODS ===============

bool Add::addInfo() {
	std::string dateAndTime_UI = Utilities::taskDateAndTimeToDisplayString(newTask);
	newTask.setDateAndTime_UI(dateAndTime_UI);
	taskStore.push_back(newTask);
	currentView.push_back(newTask);

	sortDate(taskStore);
	copyView();
	return true;
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

void Delete::execute() {
}

void Delete::undo() {
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

void Modify::execute() {
}

void Modify::undo() {
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

void Search::execute() {
}

void Search::undo() {
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

void Markdone::execute() {
}

void Markdone::undo() {
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

void View::execute() {
}

void View::undo() {
}

// ==================================================
//                    DISPLAY_ALL
// ==================================================

DisplayAll::DisplayAll() : Command(DISPLAY_ALL) {}
DisplayAll::~DisplayAll() {}

void DisplayAll::execute() {
}

void DisplayAll::undo() {
}

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

void Load::execute() {
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

void Save::execute() {
}

// ==================================================
//                        EXIT
// ==================================================

Exit::Exit() : Command(EXIT) {}
Exit::~Exit() {}

