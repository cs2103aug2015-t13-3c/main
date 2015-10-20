// @@author A0130463R (Ng Ren Zhi)

#include "stdafx.h"
#include "IO.h"
#include "Rapidjson\include\rapidjson\document.h"
//#include "Shlwapi.h"

//TODO: Refactor IO.cpp - remove repeated file names

using namespace rapidjson;

const std::string IO::lastSavedLocation = "lastSave.tbconfig";

IO::IO() {
	std::ifstream lastSave(lastSavedLocation);
	lastSave >> filePath;
}

IO::~IO() {}

// ==================================================
//                   PUBLIC METHODS
// ==================================================

// TODO: Handle empty file / improper content exceptions

std::string IO::getFilePath() {
	return filePath;
}

// Note: Directory must already exist
// Returns false if unable to set file path
bool IO::setFilePath(std::string newFilePath, std::vector<Task> taskVector) {
	if(saveFile(newFilePath,taskVector)) {
		remove(filePath.c_str());
		filePath = newFilePath;
		return true;
	}
	return false;
}

// Loads file and extracts JSON text into a task vector
// Throws an assert() if file contents are invalid
std::vector<Task> IO::loadFile(std::string fileName) {
	std::ifstream inputFile(fileName);

	std::vector<Task> taskVector;
	if(!fileIsOpen(inputFile)) {
		return taskVector;
	}

	std::string inputFileString((std::istreambuf_iterator<char>(inputFile)),
		std::istreambuf_iterator<char>());

	const char* inputFileText = inputFileString.c_str();

	Document document;
	document.Parse(inputFileText);

	assert(document.IsObject());
	assert(document["TextBuddy Items"].IsArray());
	Value& item = document["TextBuddy Items"];

	if(item.Size() > 0) {

		for(SizeType i = 0; i < item.Size(); i++) {
			Task newTask = extractTaskFromJsonObject(item[i]);
			taskVector.push_back(newTask);
		}
		initialiseRunningCount(taskVector);
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
			newfile << "\t\t,\n";
		}
	}

	closeJsonText(newfile);
	setLastSavedLocation(fileName);

	newfile.close();
	return true;
}

/*
bool IO::changeSourceFileLocation (std::string newFileLocation) {
std::string systemRoot;
systemRoot = getenv ("systemroot");

std::string newFullPath= systemRoot + "\\" + newFileLocation;
const char* newFullPathName = newFullPath.c_str();

return PathIsDirectory(newFullPathName);
}
*/

// ==================================================
//                   PRIVATE METHODS
// ==================================================

// Records down last saved location as .tbconfig file
// for IO to find where to load when it launches
void IO::setLastSavedLocation(std::string fileName) {
	remove(lastSavedLocation.c_str());
	std::ofstream lastSaved(lastSavedLocation);
	lastSaved << fileName;
	lastSaved.close();
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
		extractStartDate(newTask, item);
		extractStartTime(newTask, item);
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
	newFile << writeStartDateAttribute(task);
	newFile << writeStartTimeAttribute(task) << "\n";
	newFile << writeEndDateAttribute(task);
	newFile << writeEndTimeAttribute(task);

	newFile << insertCloseParanthese();
	return;
}

//========== Extract Task Attribute Methods ==========

void IO::extractName(Task &newTask, Value &item) {
	std::string name = item["name"].GetString();
	bool success = newTask.setName(name);

	if(!success) {
		throw std::runtime_error("NameNotFound");
	}
}

void IO::extractType(Task &newTask, Value &item) {
	std::string typeString = item["type"].GetString();
	TaskType type = Utilities::stringToTaskType(typeString);
	bool success = newTask.setType(type);

	if(!success) {
		throw std::runtime_error("TypeNotFound");
	}
}

void IO::extractID(Task &newTask, Value &item) {
	int ID = item["uniqueID"].GetInt();
	bool success = newTask.setID(ID);

	if(!success) {
		throw std::runtime_error("IDNotFound");
	}
}

void IO::extractLabel(Task &newTask, Value &item) {
	std::string label = item["label"].GetString();
	bool success = newTask.setLabel(label);

	if(!success) {
		throw std::runtime_error("LabelNotFound");
	}
}

void IO::extractDone(Task &newTask, Value &item) {
	bool isDone = item["isDone"].GetBool();
	// bool success = false;
	if(isDone) {
		newTask.markDone();
	}

	// TODO: how to throw exception for GetBool
	/*
	if(!success) {
	throw "IDNotFound";
	}
	*/
}

void IO::extractPriority(Task &newTask, Value &item) {
	bool isPriority = item["isPriority"].GetBool();
	// bool success = false;
	if(isPriority) {
		newTask.setPriority();
	}

	// TODO: how to throw exception for GetBool
	/*
	if(!success) {
	throw "IDNotFound";
	}
	*/
}

void IO::extractStartDate(Task &newTask, Value &item) {
	int date = item["startDate"].GetInt();
	bool success = newTask.setStartDate(date);

	if(!success) {
		throw std::runtime_error("StartDateNotFound");
	}
}

void IO::extractStartTime(Task &newTask, Value &item) {
	int time = item["startTime"].GetInt();
	bool success = newTask.setStartTime(time);

	if(!success) {
		throw std::runtime_error("StartTimeNotFound");
	}
}

void IO::extractEndDate(Task &newTask, Value &item) {
	int date = item["endDate"].GetInt();
	bool success = newTask.setEndDate(date);

	if(!success) {
		throw ("EndDateNotFound");
	}
}

