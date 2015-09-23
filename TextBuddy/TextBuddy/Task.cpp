// @@author Aaron Chong Jun Hao

#include "stdafx.h"
#include "Task.h"

int Task::runningCount = 0;

Task::Task() {
	type = FLOATING;
	uniqueID = runningCount++;
	label = "";

	isDone = false;
	isPriority = false;

	startDay = MON;
	startDate = 0; // YYMMDD, supports 2015-2099
	startTime = 0; // HHMM, 24-hour format

	endDay = MON;
	endDate = 0;
	endTime = 0;
}

Task::~Task() {}

// Getters

int Task::getRunningCount() {
	return runningCount;
}

std::string Task::getName() {return name;}
TaskType Task::getType() {return type;}
int Task::getID() {return uniqueID;}
std::string Task::getLabel() {return label;}
bool Task::getDoneStatus() {return isDone;}
bool Task::getPriorityStatus() {return isPriority;}
Day Task::getStartDay() {return startDay;}
int Task::getStartDate() {return startDate;}
int Task::getStartTime() {return startTime;}
Day Task::getEndDay() {return endDay;}
int Task::getEndDate() {return endDate;}
int Task::getEndTime() {return endTime;}

// Setters, return true if successful
bool Task::setName(std::string restOfCommand) {
	name = restOfCommand;
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

bool Task::toggleDone() {
	isDone = !isDone;
	return true;
}

bool Task::togglePriority() {
	isPriority = !isPriority;
	return true;
}

bool Task::setStartDay(Day newStartDay) {
	startDay = newStartDay;
	return true;
}

bool Task::setStartDate(int newStartDate) {
	startDate = newStartDate;
	return true;
}

bool Task::setStartTime(int newStartTime) {
	startTime = newStartTime;
	return true;
}

bool Task::setEndDay(Day newEndDate) {
	endDate = newEndDate;
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