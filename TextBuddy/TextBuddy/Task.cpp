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

bool Task::setName(std::string restOfCommand) {
	name = restOfCommand;
	return true;
}

std::string Task::getName() {
	return name;
}