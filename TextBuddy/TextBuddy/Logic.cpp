// @@author Chin Kiat Boon

#include "stdafx.h"
#include "Logic.h"
#include "History.h"

const std::string Logic::ERROR_INDEX_OUT_OF_BOUNDS = "invalid index";

Logic* Logic::theOne = new Logic();

Logic::Logic() {
	history = History::getInstance();
	Load initialLoad(io.getFilePath());
	initialLoad.execute();
	/*
	std::vector<Task>::iterator i;
	for(i=taskStore.begin() ; i!=taskStore.end(); ++i) {
	std::string dateAndTime_UI = Utilities::taskDateAndTimeToDisplayString(*i);
	i->setDateAndTime_UI(dateAndTime_UI);
	}
	*/
}

Logic::~Logic() {}

// ==================================================
//                      METHODS
// ==================================================

//added @RenZhi 16/10/15
Logic* Logic::getInstance() {
	return theOne;
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//modified @haoye 14/10/15
/*
bool Logic::markStar(Star toMarkStar){
int userIndex;
int id;
std::vector<Task>::iterator iter;

userIndex = toMarkDone.getDoneID();

try {
id = getIdOfIndex(userIndex);
if(id == -1) {												//error code
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

if(iter->getID() == id) {
iter->setPriority();
}

copyView();

return true;
}
*/

//modified @RenZhi 19/10/15
Feedback Logic::processCommand(std::string userCommand) {
	Feedback feedback;

	Command* command;
	Add* add;
	Delete* delet;
	Modify* modify;
	Search* search;
	Markdone* markdone;
	UnmarkDone* unmarkdone;
	View* view;
	//Clearall* clearAll; //ClearAll subclass not created yet
	DisplayAll* displayAll;
	Load* load;
	Save* save;
	Exit* exit;

	try {
		command = parser.parse(userCommand);
	} catch(std::exception e) {
		feedback.setErrorMessage(e.what());
	}

	CommandType cmd = command->getCommand();
	switch (cmd) {
	case UNDO:
		history->undo();
		break;

	case REDO:
		history->redo();
		break;

	case INVALID:
		throw "INVALID COMMAND ENTERED";
		break;

	default:
		command->execute();
		history->add(*command);
	}

	Save saveFile;
	saveFile.execute();
	feedback.setTasksToShow(command->getCurrentView());
	return feedback;
}

/*
Add* add;
Delete* delet;
Modify* modify;
Search* search;
Markdone* markdone;
UnmarkDone* unmarkdone;
View* view;
//Clearall* clearAll; //ClearAll subclass not created yet
DisplayAll* displayAll;
Load* load;
Save* save;
Exit* exit;

try {
command = parser.parse(userCommand);
} catch(std::exception e) {
feedback.setErrorMessage(e.what());
}

CommandType cmd = command->getCommand();
switch (cmd) {
case ADD:
add = (Add*)command;
add->execute();
history->add(*command);
break;
case DELETE:
case MODIFY:
case SEARCH:
case MARKDONE:
case UNMARKDONE:
case UNDO:
history->undo();
break;
case REDO:
history->redo();
break;
case VIEW:
case CLEAR_ALL:
case DISPLAY_ALL:
case LOAD:
case SAVE:
case EXIT:
case INVALID:
throw "INVALID COMMAND ENTERED";
break;

default:
command->execute();
history->add(*command);
}

*/


// CURRENTLY NOT USED
/*
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
if(iter->getType() == type) {
currentView.push_back(*iter);
}
}

sortDate(currentView);
return true;

}
*/




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
if(addValue < 60) {
int newTime;
int minutes;
int hours;

minutes = time % 100;
hours = time / 100;

minutes = minutes + addValue;

if(minutes >= 60) {
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

if(sizeOfArray < SIZE) {
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

if(index < sizeOfArray) {	
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

if(index < sizeOfArray){
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
if(sizeOfArray != 0) {
for (int i = 0; i < sizeOfArray; i++) {
for (int j = i+1; j < sizeOfArray; j++) {
if(infoArray[j].startTime < infoArray[i].startTime) {
swapElement(infoArray[i], infoArray[j]);
}
}
}

//sorts date after time
for (int i = 0; i < sizeOfArray; i++) {
for (int j = i+1; j < sizeOfArray; j++) {
if(infoArray[j].startDate < infoArray[i].startDate) {
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
if(textInput != "*") {
infoArray[index].text = textInput;	
}

if(startDateInput != 999999) {
infoArray[index].startDate = startDateInput;
}

if(startTimeInput != 9999) {
infoArray[index].startTime = startTimeInput;
}

if(endDateInput != 999999) {
infoArray[index].endDate = endDateInput;
}

if(endTimeInput != 9999) {
infoArray[index].endTime = endTimeInput;
}

return;
}


//searches for the first available slot from input date and time onwards
std::string Logic::freeSlotSearch(int date, int time) {
std::ostringstream freeSlot;

if(sizeOfArray == 0) {
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
if((infoArray[i].startDate < freeDate) || 
((infoArray[i].startDate == freeDate) && (infoArray[i].startTime <= freeTime))) {
if((infoArray[i].endDate > freeDate) ||
((infoArray[i].endDate == freeDate) && (infoArray[i].endTime >= freeTime))) {

//condition to prevent freeDate/freeTime from going back to the past
if((freeDate < infoArray[i].endDate) ||
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
