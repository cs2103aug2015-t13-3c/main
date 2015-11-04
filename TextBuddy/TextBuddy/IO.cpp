// Ng Ren Zhi @@author A0130463R

#include "stdafx.h"
#include "IO.h"
// #include "Shlwapi.h"

// TODO: Refactor IO.cpp - remove repeated file names

using namespace rapidjson;

std::string IO::filePath = "mytasks.txt";
const std::string IO::configPath = ".tbconfig";

IO* IO::theOne = new IO();

IO::IO() {
	// TbLogger::getInstance()->log(SYS,"IO instantiated");
	std::ifstream tbconfig(configPath);
	tbconfig >> filePath;
	// setCustomCommands(tbconfig);
	tbconfig.close();
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
	if(!fileIsOpen(inputFile)) {
		throw std::runtime_error("File does not exist");
	}

	std::string inputFileString((std::istreambuf_iterator<char>(inputFile)),
		std::istreambuf_iterator<char>());
	const char* inputFileText = inputFileString.c_str();

	Document document;
	document.Parse(inputFileText);
	assert(document.IsObject());
	assert(document["TextBuddy Items"].IsArray());
	Value& item = document["TextBuddy Items"];

	std::vector<Task> taskVector;
	if(item.Size() > 0) {
		for(SizeType i = 0; i < item.Size(); i++) {
			Task newTask = extractTaskFromJsonObject(item[i]);
			taskVector.push_back(newTask);
		}
		initialiseRunningCount(taskVector);
	}

	inputFile.close();
	filePath = fileName;
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

//==================================================
//                   PRIVATE METHODS
//==================================================

// Records down last saved location as .tbconfig file
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
	int arraySize = item["label"].Size();
	std::vector<std::string> labels;

	for(int i = 0; i < arraySize; i++) {
		labels.push_back(item["label"][i].GetString());
	}
	bool success = newTask.addLabels(labels);

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

	for(unsigned int i = 0; i < labelVector.size(); i++) {
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

// Aaron Chong Jun Hao @@author A0110376N

bool IO::setCommandKeyword(std::string &identifier, std::string keyword) {
	TbLogger::getInstance()->log(SYS,"Setting custom command...");

	// Count: 14
	if(keyword == "s" // Protected keyword used by UI for switching between 'tile' and 'list' views
		|| (&identifier != &Tb::COMMAND_ADD				&& keyword == Tb::COMMAND_ADD)
		|| (&identifier != &Tb::COMMAND_DELETE			&& keyword == Tb::COMMAND_DELETE)
		|| (&identifier != &Tb::COMMAND_MODIFY			&& keyword == Tb::COMMAND_MODIFY)
		|| (&identifier != &Tb::COMMAND_MODIFY_EDIT		&& keyword == Tb::COMMAND_MODIFY_EDIT)
		|| (&identifier != &Tb::COMMAND_PICK_RESERVE	&& keyword == Tb::COMMAND_PICK_RESERVE)
		|| (&identifier != &Tb::COMMAND_SEARCH			&& keyword == Tb::COMMAND_SEARCH)
		|| (&identifier != &Tb::COMMAND_MARKDONE		&& keyword == Tb::COMMAND_MARKDONE)
		|| (&identifier != &Tb::COMMAND_UNMARKDONE		&& keyword == Tb::COMMAND_UNMARKDONE)
		|| (&identifier != &Tb::COMMAND_UNDO			&& keyword == Tb::COMMAND_UNDO)
		|| (&identifier != &Tb::COMMAND_REDO			&& keyword == Tb::COMMAND_REDO)
		|| (&identifier != &Tb::COMMAND_VIEW			&& keyword == Tb::COMMAND_VIEW)
		|| (&identifier != &Tb::COMMAND_CLEAR_ALL		&& keyword == Tb::COMMAND_CLEAR_ALL)
		|| (&identifier != &Tb::COMMAND_LOAD			&& keyword == Tb::COMMAND_LOAD)
		|| (&identifier != &Tb::COMMAND_SAVE			&& keyword == Tb::COMMAND_SAVE)
		|| (&identifier != &Tb::COMMAND_EXIT			&& keyword == Tb::COMMAND_EXIT) ) {
			return false;
	}
	identifier = keyword;
	return true;
}

void IO::setCustomCommands(std::ifstream& tbconfig) {
	std::string s;
	std::vector<std::string> commandPair;
	std::string identifier;
	std::string keyword;

	std::getline(tbconfig, s); // Skip newline character
	std::getline(tbconfig, s); // Skip blank line
	std::getline(tbconfig, s);
	if(s == "Custom Commands") {
		while(std::getline(tbconfig, s)) {
			if(!s.empty()) {
				commandPair = Utilities::stringToVec(s);
				if(commandPair.size() == 2) {
					identifier = commandPair[0];
					keyword = commandPair[1];

					TbLogger::getInstance()->log(SYS,"Looking up command: " + Tb::COMMAND_ADD);
					if(identifier == "add") {
						setCommandKeyword(Tb::COMMAND_ADD,keyword);
					} else if(identifier == "delete") {
						setCommandKeyword(Tb::COMMAND_DELETE,keyword);
					} else if(identifier == "modify") {
						setCommandKeyword(Tb::COMMAND_MODIFY,keyword);
					} else if(identifier == "edit") {
						setCommandKeyword(Tb::COMMAND_MODIFY_EDIT,keyword);
					} else if(identifier == "pick") {
						setCommandKeyword(Tb::COMMAND_PICK_RESERVE,keyword);
					} else if(identifier == "search") {
						setCommandKeyword(Tb::COMMAND_SEARCH,keyword);
					} else if(identifier == "done") {
						setCommandKeyword(Tb::COMMAND_MARKDONE,keyword);
					} else if(identifier == "notdone") {
						setCommandKeyword(Tb::COMMAND_UNMARKDONE,keyword);
					} else if(identifier == "undo") {
						setCommandKeyword(Tb::COMMAND_UNDO,keyword);
					} else if(identifier == "redo") {
						setCommandKeyword(Tb::COMMAND_REDO,keyword);
					} else if(identifier == "view") {
						setCommandKeyword(Tb::COMMAND_VIEW,keyword);
					} else if(identifier == "clear") {
						setCommandKeyword(Tb::COMMAND_CLEAR_ALL,keyword);
					} else if(identifier == "display") {
						setCommandKeyword(Tb::COMMAND_DISPLAY_ALL,keyword);
					} else if(identifier == "load") {
						setCommandKeyword(Tb::COMMAND_LOAD,keyword);
					} else if(identifier == "save") {
						setCommandKeyword(Tb::COMMAND_SAVE,keyword);
					} else if(identifier == "exit") {
						setCommandKeyword(Tb::COMMAND_EXIT,keyword);
					}
				}
			}
		}
	}
	TbLogger::getInstance()->log(SYS,"Looking up command: " + Tb::COMMAND_ADD);
	return;
}

void IO::saveCustomCommands(std::ofstream& tbconfig) {
	tbconfig << std::endl;
	tbconfig << "Custom Commands" << std::endl;

	// Count: 16
	if("add" != Tb::COMMAND_ADD) {
		tbconfig << "add" << " " << Tb::COMMAND_ADD << std::endl;
	} else if("delete" != Tb::COMMAND_DELETE) {
		tbconfig << "delete" << " " << Tb::COMMAND_DELETE << std::endl;
	} else if("modify" != Tb::COMMAND_MODIFY) {
		tbconfig << "modify" << " " << Tb::COMMAND_MODIFY << std::endl;
	} else if("edit" != Tb::COMMAND_MODIFY_EDIT) {
		tbconfig << "edit" << " " << Tb::COMMAND_MODIFY_EDIT << std::endl;
	} else if("pick" != Tb::COMMAND_PICK_RESERVE) {
		tbconfig << "pick" << " " << Tb::COMMAND_PICK_RESERVE << std::endl;
	} else if("search" != Tb::COMMAND_SEARCH) {
		tbconfig << "search" << " " << Tb::COMMAND_SEARCH << std::endl;
	} else if("done" != Tb::COMMAND_MARKDONE) {
		tbconfig << "done" << " " << Tb::COMMAND_MARKDONE << std::endl;
	} else if("notdone" != Tb::COMMAND_UNMARKDONE) {
		tbconfig << "notdone" << " " << Tb::COMMAND_UNMARKDONE << std::endl;
	} else if("undo" != Tb::COMMAND_UNDO) {
		tbconfig << "undo" << " " << Tb::COMMAND_UNDO << std::endl;
	} else if("redo" != Tb::COMMAND_REDO) {
		tbconfig << "redo" << " " << Tb::COMMAND_REDO << std::endl;
	} else if("view" != Tb::COMMAND_VIEW) {
		tbconfig << "view" << " " << Tb::COMMAND_VIEW << std::endl;
	} else if("clear" != Tb::COMMAND_CLEAR_ALL) {
		tbconfig << "clear" << " " << Tb::COMMAND_CLEAR_ALL << std::endl;
	} else if("display" != Tb::COMMAND_DISPLAY_ALL) {
		tbconfig << "display" << " " << Tb::COMMAND_DISPLAY_ALL << std::endl;
	} else if("load" != Tb::COMMAND_LOAD) {
		tbconfig << "load" << " " << Tb::COMMAND_LOAD << std::endl;
	} else if("save" != Tb::COMMAND_SAVE) {
		tbconfig << "save" << " " << Tb::COMMAND_SAVE << std::endl;
	} else if("exit" != Tb::COMMAND_EXIT) {
		tbconfig << "exit" << " " << Tb::COMMAND_EXIT << std::endl;
	}
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
