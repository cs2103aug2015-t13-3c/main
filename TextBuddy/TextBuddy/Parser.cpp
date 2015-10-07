// @@author Aaron Chong Jun Hao

#include <ctime>
#include "stdafx.h"
#include "Parser.h"

Parser::Parser() {}
Parser::~Parser() {}

// This defines the file extension used by TextBuddy
const std::string Parser::FILE_EXTENSION = ".txt";

// ==================================================
//                      METHODS
// ==================================================

// This is the API

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

Command* Parser::parse(std::string userInput) {
	// userInput = u.removeSpaces(userInput);
	std::string cmdString	= u.getFirstWord(userInput);
	CommandType cmdType		= u.stringToCmdType(cmdString);

	std::string restOfInput = u.removeFirstWord(userInput);

	// Declare pointers first, rather than objects
	Add* addCmd;
	Delete* deleteCmd;
	Modify* modifyCmd;
	Search* searchCmd;
	ClearAll* clearCmd;
	DisplayAll* displayCmd;
	SortAll* sortCmd;
	Exit* exitCmd;

	switch(cmdType) {
	case ADD:
		try {
			if(restOfInput=="") {
				throw "No task to add";
			}
			addCmd = new Add(userInput); // userInput included for testing
			Task task = parseTask(restOfInput);
			addCmd->setNewTask(task);
			return addCmd;
		}
		catch(std::string NullTaskString) {
			std::cerr << NullTaskString << std::endl;
		}
		break;

	case DELETE:
		try {
			if(!u.isPositiveNonZeroInt(restOfInput)) {
				throw "Invalid integer string";
			}
			deleteCmd = new Delete;
			int deleteID = u.stringToInt(restOfInput);
			deleteCmd->setDeleteID(deleteID);
			return deleteCmd;
		}
		catch(std::string InvalidIntString) {
			std::cerr << InvalidIntString << std::endl;
		}
		break;

	case MODIFY:
		try {
			if(restOfInput=="") {
				throw "No fields to modify";
			}
			int modifyID = u.stringToInt(u.getFirstWord(restOfInput));
			std::string tempTaskString = u.removeFirstWord(restOfInput);
			std::vector<FieldType> fieldsToModify = extractFields(tempTaskString);
			Task tempTask = parseTask(tempTaskString);
			modifyCmd = new Modify(modifyID,fieldsToModify,tempTask);
			return modifyCmd;
		}
		catch(std::string NullModifyString) {
			std::cerr << NullModifyString << std::endl;
		}
		break;

	case SEARCH:
		try {
			if(restOfInput=="") {
				throw "No search phrase";
			}
			searchCmd = new Search;
			std::string searchPhrase = restOfInput;
			searchCmd->setSearchPhrase(searchPhrase);
			return searchCmd;
		}
		catch(std::string NullSearchString) {
			std::cerr << NullSearchString << std::endl;
		}
		break;

	case CLEAR_ALL:
		clearCmd = new ClearAll;
		return clearCmd;
		// break;

	case DISPLAY_ALL:
		displayCmd = new DisplayAll;
		return displayCmd;

	case SORT_ALL:
		sortCmd = new SortAll;
		return sortCmd;

	case EXIT:
		exitCmd = new Exit;
		return exitCmd;

	case INVALID:
		break;
	}

	// Invalid command
	Command* cmd = new Command;
	return cmd;
}

Command Parser::parseCommand(std::string userCommand) {
	// userCommand = u.removeSpaces(userCommand);
	std::string cmdString	= u.getFirstWord(userCommand);
	std::string restOfInput = u.removeFirstWord(userCommand);
	CommandType newCommand	= u.stringToCmdType(cmdString);

	Command cmd(newCommand,restOfInput,userCommand);
	return cmd;
}

