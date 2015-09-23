// @@author Aaron Chong Jun Hao

#include "stdafx.h"
#include <ctime>
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
const std::string Parser::FIELD_DATE_BY = "by";
const std::string Parser::FIELD_DATE_ON = "on";
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
	std::vector<std::string>::iterator curr = userInput.begin();
	Task newTask;
	FieldType inputMode = NAME;
	std::vector<std::string> inputString;

	while(curr != userInput.end()) {
		while(!equalsIgnoreCase(*curr, FIELD_DATE_BY)
			&& !equalsIgnoreCase(*curr, FIELD_DATE_ON)
			&& !equalsIgnoreCase(*curr, FIELD_TIME_AT)
			&& !equalsIgnoreCase(*curr, FIELD_TIME_FROM)
			&& !equalsIgnoreCase(*curr, FIELD_TIME_TO)
			&& !equalsIgnoreCase(*curr, FIELD_LABEL)) {
				inputString.push_back(*curr);
		}

		int newEndDate = 0;

		switch(inputMode) {
		case NAME:
			newTask.setName(vecToString(inputString));
		case START_DATE :
		case END_DATE :
		case START_DAY:
		case END_DAY :
			// if got from, newTask.setType(EVENT);
			// else newTask.setType(TODO);
			newEndDate = parseDate(inputString);
			newTask.setEndDate(newEndDate);
			break;
		case START_TIME :
		case END_TIME :
		case PRIORITY:
			//newTask.togglePriority();
		case LABEL:
			//newTask.setLabel(inputString);
		default:
			break;
		}

		if(equalsIgnoreCase(*curr, FIELD_DATE_BY)
			|| equalsIgnoreCase(*curr, FIELD_DATE_ON)) {
				inputMode = END_DATE;
		} else if(equalsIgnoreCase(*curr, FIELD_TIME_FROM)) {
			inputMode = START_TIME;
		} else if(equalsIgnoreCase(*curr, FIELD_TIME_AT)) {
			inputMode = END_TIME;
		} else if(equalsIgnoreCase(*curr, FIELD_TIME_TO)) {
			inputMode = END_TIME;
		} else if(equalsIgnoreCase(*curr, FIELD_PRIORITY)) {
			inputMode = PRIORITY;
		} else if(equalsIgnoreCase(*curr, FIELD_LABEL)) {
			inputMode = LABEL;
		}
	}

	return newTask;
}

std::string Parser::vecToString(std::vector<std::string> inputString) {
	std::string newString;
	std::vector<std::string>::iterator curr;
	for(curr=inputString.begin(); curr!=inputString.end(); ) {
		newString += *curr;
		if(++curr != inputString.end()) {
			newString += " ";
		}
	}
	return newString;
}


// Processes dates in the forms:
// - this/next DDD
// - DD MMM/MMMM
int Parser::parseDate(std::vector<std::string> inputString) {
	std::vector<std::string>::iterator curr;
	for(curr=inputString.begin(); curr!=inputString.end(); curr++) {
		std::transform((*curr).begin(), (*curr).end(), (*curr).begin(), ::tolower);
	}
	int newDate = 0;	

	time_t t = time(0); // get time now
	struct tm now;
	localtime_s(&now,&t);
	int year =	now.tm_year - 100; // tm_year: years since 1990
	int month =	now.tm_mon + 1;    // tm_mon:  Jan starts at 0
	int date =	now.tm_mday;
	Day day =	(Day)(now.tm_wday);
	// int sec =	now->tm_sec;
	// int min =	now->tm_min;
	// int hour =	now->tm_hour;

	curr = inputString.begin();
	if(*curr == "tmr" || *curr == "tomorrow") {
		date++;
		curr++;
	} else if(*curr == "this") {
		curr++;
	} else if(*curr == "next") {
		date += 7;
		curr++;
	}

	if(curr!=inputString.end()) {
		if(containsAny(*curr,"sun sunday")) {
			date += (int)SUN - (int)day;
			curr++;
		} else if(containsAny(*curr,"mon monday")) {
			date += (int)MON - (int)day;
			curr++;
		} else if(containsAny(*curr,"tue tues tuesday")) {
			date += (int)TUE - (int)day;
			curr++;
		} else if(containsAny(*curr,"wed wednesday")) {
			date += (int)WED - (int)day;
			curr++;
		} else if(containsAny(*curr,"thu thur thurs thursday")) {
			date += (int)THU - (int)day;
			curr++;
		} else if(containsAny(*curr,"fri friday")) {
			date += (int)FRI - (int)day;
			curr++;
		} else if(containsAny(*curr,"sat saturday")) {
			date += (int)SAT - (int)day;
			curr++;
		}
	}

	int maxDays;
	bool isLeap = ((year%4==0 && year%100!=0) || year%400==0);
	switch(month) {
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		maxDays = 31;
		break;
	case 4:
	case 6:
	case 9:
	case 11:
		maxDays = 30;
		break;
	case 2:
		if(isLeap) {
			maxDays = 29;
		} else {
			maxDays = 28;
		}
	default:
		break;
	}

	if(date>maxDays) {
		month++;
		date-=maxDays;
	}

	if(curr == inputString.end()) {
		newDate = year*10000 + month*100 + date;
		// newTime = hour*10000 + min*100 + sec;
	}

	return newDate;
}

bool Parser::containsAny(std::string targetWord, std::string searchWords) {
	std::vector<std::string> vecSearchWords = splitParameters(searchWords);
	std::vector<std::string>::iterator curr;

	for(curr=vecSearchWords.begin(); curr!=vecSearchWords.end(); curr++) {
		if(targetWord == *curr) {
			return true;
		}
	}

	return false;
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