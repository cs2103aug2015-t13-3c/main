// @@author Ng Ren Zhi

#include "stdafx.h"
#include "IO.h"

IO::IO() {}
IO::~IO() {}

// ==================================================
//                      METHODS
// ==================================================

std::vector<Task> IO::loadFile(std::string fileName) {
	std::ifstream inputFile(fileName);

	std::vector<Task> taskVector;
	Task newTask;

	if(!fileIsOpen(inputFile)) {
		return taskVector;
	}

	while(!inputFile.eof()) {
		newTask = getTask(inputFile);
		taskVector.push_back(newTask);
	}

	inputFile.close();
	return taskVector;
}

bool IO::saveFile(std::string fileName, std::vector<Task> taskVector) {
	remove(fileName.c_str());

	std::ofstream newfile(fileName);

	if(!fileIsOpen(newfile)) {
		return false;
	}

	for(unsigned int i = 0; i < taskVector.size(); i++) {
		Task task = taskVector[i];

		std::string taskName = task.getName();
		std::string taskType = u.taskTypeToString(task.getType());
		std::string taskID = std::to_string(task.getID());
		std::string taskLabel = task.getLabel();
		std::string taskDoneStatus = u.boolToIntString(task.getDoneStatus());
		std::string taskPriorityStatus = u.boolToIntString(task.getPriorityStatus());
		std::string taskStartDay = u.dayToString(task.getStartDay());
		std::string taskStartDate = std::to_string(task.getStartDate());
		std::string taskStartTime = std::to_string(task.getStartTime());
		std::string taskEndDay = u.dayToString(task.getEndDay());
		std::string taskEndDate = std::to_string(task.getEndDate());
		std::string taskEndTime = std::to_string(task.getEndTime());

		std::string taskText = taskName + "\n" + taskType + "\n" + taskID + "\n" + taskLabel + "\n" + taskDoneStatus
			+ "\n" + taskPriorityStatus + "\n" + taskStartDay + "\n" + taskStartDate + "\n" + taskStartTime + "\n" + 
			taskEndDay + "\n" + taskEndDate + "\n" + taskEndTime + "\n";

		newfile << taskText;
	}
	newfile.close();

	return false;
}

Task IO::getTask(std::ifstream& inputFile) {
	Task task;
	bool success = true;
	std::string line;

	getline(inputFile,line);
	success = task.setName(line);
	getline(inputFile,line);
	success = task.setType(u.stringToTaskType(line));
	getline(inputFile,line);
	success = task.setID(stoi(line));
	getline(inputFile,line);
	success = task.setLabel(line);

	/*
	bool toggleDone();
	bool togglePriority();
	*/

	getline(inputFile,line);
	success = task.setStartDay(u.stringToDay(line));
	getline(inputFile,line);
	success = task.setStartDate(stoi(line));
	getline(inputFile,line);
	success = task.setStartTime(stoi(line));

	getline(inputFile,line);
	success = task.setEndDay(u.stringToDay(line));
	getline(inputFile,line);
	success = task.setEndDate(stoi(line));
	getline(inputFile,line);
	success = task.setEndTime(stoi(line));

	return task;
}


// Overloaded function
bool IO::fileIsOpen(std::ifstream& inputFile)
{
	if(inputFile.is_open())	{
		return true;
	}
	else {
		return false;
	}
}

bool IO::fileIsOpen(std::ofstream& outputFile)
{
	if(outputFile.is_open())	{
		return true;
	}
	else {
		return false;
	}
}

enum Enum { Banana, Orange, Apple } ;
static const char * EnumStrings[] = { "bananas & monkeys", "Round and orange", "APPLE" };

const char * getTextForEnum( int enumVal )
{
	return EnumStrings[enumVal];
}
