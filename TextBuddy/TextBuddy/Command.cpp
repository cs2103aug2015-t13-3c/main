// Created and maintained by @@author A0110376N (Aaron Chong Jun Hao)
// Modified to Command Pattern by Ng Ren Zhi
// Private methods originally by Chin Kiat Boon

#include "stdafx.h"
#include "History.h"

//==================================================
//                      COMMAND
//==================================================

//========== COMMAND : PUBLIC METHODS ==============

Command::Command(CommandType newCmd, std::string rawInput) {
	cmd = newCmd;
	userInput = rawInput;
}

Command::~Command() {}

// Check with Hanrui if formatting single-step getters like this will be penalised
CommandType Command::getCommand() {return cmd;}
std::string Command::getUserInput() {return userInput;}
std::vector<Task> Command::getTaskStore() {return taskStore;}
std::vector<Task> Command::getCurrentView() {return currentView;}
std::vector<Task>* Command::getCurrentViewPtr() {return &currentView;}
int Command::getSize() {return taskStore.size();}
/*
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
*/

void Command::clearTaskStore() {
	IO* io = IO::getInstance();
	taskStore.clear();
	currentView.clear();
	remove(io->getFilePath().c_str());
	return;
}

// Virtual functions
void Command::execute() {}
void Command::undo() {throw std::runtime_error("Action cannot be undone");}

//================ COMMAND : PROTECTED METHODS ==================

std::vector<Task> Command::taskStore;
std::vector<Task> Command::currentView;
const std::string Command::ERROR_INDEX_OUT_OF_BOUNDS = "Invalid index";
const std::string Command::ERROR_TASK_START_LATER_THAN_TASK_END = "Start of task is later than end of task";

// Added by Ren Zhi 24/10/15
// Initialises the corresponding iterators with the taskID
// TaskID is the ID seen on GUI, not the unique task ID
// Use for in-place insertion / deletion for undo methods
void Command::initialiseIterators(int taskID) {
	matchIndex(taskID,currViewIter,taskStoreIter);
	currViewPos = currViewIter - currentView.begin();
	taskStorePos = taskStoreIter - taskStore.begin();
}

// Added by Ren Zhi 24/10/15
// Get iterators from their vector index
// Use for in-place insertion / deletion for undo methods
// Don't use previous iterator values, because it may go out-of-bounds
void Command::getIterator() {
	currViewIter = currentView.begin() + currViewPos;
	taskStoreIter = taskStore.begin() + taskStorePos;
}

bool Command::isDateLogical(Task task) {
	if (task.getStartDate() > task.getEndDate()) {
		return false;
	} else if (task.getStartDate() == task.getEndDate()) {
		if (task.getStartTime() > task.getEndTime()) {
			return false;
		}
	}
	return true;
}

// Sorts floating tasks to be at the bottom
void Command::sortFloating(std::vector<Task> &taskVector) {
	std::vector<Task>::iterator i;
	std::vector<Task>::iterator j;
	std::vector<Task>::iterator k;
	Task tempTask;

	// Bugfix: in-place sorting (Ren Zhi)
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
}

// Sorts priority tasks to be at the top
void Command::sortPriority(std::vector<Task> &taskVector) {
	std::vector<Task>::iterator i;
	std::vector<Task>::iterator j;
	std::vector<Task>::iterator k;
	Task tempTask;

	i = taskVector.begin();
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
}

// Sorts in increasing order of dates (except for floating tasks, which are at the bottom)
// Use this before returning to UI for display
void Command::sortDate(std::vector<Task> &taskVector) {
	std::vector<Task>::iterator i;
	std::vector<Task>::iterator j;

	for (i = taskVector.begin(); i != taskVector.end(); ++i) {
		assert((i->getStartDate() < i->getEndDate()) || 
			((i->getStartDate() == i->getEndDate()) && (i->getStartTime() <= i->getEndTime())));
		for (j = i+1; j != taskVector.end(); ++j) {
			if(j -> getStartTime() < i->getStartTime()) {
				std::swap(*i, *j);
			}
		}
	}

	// Sorts date after time to ensure date is accurately sorted
	for (i = taskVector.begin(); i != taskVector.end(); ++i) {
		for (j = i+1; j != taskVector.end(); ++j) {
			if(j -> getStartDate() < i -> getStartDate()) {
				std::swap(*i, *j);
			}
		}
	}

	sortFloating(taskVector);
	sortPriority(taskVector);
}

