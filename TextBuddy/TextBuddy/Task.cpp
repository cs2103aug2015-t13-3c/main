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
		
	day = MON;
	dateStart = 0; // YYMMDD, supports 2015-2099
	dateEnd = 0;
	timeFrom = 0;  // HHMM, 24-hour format
	timeTo = 0;    // HHMM, 24-hour format
}

Task::~Task() {}

void Task::setName(std::string restOfCommand) {
	name = restOfCommand;
	return;
}

std::string Task::getName() {
	return name;
}