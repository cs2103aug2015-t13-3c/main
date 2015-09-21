// @@author Aaron Chong Jun Hao

#include "stdafx.h"
#include "Task.h"

Task::Task() {}
Task::~Task() {}

void Task::setTaskName(std::string restOfCommand) {
	taskName = restOfCommand;
	return;
}

std::string Task::getTaskName() {
	return taskName;
}