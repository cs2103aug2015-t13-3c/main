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


// Processes dates in these formats:
// - DDD/DDDD
// - this/next DDD/DDDD
// - DD MM/MMM/MMMM
// Week is defined as Sunday to Saturday
// Returns 0 if invalid date
int Parser::parseDate(std::vector<std::string> inputString) {
	if(inputString.empty()) {
		return 0;
	}

	std::vector<std::string>::iterator curr;
	for(curr=inputString.begin(); curr!=inputString.end(); curr++) {
		std::transform((*curr).begin(), (*curr).end(), (*curr).begin(), ::tolower);
	}
	int newDate = 0;
	int maxDays = 0;

	time_t t = time(0); // get current time
	struct tm now;
	localtime_s(&now,&t);
	int year = now.tm_year - 100; // get current year, tm_year: years since 1990

	curr = inputString.begin();
	if(!((*curr).empty()) && (*curr).find_first_not_of("0123456789")==std::string::npos) {
		int dateInput = parseInt(*curr);
		curr++;

		Month monthInput = findMonth(*curr);
		if(monthInput != INVALID_MONTH) {
			maxDays = findMaxDays(monthInput);
		}

		if(0<dateInput && dateInput<=maxDays) {
			newDate = year*10000 + (int)monthInput*100 + dateInput;
		}

	} else {
		Month month =	(Month)(now.tm_mon + 1); // tm_mon:  Jan starts at 0
		Day day =		  (Day)(now.tm_wday);
		int date =				now.tm_mday;

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
			Day newDay = findDay(*curr);
			if(newDay != INVALID_DAY) {
				date += (int)newDay - (int)day;
				curr++;
			}
		}

		if(curr == inputString.end()) {
			maxDays = findMaxDays(month,year);
			if(date>maxDays) {
				month = (Month)(((int)month)+1);
				date-=maxDays;
			}

			Month maxMonth=DEC;
			if(month>maxMonth) {
				year++;
				month = (Month)((int)month-(int)maxMonth);
			}

			newDate = year*10000 + (int)month*100 + date;
		}
	}

	return newDate;
}

// Internal methods

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

int Parser::findMaxDays(Month month, int year) { // default year is 2015
	int maxDays;
	bool isLeap;
	switch(month) {
	case JAN:
	case MAR:
	case MAY:
	case JUL:
	case AUG:
	case OCT:
	case DEC:
		maxDays = 31;
		break;
	case APR:
	case JUN:
	case SEP:
	case NOV:
		maxDays = 30;
		break;
	case FEB:
		isLeap = ((year%4==0 && year%100!=0) || year%400==0);
		if(isLeap) {
			maxDays = 29;
		} else {
			maxDays = 28;
		}
	default:
		break;
	}
	return maxDays;
}

Month Parser::findMonth(std::string monthString) {
	Month monthInput = INVALID_MONTH;
	if(containsAny(monthString,"1 jan january")) {
		monthInput = JAN;
	} else if(containsAny(monthString,"2 feb february")) {
		monthInput = FEB;
	} else if(containsAny(monthString,"3 mar march")) {
		monthInput = MAR;
	} else if(containsAny(monthString,"4 apr april")) {
		monthInput = APR;
	} else if(containsAny(monthString,"5 may")) {
		monthInput = MAY;
	} else if(containsAny(monthString,"6 jun june")) {
		monthInput = JUN;
	} else if(containsAny(monthString,"7 jul july")) {
		monthInput = JUL;
	} else if(containsAny(monthString,"8 aug august")) {
		monthInput = AUG;
	} else if(containsAny(monthString,"9 sep sept september")) {
		monthInput = SEP;
	} else if(containsAny(monthString,"10 oct october")) {
		monthInput = OCT;
	} else if(containsAny(monthString,"11 nov november")) {
		monthInput = NOV;
	} else if(containsAny(monthString,"12 dec december")) {
		monthInput = DEC;
	}
	return monthInput;
}

Day Parser::findDay(std::string dayString) {
	Day newDay = INVALID_DAY;
	if(containsAny(dayString,"sun sunday")) {
		newDay = SUN;
	} else if(containsAny(dayString,"mon monday")) {
		newDay = MON;
	} else if(containsAny(dayString,"tue tues tuesday")) {
		newDay = TUE;
	} else if(containsAny(dayString,"wed wednesday")) {
		newDay = WED;
	} else if(containsAny(dayString,"thu thur thurs thursday")) {
		newDay = THU;
	} else if(containsAny(dayString,"fri friday")) {
		newDay = FRI;
	} else if(containsAny(dayString,"sat saturday")) {
		newDay = SAT;
	}
	return newDay;
}

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