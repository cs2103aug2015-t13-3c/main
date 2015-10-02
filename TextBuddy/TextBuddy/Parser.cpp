// @@author Aaron Chong Jun Hao

#include <ctime>
#include "stdafx.h"
#include "Parser.h"

Parser::Parser() {}
Parser::~Parser() {}

// This defines the file extension used by TextBuddy
const std::string Parser::FILE_EXTENSION = ".txt";

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

Command Parser::parse(std::string userInput) {
	// userInput = u.removeSpaces(userInput);
	std::string cmdString	= u.getFirstWord(userInput);
	CommandType cmdType		= u.stringToCmdType(cmdString);

	Command* cmd = new Command;
	std::string restOfInput = u.removeFirstWord(userInput);

	switch(cmdType) {
	case ADD:
		try {
			if(restOfInput=="") {
				throw "No task to add";
			}
			cmd = new Add;
			Task task = parseTask(restOfInput);
			cmd->setNewTask(task);
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
			cmd = new Delete;
			int deleteID = u.stringToInt(restOfInput);
			cmd->setTaskToDelete(deleteID);
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
			cmd = new Modify;
			Task task = parseTask(restOfInput);
			cmd->setTempTask(task);
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
			cmd = new Search;
			std::string searchPhrase;
			cmd->setSearchPhrase(searchPhrase);
		}
		catch(std::string NullSearchString) {
			std::cerr << NullSearchString << std::endl;
		}
		break;
	case CLEAR_ALL:
		cmd->setCmdType(CLEAR_ALL);
		break;
	case DISPLAY_ALL:
		cmd->setCmdType(DISPLAY_ALL);
		break;
	case SORT_ALL:
		cmd->setCmdType(SORT_ALL);
		break;
	case EXIT:
		exit(0);
	default:
		break;
	}

	return *cmd;
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

	while(curr != userInput.end()) {
		while(curr != userInput.end()
			&& !u.equalsIgnoreCase(*curr, FIELD_DATE_BY)
			&& !u.equalsIgnoreCase(*curr, FIELD_DATE_ON)
			&& !u.equalsIgnoreCase(*curr, FIELD_TIME_AT)
			&& !u.equalsIgnoreCase(*curr, FIELD_TIME_FROM)
			&& !u.equalsIgnoreCase(*curr, FIELD_TIME_TO)
			&& !u.equalsIgnoreCase(*curr, FIELD_LABEL)) {
				inputString.push_back(*curr);
				curr++;
		}

		int newStartDate;
		int newEndDate;

		switch(inputMode) {
		case NAME:
			newTask.setName(u.vecToString(inputString));
			break;
		case START_DATE :
		case START_DAY:
			newTask.setType(EVENT);
			newStartDate = parseDate(inputString);
			newTask.setStartDate(newStartDate);
			break;
		case END_DATE :
		case END_DAY :
			if(newTask.getType() == FLOATING) {
				newTask.setType(TODO);
			}
			newEndDate = parseDate(inputString);
			newTask.setEndDate(newEndDate);
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
	default:
		break;
	}
	return maxDays;
}

// Processes dates in these formats:
// - DDD/DDDD
// - this/next DDD/DDDD
// - DD MM/MMM/MMMM
// Week is defined as Sunday to Saturday
// Returns 0 if invalid date
int Parser::parseDate(std::vector<std::string> inputString) {
	Utilities u;

	if(inputString.empty()) {
		return INVALID_DATE_FORMAT;
	}

	std::vector<std::string>::iterator curr;
	for(curr=inputString.begin(); curr!=inputString.end(); curr++) {
		*curr = u.stringToLower(*curr);
	}

	int newDate = INVALID_DATE_FORMAT;
	int maxDays = 0;

	time_t t = time(0); // get current time
	struct tm now;
	localtime_s(&now,&t);
	int year = now.tm_year - 100; // get current year, tm_year: years since 1990

	curr = inputString.begin();
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

		if(curr!=inputString.end()) {
			Day newDay = u.stringToDay(*curr);
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

			// Check that valid date
			if(1<=date && date<=maxDays) {
			newDate = year*10000 + (int)month*100 + date;
			}
		}
	}

	return newDate;
}
