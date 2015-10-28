// Created and maintained by Aaron Chong Jun Hao @@author A0110376N
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

// Added on 24/10/15 by Ng Ren Zhi @@author A0130463R
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

// Modified on 27/10/15 by Chin Kiat Boon @@author A0096720A
// If end date is later than start date, start/end time should not matter
bool Command::isDateLogical(Task task) {
	if (   (task.getStartDate() >  task.getEndDate())
		|| (task.getStartDate() == task.getEndDate() && task.getStartTime() > task.getEndTime())) {
			return false;
	}
	return true;
}

// Added on 27/10/15 by Chin Kiat Boon @@author A0096720A
// Modified on 27/10/15 by Ng Ren Zhi @@author A0130463R
// Sorts floating tasks to be at the bottom
void Command::sortFloating(std::vector<Task> &taskVector) {
	std::vector<Task>::iterator i;
	std::vector<Task>::iterator j;
	std::vector<Task>::iterator k;
	Task tempTask;

	// In-place sorting
	i = taskVector.begin();	// Points to start of unsorted part
	k = taskVector.end();	// Points to end of unsorted part
	while(i < k) {
		if(i->getType() == FLOATING) {
			tempTask = *i;
			for (j = i+1; j != taskVector.end(); ++j) {
				std::swap(*j, *(j-1)); 
			}
			*(j-1) = tempTask;
			if(k == taskVector.begin()) {
				break;
			} else {
				--k;
			}
		} else {
			++i;
		}
	}
}

// Added on 27/10/15 by Chin Kiat Boon @@author A0096720A
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

// Added on 27/10/15 by Chin Kiat Boon @@author A0096720A
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

// Added on 27/10/15 by Chin Kiat Boon @@author A0096720A
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

void Command::removeFloatingTasks(std::vector<Task> &taskVector) {
	std::vector<Task>::iterator i = taskVector.begin();

	while (i != taskVector.end()) {
		if (i->getType() == FLOATING) {
			i = taskVector.erase(i);
		} else {
			++i;
		}
	}
}

