// @@author Ng Ren Zhi

#include "stdafx.h"
#include "IO.h"
#include "Rapidjson\include\rapidjson\document.h"

using namespace rapidjson;

IO::IO() {}
IO::~IO() {}

// ==================================================
//                      METHODS
// ==================================================

std::vector<Task> IO::loadFile(std::string fileName) {
	std::ifstream inputFile(fileName);
	std::string inputFileString((std::istreambuf_iterator<char>(inputFile)),
		std::istreambuf_iterator<char>());

	std::vector<Task> taskVector;
	if(!fileIsOpen(inputFile)) {
		return taskVector;
	}

	const char* inputFileText = inputFileString.c_str(); // <-- this is the error!!!

	Document document;
	document.Parse(inputFileText);

	//assert(document.IsArray());
	//assert(document.IsArray());
	assert(document.IsObject());
	assert(document["TextBuddy Items"].IsArray());

	Value& item = document["TextBuddy Items"];

	for(SizeType i = 0; i < item.Size(); i++) {
	Task newTask = extractTaskFromJSONObject(item);
	taskVector.push_back(newTask);
	}
	/*
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
	*/


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
	std::string* line;

	for(int i = 0; i < 12; i++) {
		getline(inputFile,line[i]);
	}

	success = task.setName(line[0]);
	success = task.setType(u.stringToTaskType(line[1]));
	success = task.setID(stoi(line[2]));
	success = task.setLabel(line[3]);

	/*
	bool toggleDone();
	bool togglePriority();
	*/

	success = task.setStartDay(u.stringToDay(line[6]));
	success = task.setStartDate(stoi(line[7]));
	success = task.setStartTime(stoi(line[8]));

	success = task.setEndDay(u.stringToDay(line[9]));
	success = task.setEndDate(stoi(line[10]));
	success = task.setEndTime(stoi(line[11]));

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
