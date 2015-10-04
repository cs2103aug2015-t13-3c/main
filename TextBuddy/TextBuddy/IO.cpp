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
		Task newTask = extractTaskFromJsonObject(item[i]);
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

Task IO::extractTaskFromJsonObject(Value& item) {
	Task newTask;

	try {
		extractName(newTask, item);
		extractType(newTask, item);
		extractID(newTask, item);
		extractLabel(newTask, item);
		extractDone(newTask, item);
		extractPriority(newTask, item);
		extractStartDay(newTask, item);
		extractStartDate(newTask, item);
		extractStartTime(newTask, item);
		extractEndDay(newTask, item);
		extractEndDate(newTask, item);
		extractEndTime(newTask, item);
	} catch (std::string error) {
		std::cout << "Error occured: " << error << "\n";
	}

	return newTask;
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



//============ Task Attributes Extraction Methods ===========

void IO::extractName(Task &newTask, Value &item) {
	std::string name = item["name"].GetString();
	bool success = newTask.setName(name);

	if(!success) {
		throw "NameNotFound";
	}
}
void IO::extractType(Task &newTask, Value &item) {
	std::string typeString = item["type"].GetString();
	TaskType type = Utilities::stringToTaskType(typeString);
	bool success = newTask.setType(type);

	if(!success) {
		throw "TypeNotFound";
	}
}
void IO::extractID(Task &newTask, Value &item) {
	int ID = item["uniqueID"].GetInt();
	bool success = newTask.setID(ID);

	if(!success) {
		throw "IDNotFound";
	}
}
void IO::extractLabel(Task &newTask, Value &item) {
	std::string label = item["label"].GetString();
	bool success = newTask.setLabel(label);

	if(!success) {
		throw "LabelNotFound";
	}
}
void IO::extractDone(Task &newTask, Value &item) {
	bool isDone = item["isDone"].GetBool();
	//bool success = false;
	if(isDone) {
		newTask.toggleDone();
	}

	//TODO: how to throw exception for GetBool
	/*
	if(!success) {
	throw "IDNotFound";
	}
	*/
}
void IO::extractPriority(Task &newTask, Value &item) {
	bool isPriority = item["isPriority"].GetBool();
	//bool success = false;
	if(isPriority) {
		newTask.togglePriority();
	}

	//TODO: how to throw exception for GetBool
	/*
	if(!success) {
	throw "IDNotFound";
	}
	*/
}
void IO::extractStartDay(Task &newTask, Value &item) {
	std::string dayString = item["startDay"].GetString();
	Day day = Utilities::stringToDay(dayString);
	bool success = newTask.setStartDay(day);

	if(!success) {
		throw "StartDayNotFound";
	}
}
void IO::extractStartDate(Task &newTask, Value &item) {
	int date = item["startDate"].GetInt();
	bool success = newTask.setStartDate(date);

	if(!success) {
		throw "StartDateNotFound";
	}
}
void IO::extractStartTime(Task &newTask, Value &item) {
	int time = item["startTime"].GetInt();
	bool success = newTask.setStartTime(time);

	if(!success) {
		throw "StartTimeNotFound";
	}
}
void IO::extractEndDay(Task &newTask, Value &item) {
	std::string dayString = item["endDay"].GetString();
	Day day = Utilities::stringToDay(dayString);
	bool success = newTask.setEndDay(day);

	if(!success) {
		throw "EndDayNotFound";
	}
}
void IO::extractEndDate(Task &newTask, Value &item) {
	int date = item["endDate"].GetInt();
	bool success = newTask.setEndDate(date);

	if(!success) {
		throw "EndDateNotFound";
	}
}
void IO::extractEndTime(Task &newTask, Value &item)  {
	int time = item["endTime"].GetInt();
	bool success = newTask.setEndTime(time);

	if(!success) {
		throw "EndTimeNotFound";
	}
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
