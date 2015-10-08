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
	CommandType cmdType		= Utilities::stringToCmdType(Utilities::getFirstWord(userInput));
	std::string restOfInput = Utilities::removeFirstWord(userInput);

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
			Task task = parseTask(restOfInput);
			addCmd = new Add(task,userInput); // userInput included for testing
			return addCmd;
		}
		catch(std::string NullTaskString) {
			std::cerr << NullTaskString << std::endl;
		}
		break;

	case DELETE:
		try {
			if(!Utilities::isPositiveNonZeroInt(restOfInput)) {
				throw "Invalid integer string";
			}

			int deleteID = Utilities::stringToInt(restOfInput);
			deleteCmd = new Delete(deleteID);
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
			int modifyID = Utilities::stringToInt(Utilities::getFirstWord(restOfInput));
			std::string tempTaskString = Utilities::removeFirstWord(restOfInput);
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
			std::string searchPhrase = restOfInput;
			searchCmd = new Search(searchPhrase);
			return searchCmd;
		}
		catch(std::string NullSearchString) {
			std::cerr << NullSearchString << std::endl;
		}
		break;

	case CLEAR_ALL:
		clearCmd = new ClearAll;
		return clearCmd;

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
	Command* cmd = new Command(INVALID,userInput);
	return cmd;
}

Task Parser::parseTask(std::string restOfCommand) {
	std::vector<std::string> userInput = Utilities::splitParameters(restOfCommand);
	std::vector<std::string>::iterator curr = userInput.begin();
	Task newTask;
	FieldType inputMode = NAME;
	std::vector<std::string> inputString;

	int newStartDate = INVALID_DATE_FORMAT;
	int newEndDate = INVALID_DATE_FORMAT;

	while(curr != userInput.end()) {
		inputString.clear();

		while(curr != userInput.end()
			&& !Utilities::equalsIgnoreCase(*curr, FIELD_NAME)
			&& !Utilities::equalsIgnoreCase(*curr, FIELD_DATE_BY)
			&& !Utilities::equalsIgnoreCase(*curr, FIELD_DATE_ON)
			&& !Utilities::equalsIgnoreCase(*curr, FIELD_TIME_AT)
			&& !Utilities::equalsIgnoreCase(*curr, FIELD_TIME_FROM)
			&& !Utilities::equalsIgnoreCase(*curr, FIELD_TIME_TO)
			&& !Utilities::equalsIgnoreCase(*curr, FIELD_LABEL)) {
				inputString.push_back(*curr);
				curr++;
		}

		switch(inputMode) {
		case NAME:
			newTask.setName(Utilities::vecToString(inputString));
			break;
		case START_DATE :
		case START_DAY:
			newTask.setType(EVENT);
			if(    (newStartDate = parseDate(inputString)) != INVALID_DATE_FORMAT
				|| (newStartDate = parseDay(inputString)) != INVALID_DATE_FORMAT) {
					newTask.setStartDate(newStartDate);
			}
			break;
		case END_DATE :
		case END_DAY :
			if(newTask.getType() == FLOATING) {
				newTask.setType(TODO);
			}
			if(    (newEndDate = parseDate(inputString)) != -1
				|| (newEndDate = parseDay(inputString)) != -1) {
					newTask.setEndDate(newEndDate);
			}			break;
		case START_TIME :
		case END_TIME :
			break;
		case PRIORITY:
			newTask.togglePriority();
			break;
		case LABEL:
			newTask.setLabel(Utilities::vecToString(inputString));
			break;
		default:
			break;
		}

		if(curr == userInput.end()) {
			break;
		} else if(Utilities::equalsIgnoreCase(*curr, FIELD_DATE_BY)
			|| Utilities::equalsIgnoreCase(*curr, FIELD_DATE_ON)) {
				inputMode = END_DATE;
		} else if(Utilities::equalsIgnoreCase(*curr, FIELD_TIME_FROM)) {
			inputMode = START_TIME;
		} else if(Utilities::equalsIgnoreCase(*curr, FIELD_TIME_AT)) {
			inputMode = END_TIME;
		} else if(Utilities::equalsIgnoreCase(*curr, FIELD_TIME_TO)) {
			inputMode = END_TIME;
		} else if(Utilities::equalsIgnoreCase(*curr, FIELD_PRIORITY)) {
			inputMode = PRIORITY;
		} else if(Utilities::equalsIgnoreCase(*curr, FIELD_LABEL)) {
			inputMode = LABEL;
		}
		curr++;
	}

	return newTask;
}


// These functions support user methods

