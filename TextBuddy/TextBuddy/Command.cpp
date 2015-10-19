// @@author Aaron Chong Jun Hao

#include "Command.h"
#include "stdafx.h"
#include "IO.h"
#include "Parser.h"

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

int Command::getSize() {
	return taskStore.size();
}

void Command::clearTaskStore() {
	IO io;
	taskStore.clear();
	currentView.clear();
	remove(io.getFilePath().c_str());
	return;
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

const std::string Command::ERROR_INDEX_OUT_OF_BOUNDS = "invalid index";

//sorts in increasing order of dates (except for floating tasks, they are sorted to be at the bottom)
//should use this to sort according to date before display to UI
//since tasks with earliest deadlines/event should be seen first
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

	//sorts date after time to ensure date is accurately sorted
	for (i = taskVector.begin(); i != taskVector.end(); ++i) {
		for (j = i+1; j != taskVector.end(); ++j) {
			if(j -> getStartDate() < i -> getStartDate()) {
				std::swap(*i, *j);
			}
		}
	}

	//Bugfix: in-place sorting (Ren Zhi)
	//sorts floating tasks to be at the bottom
	i = taskVector.begin(); //points to start of unsorted part
	k = taskVector.end(); //points to end of unsorted part
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

	return true;
}

// For now, currentView is set to be the same as taskStore
bool Command::copyView() {
	currentView = taskStore;
	return true;
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>added @haoye 14/10/15
//replaces getIdOfIndex()
void Command::matchIndex(int index, std::vector<Task>::iterator &currIter, 
						 std::vector<Task>::iterator &taskIter) {
							 if(isValidIndex(index)) {	
								 currIter = matchCurrentViewIndex(index);
								 index = currIter->getID();
								 taskIter = matchTaskViewIndex(index);
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

std::vector<Task>::iterator Command::matchTaskViewIndex(int index) {
	std::vector<Task>::iterator iter = taskStore.begin();
	while(iter->getID() != index && iter != taskStore.end()) {
		++iter;
	}
	assert(iter != taskStore.end());
	return iter;
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// ==================================================
//                        ADD
// ==================================================


// ============== ADD : PUBLIC METHODS ===============
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
	feedback.pushTask(newTask);
	feedback.setAddedMessage();
}

// Add must have executed before undoing,
// otherwise currViewID will not be updated
void Add::undo() {
	Delete taskToDelete(currViewID);
	taskToDelete.execute();
}

// ============== ADD : PRIVATE METHODS ===============

bool Add::addInfo() {
	std::string dateAndTime_UI = Utilities::taskDateAndTimeToDisplayString(newTask);
	newTask.setDateAndTime_UI(dateAndTime_UI);
	taskStore.push_back(newTask);
	currentView.push_back(newTask);
	currViewID = currentView.size();

	sortDate(taskStore);
	copyView();
	return true;
}

// ==================================================
//                       DELETE
// ==================================================

// ============ DELETE : PUBLIC METHODS =============

Delete::Delete(int taskID) : Command(DELETE) {
	deleteID = taskID;
	currViewIter = matchCurrentViewIndex(deleteID);
	taskStoreIter = matchTaskViewIndex(currViewIter->getID());
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
	feedback.setUpdateView(true);
}

// adds the deleted task back to the exact location it was before
void Delete::undo() {
	taskStore.insert(taskStoreIter,taskToBeDeleted);
	currentView.insert(currViewIter,taskToBeDeleted);
	feedback.setUpdateView(true);
}

// ============= DELETE : PRIVATE METHODS ===========

// Searches for Task to delete using ID
// Deleting is done according to the order of elements on currentView
//modified @haoye 14/10/15
void Delete::deleteInfo() {
	std::vector<Task>::iterator currIter;
	std::vector<Task>::iterator taskIter;

	matchIndex(deleteID,currIter,taskIter);
	taskStore.erase(taskIter);
	currentView.erase(currIter);
	sortDate(taskStore);
}

// ==================================================
//                       MODIFY
// ==================================================


// ============= MODIFY : PUBLIC METHODS ============

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
	feedback.setUpdateView(true);
}

void Modify::undo() {
	Modify *undoModify = new Modify(modifyID, fieldsToModify, originalTask);
	undoModify->execute();
}

// ============= MODIFY : PRIVATE METHODS ===========

//modified @haoye 14/10/15
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
			taskIter->setLabel(tempTask.getLabel());
			break;
		case LABELS_DELETE:
			taskIter->setLabel("");
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
			std::cout << "Error in fetching field name" << std::endl;
			break;
		}
		*currIter = *taskIter;
		std::string dateAndTime_UI = Utilities::taskDateAndTimeToDisplayString(*taskIter);
		taskIter->setDateAndTime_UI(dateAndTime_UI);

		sortDate(taskStore);
	}
}

