// Aaron Chong Jun Hao @@author A0110376N

#include "stdafx.h"

int Task::runningCount = 0;
int Task::lastEditID = 0;

Task::Task() {
	name = "";
	type = FLOATING;
	uniqueID = 0;
	labels = std::set<std::string>();

	isDone = false;
	isPriority = false;

	startDate = DATE_NOT_SET;	// YYMMDD, supports 2015-2099
	startTime = TIME_NOT_SET;	// HHMM, 24-hour format
	endDate = DATE_NOT_SET;
	endTime = TIME_NOT_SET;

	reserveStartDate = std::set<int>();
	reserveStartTime = std::set<int>();
	reserveEndDate = std::set<int>();
	reserveEndTime = std::set<int>();
}

Task::~Task() {}

//========== Static ==========

int Task::getRunningCount() {
	return runningCount;
}

void Task::setRunningCount(int lastCount) {
	runningCount = lastCount;
}

int Task::incrementRunningCount() {
	return ++runningCount;
}

//========== Getters ==========

std::string Task::getName() {
	return name;
}

TaskType Task::getType() {
	return type;
}

int Task::getID() {
	return uniqueID;
}

std::vector<std::string> Task::getLabelsToDelete() {
	return labelsToDelete;
}

bool Task::getDoneStatus() {
	return isDone;
}

bool Task::getPriorityStatus() {
	return isPriority;
}

int Task::getStartDate() {
	return startDate;
}

int Task::getStartTime() {
	return startTime;
}

int Task::getEndDate() {
	return endDate;
}

int Task::getEndTime() {
	return endTime;
}

TaskType Task::getReserveType() {
	return reserveType;
}

int Task::getReserveStartDate() {
	if (reserveStartDate.empty()) {
		return DATE_NOT_SET;
	}
	int date = *(reserveStartDate.begin());
	return date;
}

int Task::getReserveStartTime() {
	if (reserveStartTime.empty()) {
		return TIME_NOT_SET;
	}
	int time = *(reserveStartTime.begin());
	return time;
}

int Task::getReserveEndDate() {
	if (reserveEndDate.empty()) {
		return DATE_NOT_SET;
	}
	int date = *(reserveEndDate.begin());
	return date;
}

int Task::getReserveEndTime() {
	if (reserveEndTime.empty()) {
		return TIME_NOT_SET;
	}
	int time = *(reserveEndTime.begin());
	return time;
}

bool Task::getReserveStatus() {
	if (   !reserveStartDate.empty()
		|| !reserveStartTime.empty()
		|| !reserveEndDate.empty()
		|| !reserveEndTime.empty()) {
			return true;
	}
	return false;
}

void Task::setReserveType(TaskType newType) {
	reserveType = newType;
	return;
}

void Task::addReserveStartDate(int newReservation) {
	reserveStartDate.clear();
	reserveStartDate.insert(newReservation);
	return;
}

void Task::addReserveStartTime(int newReservation) {
	reserveStartTime.clear();
	reserveStartTime.insert(newReservation);
	return;
}

void Task::addReserveEndDate(int newReservation) {
	reserveEndDate.clear();
	reserveEndDate.insert(newReservation);
	return;
}

void Task::addReserveEndTime(int newReservation) {
	reserveEndTime.clear();
	reserveEndTime.insert(newReservation);
	return;
}

void Task::pickReserve() {
	startDate = getReserveStartDate();
	startTime = getReserveStartTime();
	endDate = getReserveEndDate();
	endTime = getReserveEndTime();
	return;
}

void Task::clearReserve() {
	reserveStartDate.clear();
	reserveStartTime.clear();
	reserveEndDate.clear();
	reserveEndTime.clear();
	return;
}

std::vector<std::string> Task::getLabels() {
	std::vector<std::string> labelVector;
	std::set<std::string>::iterator i = labels.begin();
	while (i != labels.end()) {
		labelVector.push_back(*i);
		++i;
	}
	return labelVector;
}

std::string Task::getLabelString() {
	std::string label;
	std::set<std::string>::iterator i = labels.begin();
	while (i != labels.end()) {
		label = label + *i + " ";
		++i;
	}
	if (!label.empty()) {
		label.pop_back();
	}
	return label;
}

std::string Task::getDisplayDate() {
	std::string date;
	if (startDate == DATE_NOT_SET) {
		date = "";
	} else if (startDate == endDate) {
		date = Utilities::toDisplayDate(startDate);
	} else if (startDate != endDate) {
		date = Utilities::toDisplayDate(startDate) + " - " + Utilities::toDisplayDate(endDate);
	}

	if (getReserveStatus() == true) {
		date += "\n";
		if (reserveStartDate == reserveEndDate) {
			date += Utilities::toDisplayDate(getReserveStartDate());
		} else {
			date += Utilities::toDisplayDate(getReserveStartDate()) + " - " +
				Utilities::toDisplayDate(getReserveEndDate());
		}
	}
	return date;
}