Task Parser::parseTask(std::string restOfCommand) {
	std::vector<std::string> userInput = u.splitParameters(restOfCommand);
	std::vector<std::string>::iterator curr = userInput.begin();
	Task newTask;
	FieldType inputMode = NAME;
	std::vector<std::string> inputString;

	int newStartDate;
	Day newStartDay;
	int newEndDate;
	Day newEndDay;

	while(curr != userInput.end()) {
		inputString.clear();

		while(curr != userInput.end()
			&& !u.equalsIgnoreCase(*curr, FIELD_NAME)
			&& !u.equalsIgnoreCase(*curr, FIELD_DATE_BY)
			&& !u.equalsIgnoreCase(*curr, FIELD_DATE_ON)
			&& !u.equalsIgnoreCase(*curr, FIELD_TIME_AT)
			&& !u.equalsIgnoreCase(*curr, FIELD_TIME_FROM)
			&& !u.equalsIgnoreCase(*curr, FIELD_TIME_TO)
			&& !u.equalsIgnoreCase(*curr, FIELD_LABEL)) {
				inputString.push_back(*curr);
				curr++;
		}

		switch(inputMode) {
		case NAME:
			newTask.setName(u.vecToString(inputString));
			break;
		case START_DATE :
		case START_DAY:
			newTask.setType(EVENT);
			newStartDate = parseDate(inputString);
			newStartDay = parseDay(newStartDate);
			
			newTask.setStartDate(newStartDate);
			newTask.setStartDay(newStartDay);
			break;
		case END_DATE :
		case END_DAY :
			if(newTask.getType() == FLOATING) {
				newTask.setType(TODO);
			}
			newEndDate = parseDate(inputString);
			newEndDay = parseDay(newEndDate);

			newTask.setEndDate(newEndDate);
			newTask.setEndDay(newEndDay);
			break;
		case START_TIME :
		case END_TIME :
			break;
		case PRIORITY:
			newTask.togglePriority();
			break;
		case LABEL:
			newTask.setLabel(u.vecToString(inputString));
			break;
		default:
			break;
		}

		if(curr == userInput.end()) {
			break;
		} else if(u.equalsIgnoreCase(*curr, FIELD_DATE_BY)
			|| u.equalsIgnoreCase(*curr, FIELD_DATE_ON)) {
				inputMode = END_DATE;
		} else if(u.equalsIgnoreCase(*curr, FIELD_TIME_FROM)) {
			inputMode = START_TIME;
		} else if(u.equalsIgnoreCase(*curr, FIELD_TIME_AT)) {
			inputMode = END_TIME;
		} else if(u.equalsIgnoreCase(*curr, FIELD_TIME_TO)) {
			inputMode = END_TIME;
		} else if(u.equalsIgnoreCase(*curr, FIELD_PRIORITY)) {
			inputMode = PRIORITY;
		} else if(u.equalsIgnoreCase(*curr, FIELD_LABEL)) {
			inputMode = LABEL;
		}
		curr++;
	}

	return newTask;
}


// These functions support user methods

std::vector<FieldType> Parser::extractFields(std::string restOfInput) {
	std::vector<std::string> vecInput = u.splitParameters(restOfInput);
	std::vector<std::string>::iterator curr = vecInput.begin();
	std::vector<FieldType> fields;

	if(u.stringToFieldType(restOfInput) == INVALID_FIELD) {
		fields.push_back(NAME);
	}

	while(curr != vecInput.end()) {
		FieldType newField = u.stringToFieldType(*curr);
		if(newField != INVALID_FIELD) {
			fields.push_back(newField);
		}
		curr++;
	}

	return fields;
}

int Parser::findMaxDays(Month month, int year) { // default year is 2015
	int maxDays = 0;
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
	case INVALID_MONTH:
		break;
	}
	return maxDays;
}


// These handle task parameters

