// Ng Ren Zhi @@author A0130463R

#include "stdafx.h"
#include "IO.h"

// TODO: Refactor IO.cpp - remove repeated file names

using namespace rapidjson;

std::string IO::filePath = "mytasks.txt";
const std::string IO::configPath = ".TSconfig";

IO* IO::theOne = new IO();

IO::IO() {
	// TsLogger::getInstance()->log(SYS,"IO instantiated");
	std::ifstream TSconfig(configPath);
	TSconfig >> filePath;
	// setCustomCommands(TSconfig);
	TSconfig.close();
}

IO::~IO() {}

//==================================================
//                   PUBLIC METHODS
//==================================================

// TODO: Handle empty file / improper content exceptions

IO* IO::getInstance() {
	return theOne;
}

std::string IO::getFilePath() {
	return filePath;
}

// Note: Directory must already exist
// Returns false if unable to set file path
bool IO::setFilePath(std::string newFilePath, std::vector<Task> taskVector, bool isRemovePrevFile) {
	if (saveFile(newFilePath,taskVector)) {
		if (isRemovePrevFile || filePath==newFilePath) {
			remove(filePath.c_str());
		}
		filePath = newFilePath;
		return true;
	}
	return false;
}

// Loads file and extracts JSON text into a task vector
// Throws an assert() if file contents are invalid
std::vector<Task> IO::loadFile(std::string fileName, bool isOverwriteLoadFile) {
	std::ifstream inputFile(fileName);
	if (!fileIsOpen(inputFile)) {
		throw std::runtime_error("File does not exist");
	}

	std::string inputFileString((std::istreambuf_iterator<char>(inputFile)),
		std::istreambuf_iterator<char>());
	const char* inputFileText = inputFileString.c_str();

	Document document;
	document.Parse(inputFileText);
	assert(document.IsObject());
	assert(document["TaskShark Items"].IsArray());
	Value& item = document["TaskShark Items"];

	std::vector<Task> taskVector;
	if (item.Size() > 0) {
		for (SizeType i = 0; i < item.Size(); i++) {
			Task newTask = extractTaskFromJsonObject(item[i]);
			taskVector.push_back(newTask);
		}
		initialiseRunningCount(taskVector);
	}

	inputFile.close();
	if (isOverwriteLoadFile) {
		filePath = fileName;
	}
	return taskVector;
}