std::string Task::getDisplayTime() {
	std::string time;
	if (endTime == TIME_NOT_SET) {
		time = "";
	} else if (startTime == TIME_NOT_SET || startTime == endTime) {
		time = Utilities::toDisplayTime(endTime);
	} else if (startTime != endTime) {
		time = Utilities::toDisplayTime(startTime) + " - " + Utilities::toDisplayTime(endTime);;
	}

	if (getReserveStatus() == true) {
		time += "\n";
		if (reserveStartTime == reserveEndTime) {
			time += Utilities::toDisplayTime(getReserveStartTime());
		} else {
			time += Utilities::toDisplayTime(getReserveStartTime()) + " - " +
				Utilities::toDisplayTime(getReserveEndTime());
		}
	}
	return time;
}

//========== Setters ==========
// Return true if successful

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

bool Task::addLabels(std::vector<std::string> newLabels) {
	std::vector<std::string>::iterator curr;
	for (curr=newLabels.begin(); curr!=newLabels.end(); ++curr) {
		labels.insert(*curr);
	}
	return true;
}

bool Task::deleteLabels(std::vector<std::string> badLabels) {
	std::vector<std::string>::iterator badCurr;
	std::set<std::string>::iterator labelsCurr;
	for (badCurr=badLabels.begin(); badCurr!=badLabels.end(); ++badCurr) {
		for (labelsCurr=labels.begin();labelsCurr!=labels.end(); ++labelsCurr) {
			if (Utilities::equalsIgnoreCase(*badCurr,*labelsCurr)) {
				labelsCurr = labels.erase(labelsCurr);
				if (labelsCurr == labels.end()) {
					break;
				}
			}
		}
	}
	return true;
}

bool Task::setLabelsToDelete(std::vector<std::string> oldLabels) {
	labelsToDelete = oldLabels;
	return true;
}

bool Task::clearLabels() {
	if (labels.empty()) {
		return false;
	}
	labels.clear();
	return true;
}

bool Task::markDone() {
	bool currDone = !isDone;
	isDone = true;
	return currDone;
}

bool Task::unmarkDone() {
	bool currDone = isDone;
	isDone = false;
	return currDone;
}

bool Task::setPriority() {
	return isPriority != (isPriority=true);
}

bool Task::unsetPriority() {
	return isPriority == (isPriority=false);;
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

void Task::resetDatesAndTimes() {
	startDate = DATE_NOT_SET;
	startTime = TIME_NOT_SET;
	endDate = DATE_NOT_SET;
	endTime = TIME_NOT_SET;
	return;
}

bool Task::isUrgent() {
	if (type == FLOATING) {
		return false;
	}

	int day	  = startDate % 100;
	int month = (startDate % 10000)/100;
	int year  = startDate/10000;

	struct tm now;
	time_t t = time(nullptr);
	localtime_s(&now,&t);

	struct tm taskDate = {0,0,0,day,month-1,year+100}; 
	std::time_t a = std::mktime(&taskDate);
	std::time_t b = std::mktime(&now);

	int difference = std::difftime(a, b) / (60 * 60 * 24);
	if((type == TODO && difference < 4)) {
		return true;
	} 
	return false;
}

bool Task::isToday() {
	int currentDay = Utilities::getLocalDay();
	int currentMonth = Utilities::getLocalMonth();
	int currentYear = Utilities::getLocalYear();

	int day = startDate % 100;
	int month = (startDate % 10000)/100;
	int year = startDate/10000;

	if (day == currentDay && month == currentMonth && year == currentYear) {
		return true;
	} 
	return false;
}

// For testing
bool Task::tasksAreEqual(Task task1, Task task2) {
	if (    (task1.getDoneStatus() != task2.getDoneStatus())
		|| (task1.getEndDate() != task2.getEndDate())
		|| (task1.getEndTime() != task2.getEndTime())
		|| (task1.getID() != task2.getID())
		|| (task1.getLabelString() != task2.getLabelString())
		|| (task1.getName() != task2.getName())
		|| (task1.getPriorityStatus() != task2.getPriorityStatus())
		|| (task1.getStartDate() != task2.getStartDate())
		|| (task1.getStartTime() != task2.getStartTime())
		|| (task1.getType() != task2.getType())) {
			return false;
	}
	return true;
}
