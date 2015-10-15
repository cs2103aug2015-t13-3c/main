// @@author Chin Kiat Boon

#include "stdafx.h"
#include "Logic.h"

const std::string Logic::ERROR_INDEX_OUT_OF_BOUNDS = "invalid index";


Logic::Logic() {
	taskStore = loadFile(io.getFilePath());
	std::vector<Task>::iterator i;
	for(i=taskStore.begin() ; i!=taskStore.end(); ++i) {
		std::string dateAndTime_UI = Utilities::taskDateAndTimeToDisplayString(*i);
		i->setDateAndTime_UI(dateAndTime_UI);
	}
	currentView = taskStore;
}

Logic::~Logic() {}

// ==================================================
//                      METHODS
// ==================================================

int Logic::getSize() {
	return taskStore.size();
}

bool Logic::saveFile(std::string fileName) {
	io.saveFile(fileName, taskStore);
	return true;
}

std::vector<Task> Logic::loadFile(std::string fileName) {
	taskStore = io.loadFile(fileName);
	copyView();
	return taskStore;
}

std::vector<Task> Logic::getTaskStore() {
	return taskStore;
}

void Logic::clearTaskStore() {
	taskStore.clear();
	remove(io.getFilePath().c_str());
	return;
}

std::vector<Task> Logic::getCurrentView() {
	return currentView;
}

