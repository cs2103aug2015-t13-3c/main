// @@author Aaron Chong Jun Hao

#include "stdafx.h"

int Task::runningCount = 0;

Task::Task() {
	name = "";
	type = FLOATING;
	uniqueID = 0;
	label = "";

	isDone = false;
	isPriority = false;

	startDate = 0; // YYMMDD, supports 2015-2099
	startTime = 0; // HHMM, 24-hour format

	endDate = 0;
	endTime = 0;
}

Task::~Task() {}


// Static

int Task::getRunningCount() {
	return runningCount;
}

void Task::setRunningCount(int lastCount) {
	runningCount = lastCount;
}

int Task::incrementRunningCount() {
	return ++runningCount;
}


// Getters

std::string Task::getName() {return name;}
TaskType Task::getType() {return type;}
int Task::getID() {return uniqueID;}
std::string Task::getLabel() {return label;}
bool Task::getDoneStatus() {return isDone;}
bool Task::getPriorityStatus() {return isPriority;}
int Task::getStartDate() {return startDate;}
int Task::getStartTime() {return startTime;}
int Task::getEndDate() {return endDate;}
int Task::getEndTime() {return endTime;}


// Setters, return true if successful

bool Task::setName(std::string newName) {
	name = newName;
	return true;
}

bool Task::setType(TaskType newType) {
	type = newType;
	return true;
}

bool Task::setID(int newID) {
	uniqueID = newID;
	return true;
}

bool Task::setLabel(std::string newLabel) {
	label = newLabel;
	return true;
}

bool Task::addLabels(std::vector<std::string> newLabels) {
	std::vector<std::string>::iterator curr;
	for(curr=newLabels.begin(); curr!=newLabels.end(); curr++) {
		labels.insert(*curr);
	}
	return true;
}

bool Task::deleteLabels(std::vector<std::string> badLabels) {
	std::vector<std::string>::iterator badCurr;
	std::set<std::string>::iterator labelsCurr;
	for(badCurr=badLabels.begin(); badCurr!=badLabels.end(); badCurr++) {
		for(labelsCurr=labels.begin();labelsCurr!=labels.end(); labelsCurr++) {
			if(Utilities::equalsIgnoreCase(*badCurr,*labelsCurr)) {
				labelsCurr = labels.erase(labelsCurr);
			}
		}
	}
	return true;
}

bool Task::markDone() {
	return isDone != (isDone=true);;
}

bool Task::unmarkDone() {
	return isDone != (isDone=false);;
}

bool Task::setPriority() {
	return isPriority != (isPriority=true);;
}

bool Task::unsetPriority() {
	return isPriority != (isPriority=false);;
}

bool Task::setStartDate(int newStartDate) {
	startDate = newStartDate;
	return true;
}

bool Task::setStartTime(int newStartTime) {
	startTime = newStartTime;
	return true;
}

bool Task::setEndDate(int newEndDate) {
	endDate = newEndDate;
	return true;
}

bool Task::setEndTime(int newEndTime) {
	endTime = newEndTime;
	return true;
}

std::set<std::string> Task::getLabels() {
	return labels;
}

void Task::setDateAndTime_UI(std::string dateAndTime_UI) {
	this->dateAndTime_UI = dateAndTime_UI;
}

std::string Task::getDateAndTime_UI() {
	return dateAndTime_UI;
}

// For testing
bool Task::tasksAreEqual(Task task1, Task task2) {
	if(    (task1.getDoneStatus() != task2.getDoneStatus())
		|| (task1.getEndDate() != task2.getEndDate())
		|| (task1.getEndTime() != task2.getEndTime())
		|| (task1.getID() != task2.getID())
		|| (task1.getLabel() != task2.getLabel())
		|| (task1.getName() != task2.getName())
		|| (task1.getPriorityStatus() != task2.getPriorityStatus())
		|| (task1.getStartDate() != task2.getStartDate())
		|| (task1.getStartTime() != task2.getStartTime())
		|| (task1.getType() != task2.getType())) {
			return false;
	}
	return true;
}
