// Chin Kiat Boon @@author A0096720A

#include "stdafx.h"
#include "Logic.h"

const std::string Logic::ERROR_NO_INPUT = "No input";
const std::string Logic::ERROR_INVALID_COMMAND = "Invalid command entered";

Logic* Logic::theOne = new Logic();

DisplayMode Logic::mode=TODAY;

Logic::Logic() {
	history = History::getInstance();
	parser = Parser::getInstance();
	io = IO::getInstance();
	Command temp;
	currentView = temp.getCurrentViewPtr();
	updater = nullptr;
}

Logic::~Logic() {
	updater->~Update();
	delete updater;
}

//==================================================
//                      METHODS
//==================================================

Logic* Logic::getInstance() {
	Load initialLoad(IO::getInstance()->getFilePath());
	try {
		initialLoad.execute();
	} catch (std::exception e) {
		TbLogger::getInstance()->log(WARN,"File not found: " + IO::getInstance()->getFilePath());
	}
	return theOne;
}

std::string Logic::processCommand(std::string userCommand) {
	if(userCommand.empty()) {
		throw std::runtime_error(ERROR_NO_INPUT);
	}
	std::string message;
	Command* command;
	command = parser->parse(userCommand);
	CommandType cmd = command->getCommand();

	switch (cmd) {
	case INVALID:
		throw std::runtime_error(ERROR_INVALID_COMMAND);
	case UNDO:
	case REDO:
		command->execute();
		break;
	default:
		command->execute();
		message = command->getMessage();
		history->add(command);
	}

	assert(updater != nullptr);
	updater->update();
	io->saveFile(io->getFilePath(),Command::getTaskStore());
	// Save saveFile;
	// saveFile.execute();
	return message;
}

void Logic::subscribe(std::vector<DisplayedTask>* tasks) {

						  assert(updater == nullptr);
						  updater = new Update(tasks,currentView);
						  updater->update();
}

DisplayMode Logic::getMode() {
	return mode;
}

void Logic::setAllMode() {
	mode = ALL;
}

void Logic::setTodayMode() {
	mode = TODAY;
}

void Logic::setWeekMode() {
	mode = WEEK;
}

void Logic::setEventsMode() {
	mode = EVENTS;
}

void Logic::setDeadlinesMode() {
	mode = DEADLINES;
}

void Logic::setFloatingMode() {
	mode = FLOATINGS;
}

void Logic::setSearchMode() {
	mode = SEARCHES;
}

void Logic::setPastMode() {
	mode = PAST_;
}

void Logic::resetUpdaterNULL() {
	updater = nullptr;
}

// Chin Kiat Boon @@author A0096720A-unused
/*
Add* add;
Delete* delet;
Modify* modify;
Search* search;
Markdone* markdone;
UnmarkDone* unmarkdone;
View* view;
// Clearall* clearAll; // ClearAll subclass not created yet
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

/*
bool Logic::markStar(Star toMarkStar){
int userIndex;
int id;
std::vector<Task>::iterator iter;

userIndex = toMarkDone.getDoneID();

try {
id = getIdOfIndex(userIndex);
if(id == -1) {												// Error code
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

// Swaps everything in an array element
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

// Adds value to time while ensuring it still follows HHMM format
// Assume addValue is lesser than 60
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
return 9999;						// Error		
}
}

// Copies infoArray into a given array
// For testing purposes
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

// Returns information stored in the array element that index points to
// Information passed in order of text, startDate, startTime, endDate, endTime separated by "|" as of now
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

// Sorts date and time in increasing order of startDate and startTime
// eg: 0th element will contain the earliest due date/time
void Logic::sortDateAndTime() {
if(sizeOfArray != 0) {
for (int i = 0; i < sizeOfArray; i++) {
for (int j = i+1; j < sizeOfArray; j++) {
if(infoArray[j].startTime < infoArray[i].startTime) {
swapElement(infoArray[i], infoArray[j]);
}
}
}

// Sorts date after time
for (int i = 0; i < sizeOfArray; i++) {
for (int j = i+1; j < sizeOfArray; j++) {
if(infoArray[j].startDate < infoArray[i].startDate) {
swapElement(infoArray[i], infoArray[j]);
}
}
}
}
}

// Changes information in a given array element
void Logic::changeInfo(std::string textInput, int startDateInput, int startTimeInput, 
int endDateInput, int endTimeInput, int index) {

// If text is not meant to be changed, textInput will be "*"
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

// Searches for the first available slot from input date and time onwards
std::string Logic::freeSlotSearch(int date, int time) {
std::ostringstream freeSlot;

if(sizeOfArray == 0) {
freeSlot << date << "|" << time;
return freeSlot.str();					
} else {
int freeDate = date;
int freeTime = time;

sortDateAndTime();

// If date and time is in between ith array element, move to next element,
// updating the possible free time to date and time right after ith array element
for (int i = 0; i < sizeOfArray; i++) {

// Below condition checks if date and time is in between the duration of ith array element
if((infoArray[i].startDate < freeDate) || 
((infoArray[i].startDate == freeDate) && (infoArray[i].startTime <= freeTime))) {
if((infoArray[i].endDate > freeDate) ||
((infoArray[i].endDate == freeDate) && (infoArray[i].endTime >= freeTime))) {

// Condition to prevent freeDate/freeTime from going back to the past
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
