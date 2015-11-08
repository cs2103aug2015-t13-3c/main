// Created and maintained by Aaron Chong Jun Hao
// Private methods originally by Chin Kiat Boon
// Converted to Command Pattern by Ng Ren Zhi @@author A0130463R

#include "stdafx.h"
#include "History.h"
#include "Logic.h"

bool		TS::firstLoad = true;
std::string TS::MESSAGE_WELCOME = "Welcome to TaskShark!";

std::string TS::COMMAND_ADD = "add";
std::string TS::COMMAND_DELETE = "delete";
std::string TS::COMMAND_MODIFY = "modify";
std::string TS::COMMAND_MODIFY_EDIT = "edit";	// Alternative keyword
std::string TS::COMMAND_PICK_RESERVE = "pick";
std::string TS::COMMAND_SEARCH = "search";
std::string TS::COMMAND_MARKDONE = "done";
std::string TS::COMMAND_UNMARKDONE = "notdone";
std::string TS::COMMAND_UNDO = "undo";
std::string TS::COMMAND_REDO = "redo";
std::string TS::COMMAND_VIEW = "view";
std::string TS::COMMAND_CLEAR_ALL = "clear";
std::string TS::COMMAND_DISPLAY_ALL = "display";
std::string TS::COMMAND_LOAD = "load";
std::string TS::COMMAND_SAVE = "save";
std::string TS::COMMAND_EXIT = "exit";

//==================================================
//                      COMMAND
//==================================================

//========== COMMAND : PUBLIC METHODS ==============