std::vector<FieldType> Parser::extractFields(std::string restOfInput) {
	std::vector<std::string> vecInput = Utilities::splitParameters(restOfInput);
	std::vector<std::string>::iterator curr = vecInput.begin();
	std::vector<FieldType> fields;

	if(Utilities::stringToFieldType(restOfInput) == INVALID_FIELD) {
		fields.push_back(NAME);
	}

	while(curr != vecInput.end()) {
		FieldType newField = Utilities::stringToFieldType(*curr);
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

// Returns year in YY
// Throws exception if invalid yearString
int Parser::findYear(std::string yearString) {
	time_t t = time(0); // get current time
	struct tm now;
	localtime_s(&now,&t);
	int year = now.tm_year - 100; // get current year, tm_year: years since 1900

	if(yearString == "") {
		return year;
	} else if(Utilities::isPositiveNonZeroInt(yearString)) {
		if(Utilities::stringToInt(yearString) == year
			|| Utilities::stringToInt(yearString) == 2000+year) {
				return year;
		} else if(Utilities::stringToInt(yearString) == year+1
			|| Utilities::stringToInt(yearString) == 2000+year+1) {
				return year+1;
		}
	} else {
		throw("Invalid year string: "+yearString+"\n");
	}
	return year;
}

// These handle task parameters

// Processes dates in these formats:
// - DDD DDDD
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
		*curr = Utilities::stringToLower(*curr);
	}
	curr = dateString.begin();

	int dateInput = 0;
	Month monthInput = INVALID_MONTH;
	int yearInput = 0;

	int maxDays = 0;
	int newDate = INVALID_DATE_FORMAT;

	// Get dateInput
	if(curr!=dateString.end() && Utilities::isInt(*curr)) {

		dateInput = Utilities::stringToInt(*curr);
		curr++;

		// Get monthInput
		if(curr!=dateString.end()) {
			monthInput = Utilities::stringToMonth(*curr);
			if(monthInput != INVALID_MONTH) {
				maxDays = findMaxDays(monthInput);
			}
			curr++;

			// Get yearInput
			if(curr==dateString.end()) {
				yearInput = findYear();
			} else if(Utilities::isInt(*curr)) {
				yearInput = findYear(*curr);
				curr++;
			}
		}
	}

	if(curr == dateString.end()
		&& yearInput != 0
		&& monthInput != INVALID_MONTH
		&& (1<=dateInput && dateInput<=maxDays)) {
			newDate = yearInput*10000 + (int)monthInput*100 + dateInput;
	}
	return newDate;
}

// Processes dates in these formats:
// - DDD/DDDD
// - this/next DDD/DDDD
// - DD MM/MMM/MMMM
// Week is defined as Sunday to Saturday
// Returns -1 if invalid date
/*
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
*/

// Processes dates in these formats:
// - DDD/DDDD
// - this/next DDD/DDDD
// - DD MM/MMM/MMMM
// Week is defined as Sunday to Saturday
// Returns -1 if invalid date
int Parser::parseDay(std::vector<std::string> dayString) {
	if(dayString.empty()) {
		return INVALID_DATE_FORMAT;
	}

	std::vector<std::string>::iterator curr;
	for(curr=dayString.begin(); curr!=dayString.end(); curr++) {
		*curr = Utilities::stringToLower(*curr);
	}
	curr = dayString.begin();

	int maxDays = 0;
	int newDate = INVALID_DATE_FORMAT;

	time_t t = time(0); // get current time
	struct tm now;
	localtime_s(&now,&t);
	int year = now.tm_year - 100; // get current year, tm_year: years since 1990

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

	if(curr!=dayString.end()) {
		Day newDay = Utilities::stringToDay(*curr);
		if(newDay != INVALID_DAY) {
			date += (int)newDay - (int)day;
			curr++;
		}
	}

	if(curr == dayString.end()) {
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

	return newDate;
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
	if(!Utilities::isPositiveNonZeroInt(*curr)) {
		// HH.MM AM/PM
		size_t iSemiColon = (*curr).find('.');
		if(iSemiColon == std::string::npos) {
			return INVALID_TIME_FORMAT;
		} else {
			hourString = (*curr).substr(0, iSemiColon);
			minString = (*curr).substr(iSemiColon + 1);
			hour = Utilities::stringToInt(hourString);
			min = Utilities::stringToInt(minString);
			if(!Utilities::isPositiveNonZeroInt(hourString) || hour >= 24 || min >= 60) {
				return INVALID_TIME_FORMAT;
			}
		}
	} else {
		time = Utilities::stringToInt(*curr);
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
		if(Utilities::containsAny(*curr,"am") && hour <= 12) {
			if(hour == 12) {
				hour -= 12;
			}
		} else if(Utilities::containsAny(*curr,"pm") && hour <= 12) {
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