void IO::extractEndTime(Task &newTask, Value &item)  {
	int time = item["endTime"].GetInt();
	bool success = newTask.setEndTime(time);

	if(!success) {
		throw std::runtime_error("EndTimeNotFound");
	}
}

//========== Write to File Methods ==========

std::string IO::insertOpenParanthese() {
	return "\t\t{\n";
}

std::string IO::insertCloseParanthese() {
	return "\t\t}\n";
}

std::string IO::writeNameAttribute(Task task) {
	std::string nameString;

	nameString = "\t\t\t\"name\": ";
	nameString += retrieveName(task);

	return nameString;
}

std::string IO::writeTypeAttribute(Task task) {
	std::string typeString;

	typeString = "\t\t\t\"type\": ";
	typeString += retrieveType(task);

	return typeString;
}

std::string IO::writeIDAttribute(Task task) {
	std::string IDString;

	IDString = "\t\t\t\"uniqueID\": ";
	IDString += retrieveID(task);

	return IDString;
}

std::string IO::writeLabelAttribute(Task task) {
	std::string labelString;

	labelString = "\t\t\t\"label\": ";
	labelString += retrieveLabel(task);

	return labelString;
}

std::string IO::writeIsDoneAttribute(Task task) {
	std::string isDoneString;

	isDoneString = "\t\t\t\"isDone\": ";
	isDoneString += retrieveIsDone(task);

	return isDoneString;
}

std::string IO::writeIsPriorityAttribute(Task task) {
	std::string isPriorityString;

	isPriorityString = "\t\t\t\"isPriority\": ";
	isPriorityString += retrieveIsPriority(task);

	return isPriorityString;
}

std::string IO::writeStartDateAttribute(Task task) {
	std::string startDateString;

	startDateString = "\t\t\t\"startDate\": ";
	startDateString += retrieveStartDate(task);

	return startDateString;
}

std::string IO::writeStartTimeAttribute(Task task) {
	std::string startTimeString;

	startTimeString = "\t\t\t\"startTime\": ";
	startTimeString += retrieveStartTime(task);

	return startTimeString;
}

std::string IO::writeEndDateAttribute(Task task) {
	std::string EndDateString;

	EndDateString = "\t\t\t\"endDate\": ";
	EndDateString += retrieveEndDate(task);

	return EndDateString;
}

std::string IO::writeEndTimeAttribute(Task task) {
	std::string EndTimeString;

	EndTimeString = "\t\t\t\"endTime\": ";
	EndTimeString += retrieveEndTime(task);

	return EndTimeString;
}

//========== Retrieve Task Attribute Methods ==========

std::string IO::retrieveName(Task task) {
	std::string string;
	string = "\"" + task.getName() + "\",\n";
	return string;
}

std::string IO::retrieveType(Task task) {
	std::string string;
	TaskType type = task.getType();
	string = "\"" + Utilities::taskTypeToString(type) + "\",\n";
	return string;
}

std::string IO::retrieveID(Task task) {
	std::string string;
	int ID = task.getID();
	string = std::to_string(ID) + ",\n";
	return string;
}

std::string IO::retrieveLabel(Task task) {
	std::string string;
	string = "\"" + task.getLabel() + "\",\n";
	return string;
}

std::string IO::retrieveIsDone(Task task) {
	std::string string;
	bool status = task.getDoneStatus();
	string = Utilities::boolToString(status) + ",\n";
	return string;
}

std::string IO::retrieveIsPriority(Task task) {
	std::string string;
	bool status = task.getPriorityStatus();
	string = Utilities::boolToString(status) + ",\n";
	return string;
}

std::string IO::retrieveStartDate(Task task) {
	std::string string;
	int date = task.getStartDate();
	string = std::to_string(date) + ",\n";
	return string;
}

std::string IO::retrieveStartTime(Task task) {
	std::string string;
	int time = task.getStartTime();
	string = std::to_string(time) + ",\n";
	return string;
}

std::string IO::retrieveEndDate(Task task) {
	std::string string;
	int date = task.getEndDate();
	string = std::to_string(date) + ",\n";
	return string;
}

std::string IO::retrieveEndTime(Task task) {
	std::string string;
	int time = task.getEndTime();
	string = std::to_string(time) + "\n";
	return string;
}

//========== Overloaded Functions ==========

bool IO::fileIsOpen(std::ifstream& inputFile) {
	if(inputFile.is_open())	{
		return true;
	} else {
		return false;
	}
}

bool IO::fileIsOpen(std::ofstream& outputFile) {
	if(outputFile.is_open())	{
		return true;
	} else {
		return false;
	}
}

void IO::initialiseJsonText(std::ofstream& newfile) {
	newfile << "{\n\t\"TextBuddy Items\":\n\t[\n";
	return;
}

void IO::closeJsonText(std::ofstream& newfile) {
	newfile << "\t]\n}";
	return;
}

void IO::initialiseRunningCount(std::vector<Task> taskVector) {
	Task lastTask = taskVector.back();
	int lastCount = lastTask.getID();
	lastTask.setRunningCount(lastCount);

	return;
}

//========== Getter for Testing ==========

std::vector<std::string> IO::getText(std::string fileName) {
	std::ifstream inputFile(fileName);
	std::vector<std::string> textVector;

	assert(fileIsOpen(inputFile));

	while(!inputFile.eof()) {
		std::string line;
		getline(inputFile,line);

		if(line != "") {
			textVector.push_back(line);
		}
	}

	inputFile.close();
	return textVector;
}
