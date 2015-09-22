// @@author Aaron Chong Jun Hao

#include "stdafx.h"
#include "Parser.h"

Parser::Parser() {
	isActive = false;
}

Parser::Parser(bool status): isActive(status) {}

Parser::~Parser() {}

bool Parser::getStatus() {
	return isActive;
}

// This defines the file extension used by TextBuddy
const std::string Parser::FILE_EXTENSION = ".txt";

// These are the possible command types
const std::string Parser::COMMAND_ADD = "add";
const std::string Parser::COMMAND_DELETE = "delete";
const std::string Parser::COMMAND_SEARCH = "search";
const std::string Parser::COMMAND_CLEAR_ALL = "clear";
const std::string Parser::COMMAND_DISPLAY_ALL = "display";
const std::string Parser::COMMAND_SORT_ALL = "sort";
const std::string Parser::COMMAND_EXIT = "exit";

// These are the possible field types
const std::string Parser::FIELD_DATE = "by";
const std::string Parser::FIELD_DAY = "on";
const std::string Parser::FIELD_TIME_AT = "at";
const std::string Parser::FIELD_TIME_FROM = "from";
const std::string Parser::FIELD_TIME_TO = "to";
const std::string Parser::FIELD_PRIORITY = "star";
const std::string Parser::FIELD_LABEL = ":";

// ==================================================
//                      METHODS
// ==================================================

std::string Parser::parseFileName(char* argv[]) {
	std::string newFileName = argv[1];
	std::size_t fileExtensionPos = newFileName.size() - FILE_EXTENSION.size();
	if(argv[1] + fileExtensionPos == FILE_EXTENSION) 	{

		newFileName = argv[1];

	} else {
		newFileName = argv[1] + FILE_EXTENSION;
	}
	return newFileName;
}

Command Parser::parseCommand(std::string userCommand) {
	userCommand = removeSpaces(userCommand);
	std::string commandString = getFirstWord(userCommand);

	Command cmd;
	CommandType newCommand;

	if(equalsIgnoreCase(commandString, COMMAND_ADD))	{
		newCommand = ADD;
	} else if(equalsIgnoreCase(commandString, COMMAND_DELETE)) {
		newCommand = DELETE;
	} else if(equalsIgnoreCase(commandString, COMMAND_SEARCH)) {
		newCommand = SEARCH;
	} else if(equalsIgnoreCase(commandString, COMMAND_CLEAR_ALL)) {
		newCommand = CLEAR_ALL;
	} else if(equalsIgnoreCase(commandString, COMMAND_DISPLAY_ALL)) {
		newCommand = DISPLAY_ALL;
	} else if(equalsIgnoreCase(commandString, COMMAND_SORT_ALL)) {
		newCommand = SORT_ALL;
	} else if(equalsIgnoreCase(commandString, COMMAND_EXIT)) {
		newCommand = EXIT;
	} else {
		newCommand = INVALID;
	}

	return Command(newCommand,removeFirstWord(userCommand),userCommand);
}

