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



//================= PUBLIC METHODS ======================

// Loads file and extracts JSON text into a task vector
// throws an assert if file contents are invalid
std::vector<Task> IO::loadFile(std::string fileName) {
	std::ifstream inputFile(fileName);
	std::string inputFileString((std::istreambuf_iterator<char>(inputFile)),
		std::istreambuf_iterator<char>());

	std::vector<Task> taskVector;
	if(!fileIsOpen(inputFile)) {
		return taskVector;
	}

	const char* inputFileText = inputFileString.c_str();

	Document document;
	document.Parse(inputFileText);

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

	initialiseJsonText(newfile);

	for(unsigned int i = 0; i < taskVector.size(); i++) {
		Task task = taskVector[i];
		writeTaskIntoJsonFormat(newfile,task);
		
		if(i+1 < taskVector.size()) {
			newfile << ",\n";
		}
	}

	closeJsonText(newfile);
	newfile.close();

	return false;
}

//=============== PRIVATE METHODS ===================

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

void IO::writeTaskIntoJsonFormat(std::ofstream &newFile, Task task) {
	newFile << insertOpenParanthese();

	newFile << writeNameAttribute(task);
	newFile << writeTypeAttribute(task);
	newFile << writeIDAttribute(task);
	newFile << writeLabelAttribute(task) << "\n";
	newFile << writeIsDoneAttribute(task);
	newFile << writeIsPriorityAttribute(task) << "\n";
	newFile << writeStartDayAttribute(task);
	newFile << writeStartDateAttribute(task);
	newFile << writeStartTimeAttribute(task) << "\n";
	newFile << writeEndDayAttribute(task);
	newFile << writeEndDateAttribute(task);
	newFile << writeEndTimeAttribute(task);

	newFile << insertCloseParanthese();

	return;
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



//======= Retrieve Task Attributes and Write to File Methods =======

	std::string IO::insertOpenParanthese() {
		return "\t{\n";
	}
	std::string IO::insertCloseParanthese() {
		return "\t}\n";
	}
	std::string IO::writeNameAttribute(Task task) {
		std::string nameString;

		nameString = "\t\t\"name\": ";
		nameString += retrieveName(task);

		return nameString;
	}
	std::string IO::writeTypeAttribute(Task task) {
		std::string typeString;

		typeString = "\t\t\"type\": ";
		typeString += retrieveType(task);

		return typeString;
	}
	std::string IO::writeIDAttribute(Task task) {
		std::string IDString;

		IDString = "\t\t\"uniqueID\": ";
		IDString += retrieveID(task);

		return IDString;
	}
	std::string IO::writeLabelAttribute(Task task) {
		std::string labelString;

		labelString = "\t\t\"label\": ";
		labelString += retrieveLabel(task);

		return labelString;
	}
	std::string IO::writeIsDoneAttribute(Task task) {
		std::string isDoneString;

		isDoneString = "\t\t\"isDone\": ";
		isDoneString += retrieveIsDone(task);

		return isDoneString;
	}
	std::string IO::writeIsPriorityAttribute(Task task) {
		std::string isPriorityString;

		isPriorityString = "\t\t\"isPriority\": ";
		isPriorityString += retrieveIsPriority(task);

		return isPriorityString;
	}
	std::string IO::writeStartDayAttribute(Task task) {
		std::string startDayString;

		startDayString = "\t\t\"startDay\": ";
		startDayString += retrieveStartDay(task);

		return startDayString;
	}
	std::string IO::writeStartDateAttribute(Task task) {
		std::string startDateString;

		startDateString = "\t\t\"startDate\": ";
		startDateString += retrieveStartDate(task);

		return startDateString;
	}
	std::string IO::writeStartTimeAttribute(Task task) {
		std::string startTimeString;

		startTimeString = "\t\t\"startTime\": ";
		startTimeString += retrieveStartTime(task);

		return startTimeString;
	}
	std::string IO::writeEndDayAttribute(Task task) {
		std::string EndDayString;

		EndDayString = "\t\t\"EndDay\": ";
		EndDayString += retrieveEndDay(task);

		return EndDayString;
	}
	std::string IO::writeEndDateAttribute(Task task) {
		std::string EndDateString;

		EndDateString = "\t\t\"EndDate\": ";
		EndDateString += retrieveEndDate(task);

		return EndDateString;
	}
	std::string IO::writeEndTimeAttribute(Task task) {
		std::string EndTimeString;

		EndTimeString = "\t\t\"EndTime\": ";
		EndTimeString += retrieveEndTime(task);

		return EndTimeString;
	}


	std::string IO::retrieveName(Task task) {
		std::string string;
		string = "\"" + task.getName() + "\"\n";
		return string;
	}
	std::string IO::retrieveType(Task task) {
		std::string string;
		TaskType type = task.getType();
		string = "\"" + Utilities::taskTypeToString(type) + "\"\n";
		return string;
	}
	std::string IO::retrieveID(Task task) {
		std::string string;
		int ID = task.getID();
		string = "\"" + std::to_string(ID) + "\"\n";
		return string;
	}
	std::string IO::retrieveLabel(Task task) {
		std::string string;
		string = "\"" + task.getLabel() + "\"\n";
		return string;
	}
	std::string IO::retrieveIsDone(Task task) {
		std::string string;
		bool status = task.getDoneStatus();
		string = "\"" + Utilities::boolToString(status) + "\"\n";
		return string;
	}
	std::string IO::retrieveIsPriority(Task task) {
		std::string string;
		bool status = task.getPriorityStatus();
		string = "\"" + Utilities::boolToString(status) + "\"\n";
		return string;
	}
	std::string IO::retrieveStartDay(Task task) {
		std::string string;
		Day day = task.getStartDay();
		string = "\"" + Utilities::dayToString(day) + "\"\n";
		return string;
	}
	std::string IO::retrieveStartDate(Task task) {
		std::string string;
		int date = task.getStartDate();
		string = "\"" + std::to_string(date) + "\"\n";
		return string;
	}
	std::string IO::retrieveStartTime(Task task) {
		std::string string;
		int time = task.getStartTime();
		string = "\"" + std::to_string(time) + "\"\n";
		return string;
	}
	std::string IO::retrieveEndDay(Task task) {
		std::string string;
		Day day = task.getEndDay();
		string = "\"" + Utilities::dayToString(day) + "\"\n";
		return string;
	}
	std::string IO::retrieveEndDate(Task task) {
		std::string string;
		int date = task.getEndDate();
		string = "\"" + std::to_string(date) + "\"\n";
		return string;
	}
	std::string IO::retrieveEndTime(Task task) {
		std::string string;
		int time = task.getEndTime();
		string = "\"" + std::to_string(time) + "\"\n";
		return string;
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

	void IO::initialiseJsonText(std::ofstream& newfile) {
		newfile << "\"TextBuddy Items\":\n[\n";
		return;
	}
	void IO::closeJsonText(std::ofstream& newfile) {
		newfile << "]";
		return;
	}