void Command::removeDoneTask() {
	std::vector<Task>::iterator i = currentView.begin();

	while (i != currentView.end()) {
		if (i->getDoneStatus() == true) {
			i = currentView.erase(i);
		} else {
			++i;
		}
	}
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
								 TbLogger::getInstance()->log(WARN,"Invalid index: " + std::to_string(index));
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
	return "";
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

	//added @kiatboon 24/10/15 
	if (isDateLogical(newTask) == false) {
		throw std::runtime_error(ERROR_TASK_START_LATER_THAN_TASK_END);
	}

	taskStore.push_back(newTask);
	currentView.push_back(newTask);
	currViewID = currentView.size();

	sortDate(taskStore);
	copyView();
	removeDoneTask();
	return true;
}

//==================================================
//                       DELETE
//==================================================

//============ DELETE : PUBLIC METHODS =============

Delete::Delete(int taskID) : Command(DELETE) {
	deleteID = taskID;	
}

Delete::~Delete() {}

int Delete::getDeleteID() {
	return deleteID;
}

void Delete::execute() {
	// userIndex refers to the nth task of currentView presented to user
	// eg. delete 1 means deleting the first task
	initialiseIterators(deleteID);
	deleteInit();
	deleteInfo();
}

// Adds the deleted task back to the exact location it was before
void Delete::undo() {
	if((unsigned int)taskStorePos < taskStore.size()) {
		taskStore.insert(taskStore.begin() + taskStorePos,taskToBeDeleted);
	} else {
		taskStore.push_back(taskToBeDeleted);
	}

	if((unsigned int)currViewPos < currentView.size()) {
		currentView.insert(currentView.begin() + currViewPos,taskToBeDeleted);
	} else {
		currentView.push_back(taskToBeDeleted);
	}
}

std::string Delete::getMessage() {
	return("\"" + taskToBeDeleted.getName() + "\"" + " deleted");
}

//============= DELETE : PRIVATE METHODS ===========

// Searches for Task to delete using ID
// Deleting is done according to the order of elements on currentView
// Modified by Hao Ye 14/10/15
void Delete::deleteInfo() {
	taskStore.erase(taskStoreIter);
	currentView.erase(currViewIter);
	sortDate(taskStore);
	removeDoneTask();
}

// Added by Ren Zhi 24/10/15
// Initialises undo info for delete command
void Delete::deleteInit() {
	taskToBeDeleted = *currViewIter;
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
	matchIndex(modifyID,currIter,taskIter);
	originalTask = *currIter;
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

// Modified by Hao Ye	14/10/15
// Modified by Ren Zhi	20/10/15 Updated add/deleteLabels
// Modified by Aaron	24/10/15 Fix 'for' to only loop 'switch'
void Modify::modifyInfo() {
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
			taskIter->deleteLabels(tempTask.getLabelsToDelete());
			break;
		case LABELS_CLEAR:
			taskIter->clearLabels();
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
		}
	}

	if(taskIter->getStartDate()==0 && taskIter->getStartTime()==0
		&& taskIter->getEndDate()==0 && taskIter->getEndTime()==0) {
			taskIter->setType(FLOATING);
	} else if(taskIter->getStartDate() == taskIter->getEndDate()
		&& taskIter->getStartTime() == taskIter->getEndTime()) {
			taskIter->setType(TODO);
	} else {
		taskIter->setType(EVENT);
	}

	*currIter =	*taskIter;
	sortDate(taskStore);
	copyView(); // Update currentView immediately
	removeDoneTask();
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
		getIterator();
		taskStoreIter->unmarkDone();
		currentView.insert(currViewIter,*taskStoreIter);
	}	
}



//============= MARKDONE : PRIVATE METHODS ===========

// Modified by Hao Ye 14/10/15
void Markdone::markDone() {
	initialiseIterators(doneID);

	successMarkDone = taskStoreIter->markDone();

	if(successMarkDone) {
		currentView.erase(currViewIter);
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
		getIterator();
		taskStoreIter->markDone();
		currentView.insert(currViewIter,*taskStoreIter);
	}
}

//=========== UNMARKDONE : PRIVATE METHODS ==========

void UnmarkDone::unmarkDone() {
	initialiseIterators(undoneID);
	successUnmarkDone = taskStoreIter->unmarkDone();

	if(successUnmarkDone) {
		currentView.erase(currViewIter);
	} // Remove from current view only if unmark done successful (Ren Zhi)
}

//==================================================
//                        UNDO
//==================================================

Undo::Undo() : Command(UNDO) {}

Undo::~Undo() {}

//==================================================
//                        REDO
//==================================================