// For now, currentView is set to be the same as taskStore
bool Logic::copyView() {
	currentView = taskStore;
	return true;
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>added @haoye 14/10/15
//replaces getIdOfIndex()
void Logic::matchIndex(int index, std::vector<Task>::iterator &currIter, 
	std::vector<Task>::iterator &taskIter) {
	if(isValidIndex(index)) {	
		currIter = matchCurrentViewIndex(index);
		index = currIter->getID();
		taskIter = matchTaskViewIndex(index);
	} else {
		throw std::runtime_error(ERROR_INDEX_OUT_OF_BOUNDS);
	}
}

bool Logic::isValidIndex(int index) {
	if(index <1 || index > (int)currentView.size()) {
		return false;
	} 
	return true;
} 

std::vector<Task>::iterator Logic::matchCurrentViewIndex(int index) {
	assert(index >0 && index <= (int)currentView.size());
	std::vector<Task>::iterator iter = currentView.begin();
	for(int i=1 ; i< index ; ++i) {
		++iter;
	}
	return iter;
}

std::vector<Task>::iterator Logic::matchTaskViewIndex(int index) {
	std::vector<Task>::iterator iter = taskStore.begin();
	while(iter->getID() != index && iter != taskStore.end()) {
		++iter;
	}
	assert(iter != taskStore.end());
	return iter;
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//modified @haoye 14/10/15
void Logic::markDone(Markdone toMarkDone) {
	int index = toMarkDone.getDoneID();
	std::vector<Task>::iterator currIter;
	std::vector<Task>::iterator taskIter;

	matchIndex(index,currIter,taskIter);
	taskIter->markDone();
	currentView.erase(currIter);
}

/*
bool Logic::markStar(Star toMarkStar){
	int userIndex;
	int id;
	std::vector<Task>::iterator iter;

	userIndex = toMarkDone.getDoneID();
	
	try {
		id = getIdOfIndex(userIndex);
		if (id == -1) {												//error code
			throw "User input exceeded bounds.";
		}
	} catch (std::string exceedBoundStr) {
		std::cerr << exceedBoundStr << std::endl;
		return false;
	}

	iter = taskStore.begin();
	while ((iter != taskStore.end()) && (iter->getID() != id)) {
		++iter;
	}

	if (iter->getID() == id) {
		iter->setPriority();
	}

	copyView();

	return true;
}
*/

bool Logic::addInfo(Add taskName) {
	Task task = taskName.getNewTask();
	std::string dateAndTime_UI = Utilities::taskDateAndTimeToDisplayString(task);
	task.setDateAndTime_UI(dateAndTime_UI);
	taskStore.push_back(task);
	currentView.push_back(task);

//	sortDate(taskStore);
//	copyView();
	return true;
}

// Searches for Task to delete using ID
// Deleting is done according to the order of elements on currentView
//modified @haoye 14/10/15
void Logic::deleteInfo(Delete idToDelete) {
	std::vector<Task>::iterator currIter;
	std::vector<Task>::iterator taskIter;
	int index = idToDelete.getDeleteID();

	matchIndex(index,currIter,taskIter);
	taskStore.erase(taskIter);
	currentView.erase(currIter);
	sortDate(taskStore);

}

//modified @haoye 14/10/15
void Logic::modifyInfo(Modify toModify) {
	std::vector<Task>::iterator currIter;
	std::vector<Task>::iterator taskIter;
	Task tempTask = toModify.getTempTask();

	int index = toModify.getModifyID();
	matchIndex(index,currIter,taskIter);

	std::vector<FieldType> tempField = toModify.getFieldsToModify();
	std::vector<FieldType>::iterator fieldIter;

	for (fieldIter = tempField.begin(); fieldIter != tempField.end(); ++fieldIter) {
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

// Searches name for a phrase match, returns IDs of all matching tasks
std::string Logic::searchInfo(Search toSearch) {
	std::ostringstream indexString;
	std::string searchPhrase;
	std::string taskName;
	std::string returnString;
	int id;

	searchPhrase = toSearch.getSearchPhrase();
	std::vector<Task>::iterator iter;

	for (iter = taskStore.begin(); iter != taskStore.end(); ++iter) {
		taskName = iter->getName();
		if (Utilities::isSubstring(searchPhrase,taskName)) {
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
bool Logic::amendView(std::string listOfIds) {
	std::string idToken;
	int id;
	int index;
	std::vector<Task>::iterator iter;

	currentView.clear();

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
		for (iter = taskStore.begin() ; iter != taskStore.end(); ++iter) {
			if (id == iter->getID()) {
				currentView.push_back(*iter);
			}
		}
	}
	return true;
}

Feedback Logic::processCommand(std::string userCommand) {
	//========== FOR UI PURPOSE ==========
	Feedback feedback;				
	bool isFound = true; 
	//====================================

	Command*	command;
	Add*		addTask;
	Delete*		taskToDelete;
	Modify*		taskToModify;
	Search*		searchPhrase;
	View*		tasksToView;
	Markdone*	taskToMarkDone;
	std::string newFilePath;

	// For temporary method to return string of names followed by commas	
	std::ostringstream tempOutput;
	std::vector<Task>::iterator iter;
	std::string convertStr;
	std::string output;

	try {
		command = parser.parse(userCommand);
	} catch(std::exception e) {
		feedback.setErrorMessage(e.what());
	}

	// cmd obtained from command (Aaron)
	CommandType cmd = command->getCommand();

	switch (cmd) {
	case DISPLAY_ALL:
		currentView = taskStore;
		feedback.setUpdateView(true);
		break;

	case ADD:
		addTask = ((Add*)command);
		addInfo(*addTask);
		feedback.pushTask(addTask->getNewTask());
		feedback.setAddedMessage();
		break;

	case DELETE: 
		// userIndex refers to the nth task of currentView presented to user
		// eg. delete 1 means deleting the first task
		taskToDelete = ((Delete*)command);
		try {	
			deleteInfo(*taskToDelete);
			feedback.setUpdateView(true);
		} catch (std::exception e) {
			feedback.setErrorMessage(e.what());
		}
		break;

	case MODIFY:
		taskToModify = ((Modify*)command);
		try {
			modifyInfo(*taskToModify);
		} catch (std::exception e) {
			feedback.setErrorMessage(e.what());
		}
		feedback.setUpdateView(true);
		break;

	case SEARCH:
		// Currently search returns string of names
		// currentView is also amended, can refer to items from currentView after every processInfo cmd
		// eg. add/delete will display new list under UI
		// If it is unnecessary info for add/delete, will change output of processInfo to vector<Task>
		searchPhrase = ((Search*)command);
		output = searchInfo(*searchPhrase);
		amendView(output);
		if(output.empty()) {
			isFound = false;
		}
		feedback.setSearchMessage(searchPhrase->getSearchPhrase(),isFound);
		feedback.setUpdateView(isFound);
		break;

	case VIEW:
		tasksToView = ((View*)command);
		break;

	case LOAD:
		break;

	case SAVE:
		// setFilePath returns false if unable to change file path
		newFilePath = parser.parseFileName(((Save*)command)->getFilePath());
		io.setFilePath(newFilePath,taskStore);
		// TODO: feedback
		break;

	case EXIT:
		feedback.setExit();
		break;

	case MARKDONE:
		taskToMarkDone = ((Markdone*)command);
		try {
			markDone(*taskToMarkDone);
			feedback.setUpdateView(true);
		} catch (std::exception e) {
			feedback.setErrorMessage(e.what());
		}	
		break;

	case UNDO:
		break;
	}

	saveFile(io.getFilePath());
	feedback.setTasksToShow(currentView);
	return feedback;
}

std::vector<Task> Logic::getFloatingTasks() {
	std::vector<Task>::iterator i;
	std::vector<Task> floatingTasks;
	for(i = taskStore.begin(); i<taskStore.end();++i) {
		if(i->getType() == FLOATING) {
			floatingTasks.push_back(*i);
		}
	}
	return floatingTasks;
}

bool Logic::viewTaskType(TaskType type) {
	currentView.clear();
	std::vector<Task>::iterator iter;
	
	for (iter = taskStore.begin(); iter != taskStore.end(); ++iter) {
		if (iter->getType() == type) {
			currentView.push_back(*iter);
		}
	}

	sortDate(currentView);
	return true;

}


//sorts in increasing order of dates (except for floating tasks, they are sorted to be at the bottom)
//should use this to sort according to date before display to UI
//since tasks with earliest deadlines/event should be seen first
bool Logic::sortDate(std::vector<Task> &taskVector) {

	std::vector<Task>::iterator i;
	std::vector<Task>::iterator j;
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

	//sorts floating tasks to be at the bottom
	for (i = taskVector.begin(); i != taskVector.end(); ++i) {
		if (i->getType() == FLOATING) {
			tempTask = *i;

			for (j = i+1; j != taskVector.end(); ++j) {
				std::swap(*j, *(j-1));
			}
			*(j-1) = tempTask;
		}
	}

	return true;
}



/* Keep for reference */
/*
int Logic::getSize(void) {
return sizeOfArray;
}

//swaps everything in an array element
void Logic::swapElement(textInfo& ElemA, textInfo& ElemB) {
std::string tempStr;
int tempInt;
tempStr = ElemA.text;
ElemA.text = ElemB.text;
ElemB.text = tempStr;

tempInt = ElemA.startDate;
ElemA.startDate = ElemB.startDate;
ElemB.startDate = tempInt;

tempInt = ElemA.startTime;
ElemA.startTime = ElemB.startTime;
ElemB.startTime = tempInt;

tempInt = ElemA.endDate;
ElemA.endDate = ElemB.endDate;
ElemB.endDate = tempInt;

tempInt = ElemA.endTime;
ElemA.endTime = ElemB.endTime;
ElemB.endTime = tempInt;

return;
}

//adds value to time while ensuring it still follows HHMM format
//assume addValue is lesser than 60
//**problem for case of 2359, change later (need to add day parameter)
int Logic::addTime(int time, int addValue) {
if (addValue < 60) {
int newTime;
int minutes;
int hours;

minutes = time % 100;
hours = time / 100;

minutes = minutes + addValue;

if (minutes >= 60) {
minutes = minutes - 60;
hours = hours + 1;
newTime = hours * 100 + minutes;
} else {
newTime = hours * 100 + minutes;
}

return newTime;
} else {
return 9999;						//error		
}
}

//copies infoArray into a given array
//for testing purposes
void Logic::copyArray(struct textInfo stringArray[]) {
for (int i = 0; i < sizeOfArray; i++) {
infoArray[i].text = stringArray[i].text;
infoArray[i].startDate = stringArray[i].startDate;
infoArray[i].startTime = stringArray[i].startTime;
infoArray[i].endDate = stringArray[i].endDate;
infoArray[i].endTime = stringArray[i].endTime;
}

}

void Logic::addInfo(std::string textInput, int startDateInput, int startTimeInput, 
int endDateInput, int endTimeInput) {

if (sizeOfArray < SIZE) {
infoArray[sizeOfArray].text = textInput;
infoArray[sizeOfArray].startDate = startDateInput;
infoArray[sizeOfArray].startTime = startTimeInput;
infoArray[sizeOfArray].endDate = endDateInput;
infoArray[sizeOfArray].endTime = endTimeInput;

sizeOfArray = sizeOfArray + 1;
}
return;
}

void Logic::deleteInfo(int index) {

if (index < sizeOfArray) {	
for (int i = index; i < sizeOfArray - 1; i++) {
infoArray[i].text = infoArray[i+1].text;
infoArray[i].startDate = infoArray[i+1].startDate;
infoArray[i].startTime = infoArray[i+1].startTime;
infoArray[i].endDate = infoArray[i+1].endDate;
infoArray[i].endTime = infoArray[i+1].endTime;
}
sizeOfArray = sizeOfArray - 1;
}

return;
}

//returns information stored in the array element that index points to
//information passed in order of text, startDate, startTime, endDate, endTime separated by "|" as of now
std::string Logic::returnInfo(int index) {

if (index < sizeOfArray){
std::ostringstream infoContainer;

infoContainer << infoArray[index].text << "|";
infoContainer << infoArray[index].startDate << "|";
infoContainer << infoArray[index].startTime << "|";
infoContainer << infoArray[index].endDate << "|";
infoContainer << infoArray[index].endTime;

return infoContainer.str();
} else {
return "Nothing to return.";
}
}

//sorts date and time in increasing order of startDate and startTime
//eg: 0th element will contain the earliest due date/time
void Logic::sortDateAndTime() {
if (sizeOfArray != 0) {
for (int i = 0; i < sizeOfArray; i++) {
for (int j = i+1; j < sizeOfArray; j++) {
if (infoArray[j].startTime < infoArray[i].startTime) {
swapElement(infoArray[i], infoArray[j]);
}
}
}

//sorts date after time
for (int i = 0; i < sizeOfArray; i++) {
for (int j = i+1; j < sizeOfArray; j++) {
if (infoArray[j].startDate < infoArray[i].startDate) {
swapElement(infoArray[i], infoArray[j]);
}
}
}
}
}

//changes information in a given array element
void Logic::changeInfo(std::string textInput, int startDateInput, int startTimeInput, 
int endDateInput, int endTimeInput, int index) {

//if text is not meant to be changed, textInput will be "*"
if (textInput != "*") {
infoArray[index].text = textInput;	
}

if (startDateInput != 999999) {
infoArray[index].startDate = startDateInput;
}

if (startTimeInput != 9999) {
infoArray[index].startTime = startTimeInput;
}

if (endDateInput != 999999) {
infoArray[index].endDate = endDateInput;
}

if (endTimeInput != 9999) {
infoArray[index].endTime = endTimeInput;
}

return;
}


//searches for the first available slot from input date and time onwards
std::string Logic::freeSlotSearch(int date, int time) {
std::ostringstream freeSlot;

if (sizeOfArray == 0) {
freeSlot << date << "|" << time;
return freeSlot.str();					
} else {
int freeDate = date;
int freeTime = time;

sortDateAndTime();

//if date and time is in between ith array element,
//move to next element, updating the possible free time to date and time right after
//ith array element
for (int i = 0; i < sizeOfArray; i++) {

//below condition checks if date and time is in between the duration of ith array element
if ((infoArray[i].startDate < freeDate) || 
((infoArray[i].startDate == freeDate) && (infoArray[i].startTime <= freeTime))) {
if ((infoArray[i].endDate > freeDate) ||
((infoArray[i].endDate == freeDate) && (infoArray[i].endTime >= freeTime))) {

//condition to prevent freeDate/freeTime from going back to the past
if ((freeDate < infoArray[i].endDate) ||
((freeDate == infoArray[i].endDate) && (freeTime < infoArray[i].endTime))) {
freeDate = infoArray[i].endDate;
freeTime = addTime(infoArray[i].endTime, 1);
}
} 
}
}

freeSlot << freeDate << "|" << freeTime;
return freeSlot.str();
}
}
*/