Task Parser::parseTask(std::string restOfCommand) {
	std::vector<std::string> userInput = splitParameters(restOfCommand);
	std::vector<std::string>::iterator curr;
	Task newTask;
	FieldType inputMode = NAME;
	std::string inputString = "";

	while(curr != userInput.end()) {
		while(!equalsIgnoreCase(*curr, FIELD_DATE)
			&& !equalsIgnoreCase(*curr, FIELD_DAY)
			&& !equalsIgnoreCase(*curr, FIELD_TIME_AT)
			&& !equalsIgnoreCase(*curr, FIELD_TIME_FROM)
			&& !equalsIgnoreCase(*curr, FIELD_TIME_TO)
			&& !equalsIgnoreCase(*curr, FIELD_LABEL)) {
				inputString += *curr;
		}

		switch(inputMode) {
//enum FieldType has changed
/*
		case NAME:
			newTask.setName(restOfCommand);
		case START_DATE :
			break;
		case END_DATE :
			break;
		case START_DAY:
			break;
		case END_DAY :
			break;
		case START_TIME :
			break;
		case END_TIME :
			break;
*/

/*
			case DATE:
			case DAY:
			//newTask.toggleFloating();
			//newTask.setDate(inputString);
			case TIME_AT:
			//newTask.setTimeAt(inputString);
			case TIME_FROM:
			//newTask.setTimeFrom(inputString);
			case TIME_TO:
			//newTask.setTimeTo(inputString);
			case PRIORITY:
			//newTask.togglePriority();
			case LABEL:
			//newTask.setLabel(inputString);
		default:
			break;
*/
		}
/*
		if(equalsIgnoreCase(*curr, FIELD_DATE)) {
			inputMode = DATE;
		} else if(equalsIgnoreCase(*curr, FIELD_DAY)) {
			inputMode = DAY;
		} else if(equalsIgnoreCase(*curr, FIELD_TIME_AT)) {
			inputMode = TIME_AT;
		} else if(equalsIgnoreCase(*curr, FIELD_TIME_FROM)) {
			inputMode = TIME_FROM;
		} else if(equalsIgnoreCase(*curr, FIELD_TIME_TO)) {
			inputMode = TIME_TO;
		} else if(equalsIgnoreCase(*curr, FIELD_TIME_TO)) {
			inputMode = PRIORITY;
		} else if(equalsIgnoreCase(*curr, FIELD_LABEL)) {
			inputMode = LABEL;
		}
*/
	}

	return newTask;
}


// Internal methods
// Credits: Adapted from CityConnect.cpp (CS2103 Tutorial 2)

bool Parser::isPositiveAndValidInt(std::string s) {
	int i = parseInt(s);

	if(i == INVALID_NUMBER_FORMAT || i <= 0 ) {
		return false;
	} else {
		return true;
	}
}

// This method only splits strings based on delimiter space
std::vector<std::string> Parser::splitParameters(std::string commandParametersString) {
	std::vector<std::string> tokens;
	std::istringstream iss(commandParametersString);
	std::copy(std::istream_iterator<std::string>(iss),
		std::istream_iterator<std::string>(),
		std::back_inserter<std::vector<std::string>>(tokens));
	return tokens;
}

std::string Parser::getFirstWord(std::string userCommand) {
	return splitParameters(userCommand)[0];
}

std::string Parser::removeFirstWord(std::string userCommand) {
	std::string commandTypeString = getFirstWord(userCommand);
	std::string parameters = removeSpaces(replace(userCommand, commandTypeString, ""));
	return parameters;
}

std::string Parser::removeSpaces(const std::string& s, const std::string& delimiters) {
	if(!s.empty()) {
		std::string trimEnd = s.substr(0, s.find_last_not_of(delimiters) + 1);
		std::string trimStart = trimEnd.substr(trimEnd.find_first_not_of(delimiters));
		return trimStart;
	} else {
		return s;
	}
}

bool Parser::equalsIgnoreCase(const std::string& str1, const std::string& str2) {
	if(str1.size() != str2.size()) {
		return false;
	} else {
		for (std::string::const_iterator c1 = str1.begin(), c2 = str2.begin(); c1 != str1.end(); ++c1 , ++c2) {
			if(tolower(*c1) != tolower(*c2)) {
				return false;
			}
		}
	}
	return true;
}

int Parser::parseInt(std::string str) {
	char c;
	int i = 0;
	std::stringstream ss(str);
	ss >> i;
	if(ss.fail() || ss.get(c)) {
		return INVALID_NUMBER_FORMAT;
	} else {
		return i;
	}
}

std::string Parser::replace(std::string a, std::string b, std::string c) {
	int pos;
	do {
		pos = a.find(b);
		if(pos != -1) a.replace(pos, b.length(), c);
	} while (pos != -1);
	return a;
}