Redo::Redo() : Command(REDO) {}

Redo::~Redo() {}

//==================================================
//                        VIEW
//==================================================

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
	TbLogger* logger = TbLogger::getInstance();
	logger->log(DEBUG,"Viewing...");

	switch (view) {
	case VIEWTYPE_ALL:
		viewAll();
		break;

	case VIEWTYPE_FLOATING:
		viewTaskType(FLOATING);
		break;

	case VIEWTYPE_EVENT:
		viewTaskType(EVENT);
		break;

	case VIEWTYPE_TODO:
		viewTaskType(TODO);
		break;

	case VIEWTYPE_PAST:
		viewDone();
		break;

	case VIEWTYPE_WEEK: {
		logger->log(DEBUG,"Viewing week");
		int currentDate = logger->getDate();
		pwrSearch.setTasksWithinPeriod(currentDate, 0, currentDate+7, 2359);
		break;}

	case VIEWTYPE_LABELS:
		viewLabel(viewLabels);
		break;

	case VIEWTYPE_NOTDONE:
		viewNotdone();
		break;

	case VIEWTYPE_INVALID:
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
	removeDoneTask();				//when viewing tasks based on task type, should done tasks be displayed?
	return true;
}

bool View::viewDone() {
	std::vector<Task>::iterator iter;

	currentView.clear();

	for (iter = taskStore.begin(); iter != taskStore.end(); ++iter) {
		if (iter->getDoneStatus() == true) {
			currentView.push_back(*iter);
		}
	}
	return true;
}

bool View::viewNotdone() {
	std::vector<Task>::iterator iter;

	currentView.clear();

	for (iter = taskStore.begin(); iter != taskStore.end(); ++iter) {
		if (iter->getDoneStatus() == false) {
			currentView.push_back(*iter);
		}
	}
	return true;
}

// Delete viewLabel if we use search to search for label
// If view is used to view labels, need to add string object for this method
bool View::viewLabel(std::vector<std::string> label) {
	std::vector<std::string> searchSet;

	std::vector<Task>::iterator taskIter;
	std::vector<std::string>::iterator setIter;
	std::vector<std::string>::iterator labelIter;

	currentView.clear();

	for (taskIter = taskStore.begin(); taskIter != taskStore.end(); ++taskIter) {
		searchSet = taskIter->getLabels();
		
		for (setIter = searchSet.begin(); setIter != searchSet.end(); ++setIter) {
			for (labelIter = label.begin(); labelIter != label.end(); ++labelIter) {
				if (*setIter == *labelIter) {
					currentView.push_back(*taskIter);
					break;
				}
			}
		}
	}

	removeDoneTask();				// When viewing tasks based on task type, should done tasks be displayed?
	// Nope, display done tasks only when viewing "all" and "past" (Aaron)
	return true;
}

//==================================================
//                    CLEAR_ALL
//==================================================
// Added by Aaron 20/10/15
ClearAll::ClearAll() : Command(CLEAR_ALL) {
	previousView = currentView;
}

ClearAll::~ClearAll() {}

void ClearAll::execute() {
	currentView.clear();
	taskStore.clear();
}

void ClearAll::undo() {
	currentView = previousView;
	taskStore = previousView;
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
	formatDefaultView();
}

void DisplayAll::formatDefaultView() {
	// requires refinement, possibly separating deadlines and events
	std::vector<Task> startUpView;
	std::vector<Task> noStar;
	std::vector<Task>::iterator i = currentView.begin();
	while(i != currentView.end()) {
		if(i->getPriorityStatus()) {
			startUpView.push_back(*i);
		} else {
			noStar.push_back(*i);
		}
		++i;
	}
	sortDate(noStar);
	noStar.insert(noStar.begin(),startUpView.begin(),startUpView.end());
	currentView = noStar;
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
	filePath = newFilePath;
}

Load::~Load() {}

std::string Load::getFilePath() {
	return filePath;
}

void Load::execute() {
	// Note: filePath should already have been parsed (Aaron)
	std::vector<Task> temp = taskStore;
	try {
		taskStore = io->loadFile(filePath);		// Exception thrown if file does not exist
		History::getInstance()->clearHistory(); // Clear history after load, to avoid seg fault
		copyView();								// Update currentView
	} catch (std::exception e) {
		taskStore = temp;
		throw e;
	}
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
	IO* io = IO::getInstance();
	io->saveFile(io->getFilePath(),taskStore); // In case user or system deletes file or .tbconfig
	delete TbLogger::getInstance();
	exit(0);
}