// Added on 27/10/15 by Chin Kiat Boon @@author A0096720A
// Modified on 28/10/15 by Ng Ren Zhi @@author A0130463 - resolve iterator out-of-bounds
void Command::findOverlapPeriods() {
	overlapPeriods.clear();

	std::vector<Task> taskStoreCopy = taskStore;

	// Tasks that are already done will not be marked as "overlap"
	removeDoneTasks(taskStoreCopy);
	sortDate(taskStoreCopy);
	// No overlap to check
	if(taskStoreCopy.size() <= 1) {
		return;
	}

	std::vector<Task>::iterator i = taskStoreCopy.begin();
	std::vector<Task>::iterator j = taskStoreCopy.begin()+1;

	while ((i != taskStoreCopy.end()) && (j != taskStoreCopy.end())) {
		// Overlap occurs
		if (j->getStartDate() < i->getEndDate()) {
			// Scenario where j is completely overlapped
			if (j->getEndDate() < i->getEndDate()) {
				addPeriod(overlapPeriods, j->getStartDate(), j->getStartTime(), j->getEndDate(), j->getEndTime());
			}
			// Scenario where j is not completely overlapped
			if (j->getEndDate() > i->getEndDate()) {
				addPeriod(overlapPeriods, j->getStartDate(), j->getStartTime(), i->getEndDate(), i->getEndTime());
				++i;
			}
		} else if (j->getStartDate() >= i->getEndDate()) {
			++i;
		}
		++j;

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
bool Command::copyView() {
	currentView = taskStore;
	return true;
}

// Updates only the modified task on the UI 
void Command::updateView() {
	*currViewIter = *taskStoreIter;
	return;
}

// Added on 14/10/15 by Soon Hao Ye @@author A0126677U
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
	isOverlap = false;
}

Add::~Add() {}

Task Add::getNewTask() {
	return newTask;
}

void Add::execute() {
	doAdd();
}

// Add must have executed before undoing,
// otherwise currViewID will not be updated
void Add::undo() {
	Delete taskToDelete(currViewID);
	taskToDelete.execute();
}

std::string Add::getMessage() {
	if (isOverlap) {
		return "Task to be added overlaps with existing task!";
	} else {
		return("\"" + newTask.getName() + "\"" + " added");
	}
}

//============== ADD : PRIVATE METHODS ===============

// Modified on 24/10/15 by Chin Kiat Boon @@author A0096720A
// Modified on 25/10/15 by Ng Ren Zhi @@author A0130463
bool Add::doAdd() {
	if (isDateLogical(newTask) == false) {
		throw std::runtime_error(ERROR_TASK_START_LATER_THAN_TASK_END);
	}


	checkOverlap();
	taskStore.push_back(newTask);
	currentView.push_back(newTask);
	currViewID = currentView.size();

	sortDate(taskStore);
	sortDate(currentView);
	removeDoneTasks(currentView);
	return true;
}

void Add::checkOverlap() {
	std::vector<Task> taskStoreCopy = taskStore;
	removeDoneTasks(taskStoreCopy);
	removeFloatingTasks(taskStoreCopy);
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
	findOverlapPeriods();
}

std::string Delete::getMessage() {
	return("\"" + taskToBeDeleted.getName() + "\"" + " deleted");
}

//============= DELETE : PRIVATE METHODS ===========

void Delete::doDelete() {
	taskStore.erase(taskStoreIter);
	currentView.erase(currViewIter);
	sortDate(taskStore);
	removeDoneTasks(currentView);
	findOverlapPeriods();
}

void Delete::setUndoDeleteInfo() {
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
	initialiseIterators(modifyID);
	originalTask = *currViewIter;
	doModify();
}

void Modify::undo() {
	taskStore.erase(taskStoreIter);
	currentView.erase(currViewIter);
	taskStore.insert(taskStoreIter,originalTask);
	currentView.insert(currViewIter,originalTask);

	/*
	Modify *undoModify = new Modify(modifyID, fieldsToModify, originalTask);
	undoModify->execute();
	moveToPrevPos();
	*/
	findOverlapPeriods();
}

std::string Modify::getMessage() {
	return "successfully modified!";
}

//============= MODIFY : PRIVATE METHODS ===========

// Modified on 24/10/15 by Aaron Chong Jun Hao @@author A0110376N
void Modify::doModify() {
	std::vector<FieldType>::iterator fieldIter;
	bool isTODO = false;

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
			break;
		case END_DATE:
			taskStoreIter->setEndDate(tempTask.getEndDate());
			break;
		case END_TIME:
			taskStoreIter->setEndTime(tempTask.getEndTime());
			break;
		case TODO_DATE:
			isTODO = true;
			taskStoreIter->setEndDate(tempTask.getEndDate());
			// taskStoreIter->setStartDate(tempTask.getStartDate());
			break;
		case TODO_TIME:
			isTODO = true;
			taskStoreIter->setEndTime(tempTask.getEndTime());
			// taskStoreIter->setStartTime(tempTask.getStartTime());
			break;
		case INVALID_FIELD:
			throw std::runtime_error("Error in fetching field name"); 
		}
	}

	if(isTODO) {
		taskStoreIter->setStartDate(taskStoreIter->getEndDate());
		taskStoreIter->setStartTime(taskStoreIter->getEndTime());
	}

	updateTaskTypes();
	updateView();
	sortDate(taskStore);
	initialiseIterators(modifyID);
	findOverlapPeriods();
}

//<<<<<Update task type methods added by Ren Zhi 25/10/15
// If start date == 0 && end date == 0: FLOATING
// If end date == start date: TODO
// All others: EVENTS
void Modify::updateTaskTypes() {
	if(!updateFLOATING()) {
		if(!updateTODO()) {
			updateEVENT();
		}
	}
}

bool Modify::updateFLOATING() {
	if(taskStoreIter->getStartDate()==DATE_NOT_SET && taskStoreIter->getStartTime()==TIME_NOT_SET
		&& taskStoreIter->getEndDate()==DATE_NOT_SET && taskStoreIter->getEndTime()==TIME_NOT_SET) {
			taskStoreIter->setType(FLOATING);
			return true;
	}
	return false;
}

