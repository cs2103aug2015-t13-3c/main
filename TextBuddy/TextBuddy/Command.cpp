// @@author A0110376N (Aaron Chong Jun Hao)
// Modified to Command Pattern by Ng Ren Zhi
// Private methods originally by @@author A0096720A (Chin Kiat Boon)

#include "stdafx.h"
#include "Command.h"

//==================================================
//                      COMMAND
//==================================================

//========== COMMAND : PUBLIC METHODS ==============

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

std::vector<Task>* Command::getCurrentViewPtr() {
	return &currentView;
}

int Command::getSize() {
	return taskStore.size();
}

void Command::clearTaskStore() {
	IO* io = IO::getInstance();
	taskStore.clear();
	currentView.clear();
	remove(io->getFilePath().c_str());
	return;
}

// Virtual function does nothing
void Command::execute() {
}

// Virtual function does nothing
void Command::undo() {
	throw ("Action cannot be undone");
}

//================ COMMAND : PROTECTED METHODS ==================

std::vector<Task> Command::taskStore;
std::vector<Task> Command::currentView;
const std::string Command::ERROR_INDEX_OUT_OF_BOUNDS = "Invalid index";

// Sorts in increasing order of dates (except for floating tasks, which are at the bottom)
// Use this before returning to UI for display
bool Command::sortDate(std::vector<Task> &taskVector) {

	std::vector<Task>::iterator i;
	std::vector<Task>::iterator j;
	std::vector<Task>::iterator k;
	Task tempTask;
	if(taskVector.size() == 0) {
		return false;
	}
	/*	
	for (i = taskVector.begin(); i != taskVector.end(); ++i) {
	for (j = i+1; j != taskVector.end(); ++j) {
	if(j -> getStartTime() < i.getStartTime) {
	swapTaskPosition(i->getID(), j->getID());
	}
	}
	}
	*/

	// Sorts date after time to ensure date is accurately sorted
	for (i = taskVector.begin(); i != taskVector.end(); ++i) {
		for (j = i+1; j != taskVector.end(); ++j) {
			if(j -> getStartDate() < i -> getStartDate()) {
				std::swap(*i, *j);
			}
		}
	}

	// Bugfix: in-place sorting (Ren Zhi)
	// Sorts floating tasks to be at the bottom
	i = taskVector.begin();	// Points to start of unsorted part
	k = taskVector.end();	// Points to end of unsorted part
	while(i != k) {
		if(i->getType() == FLOATING) {
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

	// Sorts priority tasks to be at the top
	i = taskVector.begin(); // Points to start of unsorted part
	while (i != taskVector.end()) {
		for (j = i; j != taskVector.end(); ++j) {
			if (j->getPriorityStatus() == true) {
				tempTask = *j;
				for (k = j; k != i; --k) {
					std::swap(*k, *(k-1)); 
				}
				*i = tempTask;
				break;
			}
		}
		++i;
	}
	return true;
}

// For now, currentView is set to be the same as taskStore
bool Command::copyView() {
	currentView = taskStore;
	return true;
}

// Added by Hao Ye 14/10/15
// Replaces getIdOfIndex()
void Command::matchIndex(int index, std::vector<Task>::iterator &currIter,
						 std::vector<Task>::iterator &taskIter) {
							 if(isValidIndex(index)) {	
								 currIter = matchCurrentViewIndex(index);
								 index = currIter->getID();
								 taskIter = matchTaskStoreIndex(index);
							 } else {
								 throw std::runtime_error(ERROR_INDEX_OUT_OF_BOUNDS);
							 }
}

bool Command::isValidIndex(int index) {
	if(index <1 || index > (int)currentView.size()) {
		return false;
	} 
	return true;
} 

std::vector<Task>::iterator Command::matchCurrentViewIndex(int index) {
	assert(index >0 && index <= (int)currentView.size());
	std::vector<Task>::iterator iter = currentView.begin();
	for(int i=1 ; i< index ; ++i) {
		++iter;
	}
	return iter;
}

std::vector<Task>::iterator Command::matchTaskStoreIndex(int index) {
	std::vector<Task>::iterator iter = taskStore.begin();
	while(iter->getID() != index && iter != taskStore.end()) {
		++iter;
	}
	assert(iter != taskStore.end());
	return iter;
}

std::string Command::getMessage() {
	return "foobar";
}

//==================================================
//                        ADD
//==================================================

//============== ADD : PUBLIC METHODS ===============
Add::Add(Task task) : Command(ADD) {
	newTask = task;
	currViewID = 0;
}

Add::~Add() {}

Task Add::getNewTask() {
	return newTask;
}

void Add::execute() {
	addInfo();
}

// Add must have executed before undoing,
// otherwise currViewID will not be updated
void Add::undo() {
	Delete taskToDelete(currViewID);
	taskToDelete.execute();
}

//added @haoye 21/10/2015
std::string Add::getMessage() {
	return("\"" + newTask.getName() + "\"" + " added");
}

//============== ADD : PRIVATE METHODS ===============

bool Add::addInfo() {
	std::string dateAndTime_UI = Utilities::taskDateAndTimeToDisplayString(newTask);
	taskStore.push_back(newTask);
	currentView.push_back(newTask);
	currViewID = currentView.size();

	sortDate(taskStore);
	copyView();
	return true;
}

//==================================================
//                       DELETE
//==================================================

//============ DELETE : PUBLIC METHODS =============

Delete::Delete(int taskID) : Command(DELETE) {
	deleteID = taskID;
	currViewIter = matchCurrentViewIndex(deleteID);
	taskStoreIter = matchTaskStoreIndex(currViewIter->getID());
	taskToBeDeleted = *currViewIter;
}

Delete::~Delete() {}

int Delete::getDeleteID() {
	return deleteID;
}

void Delete::execute() {
	// userIndex refers to the nth task of currentView presented to user
	// eg. delete 1 means deleting the first task
	deleteInfo();
}

// Adds the deleted task back to the exact location it was before
void Delete::undo() {
	taskStore.insert(taskStoreIter,taskToBeDeleted);
	currentView.insert(currViewIter,taskToBeDeleted);
}

std::string Delete::getMessage() {
	return("\"" + taskToBeDeleted.getName() + "\"" + " deleted");
}

//============= DELETE : PRIVATE METHODS ===========

// Searches for Task to delete using ID
// Deleting is done according to the order of elements on currentView
// Modified by Hao Ye 14/10/15
void Delete::deleteInfo() {
	std::vector<Task>::iterator currIter;
	std::vector<Task>::iterator taskIter;

	matchIndex(deleteID,currIter,taskIter);
	taskStore.erase(taskIter);
	currentView.erase(currIter);
	sortDate(taskStore);
}

//==================================================
//                       MODIFY
//==================================================

//============= MODIFY : PUBLIC METHODS ============

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
	modifyInfo();
}

void Modify::undo() {
	Modify *undoModify = new Modify(modifyID, fieldsToModify, originalTask);
	undoModify->execute();
}

std::string Modify::getMessage() {
	return "successfully modified!";
}

//============= MODIFY : PRIVATE METHODS ===========

// Modified by Hao Ye 14/10/15
// Modified by Ren Zhi 20/10/15 updated add/deleteLabels
void Modify::modifyInfo() {
	std::vector<Task>::iterator currIter;
	std::vector<Task>::iterator taskIter;

	int index = modifyID;
	matchIndex(index,currIter,taskIter);
	originalTask = *currIter;

	std::vector<FieldType>::iterator fieldIter;

	for (fieldIter = fieldsToModify.begin(); fieldIter != fieldsToModify.end(); ++fieldIter) {
		switch (*fieldIter) {
		case NAME:
			taskIter->setName(tempTask.getName());			
			break;
		case LABELS_ADD:
			taskIter->addLabels(tempTask.getLabels());
			break;
		case LABELS_DELETE:
			taskIter->deleteLabels(taskIter->getLabels());
			break;
		case PRIORITY_SET:
			taskIter->setPriority();
			break;
		case PRIORITY_UNSET:
			taskIter->unsetPriority();
			break;
		case START_DATE:
			taskIter->setStartDate(tempTask.getStartDate());
			break;
		case START_TIME:
			taskIter->setStartTime(tempTask.getStartTime());
			break;
		case END_DATE:
			taskIter->setEndDate(tempTask.getEndDate());
			break;
		case END_TIME:
			taskIter->setEndTime(tempTask.getEndTime());
			break;
		case INVALID_FIELD:
			throw std::runtime_error("Error in fetching field name"); 
			break;
		}
		*currIter = *taskIter;
		sortDate(taskStore);
	}
}

//==================================================
//                       SEARCH
//==================================================

//============== SEARCH : PUBLIC METHODS ===========

Search::Search(std::string phraseString) : Command(SEARCH) {
	searchPhrase = phraseString;
	currentViewBeforeSearch = currentView;
}

Search::~Search() {}

std::string Search::getSearchPhrase() {
	return searchPhrase;
}

void Search::execute() {
	// Currently search returns string of names
	// currentView is also amended, can refer to items from currentView after every processInfo cmd
	// eg. add/delete will display new list under UI
	// If it is unnecessary info for add/delete, will change output of processInfo to vector<Task>
	std::string output = searchInfo();
	amendView(output);
}

void Search::undo() {
	currentView = currentViewBeforeSearch;
}

std::string Search::getMessage() {
	return "results for \"" + searchPhrase;
}

//================ SEARCH : PRIVATE METHODS =================

// Searches name for a phrase match, returns IDs of all matching tasks
std::string Search::searchInfo() {
	std::ostringstream indexString;
	std::string taskName;
	std::string returnString;
	int id;

	std::vector<Task>::iterator iter;

	for (iter = taskStore.begin(); iter != taskStore.end(); ++iter) {
		taskName = iter->getName();
		if(Utilities::isSubstring(searchPhrase,taskName)) {
			id = iter->getID();
			indexString << id << ",";
		}
	}
	returnString = indexString.str();

	if(!returnString.empty()) {
		returnString.pop_back();
	}

	return returnString;
}

// The list is separated by commas
bool Search::amendView(std::string listOfIds) {
	std::string idToken;
	int id;
	int index;
	std::vector<Task>::iterator iter;

	currentView.clear();

	while (listOfIds != "") {
		index = listOfIds.find(",");

		if(index == -1) {
			idToken = listOfIds;
			listOfIds = "";
		} else {
			idToken = listOfIds.substr(0, index);
			listOfIds = listOfIds.substr(index+1);
		}

		id = stoi(idToken);
		iter = taskStore.begin();
		for (iter = taskStore.begin() ; iter != taskStore.end(); ++iter) {
			if(id == iter->getID()) {
				currentView.push_back(*iter);
			}
		}
	}
	return true;
}

//==================================================
//                      MARKDONE
//==================================================

//============= MARKDONE : PUBLIC METHODS ===========

Markdone::Markdone(int taskID) : Command(MARKDONE) {
	doneID = taskID;
}

Markdone::~Markdone() {}

int Markdone::getDoneID() {
	return doneID;
}

void Markdone::execute() {
	markDone();
}

void Markdone::undo() {
	if(successMarkDone) {
		taskIter->unmarkDone();
		currentView.insert(currIter,*taskIter);
	}
}

//============= MARKDONE : PRIVATE METHODS ===========

// Modified by Hao Ye 14/10/15
void Markdone::markDone() {
	matchIndex(doneID,currIter,taskIter);
	successMarkDone = taskIter->markDone();
	if(successMarkDone) {
		currentView.erase(currIter);
	} // Remove from current view only if mark done successful (Ren Zhi)
}

//==================================================
//                      UNMARKDONE
//==================================================
// Added by Ren Zhi 19/10/15
//=========== UNMARKDONE : PUBLIC METHODS ==========

UnmarkDone::UnmarkDone(int taskID) : Command(MARKDONE) {
	undoneID = taskID;
}

UnmarkDone::~UnmarkDone() {}

int UnmarkDone::getUndoneID() {
	return undoneID;
}

void UnmarkDone::execute() {
	unmarkDone();
}

void UnmarkDone::undo() {
	if(successUnmarkDone) {
		taskIter->markDone();
		currentView.insert(currIter,*taskIter);
	}
}

//=========== UNMARKDONE : PRIVATE METHODS ==========

void UnmarkDone::unmarkDone() {
	matchIndex(undoneID,currIter,taskIter);
	successUnmarkDone = taskIter->unmarkDone();

	if(successUnmarkDone) {
		currentView.erase(currIter);
	} // Remove from current view only if unmark done successful (Ren Zhi)
}

//==================================================
//                        UNDO
//==================================================

Undo::Undo() : Command(UNDO) {}

Undo::~Undo() {}

//==================================================
//                        VIEW
//==================================================
/* For reference
// These are the valid View keywords
// Count: 6
const std::string VIEW_ALL = "all";
const std::string VIEW_FLOATING = "floating";
const std::string VIEW_PAST = "past";
const std::string VIEW_TODO = "todo";
const std::string VIEW_WEEK = "week";
const std::string VIEW_LABEL = "label";

// These are the View enums
// Count: 6 + VIEWTYPE_INVALID
enum ViewType {
VIEWTYPE_ALL,
VIEWTYPE_FLOATING,
VIEWTYPE_PAST,
VIEWTYPE_TODO,
VIEWTYPE_WEEK,
VIEWTYPE_LABELS,
VIEWTYPE_INVALID
};
*/
View::View(ViewType newView, std::string restOfInput) : Command(VIEW) {
	view = newView;
	viewLabels = Utilities::stringToVec(restOfInput);
	previousView = currentView;
}

View::~View() {}

ViewType View::getViewType() {
	return view;
}

void View::execute() {
	switch (view) {
	case VIEWTYPE_ALL:
		viewAll();
		break;

	case VIEWTYPE_FLOATING:
		viewTaskType(FLOATING);
		break;

	case VIEWTYPE_TODO:
		viewTaskType(TODO);
		break;

	case VIEWTYPE_PAST:
		viewDone();
		break;

	case VIEWTYPE_WEEK:
		// pwrSearch.setTasksWithinPeriod(currentTime, currentTime + 7);
		break;

		// case VIEWTYPE_LABELS:
		//	viewLabel(label);

	case VIEWTYPE_NOTDONE:
		viewNotdone();
		break;
	}
}

void View::undo() {
	currentView = previousView;
}

//============== VIEW : PRIVATE METHODS ============
// Added by Kiat Boon 20/10/15

bool View::viewAll() {
	currentView = taskStore;
	return true;
}

bool View::viewTaskType(TaskType type) {
	currentView.clear();
	std::vector<Task>::iterator iter;

	for (iter = taskStore.begin(); iter != taskStore.end(); ++iter) {
		if(iter->getType() == type) {
			currentView.push_back(*iter);
		}
	}

	sortDate(currentView);
	return true;
}

bool View::viewDone() {
	std::vector<Task>::iterator iter;

	currentView.clear();

	for (iter == taskStore.begin(); iter != taskStore.end(); ++iter) {
		if (iter->getDoneStatus() == true) {
			currentView.push_back(*iter);
		}
	}
	return true;
}

bool View::viewNotdone() {
	std::vector<Task>::iterator iter;

	currentView.clear();

	for (iter == taskStore.begin(); iter != taskStore.end(); ++iter) {
		if (iter->getDoneStatus() == false) {
			currentView.push_back(*iter);
		}
	}
	return true;
}
// Delete viewLabel if we use search to search for label
// If view is used to view labels, need to add string object for this method
bool View::viewLabel(std::string label) {
	std::vector<std::string> searchSet;

	std::vector<Task>::iterator taskIter;
	std::vector<std::string>::iterator setIter;

	currentView.clear();

	for (taskIter == taskStore.begin(); taskIter != taskStore.end(); ++taskIter) {
		searchSet = taskIter->getLabels();

		for (setIter == searchSet.begin(); setIter != searchSet.end(); ++setIter) {
			if (*setIter == label) {
				currentView.push_back(*taskIter);
				break;
			}
		}
	}

	return true;
}

//==================================================
//                    CLEAR_ALL
//==================================================
// Added by Aaron 20/10/15
ClearAll::ClearAll() : Command(CLEAR_ALL) {
	currentView = *(new std::vector<Task>);
}

ClearAll::~ClearAll() {}

void ClearAll::execute() {
	currentView = *(new std::vector<Task>);
}

void ClearAll::undo() {
	currentView = previousView;
	// TODO: feedback
}

//==================================================
//                    DISPLAY_ALL
//==================================================

DisplayAll::DisplayAll() : Command(DISPLAY_ALL) {
	previousView = currentView;
}

DisplayAll::~DisplayAll() {}

void DisplayAll::execute() {
	copyView();
}

void DisplayAll::undo() {
	currentView = previousView;
	// TODO: feedback
}

//==================================================
//                        LOAD
//==================================================

// Load most recent file path (or default)
Load::Load() : Command(LOAD) {
	IO* io = IO::getInstance();
	filePath = io->getFilePath();
}

// Load new file path
Load::Load(std::string newFilePath) : Command(LOAD) {
	IO* io = IO::getInstance();
	filePath = newFilePath;
}

Load::~Load() {}

std::string Load::getFilePath() {
	return filePath;
}

// TODO: Clear history after load, to avoid seg fault
void Load::execute() {
	// Note: filePath should already have been parsed (Aaron)
	// Parser* parser = Parser::getInstance();
	// std::string newFilePath = parser->parseFileName(filePath);

	taskStore = io->loadFile(filePath);
	io->setFilePath(filePath,taskStore);
	copyView();
	// TODO: update feedback
}

//==================================================
//                        SAVE
//==================================================

// Save to current file path (or default)
Save::Save() : Command(SAVE) {
	io = IO::getInstance();
	filePath = io->getFilePath();
}

// Save to new file path
Save::Save(std::string newFilePath) : Command(SAVE) {
	filePath = newFilePath;
}

Save::~Save() {}

std::string Save::getFilePath() {
	return filePath;
}

void Save::execute() {
	// Note: filePath should already have been parsed (Aaron)
	// Parser* parser = Parser::getInstance();
	// std::string newFilePath = parser->parseFileName(filePath);

	// setFilePath returns false if unable to change file path
	io->setFilePath(filePath,taskStore);
	// TODO: feedback
}

//==================================================
//                        EXIT
//==================================================

Exit::Exit() : Command(EXIT) {}

Exit::~Exit() {}

void Exit::execute() {
	exit(0);
}