// ==================================================
//                       SEARCH
// ==================================================

// ============== SEARCH : PUBLIC METHODS ===========

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
	bool isFound = !output.empty();
	feedback.setSearchMessage(searchPhrase,isFound);
	feedback.setUpdateView(isFound);
}

void Search::undo() {
	currentView = currentViewBeforeSearch;
}

// ================ SEARCH : PRIVATE METHODS =================

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


// ==================================================
//                      MARKDONE
// ==================================================

// ============= MARKDONE : PUBLIC METHODS ===========

Markdone::Markdone(int taskID) : Command(MARKDONE) {
	doneID = taskID;
}

Markdone::~Markdone() {}

int Markdone::getDoneID() {
	return doneID;
}

void Markdone::execute() {
	try {
		markDone();
		feedback.setUpdateView(true);
	} catch (std::exception e) {
		feedback.setErrorMessage(e.what());
	}	
}

void Markdone::undo() {
	if(successMarkDone) {
		taskIter->unmarkDone();
		currentView.insert(currIter,*taskIter);
	}
}

// ============= MARKDONE : PRIVATE METHODS ===========

//modified @haoye 14/10/15
void Markdone::markDone() {
	matchIndex(doneID,currIter,taskIter);
	successMarkDone = taskIter->markDone();
	if(successMarkDone) {
		currentView.erase(currIter);
	} // Remove from current view only if mark done successful (Ren Zhi)
}

// >>>>>>> UnmarkDone class added by Ren Zhi @19/10/15 >>>>>>>>>>>>

// ==================================================
//                      UNMARKDONE
// ==================================================

// =========== UNMARKDONE : PUBLIC METHODS ==========

UnmarkDone::UnmarkDone(int taskID) : Command(MARKDONE) {
	undoneID = taskID;
}

UnmarkDone::~UnmarkDone() {}

int UnmarkDone::getUndoneID() {
	return undoneID;
}

void UnmarkDone::execute() {
	try {
		unmarkDone();
		feedback.setUpdateView(true);
	} catch (std::exception e) {
		feedback.setErrorMessage(e.what());
	}
}

void UnmarkDone::undo() {
	if(successUnmarkDone) {
		taskIter->markDone();
		currentView.insert(currIter,*taskIter);
	}
}

// =========== UNMARKDONE : PRIVATE METHODS ==========

void UnmarkDone::unmarkDone() {
	matchIndex(undoneID,currIter,taskIter);
	successUnmarkDone = taskIter->unmarkDone();

	if(successUnmarkDone) {
		currentView.erase(currIter);
	} // Remove from current view only if unmark done successful (Ren Zhi)
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

DisplayAll::DisplayAll() : Command(DISPLAY_ALL) {
	previousView = currentView;
}

DisplayAll::~DisplayAll() {}

void DisplayAll::execute() {
	copyView();
	feedback.setUpdateView(true);
}

void DisplayAll::undo() {
	currentView = previousView;
	//TODO: feedback
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

// TODO: Clear history after load, to avoid seg fault
void Load::execute() {
	Parser parser;
	IO io;
	std::string newFilePath = parser.parseFileName(filePath);
	taskStore = io.loadFile(newFilePath);
	copyView();
	//TODO: update feedback
}

// ==================================================
//                        SAVE
// ==================================================

Save::Save(std::string newFilePath) : Command(SAVE) {
	filePath = newFilePath;
}

Save::~Save() {}

std::string Save::getFilePath() {
	return filePath;
}

void Save::execute() {
	Parser parser;
	IO io;
	// setFilePath returns false if unable to change file path
	std::string newFilePath = parser.parseFileName(filePath);
	io.setFilePath(newFilePath,taskStore);
	// TODO: feedback
}

// ==================================================
//                        EXIT
// ==================================================

Exit::Exit() : Command(EXIT) {}
Exit::~Exit() {}

void Exit::execute() {
	feedback.setExit();
}