bool Modify::updateTODO() {
	if(taskStoreIter->getStartDate() == taskStoreIter->getEndDate()
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
void Modify::moveToPrevPos() {
	std::vector<Task>::iterator preCurrViewIter;
	Task tempTask = *currViewIter;
	currentView.erase(currViewIter);

	preCurrViewIter = currentView.begin() + prevCurrPos;
	currentView.insert(preCurrViewIter, tempTask);
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

// Added on 27/10/15 by Chin Kiat Boon @@author A0096720A
// Virtual method for accessing of PowerSearch
void Search::setTasksWithinPeriod(int startDate, int startTime, int endDate, int endTime) {}

// Returns a string of names
// If it is unnecessary info for add/delete, will change output of processInfo to vector<Task>
void Search::execute() {
	std::string output = doSearch();
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
std::string Search::doSearch() {
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

// Processes a list separated by commas
bool Search::amendView(std::string listOfIds) {
	currentView.clear();

	int id;
	int index;
	std::string idToken;
	std::vector<Task>::iterator iter;

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

	findOverlapPeriods();
}

std::string Markdone::getMessage() {
	return "\"" + currViewIter->getName() + "\" marked as done";
}

//============= MARKDONE : PRIVATE METHODS ===========

void Markdone::markDone() {
	initialiseIterators(doneID);

	successMarkDone = taskStoreIter->markDone();
	if(successMarkDone) {
		currentView.erase(currViewIter);
	}

	findOverlapPeriods();
}

//==================================================
//                      UNMARKDONE
//==================================================
// Added on 19/10/15 by Ng Ren Zhi @@author A0130463R

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

	findOverlapPeriods();
}

std::string UnmarkDone::getMessage() {
	return "\"" + currViewIter->getName() + "\" marked as not done";
}

//=========== UNMARKDONE : PRIVATE METHODS ==========

void UnmarkDone::unmarkDone() {
	initialiseIterators(undoneID);

	successUnmarkDone = taskStoreIter->unmarkDone();
	if(successUnmarkDone) {
		currentView.erase(currViewIter);
	}

	findOverlapPeriods();
}

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
		viewWeek(currentDate, 0, currentDate+7, 2359);
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

std::string View::getMessage() {
	std::string viewMsg = "Viewing: " + Utilities::viewTypeToString(view);
	if(Utilities::viewTypeToString(view) == VIEW_LABEL) {
		viewMsg += " " + Utilities::vecToString(viewLabels);
	}
	return viewMsg;
}

//============== VIEW : PRIVATE METHODS ============
// Added on 20/10/15 by Chin Kiat Boon @@author A0096720A

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
	removeDoneTasks(currentView);		// Done tasks only displayed when viewing "all" or "past" (Aaron)
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

bool View::viewNotdone() {
	currentView.clear();

	std::vector<Task>::iterator iter;
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

	removeDoneTasks(currentView);	// Display done tasks only when viewing "all" and "past" (Aaron)
	return true;
}

void View::viewWeek(int startDate, int startTime, int endDate, int endTime) {
	currentView.clear();
	copyView();
	sortDate(currentView);
	removeDoneTasks(currentView);
	std::vector<Task>::iterator iter = currentView.begin();

	for (iter = currentView.begin(); iter != currentView.end(); ++iter) {
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

//==================================================
//                    CLEAR_ALL
//==================================================
// Added on 20/10/15 by Aaron Chong Jun Hao @@author A0110376N

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
	return "TextBuddy cleared";
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
	// Requires refinement, possibly separating deadlines and events
	std::vector<Task> startUpView;
	std::vector<Task> noStar;
	std::vector<Task>::iterator i = currentView.begin();
	while(i != currentView.end()) {
		if(!(i->getDoneStatus())) {
			if(i->getPriorityStatus()) {
				startUpView.push_back(*i);
			} else {
				noStar.push_back(*i);
			}
		}
		++i;
	}
	sortDate(noStar);
	noStar.insert(noStar.begin(),startUpView.begin(),startUpView.end());
	currentView = noStar;
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

//==================================================
//                        LOAD
//==================================================

// Load most recent file path (or default)
Load::Load() : Command(LOAD) {
	IO* io = IO::getInstance();
	filePath = io->getFilePath();
}

// Load new file path, which is already parsed
Load::Load(std::string newFilePath) : Command(LOAD) {
	filePath = newFilePath;
	loadSuccess = true;
}

Load::~Load() {}

std::string Load::getFilePath() {
	return filePath;
}

void Load::execute() {
	std::vector<Task> temp = taskStore;
	try {
		taskStore = io->loadFile(filePath);		// Exception thrown if file does not exist
		History::getInstance()->clearHistory(); // Clear history after load, to avoid seg fault
		copyView();
	} catch (std::exception e) {
		taskStore = temp;
		loadSuccess = false;
		throw e;
	}
}

std::string Load::getMessage() {
	if(loadSuccess) {
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
Save::Save(std::string newFilePath) : Command(SAVE) {
	filePath = newFilePath;
}

Save::~Save() {}

std::string Save::getFilePath() {
	return filePath;
}

void Save::execute() {
	saveSuccess = io->setFilePath(filePath,taskStore); // Returns false if unable to change file path
}

std::string Save::getMessage() {
	if(saveSuccess) {
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
	io->saveFile(io->getFilePath(),taskStore); // In case user or system deletes file or .tbconfig
	delete TbLogger::getInstance();
	exit(0);
}