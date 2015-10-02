// @@author Aaron Chong Jun Hao

#include <ctime>
#include "stdafx.h"
#include "Parser.h"

Parser::Parser() {}
Parser::~Parser() {}

// This defines the file extension used by TextBuddy
const std::string Parser::FILE_EXTENSION = ".txt";

// These are the possible command types
const std::string Parser::COMMAND_ADD = "add";
const std::string Parser::COMMAND_DELETE = "delete";
const std::string Parser::COMMAND_MODIFY = "modify";
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

CommandType Parser::extractCmdType(std::string cmdString) {
	CommandType cmd;
	Utilities u;

	if(u.equalsIgnoreCase(cmdString, COMMAND_ADD))	{
		cmd = ADD;
	} else if(u.equalsIgnoreCase(cmdString, COMMAND_DELETE)) {
		cmd = DELETE;
	} else if(u.equalsIgnoreCase(cmdString, COMMAND_SEARCH)) {
		cmd = SEARCH;
	} else if(u.equalsIgnoreCase(cmdString, COMMAND_CLEAR_ALL)) {
		cmd = CLEAR_ALL;
	} else if(u.equalsIgnoreCase(cmdString, COMMAND_DISPLAY_ALL)) {
		cmd = DISPLAY_ALL;
	} else if(u.equalsIgnoreCase(cmdString, COMMAND_SORT_ALL)) {
		cmd = SORT_ALL;
	} else if(u.equalsIgnoreCase(cmdString, COMMAND_EXIT)) {
		cmd = EXIT;
	} else {
		cmd = INVALID;
	}

	return cmd;
}

Command Parser::parse(std::string userInput) {
	// userInput = u.removeSpaces(userInput);
	std::string cmdString = u.getFirstWord(userInput);
	std::string restOfInput = u.removeFirstWord(userInput);

	CommandType cmdType = extractCmdType(cmdString);
	Command* cmd;
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
		catch(std::string NullString) {
			std::cerr << NullString << std::endl;
		}
		break;
	case DELETE:
		try {
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
			cmd = new Modify;
		}
		catch(std::string e) {

		}
		break;
	case SEARCH:
		try {
			cmd = new Search;
		}
		catch(std::string e) {

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
	userCommand = u.removeSpaces(userCommand);
	std::string cmdString = u.getFirstWord(userCommand);

	Command cmd;
	CommandType newCommand;

	if(u.equalsIgnoreCase(cmdString, COMMAND_ADD))	{
		newCommand = ADD;
	} else if(u.equalsIgnoreCase(cmdString, COMMAND_DELETE)) {
		newCommand = DELETE;
	} else if(u.equalsIgnoreCase(cmdString, COMMAND_SEARCH)) {
		newCommand = SEARCH;
	} else if(u.equalsIgnoreCase(cmdString, COMMAND_CLEAR_ALL)) {
		newCommand = CLEAR_ALL;
	} else if(u.equalsIgnoreCase(cmdString, COMMAND_DISPLAY_ALL)) {
		newCommand = DISPLAY_ALL;
	} else if(u.equalsIgnoreCase(cmdString, COMMAND_SORT_ALL)) {
		newCommand = SORT_ALL;
	} else if(u.equalsIgnoreCase(cmdString, COMMAND_EXIT)) {
		newCommand = EXIT;
	} else {
		newCommand = INVALID;
	}

	return Command(newCommand,u.removeFirstWord(userCommand),userCommand);
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

		int newStartDate = 0;
		int newEndDate = 0;

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

// Processes dates in these formats:
// - DDD/DDDD
// - this/next DDD/DDDD
// - DD MM/MMM/MMMM
// Week is defined as Sunday to Saturday
// Returns 0 if invalid date
int Parser::parseDate(std::vector<std::string> inputString) {
	Utilities u;

	if(inputString.empty()) {
		return 0;
	}

	std::vector<std::string>::iterator curr;
	for(curr=inputString.begin(); curr!=inputString.end(); curr++) {
		*curr = u.stringToLower(*curr);
	}
	int newDate = 0;
	int maxDays = 0;

	time_t t = time(0); // get current time
	struct tm now;
	localtime_s(&now,&t);
	int year = now.tm_year - 100; // get current year, tm_year: years since 1990

	curr = inputString.begin();
	if(!((*curr).empty()) && (*curr).find_first_not_of("0123456789")==std::string::npos) {
		Utilities u;
		int dateInput = u.stringToInt(*curr);
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
	Utilities u;

	Month monthInput = INVALID_MONTH;
	if(u.containsAny(monthString,"1 jan january")) {
		monthInput = JAN;
	} else if(u.containsAny(monthString,"2 feb february")) {
		monthInput = FEB;
	} else if(u.containsAny(monthString,"3 mar march")) {
		monthInput = MAR;
	} else if(u.containsAny(monthString,"4 apr april")) {
		monthInput = APR;
	} else if(u.containsAny(monthString,"5 may")) {
		monthInput = MAY;
	} else if(u.containsAny(monthString,"6 jun june")) {
		monthInput = JUN;
	} else if(u.containsAny(monthString,"7 jul july")) {
		monthInput = JUL;
	} else if(u.containsAny(monthString,"8 aug august")) {
		monthInput = AUG;
	} else if(u.containsAny(monthString,"9 sep sept september")) {
		monthInput = SEP;
	} else if(u.containsAny(monthString,"10 oct october")) {
		monthInput = OCT;
	} else if(u.containsAny(monthString,"11 nov november")) {
		monthInput = NOV;
	} else if(u.containsAny(monthString,"12 dec december")) {
		monthInput = DEC;
	}
	return monthInput;
}

Day Parser::findDay(std::string dayString) {
	Utilities u;

	Day newDay = INVALID_DAY;
	if(u.containsAny(dayString,"sun sunday")) {
		newDay = SUN;
	} else if(u.containsAny(dayString,"mon monday")) {
		newDay = MON;
	} else if(u.containsAny(dayString,"tue tues tuesday")) {
		newDay = TUE;
	} else if(u.containsAny(dayString,"wed wednesday")) {
		newDay = WED;
	} else if(u.containsAny(dayString,"thu thur thurs thursday")) {
		newDay = THU;
	} else if(u.containsAny(dayString,"fri friday")) {
		newDay = FRI;
	} else if(u.containsAny(dayString,"sat saturday")) {
		newDay = SAT;
	}
	return newDay;
}