bool IO::saveFile(std::string fileName, std::vector<Task> taskVector) {
	remove(fileName.c_str());
	std::ofstream newfile(fileName);

	if (!fileIsOpen(newfile)) {
		return false;
	}

	initialiseJsonText(newfile);

	for (unsigned int i = 0; i < taskVector.size(); i++) {
		Task task = taskVector[i];
		writeTaskIntoJsonFormat(newfile,task);

		if (i+1 < taskVector.size()) {
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

//==================================================
//                   PRIVATE METHODS
//==================================================

// Records down last saved location as .TSconfig file
// for IO to find where to load when it launches
void IO::setLastSavedLocation(std::string fileName) {
	remove(configPath.c_str());
	std::ofstream lastSaved(configPath);
	lastSaved << fileName << std::endl;

	saveCustomCommands(lastSaved);
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
		throw std::runtime_error("Error occured: " + error);
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

	if (!success) {
		throw std::runtime_error("NameNotFound");
	}
}

void IO::extractType(Task &newTask, Value &item) {
	std::string typeString = item["type"].GetString();
	TaskType type = Utilities::stringToTaskType(typeString);
	bool success = newTask.setType(type);

	if (!success) {
		throw std::runtime_error("TypeNotFound");
	}
}

void IO::extractID(Task &newTask, Value &item) {
	int ID = item["uniqueID"].GetInt();
	bool success = newTask.setID(ID);

	if (!success) {
		throw std::runtime_error("IDNotFound");
	}
}

void IO::extractLabel(Task &newTask, Value &item) {
	int arraySize = item["label"].Size();
	std::vector<std::string> labels;

	for (int i = 0; i < arraySize; i++) {
		labels.push_back(item["label"][i].GetString());
	}
	bool success = newTask.addLabels(labels);

	if (!success) {
		throw std::runtime_error("LabelNotFound");
	}
}

void IO::extractDone(Task &newTask, Value &item) {
	bool isDone = item["isDone"].GetBool();
	// bool success = false;
	if (isDone) {
		newTask.markDone();
	}

	// TODO: how to throw exception for GeTSool
	/*
	if (!success) {
	throw "IDNotFound";
	}
	*/
}

void IO::extractPriority(Task &newTask, Value &item) {
	bool isPriority = item["isPriority"].GetBool();
	// bool success = false;
	if (isPriority) {
		newTask.setPriority();
	}

	// TODO: how to throw exception for GeTSool
	/*
	if (!success) {
	throw "IDNotFound";
	}
	*/
}

void IO::extractStartDate(Task &newTask, Value &item) {
	int date = item["startDate"].GetInt();
	bool success = newTask.setStartDate(date);

	if (!success) {
		throw std::runtime_error("StartDateNotFound");
	}
}

void IO::extractStartTime(Task &newTask, Value &item) {
	int time = item["startTime"].GetInt();
	bool success = newTask.setStartTime(time);

	if (!success) {
		throw std::runtime_error("StartTimeNotFound");
	}
}

void IO::extractEndDate(Task &newTask, Value &item) {
	int date = item["endDate"].GetInt();
	bool success = newTask.setEndDate(date);

	if (!success) {
		throw ("EndDateNotFound");
	}
}

void IO::extractEndTime(Task &newTask, Value &item)  {
	int time = item["endTime"].GetInt();
	bool success = newTask.setEndTime(time);

	if (!success) {
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
	std::string extractedName;

	nameString = "\t\t\t\"name\": ";
	//extractedName = ;
	// Handle inverted commas in case task name contains inverted commas
	//extractedName = Utilities::removeFirstAndLastInvertedCommas(extractedName);
	//extractedName = Utilities::addSlashForInvertedComma(extractedName);
	nameString += retrieveName(task);//extractedName;

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
	string = "\"" + Utilities::addSlashForInvertedComma(task.getName()) + "\",\n";
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
	std::vector<std::string> labelVector = task.getLabels();
	string = "[";

	for (unsigned int i = 0; i < labelVector.size(); i++) {
		string += "\"" + labelVector[i] + "\"";
		if (i+1 < labelVector.size()) {
			string += ",\n\t\t\t\t";
		}
	}
	string += "],\n";

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
	if (inputFile.is_open())	{
		return true;
	} else {
		return false;
	}
}

bool IO::fileIsOpen(std::ofstream& outputFile) {
	if (outputFile.is_open())	{
		return true;
	} else {
		return false;
	}
}

void IO::initialiseJsonText(std::ofstream& newfile) {
	newfile << "{\n\t\"TaskShark Items\":\n\t[\n";
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

// Aaron Chong Jun Hao @@author A0110376N

bool IO::setCommandKeyword(std::string &identifier, std::string keyword) {
	TsLogger::getInstance()->log(SYS,"Setting custom command...");

	// Count: 14
	if (keyword == "s" // Protected keyword used by UI for switching between 'tile' and 'list' views
		|| (&identifier != &TS::COMMAND_ADD				&& keyword == TS::COMMAND_ADD)
		|| (&identifier != &TS::COMMAND_DELETE			&& keyword == TS::COMMAND_DELETE)
		|| (&identifier != &TS::COMMAND_MODIFY			&& keyword == TS::COMMAND_MODIFY)
		|| (&identifier != &TS::COMMAND_MODIFY_EDIT		&& keyword == TS::COMMAND_MODIFY_EDIT)
		|| (&identifier != &TS::COMMAND_PICK_RESERVE	&& keyword == TS::COMMAND_PICK_RESERVE)
		|| (&identifier != &TS::COMMAND_SEARCH			&& keyword == TS::COMMAND_SEARCH)
		|| (&identifier != &TS::COMMAND_MARKDONE		&& keyword == TS::COMMAND_MARKDONE)
		|| (&identifier != &TS::COMMAND_UNMARKDONE		&& keyword == TS::COMMAND_UNMARKDONE)
		|| (&identifier != &TS::COMMAND_UNDO			&& keyword == TS::COMMAND_UNDO)
		|| (&identifier != &TS::COMMAND_REDO			&& keyword == TS::COMMAND_REDO)
		|| (&identifier != &TS::COMMAND_VIEW			&& keyword == TS::COMMAND_VIEW)
		|| (&identifier != &TS::COMMAND_CLEAR_ALL		&& keyword == TS::COMMAND_CLEAR_ALL)
		|| (&identifier != &TS::COMMAND_LOAD			&& keyword == TS::COMMAND_LOAD)
		|| (&identifier != &TS::COMMAND_SAVE			&& keyword == TS::COMMAND_SAVE)
		|| (&identifier != &TS::COMMAND_EXIT			&& keyword == TS::COMMAND_EXIT) ) {
			return false;
	}
	identifier = keyword;
	return true;
}

void IO::setCustomCommands(std::ifstream& TSconfig) {
	std::string s;
	std::vector<std::string> commandPair;
	std::string identifier;
	std::string keyword;

	std::getline(TSconfig, s); // Skip newline character
	std::getline(TSconfig, s); // Skip blank line
	std::getline(TSconfig, s);
	if (s == "Custom Commands") {
		while (std::getline(TSconfig, s)) {
			if (!s.empty()) {
				commandPair = Utilities::stringToVec(s);
				if (commandPair.size() == 2) {
					identifier = commandPair[0];
					keyword = commandPair[1];

					TsLogger::getInstance()->log(SYS,"Looking up command: " + TS::COMMAND_ADD);
					if (identifier == "add") {
						setCommandKeyword(TS::COMMAND_ADD,keyword);
					} else if (identifier == "delete") {
						setCommandKeyword(TS::COMMAND_DELETE,keyword);
					} else if (identifier == "modify") {
						setCommandKeyword(TS::COMMAND_MODIFY,keyword);
					} else if (identifier == "edit") {
						setCommandKeyword(TS::COMMAND_MODIFY_EDIT,keyword);
					} else if (identifier == "pick") {
						setCommandKeyword(TS::COMMAND_PICK_RESERVE,keyword);
					} else if (identifier == "search") {
						setCommandKeyword(TS::COMMAND_SEARCH,keyword);
					} else if (identifier == "done") {
						setCommandKeyword(TS::COMMAND_MARKDONE,keyword);
					} else if (identifier == "notdone") {
						setCommandKeyword(TS::COMMAND_UNMARKDONE,keyword);
					} else if (identifier == "undo") {
						setCommandKeyword(TS::COMMAND_UNDO,keyword);
					} else if (identifier == "redo") {
						setCommandKeyword(TS::COMMAND_REDO,keyword);
					} else if (identifier == "view") {
						setCommandKeyword(TS::COMMAND_VIEW,keyword);
					} else if (identifier == "clear") {
						setCommandKeyword(TS::COMMAND_CLEAR_ALL,keyword);
					} else if (identifier == "display") {
						setCommandKeyword(TS::COMMAND_DISPLAY_ALL,keyword);
					} else if (identifier == "load") {
						setCommandKeyword(TS::COMMAND_LOAD,keyword);
					} else if (identifier == "save") {
						setCommandKeyword(TS::COMMAND_SAVE,keyword);
					} else if (identifier == "exit") {
						setCommandKeyword(TS::COMMAND_EXIT,keyword);
					}
				}
			}
		}
	}
	TsLogger::getInstance()->log(SYS,"Looking up command: " + TS::COMMAND_ADD);
	return;
}

void IO::saveCustomCommands(std::ofstream& TSconfig) {
	TSconfig << std::endl;
	TSconfig << "Custom Commands" << std::endl;

	// Count: 16
	if ("add" != TS::COMMAND_ADD) {
		TSconfig << "add" << " " << TS::COMMAND_ADD << std::endl;
	} else if ("delete" != TS::COMMAND_DELETE) {
		TSconfig << "delete" << " " << TS::COMMAND_DELETE << std::endl;
	} else if ("modify" != TS::COMMAND_MODIFY) {
		TSconfig << "modify" << " " << TS::COMMAND_MODIFY << std::endl;
	} else if ("edit" != TS::COMMAND_MODIFY_EDIT) {
		TSconfig << "edit" << " " << TS::COMMAND_MODIFY_EDIT << std::endl;
	} else if ("pick" != TS::COMMAND_PICK_RESERVE) {
		TSconfig << "pick" << " " << TS::COMMAND_PICK_RESERVE << std::endl;
	} else if ("search" != TS::COMMAND_SEARCH) {
		TSconfig << "search" << " " << TS::COMMAND_SEARCH << std::endl;
	} else if ("done" != TS::COMMAND_MARKDONE) {
		TSconfig << "done" << " " << TS::COMMAND_MARKDONE << std::endl;
	} else if ("notdone" != TS::COMMAND_UNMARKDONE) {
		TSconfig << "notdone" << " " << TS::COMMAND_UNMARKDONE << std::endl;
	} else if ("undo" != TS::COMMAND_UNDO) {
		TSconfig << "undo" << " " << TS::COMMAND_UNDO << std::endl;
	} else if ("redo" != TS::COMMAND_REDO) {
		TSconfig << "redo" << " " << TS::COMMAND_REDO << std::endl;
	} else if ("view" != TS::COMMAND_VIEW) {
		TSconfig << "view" << " " << TS::COMMAND_VIEW << std::endl;
	} else if ("clear" != TS::COMMAND_CLEAR_ALL) {
		TSconfig << "clear" << " " << TS::COMMAND_CLEAR_ALL << std::endl;
	} else if ("display" != TS::COMMAND_DISPLAY_ALL) {
		TSconfig << "display" << " " << TS::COMMAND_DISPLAY_ALL << std::endl;
	} else if ("load" != TS::COMMAND_LOAD) {
		TSconfig << "load" << " " << TS::COMMAND_LOAD << std::endl;
	} else if ("save" != TS::COMMAND_SAVE) {
		TSconfig << "save" << " " << TS::COMMAND_SAVE << std::endl;
	} else if ("exit" != TS::COMMAND_EXIT) {
		TSconfig << "exit" << " " << TS::COMMAND_EXIT << std::endl;
	}
	return;
}

//========== Getter for Testing ==========

std::vector<std::string> IO::getText(std::string fileName) {
	std::ifstream inputFile(fileName);
	std::vector<std::string> textVector;

	assert(fileIsOpen(inputFile));

	while (!inputFile.eof()) {
		std::string line;
		getline(inputFile,line);

		if (line != "") {
			textVector.push_back(line);
		}
	}

	inputFile.close();
	return textVector;
}
