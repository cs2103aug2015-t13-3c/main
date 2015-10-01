// @@author Ng Ren Zhi

#include "stdafx.h"
#include "IO.h"

IO::IO() {
	// isActive = false;
}

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
		std::string taskType = enumTypeToString(task.getType());
		std::string taskID = std::to_string(task.getID());
		std::string taskLabel = task.getLabel();
		std::string taskDoneStatus = boolToIntString(task.getDoneStatus());
		std::string taskPriorityStatus = boolToIntString(task.getPriorityStatus());
		std::string taskStartDay = enumDaytoString(task.getStartDay());
		std::string taskStartDate = std::to_string(task.getStartDate());
		std::string taskStartTime = std::to_string(task.getStartTime());
		std::string taskEndDay = enumDaytoString(task.getEndDay());
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
	success = task.setType(stringToEnumType(line));
	getline(inputFile,line);
	success = task.setID(stoi(line));
	getline(inputFile,line);
	success = task.setLabel(line);

	/*
	bool toggleDone();
	bool togglePriority();
	*/

	getline(inputFile,line);
	success = task.setStartDay(stringToEnumDay(line));
	getline(inputFile,line);
	success = task.setStartDate(stoi(line));
	getline(inputFile,line);
	success = task.setStartTime(stoi(line));

	getline(inputFile,line);
	success = task.setEndDay(stringToEnumDay(line));
	getline(inputFile,line);
	success = task.setEndDate(stoi(line));
	getline(inputFile,line);
	success = task.setEndTime(stoi(line));

	return task;
}


//Overloaded function
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

TaskType IO::stringToEnumType(std::string line) {
	TaskType type;

	if(line == "FLOATING") {
		type = FLOATING;
	} else if (line == "EVENT") {
		type = EVENT;
	} else if (line == "TODO") {
		type = TODO;
	}

	return type;
}
Day IO::stringToEnumDay(std::string line) {
	Day day = INVALID_DAY;

	if (line == "SUN") {
		day = SUN;
	} else if (line == "MON") {
		day = MON;
	} else if (line == "TUE") {
		day = TUE;
	} else if (line == "WED") {
		day = WED;
	} else if (line == "THU") {
		day = THU;
	} else if (line == "FRI") {
		day = FRI;
	} else if (line == "SAT") {
		day = SAT;
	} else if (line == "INVALID_DAY") {
		day = INVALID_DAY;
	}

	return day;
}
std::string IO::enumTypeToString(TaskType type) {
	std::string typeString;
	switch(type) {
	case FLOATING:
		typeString = "FLOATING";
		break;
	case EVENT:
		typeString = "EVENT";
		break;
	case TODO:
		typeString = "TODO";
		break;
	}

	return typeString;
}
std::string IO::enumDaytoString(Day day) {
	std::string dayString;
	switch(day) {
	case SUN:
		dayString = "SUN";
		break;
	case MON:
		dayString = "MON";
		break;
	case TUE:
		dayString = "TUE";
		break;
	case WED:
		dayString = "WED";
		break;
	case THU:
		dayString = "THU";
		break;
	case FRI:
		dayString = "FRI";
		break;
	case SAT:
		dayString = "SAT";
		break;
	case INVALID_DAY:
		dayString = "INVALID_DAY";
		break;

	}

	return dayString;
}
std::string IO::boolToIntString(bool boolean) {
	if(boolean) {
		return "1";
	} else {
		return "0";
	}
}


enum Enum{ Banana, Orange, Apple } ;
static const char * EnumStrings[] = { "bananas & monkeys", "Round and orange", "APPLE" };

const char * getTextForEnum( int enumVal )
{
	return EnumStrings[enumVal];
}


// ==================================================
//                      TO DELETE
// ==================================================

/*
IO::IO(bool status): isActive(status) {}

bool IO::getStatus() {
return isActive;
}
*/