Command::Command(CommandType newCmd, std::string rawInput) {
	logger = TsLogger::getInstance();
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

// Virtual functions
void Command::execute() {}

void Command::undo() {
	throw std::runtime_error("Action cannot be undone");
}

//================ COMMAND : PROTECTED METHODS ==================

const std::string Command::ERROR_INDEX_OUT_OF_BOUNDS = "Invalid index";
const std::string Command::ERROR_TASK_START_LATER_THAN_TASK_END = "Start of task is later than end of task";

std::vector<Task> Command::taskStore;
std::vector<Task> Command::currentView;

// Initialises the corresponding iterators with the taskID
// TaskID is the ID seen on GUI, not the unique task ID
// Use for in-place insertion / deletion for undo methods
void Command::initialiseIterators(int taskID) {
	matchIndex(taskID,currViewIter,taskStoreIter);
	currViewPos = currViewIter - currentView.begin();
	taskStorePos = taskStoreIter - taskStore.begin();
}

// Get iterators from their vector index
// Use for in-place insertion / deletion for undo methods
// Don't use previous iterator values, because it may go out-of-bounds
void Command::getIterator() {
	currViewIter = currentView.begin() + currViewPos;
	taskStoreIter = taskStore.begin() + taskStorePos;
}

// If end date is later than start date, start/end time should not matter
bool Command::isDateLogical(Task task) {
	if (   (task.getStartDate() >  task.getEndDate())
		|| (task.getStartDate() == task.getEndDate() && task.getStartTime() > task.getEndTime())) {
			return false;
	}
	return true;
}

// Sorts floating tasks to be at the bottom
void Command::sortFloating(std::vector<Task> &taskVector) {
	std::vector<Task>::iterator i;
	std::vector<Task>::iterator j;
	std::vector<Task>::iterator k;
	Task tempTask;

	// In-place sorting
	i = taskVector.begin();	// Points to start of unsorted part
	k = taskVector.end();	// Points to end of unsorted part
	while (i < k) {
		if (i->getType() == FLOATING) {
			tempTask = *i;
			for (j = i+1; j != taskVector.end(); ++j) {
				std::swap(*j, *(j-1)); 
			}
			*(j-1) = tempTask;
			if (k == taskVector.begin()) {
				break;
			} else {
				--k;
			}
		} else {
			++i;
		}
	}
}

// Sorts Event tasks to be at the top
void Command::sortEvent(std::vector<Task> &taskVector) {
	std::vector<Task>::iterator i;
	std::vector<Task>::iterator j;
	std::vector<Task>::iterator k;
	Task tempTask;

	// In-place sorting
	i = taskVector.end()-1;	// Points to start of unsorted part
	k = taskVector.begin();	// Points to end of unsorted part
	while (i > k) {
		if (i->getType() == EVENT) {
			tempTask = *i;
			for (j = i; j != taskVector.begin(); ++j) {
				std::swap(*j, *(j-1)); 
			}
			*j = tempTask;
			if (k == taskVector.end()) {
				break;
			} else {
				++k;
			}
		} else {
			--i;
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

void Command::viewPeriod(int startDate, int startTime, int endDate, int endTime) {
	std::vector<Task> weekStore;
	currentView.clear();
	weekStore = taskStore;
	sortDate(weekStore);
	removeDoneTasks(weekStore);
	std::vector<Task>::iterator iter = weekStore.begin();

	for (iter = weekStore.begin(); iter != weekStore.end(); ++iter) {
		if (iter->getType() == FLOATING) {
			currentView.push_back(*iter);
		} else {
			if ((iter->getStartDate() > startDate) && (iter->getStartDate() < endDate)) {
				currentView.push_back(*iter);
			}

			// If date is the same, further filter using time in the date
			if (iter->getStartDate() == startDate) {
				if (iter->getStartTime() >= startTime) {
					currentView.push_back(*iter);
				}
			}

			if (iter->getStartDate() == endDate) {
				if (iter->getStartTime() <= endTime) {
					currentView.push_back(*iter);				
				}
			}
		}
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
			if (j -> getStartTime() < i->getStartTime()) {
				std::swap(*i, *j);
			}
		}
	}

	// Sorts date after time to ensure date is accurately sorted
	for (i = taskVector.end(); i != taskVector.begin(); --i) {
		for (j = taskVector.begin()+1; j != i; ++j) {
			if ((j-1) -> getStartDate() > j -> getStartDate()) {
				std::swap(*j, *(j-1));
			}
		}
	}

	sortFloating(taskVector);
	sortPriority(taskVector);
}

void Command::removeDoneTasks(std::vector<Task> &taskVector) {
	std::vector<Task>::iterator i = taskVector.begin();

	while (i != taskVector.end()) {
		if (i->getDoneStatus() == true) {
			i = taskVector.erase(i);
		} else {
			++i;
		}
	}
}

void Command::removeTaskType(std::vector<Task> &taskVector, TaskType type) {
	std::vector<Task>::iterator i = taskVector.begin();

	while (i != taskVector.end()) {
		if (i->getType() == type) {
			i = taskVector.erase(i);
		} else {
			++i;
		}
	}
}

// Adds a period where there are no undone tasks on hand to freeSlots
void Command::addPeriod(std::vector<Task> &taskVector, int startDate, int startTime, int endDate, int endTime) {
	Task freePeriod;

	freePeriod.setStartDate(startDate);
	freePeriod.setStartTime(startTime);
	freePeriod.setEndDate(endDate);
	freePeriod.setEndTime(endTime);

	taskVector.push_back(freePeriod);
}

// Set currentView to be the same as taskStore
bool Command::updateCurrView() {
	currentView = taskStore;
	return true;
}

// Updates only the modified task on the UI 
void Command::updateViewIter() {
	*currViewIter = *taskStoreIter;
	return;
}

// Switch back to default view:
// Shows only uncompleted items from today, tomorrow and the day after
// Tasks are shown in the following order: Events, Todo, Float
void Command::defaultView() {
	sortDate(taskStore);
	int today = Utilities::getLocalDay() + Utilities::getLocalMonth()*100 + Utilities::getLocalYear()*10000;
	// TODO: Handle dates at end of month
	viewPeriod(today,0,today+2,2400);
	return;
}

void Command::matchIndex(int index, std::vector<Task>::iterator &currIter,
						 std::vector<Task>::iterator &taskIter) {
							 if (index == 0) {
								 index = Task::lastEditID;
							 }
							 if (isValidIndex(index)) {	
								 currIter = matchCurrentViewIndex(index);
								 index = currIter->getID();
								 taskIter = matchTaskStoreIndex(index);
							 } else {
								 logger->log(WARN,"Invalid index: " + std::to_string(index));
								 throw std::runtime_error(ERROR_INDEX_OUT_OF_BOUNDS);
							 }
}

bool Command::isValidIndex(int index) {
	if (index <1 || index > (int)currentView.size()) {
		return false;
	} 
	return true;
} 

std::vector<Task>::iterator Command::matchCurrentViewIndex(int index) {
	assert(index >0 && index <= (int)currentView.size());
	std::vector<Task>::iterator iter = currentView.begin();
	for (int i=1; i< index; ++i) {
		++iter;
	}
	return iter;
}

std::vector<Task>::iterator Command::matchTaskStoreIndex(int index) {
	std::vector<Task>::iterator iter = taskStore.begin();
	while (iter->getID() != index && iter != taskStore.end()) {
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

Add::Add(Task task, std::string restOfCommand) : Command(ADD) {
	newTask = task;
	currViewID = 0;
	isOverlap = false;
	invalidDateTimeString = restOfCommand;
}

Add::~Add() {}

Task Add::getNewTask() {
	return newTask;
}

void Add::execute() {
	doAdd();
	Task::lastEditID = newTask.getID();
	defaultView();
}

// Add must have executed before undoing,
// otherwise currViewID will not be updated
void Add::undo() {
	Delete taskToDelete(currViewID);
	taskToDelete.execute();
	Task::lastEditID = 0;
}

std::string Add::getMessage() {
	std::string msg;
	if (isOverlap) {
		msg = "Task added overlaps with existing task!";
	} else if (invalidDateTimeString != "") {
		msg = "Task added contains invalid date/time: " + invalidDateTimeString;
	} else {
		msg = "\"" + newTask.getName() + "\" added";
	}
	return msg;
}

//============== ADD : PRIVATE METHODS ===============

bool Add::doAdd() {
	if (isDateLogical(newTask) == false) {
		throw std::runtime_error(ERROR_TASK_START_LATER_THAN_TASK_END);
	}

	updateCurrView();
	checkOverlap();
	taskStore.push_back(newTask);
	currentView.push_back(newTask);
	currViewID = currentView.size();

	return true;
}

void Add::checkOverlap() {
	std::vector<Task> taskStoreCopy = taskStore;
	removeDoneTasks(taskStoreCopy);
	removeTaskType(taskStoreCopy, FLOATING);
	sortDate(taskStoreCopy);

	std::vector<Task>::iterator iter = taskStoreCopy.begin();

	while (iter != taskStoreCopy.end()) {
		// Will not overlap if start of task added is later than end of task alr present in list
		if ((newTask.getStartDate() > (iter->getEndDate()) || 
			((newTask.getStartDate() == iter->getEndDate()) && (newTask.getStartTime() >= iter->getEndTime())))) {
				++iter;
		} else if ((newTask.getEndDate() < (iter->getStartDate()) || 
			((newTask.getEndDate() == iter->getStartDate()) && (newTask.getEndTime() <= iter->getStartTime())))) {
				++iter;
		} else {
			isOverlap = true;
			break;
		}
	}
}
//==================================================
//                       DELETE
//==================================================

//============ DELETE : PUBLIC METHODS =============

Delete::Delete(int currentViewID) : Command(DELETE) {
	deleteID = currentViewID;	
}

Delete::~Delete() {}

int Delete::getDeleteID() {
	return deleteID;
}

void Delete::execute() {
	initialiseIterators(deleteID); // Sets taskStoreIter and currViewIter, using currentViewID
	setUndoDeleteInfo();
	doDelete();
	Task::lastEditID = 0;
	defaultView();
}

// Adds the deleted task back to the exact location it was before
void Delete::undo() {
	if ((unsigned int)taskStorePos < taskStore.size()) {
		taskStore.insert(taskStore.begin() + taskStorePos,taskToBeDeleted);
	} else {
		taskStore.push_back(taskToBeDeleted);
	}
	Task::lastEditID = taskToBeDeleted.getID();

	/*
	// For in-place undoing, if view is not set back to default
	if ((unsigned int)currViewPos < currentView.size()) {
	currentView.insert(currentView.begin() + currViewPos,taskToBeDeleted);
	} else {
	currentView.push_back(taskToBeDeleted);
	}
	*/
	defaultView();
}

std::string Delete::getMessage() {
	return("\"" + taskToBeDeleted.getName() + "\"" + " deleted");
}

//============= DELETE : PRIVATE METHODS ===========

void Delete::doDelete() {
	taskStore.erase(taskStoreIter);
}

void Delete::setUndoDeleteInfo() {
	taskToBeDeleted = *currViewIter;
}

//==================================================
//                       MODIFY
//==================================================

//============= MODIFY : PUBLIC METHODS ============

Modify::Modify(int taskID, bool isModifyFloating) : Command(MODIFY) {
	modifyID = taskID;
	isSetFloating = isModifyFloating;
}

Modify::Modify(int taskID, std::vector<FieldType> fields,
			   Task task, std::string restOfInput) : Command(MODIFY) {
				   modifyID = taskID;
				   isSetFloating = false;
				   fieldsToModify = fields;
				   tempTask = task;
				   invalidDateTimeString = restOfInput;
}

Modify::Modify(CommandType pick) : Command(pick) {}

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
	initialiseIterators(modifyID);
	originalTask = *currViewIter;
	if (isSetFloating) {
		taskStoreIter->setType(FLOATING);
		taskStoreIter->resetDatesAndTimes();
	} else {
		doModify();
	}
	updateTaskTypes();
	updateViewIter();
	sortDate(taskStore);
	initialiseIterators(modifyID);
	Task::lastEditID = originalTask.getID();
	// defaultView();
}

void Modify::undo() {
	taskStore.erase(taskStoreIter);
	currentView.erase(currViewIter);
	taskStore.insert(taskStoreIter,originalTask);
	currentView.insert(currViewIter,originalTask);
	Task::lastEditID = originalTask.getID();
}

std::string Modify::getMessage() {
	std::string msg;
	if (invalidDateTimeString != "") {
		msg = "Task added contains invalid date/time: " + invalidDateTimeString;
	} else {
		msg = "successfully modified!";
	}
	return msg;
}

//============= MODIFY : PRIVATE METHODS ===========

void Modify::doModify() {
	std::vector<FieldType>::iterator fieldIter;
	bool isTODO = false;
	bool isTODOreserve = false;
	bool isStartTimeSet = false;
	bool isEndTimeSet = false;

	if (tempTask.getType() == EVENT) {
		taskStoreIter->setType(EVENT);
	}
	if (tempTask.getReserveType() == EVENT) {
		taskStoreIter->setReserveType(EVENT);
	}

	for (fieldIter = fieldsToModify.begin(); fieldIter != fieldsToModify.end(); ++fieldIter) {
		switch (*fieldIter) {
		case NAME:
			taskStoreIter->setName(tempTask.getName());			
			break;
		case LABELS_ADD:
			taskStoreIter->addLabels(tempTask.getLabels());
			break;
		case LABELS_DELETE:
			taskStoreIter->deleteLabels(tempTask.getLabelsToDelete());
			break;
		case LABELS_CLEAR:
			taskStoreIter->clearLabels();
			break;
		case PRIORITY_SET:
			taskStoreIter->setPriority();
			break;
		case PRIORITY_UNSET:
			taskStoreIter->unsetPriority();
			break;
		case START_DATE:
			taskStoreIter->setStartDate(tempTask.getStartDate());
			break;
		case START_TIME:
			taskStoreIter->setStartTime(tempTask.getStartTime());
			if ((fieldIter+1 != fieldsToModify.end()) && (fieldIter+2 != fieldsToModify.end())
				&& (*(fieldIter+2) == START_TIME)) {	// Accounts for events that modifies endTime
					*(fieldIter+2) = END_TIME;
			}
			isStartTimeSet = true;
			break;
		case END_DATE:
			taskStoreIter->setEndDate(tempTask.getEndDate());
			break;
		case END_TIME:
			if (taskStoreIter->getEndDate() == DATE_NOT_SET) {
				taskStoreIter->setEndDate(tempTask.getStartDate());
			}
			taskStoreIter->setEndTime(tempTask.getEndTime());
			isEndTimeSet = true;
			break;
		case TODO_DATE:
			isTODO = true;
			taskStoreIter->setEndDate(tempTask.getEndDate());
			taskStoreIter->setEndTime(tempTask.getEndTime());
			break;
		case TODO_TIME:
			isTODO = true;
			isEndTimeSet = true;
			taskStoreIter->setEndTime(tempTask.getEndTime());
			break;
		case RESERVE_START_DATE:
			taskStoreIter->addReserveStartDate(tempTask.getReserveStartDate());
			break;
		case RESERVE_START_TIME:
			taskStoreIter->addReserveStartTime(tempTask.getReserveStartTime());
			break;
		case RESERVE_END_DATE:
			taskStoreIter->addReserveEndDate(tempTask.getReserveEndDate());
			break;
		case RESERVE_END_TIME:
			taskStoreIter->addReserveEndTime(tempTask.getReserveEndTime());
			break;
		case RESERVE_TODO_DATE:
			isTODOreserve = true;
			taskStoreIter->addReserveEndDate(tempTask.getEndDate());
			// taskStoreIter->addReserveStartDate(tempTask.getStartDate());
			break;
		case RESERVE_TODO_TIME:
			isTODOreserve = true;
			taskStoreIter->addReserveEndTime(tempTask.getEndTime());
			// taskStoreIter->addReserveStartTime(tempTask.getStartTime());
			break;
		case RESERVE:
			break;
		case INVALID_FIELD:
			throw std::runtime_error("Error in fetching field name"); 
		}
	}

	if (isTODO) {
		taskStoreIter->setType(TODO);
		taskStoreIter->setStartDate(taskStoreIter->getEndDate());
		if (isStartTimeSet) {
			taskStoreIter->setEndTime(taskStoreIter->getStartTime());	
		} else if (isEndTimeSet) {
			taskStoreIter->setStartTime(taskStoreIter->getEndTime());		
		}
	}
	if (isTODOreserve) {
		taskStoreIter->setReserveType(TODO);
		taskStoreIter->addReserveStartDate(taskStoreIter->getReserveEndDate());
		taskStoreIter->addReserveStartTime(taskStoreIter->getReserveEndTime());
	}
}

// If start date == 0 && end date == 0: FLOATING
// If end date == start date: TODO
// All others: EVENTS
void Modify::updateTaskTypes() {
	if (!updateFLOATING()) {
		if (!updateTODO()) {
			updateEVENT();
		}
	}
}

bool Modify::updateFLOATING() {
	if (taskStoreIter->getStartDate()==DATE_NOT_SET && taskStoreIter->getStartTime()==TIME_NOT_SET
		&& taskStoreIter->getEndDate()==DATE_NOT_SET && taskStoreIter->getEndTime()==TIME_NOT_SET) {
			taskStoreIter->setType(FLOATING);
			return true;
	}
	return false;
}

bool Modify::updateTODO() {
	if (taskStoreIter->getStartDate() == taskStoreIter->getEndDate()
		&& taskStoreIter->getStartTime() == taskStoreIter->getEndTime()) {
			taskStoreIter->setType(TODO);
			return true;
	}
	return false;
}

bool Modify::updateEVENT() {
	taskStoreIter->setType(EVENT);
	return true;
}

// Moves modified back to previous position before executing
// In case sequence was swapped during sorting
/*
void Modify::moveToPrevPos() {
std::vector<Task>::iterator preCurrViewIter;
Task tempTask = *currViewIter;
currentView.erase(currViewIter);

preCurrViewIter = currentView.begin() + prevCurrPos;
currentView.insert(preCurrViewIter, tempTask);
}
*/

// Chin Kiat Boon @@author A0096720A

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

// Returns a string of names
// If it is unnecessary info for add/delete, will change output of processInfo to vector<Task>
void Search::execute() {
	std::string output;
	if (Utilities::isSubstring("*", searchPhrase) || Utilities::isSubstring("+", searchPhrase) ||
		Utilities::isSubstring("?", searchPhrase) || Utilities::isSubstring(".", searchPhrase)) { 
			output = doRegexSearch();
	} else {
		output = doSearch();
	}
	Logic::setSearchMode();
	amendView(output);
}

void Search::undo() {
	currentView = currentViewBeforeSearch;
}

std::string Search::getMessage() {
	return "results for \"" + searchPhrase + "\"";
}

//================ SEARCH : PRIVATE METHODS =================

// Searches name for a phrase match, returns IDs of all matching tasks
std::string Search::doSearch() {
	std::ostringstream indexString;
	std::string taskName;
	std::string returnString;
	int id;

	std::vector<Task> taskVector;
	std::vector<Task>::iterator iter;

	taskVector = taskStore;
	sortDate(taskVector);

	bool isMatch = true;
	std::vector<std::string> tokens = Utilities::stringToVec(searchPhrase);
	std::vector<std::string>::iterator curr;

	for (iter = taskVector.begin(); iter != taskVector.end(); ++iter) {
		taskName = iter->getName();
		for (curr=tokens.begin(); curr!=tokens.end(); ++curr) {
			if (!Utilities::isSubstring(*curr,taskName)) {
				isMatch = false;
			}
		}

		if (isMatch) {
			id = iter->getID();
			indexString << id << ",";
		} else {
			isMatch = true;
		}
	}
	returnString = indexString.str();

	if (!returnString.empty()) {
		returnString.pop_back();
	}
	return returnString;
}

// Takes in input of regex format and allows UI to display matching searches
// Supports "*", "+", "?"
// If no time boundary, all time-related parameters to be set to -1
// If time boundary present, floating tasks will not be added into the search
std::string Search::doRegexSearch() {
	std::ostringstream indexString;
	std::string returnString;
	int id;
	bool isMatch = false;
	std::vector<Task> taskVector;
	std::vector<Task>::iterator taskIter;	
	std::vector<std::string> tokens;
	std::vector<std::string>::iterator tokenIter;


	taskVector = taskStore;
	sortDate(taskVector);

	for (taskIter = taskVector.begin(); taskIter != taskVector.end(); ++taskIter) {
		tokens = Utilities::stringToVec(taskIter->getName());
		for (tokenIter=tokens.begin(); tokenIter!=tokens.end(); ++tokenIter) {
			if (std::regex_match(*tokenIter, std::regex(searchPhrase))) {
				isMatch = true;
			}
		}

		if (isMatch) {
			id = taskIter->getID();
			indexString << id << ",";
		}

		isMatch = false;
	}

	returnString = indexString.str();

	if (!returnString.empty()) {
		returnString.pop_back();
	}
	return returnString;
}

// Processes a list separated by commas
bool Search::amendView(std::string listOfIds) {
	currentView.clear();

	int id;
	int index;
	std::string idToken;
	std::vector<Task>::iterator iter;

	while (listOfIds != "") {
		index = listOfIds.find(",");
		if (index == -1) {
			idToken = listOfIds;
			listOfIds = "";
		} else {
			idToken = listOfIds.substr(0, index);
			listOfIds = listOfIds.substr(index+1);
		}

		id = stoi(idToken);
		iter = taskStore.begin();
		for (iter = taskStore.begin(); iter != taskStore.end(); ++iter) {
			if (id == iter->getID()) {
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
	taskName = "";
}

Markdone::~Markdone() {}

int Markdone::getDoneID() {
	return doneID;
}

void Markdone::execute() {
	markDone();
	defaultView();
}

void Markdone::undo() {
	if (successMarkDone) {
		getIterator();
		taskStoreIter->unmarkDone();
		// currentView.insert(currViewIter,*taskStoreIter);
	}	
	defaultView();
}

std::string Markdone::getMessage() {
	return "\"" + taskName + "\" marked as done";
}

//============= MARKDONE : PRIVATE METHODS ===========

void Markdone::markDone() {
	initialiseIterators(doneID);

	successMarkDone = taskStoreIter->markDone();
	if (successMarkDone) {
		taskName = currViewIter->getName();
		// currentView.erase(currViewIter);
	}
}

//==================================================
//                      UNMARKDONE
//==================================================

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
	defaultView();
}

void UnmarkDone::undo() {
	if (successUnmarkDone) {
		getIterator();
		taskStoreIter->markDone();
		// currentView.insert(currViewIter,*taskStoreIter);
	}
	defaultView();
}

std::string UnmarkDone::getMessage() {
	getIterator();
	currViewIter->getName();
	return "\"" + currViewIter->getName() + "\" marked as not done";
}

//=========== UNMARKDONE : PRIVATE METHODS ==========

void UnmarkDone::unmarkDone() {
	initialiseIterators(undoneID);

	successUnmarkDone = taskStoreIter->unmarkDone();
	if (successUnmarkDone) {
		Logic::setTodayMode();
		// currentView.erase(currViewIter);
	}
}

//==================================================
//                        VIEW
//==================================================

View::View(ViewType newView, std::string labels) : Command(VIEW) {
	view = newView;
	viewLabels = Utilities::stringToVec(labels);
	previousView = currentView;
}

View::View(std::vector<std::string> viewParameters, std::string periodInput, ViewType period) : Command(VIEW) {
	view = VIEWTYPE_PERIOD;
	logger->log(DEBUG,"Setting view period");
	periodParams = viewParameters;
	periodString = periodInput;
	previousView = currentView;
}

View::~View() {}

ViewType View::getViewType() {
	return view;
}

void View::execute() {
	logger->log(DEBUG,"Viewing...");
	if (TS::firstLoad == true) {
		defaultView();
		Logic::setTodayMode();
		return;
	}

	switch (view) {
	case VIEWTYPE_ALL:
		viewAll();
		Logic::setAllMode();
		break;
	case VIEWTYPE_FLOATING:
		viewTaskType(FLOATING);
		Logic::setFloatingMode();
		break;
	case VIEWTYPE_EVENT:
		viewTaskType(EVENT);
		Logic::setEventsMode();
		break;
	case VIEWTYPE_TODO:	
		viewTaskType(TODO);
		Logic::setDeadlinesMode();
		break;
	case VIEWTYPE_PAST:	
		viewDone();
		Logic::setPastMode();
		break;
	case VIEWTYPE_WEEK: {
		logger->log(DEBUG,"Viewing week");
		int currentDate = logger->getDate();
		int weekDate = currentDate + 7;
		// In case weekDate overruns to next month
		if (((weekDate%10000)/100) == 1 || 
			((weekDate%10000)/100) == 3 || 
			((weekDate%10000)/100) == 5 || 
			((weekDate%10000)/100) == 7 || 
			((weekDate%10000)/100) == 8 || 
			((weekDate%10000)/100) == 10 || 
			((weekDate%10000)/100) == 12) {
				if (weekDate%100 > 31) {
					weekDate += 100 - 31;
				}
		} else if (((weekDate%10000)/100) == 4 || 
			((weekDate%10000)/100) == 6 || 
			((weekDate%10000)/100) == 9 || 
			((weekDate%10000)/100) == 11) {
				if (weekDate%100 > 30) {
					weekDate += 100 - 30;
				}
		} else if (((weekDate%10000)/100) == 2) {
			if (weekDate%100 > 28) {
				weekDate += 100 - 28;
			}
		}
		viewPeriod(currentDate, 0, weekDate, 2359);
		Logic::setWeekMode();
		break; }
	case VIEWTYPE_LABELS:
		viewLabel(viewLabels);
		Logic::setSearchMode();
		break;
	case VIEWTYPE_TODAY:
		viewToday();
		Logic::setTodayMode();
		break;
	case VIEWTYPE_PERIOD: {
		int startDate = Utilities::stringToInt(periodParams[1]);
		int startTime = Utilities::stringToInt(periodParams[2]);
		int endDate = Utilities::stringToInt(periodParams[3]);
		int endTime = Utilities::stringToInt(periodParams[4]);
		viewPeriod(startDate, startTime, endDate, endTime);
		logger->log(DEBUG, "Viewing period from " + std::to_string(startDate) + " " + std::to_string(startTime) + " to " + std::to_string(endDate) + " " + std::to_string(endTime));
		Logic::setEventsMode();
		break;}
	case VIEWTYPE_INVALID:
		break;
	}
	return;
}

void View::undo() {
	currentView = previousView;
}

std::string View::getMessage() {
	std::string viewMsg;

	if (TS::firstLoad == true) {
		TS::firstLoad = false;
		viewMsg = TS::MESSAGE_WELCOME;
	} else {
		viewMsg = "Viewing: " + Utilities::viewTypeToString(view);
		if (view == VIEWTYPE_LABELS) {
			viewMsg += " " + Utilities::vecToString(viewLabels);
		} else if (view == VIEWTYPE_PERIOD) {
			viewMsg = "Viewing: " + periodString;
		}
	}
	return viewMsg;
}

//============== VIEW : PRIVATE METHODS ============

bool View::viewAll() {
	currentView = taskStore;
	removeDoneTasks(currentView);
	return true;
}

bool View::viewTaskType(TaskType type) {
	currentView.clear();
	std::vector<Task>::iterator iter;

	for (iter = taskStore.begin(); iter != taskStore.end(); ++iter) {
		if (iter->getType() == type) {
			currentView.push_back(*iter);
		}
	}

	sortDate(currentView);
	removeDoneTasks(currentView);
	return true;
}

bool View::viewDone() {
	currentView.clear();

	std::vector<Task>::iterator iter;
	for (iter = taskStore.begin(); iter != taskStore.end(); ++iter) {
		if (iter->getDoneStatus() == true) {
			currentView.push_back(*iter);
		}
	}
	return true;
}

bool View::viewToday() {
	currentView.clear();

	std::vector<Task>::iterator iter;
	for (iter = taskStore.begin(); iter != taskStore.end(); ++iter) {
		if (iter->isToday()) {
			currentView.push_back(*iter);
		}
	}
	return true;
}

bool View::viewLabel(std::vector<std::string> label) {
	currentView.clear();

	std::vector<std::string> searchSet;
	std::vector<Task>::iterator taskIter;
	std::vector<std::string>::iterator setIter;
	std::vector<std::string>::iterator labelIter;

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

	removeDoneTasks(currentView);
	return true;
}

//==================================================
//                    CLEAR_ALL
//==================================================

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
}

std::string ClearAll::getMessage() {
	return "TaskShark cleared";
}

//==================================================
//                    DISPLAY_ALL
//==================================================

DisplayAll::DisplayAll() : Command(DISPLAY_ALL) {
	previousView = currentView;
}

DisplayAll::~DisplayAll() {}

void DisplayAll::execute() {
	updateCurrView();
	View view(VIEWTYPE_ALL,"");
	view.execute();
}


void DisplayAll::undo() {
	currentView = previousView;	
}

std::string DisplayAll::getMessage() {
	return "All tasks displayed";
}

//==================================================
//                        UNDO
//==================================================

Undo::Undo() : Command(UNDO) {}

Undo::~Undo() {}

void Undo::execute() {
	History::getInstance()->undo();
}

//==================================================
//                        REDO
//==================================================

Redo::Redo() : Command(REDO) {}

Redo::~Redo() {}

void Redo::execute() {
	History::getInstance()->redo();
}

// Aaron Chong @@author A0110376N

//==================================================
//                       PICK
//==================================================

//============= PICK : PUBLIC METHODS ============

Pick::Pick(int taskID, bool isPick) : Modify(PICK) {
	modifyID = taskID;
	pickReserve = isPick;
}

Pick::~Pick() {}

void Pick::execute() {
	initialiseIterators(modifyID);
	originalTask = *currViewIter;
	doPick();
	Task::lastEditID = originalTask.getID();
	logger->log(DEBUG,"Pick executed");
	// DefaultView();
	initialiseIterators(modifyID);
}

void Pick::undo() {
	*taskStoreIter = originalTask;
	*currViewIter = originalTask;
	Task::lastEditID = originalTask.getID();
	// DefaultView();
}

std::string Pick::getMessage() {
	return "successfully picked!";
}

//============= PICK : PRIVATE METHODS ===========

void Pick::doPick() {
	if (pickReserve) {
		if (taskStoreIter->getReserveStatus() == true) {
			taskStoreIter->pickReserve();
		}
	}
	taskStoreIter->clearReserve();

	updateTaskTypes();
	return;
}

//==================================================
//                        LOAD
//==================================================

// Load most recent file path (or default)
Load::Load() : Command(LOAD) {
	IO* io = IO::getInstance();
	filePath = io->getFilePath();
}

// Load new file path, which is already parsed
Load::Load(std::string newFilePath, bool isOverwriteFile) : Command(LOAD) {
	filePath = newFilePath;
	loadSuccess = true;
	isOverwriteLoadFile = isOverwriteFile;
}

Load::~Load() {}

std::string Load::getFilePath() {
	return filePath;
}

void Load::execute() {
	std::vector<Task> temp = taskStore;
	try {
		taskStore = io->loadFile(filePath,isOverwriteLoadFile); // Exception thrown if file does not exist
		History::getInstance()->clearHistory();				 // Clear history after load, to avoid seg fault
		updateCurrView();
	} catch (std::exception e) {
		taskStore = temp;
		loadSuccess = false;
		throw e;
	}
}

std::string Load::getMessage() {
	if (loadSuccess) {
		return "\"" + filePath + "\" loaded successfully!";
	} else {
		return "\"" + filePath + "\" does not exist";
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

// Save to new file path, which is already parsed
Save::Save(std::string newFilePath, bool isDeletePrevFile) : Command(SAVE) {
	filePath = newFilePath;
	isRemovePrevFile = isDeletePrevFile;
}

Save::~Save() {}

std::string Save::getFilePath() {
	return filePath;
}

void Save::execute() {
	saveSuccess = io->setFilePath(filePath,taskStore,isRemovePrevFile);
}

std::string Save::getMessage() {
	if (saveSuccess) {
		return "\"" + filePath + "\" saved successfully!";
	} else {
		return "Unable to save \"" + filePath + "\". Invalid path name.";
	}
}

//==================================================
//                        EXIT
//==================================================

Exit::Exit() : Command(EXIT) {}

Exit::~Exit() {}

void Exit::execute() {
	IO* io = IO::getInstance();
	io->saveFile(io->getFilePath(),taskStore); // In case user or system deletes file or .TSconfig
	delete logger;
	exit(0);
}