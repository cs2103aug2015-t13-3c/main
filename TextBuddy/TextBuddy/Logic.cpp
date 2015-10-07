// @@author Chin Kiat Boon

#include "stdafx.h"
#include "Logic.h"

Logic::Logic() {}
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

bool Logic::loadFile(std::string fileName) {
	taskStore = io.loadFile(fileName);
	copyView();
	return true;
}

std::vector<Task> Logic::getTaskStore() {
	return taskStore;
}

std::vector<Task> Logic::getCurrentView() {
	return currentView;
}

// For now, currentView is set to be the same as taskStore
bool Logic::copyView() {
	currentView = taskStore;
	return true;
}

bool Logic::matchPhrase(std::string phr, std::string str) {
	int j;
	int k;
	int strSize;
	
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	std::transform(phr.begin(), phr.end(), phr.begin(), ::tolower);
	
	strSize = str.size();

	for (int i = 0; i < strSize; i++) {
		j = i;
		k = 0;
		if (str[j] == phr[k]) {
			while (str[j] == phr[k]) {
				j++;
				k++;

				if (k == phr.size()) {
					return true;
				}
			}
		}
	}
	return false;
}

// Returns the uniqueID of the task pointed to by userIndex
// For deleting taskStore by referring to tasks in currentView
int Logic::getIdOfIndex(int userIndex) {
	int id;
	std::vector<Task>::iterator iter = currentView.begin();

	for (int i = 1; i < userIndex; i++) {
		if (iter != currentView.end()) {
				++iter;
		} else {
			return false;				//error since index put in exceeds the currentView capacity
		}								//error for exceeding upper bound of acceptable inputs
	}

	if (userIndex >= 1) {
		id = iter->getID();
	} else {
		return false;					//error for index going below the lower bound of acceptable inputs
	}
	return id;
}

bool Logic::addInfo(Add taskName) {
	taskStore.push_back(taskName.getNewTask());
	copyView();
	return true;
}

// Searches for Task to delete using ID
// Deleting is done according to the order of elements on currentView
bool Logic::deleteInfo(Delete idToDelete) {
	std::vector<Task>::iterator iter;
	int id;
	int index;
	index = idToDelete.getDeleteID();
	id = getIdOfIndex(index);

	iter = taskStore.begin();
	while ((iter != taskStore.end()) && (iter->getID() != id)) {
		++iter;
	}
	
	if (iter->getID() == id) {
		taskStore.erase(iter);
	}

	copyView();
	return true;
}

bool Logic::modifyInfo(Modify toModify) {
	std::vector<Task>::iterator taskIter;
	Task tempTask = toModify.getTempTask();
	
	int index = toModify.getModifyID();
	int id;
	id = getIdOfIndex(index);
	
	//matches id obtained from currentView with id in taskstore
	taskIter = taskStore.begin();
	while ((taskIter != taskStore.end()) && (taskIter->getID() != id)) {
		++taskIter;
	}

	if (taskIter->getID() == id) {
		std::vector<FieldType> tempField = toModify.getFieldsToModify();
		std::vector<FieldType>::iterator fieldIter;

		for (fieldIter = tempField.begin(); fieldIter != tempField.end(); ++fieldIter) {
			switch (*fieldIter) {
			case NAME :
				taskIter->setName(tempTask.getName());
				break;
			case START_DATE :
				taskIter->setStartDate(tempTask.getStartDate());
				break;
			case END_DATE :
				taskIter->setEndDate(tempTask.getEndDate());
				break;
			case START_DAY:
				taskIter->setStartDay(tempTask.getStartDay());
				break;
			case END_DAY :
				taskIter->setEndDay(tempTask.getEndDay());
				break;
			case START_TIME :
				taskIter->setStartTime(tempTask.getStartTime());
				break;
			case END_TIME :
				taskIter->setEndTime(tempTask.getEndTime());
				break;
			default:
				std::cout << "Error in fetching field name" << std::endl;
				break;
			}
			copyView();
		}
		return true;
	} else {
		return false;
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

		if (matchPhrase(searchPhrase, taskName) == true) {
			id = iter->getID();
			indexString << id << ",";
		}
	}

	returnString = indexString.str();
	returnString.pop_back();
	return returnString;
}

//the list is separated by commas
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

// Input command is obtained from parseCommand
// Returns string of IDs with search, returns "*" for add/delete
Feedback Logic::processCommand(std::string userCommand) {
	//inputCmd obtained from parser
	Command inputCmd(parser.parseCommand(userCommand));
	CommandType cmd = inputCmd.getCommand();
	Add* addTask;
	Delete* taskToDelete;
	Modify* taskToModify;
	Search* searchPhrase;
	Feedback feedback;
	std::string output;

	int userIndex;						//userIndex in currentView
	int id;								//id for both currentView and taskStore
	
	//for temporary method to return string of names followed by commas	
	std::ostringstream tempOutput;
	std::vector<Task>::iterator iter;

	Command* command = parser.parse(userCommand);

	switch (cmd) {

	case ADD:
		addTask = ((Add*)command);
		addInfo(*addTask);
		feedback.pushTask(addTask->getNewTask());
		feedback.setAddedMessage();
		break;
	case DELETE:
		//userIndex refers to the nth task of currentView presented to user
		//eg. delete 1 means deleting the first task
		taskToDelete = ((Delete*)command);
		deleteInfo(*taskToDelete);
		break;
	case MODIFY:
		taskToModify = ((Modify*)command);
		modifyInfo(*taskToModify);
		break;
	case SEARCH:
		//currently search returns string of names
		//currentView is also amended, can refer to items from currentView after every processInfo cmd
		//eg. add/delete will display new list under UI
		//if it is unnecessary info for add/delete, will change output of processInfo to vector<Task>
		searchPhrase = ((Search*)command);
		output = searchInfo(*searchPhrase);
		amendView(output);

		//temporary method to return string of names followed by commas
		for (iter = currentView.begin(); iter != currentView.end(); ++iter) {
			tempOutput << iter->getName() << ",";
		}
		
		output = tempOutput.str();
		output.erase(output.size()-1);

		feedback.setSearchMessage(searchPhrase->getSearchPhrase());
		feedback.setUpdateView(true);
		break;
	default:
		break;
	}
	feedback.setTasksToShow(currentView);
	return feedback;
}

/* Keep for reference */
/*
bool Logic::getStatus() {
return isActive && canCallIO();
}

bool Logic::canCallIO()
{
return io.getStatus();
}

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