// Processes dates in these formats:
// - DDD/DDDD
// - this/next DDD/DDDD
// - DD MM/MMM/MMMM
// Week is defined as Sunday to Saturday
// Returns -1 if invalid date
int Parser::parseDate(std::vector<std::string> dateString) {
	if(dateString.empty()) {
		return INVALID_DATE_FORMAT;
	}

	std::vector<std::string>::iterator curr;
	for(curr=dateString.begin(); curr!=dateString.end(); curr++) {
		*curr = u.stringToLower(*curr);
	}

	int newDate = INVALID_DATE_FORMAT;
	int maxDays = 0;

	time_t t = time(0); // get current time
	struct tm now;
	localtime_s(&now,&t);
	int year = now.tm_year - 100; // get current year, tm_year: years since 1990

	curr = dateString.begin();
	if(!((*curr).empty()) && (*curr).find_first_not_of("0123456789")==std::string::npos) {
		int dateInput = u.stringToInt(*curr);
		curr++;

		Month monthInput = u.stringToMonth(*curr);
		if(monthInput != INVALID_MONTH) {
			maxDays = findMaxDays(monthInput);
		}

		if(1<=dateInput && dateInput<=maxDays) {
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

		if(curr!=dateString.end()) {
			Day newDay = u.stringToDay(*curr);
			if(newDay != INVALID_DAY) {
				date += (int)newDay - (int)day;
				curr++;
			}
		}

		if(curr == dateString.end()) {
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

			// Check that valid date
			if(1<=date && date<=maxDays) {
				newDate = year*10000 + (int)month*100 + date;
			}
		}
	}

	return newDate;
}

// Processes dates in these formats:
// - DDD/DDDD
// - this/next DDD/DDDD
// - DD MM/MMM/MMMM
// Week is defined as Sunday to Saturday
// Returns -1 if invalid date
Day Parser::parseDay(int date) {
	// To-do: Check if valid date

	int year = date/10000 + 2000;
	int month = (date/100)%100;
	int day = date%100;

	tm timeStruct = {};
	timeStruct.tm_year = year - 1900;
	timeStruct.tm_mon = month - 1;
	timeStruct.tm_mday = day;
	timeStruct.tm_hour = 12;		//  To avoid any doubts about summer time, etc.
	mktime(&timeStruct);
	return (Day)timeStruct.tm_wday; //  0...6 for Sunday...Saturday
}


// Processes times in these formats:
// - HH    AM/PM (default: assume AM)
// - HH.MM AM/PM (default: assume AM)
// - HHMM        (24-hour)
// Returns -1 if invalid date
int Parser::parseTime(std::vector<std::string> timeString) {
	int time;
	std::string hourString;
	int hour;
	std::string minString;
	int min = 0;

	if(timeString.empty()) {
		return INVALID_TIME_FORMAT;
	}

	std::vector<std::string>::iterator curr = timeString.begin();
	if(!u.isPositiveNonZeroInt(*curr)) {
		// HH.MM AM/PM
		size_t iSemiColon = (*curr).find('.');
		if(iSemiColon == std::string::npos) {
			return INVALID_TIME_FORMAT;
		} else {
			hourString = (*curr).substr(0, iSemiColon);
			minString = (*curr).substr(iSemiColon + 1);
			hour = u.stringToInt(hourString);
			min = u.stringToInt(minString);
			if(!u.isPositiveNonZeroInt(hourString) || hour >= 24 || min >= 60) {
				return INVALID_TIME_FORMAT;
			}
		}
	} else {
		time = u.stringToInt(*curr);
		if(time < 13) {
			// HH AM/PM
			hour = time;
		} else if(time <= 2359) {
			// HHMM
			hour = time/100;
			min  = time%100;
			if(hour >= 24 || min >= 60) {
				return INVALID_TIME_FORMAT;
			}
		} else {
			return INVALID_TIME_FORMAT;
		}
	}

	curr++;
	if(curr != timeString.end()) {
		if(u.containsAny(*curr,"am") && hour <= 12) {
			if(hour == 12) {
				hour -= 12;
			}
		} else if(u.containsAny(*curr,"pm") && hour <= 12) {
			if(hour < 12) {
				hour += 12;
			}
		} else {
			return INVALID_TIME_FORMAT;
		}
		curr++;
	}

	if(curr == timeString.end()) {
		time = hour*100 + min;
	} else {
		return INVALID_TIME_FORMAT;
	}

	return time;
}
