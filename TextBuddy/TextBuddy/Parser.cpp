// @@author A0110376N (Aaron Chong Jun Hao)
// Parser converts flexible natural language into commands and parameters for TextBuddy.

#include "stdafx.h"
#include "Parser.h"

Parser* Parser::theOne;

Parser::Parser() {
	logger = TbLogger::getInstance();
	logger->setLogLevel(DEBUG);
	logger->log(SYS,"Parser instantiated");
}

Parser::~Parser() {
	logger->log(SYS,"Parser destructed");
	// logger->close(); // To be transferred to UI
}

// This defines the file extension used by TextBuddy
const std::string Parser::FILE_EXTENSION = ".txt";

void Parser::log(Level level, std::string message) {
	// logger->log(level,message);
	return;
}

//==================================================
//                      METHODS
//==================================================

//========== This is the API ==========

Parser* Parser::getInstance() {
	return theOne;
}

std::string Parser::parseFileName(char* argv[]) {
	char* charFilePath = argv[1];
	// Replace single backslash '\' with double backslash '\\'
	std::string newFilePath = Utilities::replace(charFilePath,"\\","\\\\");
	// Replace forwardslash '/' with double backslash '\\'
	newFilePath = Utilities::replace(newFilePath,"/","\\\\");
	// Find where it is supposed to be the file extension ".txt"
	std::size_t fileExtensionPos = newFilePath.size() - FILE_EXTENSION.size();
	// Append the file extension ".txt" if neccessary
	charFilePath = &newFilePath[0u];
	if(charFilePath + fileExtensionPos == FILE_EXTENSION) 	{
		newFilePath = charFilePath;
	} else {
		newFilePath = charFilePath + FILE_EXTENSION;
	}
	return newFilePath;
}

std::string Parser::parseFileName(std::string stringFilePath) {
	char* charFilePath = &stringFilePath[0u];
	char* argv[2]={"",charFilePath};
	return parseFileName(argv);
}

// Throws exceptions for:
// ADD		- NullTaskString	"No tasks to add!"
// DELETE	- InvalidIntString	"Invalid integer string!"
// MODIFY	- NullModifyString	"No fields to modify!"
// SEARCH	- NullSearchString	"No search phrase!"
// MARKDONE	- InvalidIntString	"Invalid integer string!"
// LOAD		- NullFilePath		"No file path specified!"
// SAVE		- NullFilePath		"No file path specified!"
Command* Parser::parse(std::string userInput) {
	CommandType cmdType		= Utilities::stringToCmdType(Utilities::getFirstWord(userInput));
	std::string restOfInput = Utilities::removeFirstWord(userInput);

	Command* cmd = new Command(INVALID,userInput);
	log(INFO,"Parsing \"" + Utilities::getFirstWord(userInput) + "\"");

	switch(cmdType) {
	case ADD: {
		if(restOfInput=="") {
			log(WARN,"No task to add: " + restOfInput);
			throw std::runtime_error("No task to add!");
		}
		Task* taskPtr = parseTask(restOfInput);
		taskPtr->setID(Task::incrementRunningCount());
		cmd = new Add(*taskPtr);
		break;}

	case DELETE: {
		if(!Utilities::isPositiveNonZeroInt(restOfInput)) {
			log(WARN,"Invalid integer string: " + restOfInput);
			throw std::runtime_error("Invalid integer string!");
		}
		int deleteID = Utilities::stringToInt(restOfInput);
		cmd = new Delete(deleteID);
		break;}

	case MODIFY: {
		std::string tempTaskString;
		if(restOfInput == "" ||
			(tempTaskString = Utilities::removeFirstWord(restOfInput)) == "") {
				log(WARN,"No fields to modify: " + restOfInput);
				throw std::runtime_error("No fields to modify!");
		}
		int modifyID = Utilities::stringToInt(Utilities::getFirstWord(restOfInput));
		std::vector<FieldType> fieldsToModify = extractFields(restOfInput);
		Task* tempTaskPtr = parseTask(tempTaskString);
		cmd = new Modify(modifyID,fieldsToModify,*tempTaskPtr);
		break;}

	case SEARCH: {
		if(restOfInput=="") {
			log(WARN,"No search phrase: " + restOfInput);
			throw std::runtime_error("No search phrase!");
		}
		std::string searchPhrase = restOfInput;
		cmd = new Search(searchPhrase);
		break;}

	case MARKDONE: {
		if(!Utilities::isPositiveNonZeroInt(restOfInput)) {
			log(WARN,"Invalid integer string: " + restOfInput);
			throw std::runtime_error("Invalid integer string!");
		}
		int doneID = Utilities::stringToInt(restOfInput);
		cmd = new Markdone(doneID);
		break;}

	case UNMARKDONE: {
		if(!Utilities::isPositiveNonZeroInt(restOfInput)) {
			log(WARN,"Invalid integer string: " + restOfInput);
			throw std::runtime_error("Invalid integer string!");
		}
		int notdoneID = Utilities::stringToInt(restOfInput);
		cmd = new UnmarkDone(notdoneID);
		break;}

	case UNDO:
		cmd = new Undo;
		break;

	case REDO:
		cmd = new Redo;
		break;

	case VIEW: {
		ViewType newView = Utilities::stringToViewType(restOfInput);
		cmd = new View(newView,restOfInput);
		break;}

	case CLEAR_ALL:
		cmd = new ClearAll;
		break;

	case DISPLAY_ALL:
		cmd = new DisplayAll;
		break;

	case LOAD: {
		if(restOfInput=="") {
			log(WARN,"No file path specified: " + restOfInput);
			throw std::runtime_error("No file path specified!");
		}
		std::string newFileName = parseFileName(restOfInput);
		cmd = new Load(parseFileName(newFileName));
		break;}

	case SAVE:
		if(restOfInput=="") {
			log(WARN,"No file path specified: " + restOfInput);
			throw std::runtime_error("No file path specified!");
		}
		cmd = new Save(parseFileName(restOfInput));
		break;

	case EXIT:
		cmd = new Exit;
		break;

	case INVALID:
		log(WARN,"Invalid command: " + restOfInput);
		break;
	}

	return cmd;
}

Task* Parser::parseTask(std::string restOfCommand) {
	log(INFO,"Parsing task");
	std::vector<std::string> userInput = Utilities::stringToVec(restOfCommand);
	std::vector<std::string>::iterator curr = userInput.begin();
	Task* newTask = new Task;
	FieldType inputMode = NAME;
	log(DEBUG,"Parsing field: name");

	std::vector<std::string> inputString;
	int newDate = INVALID_DATE_FORMAT;
	int newTime = INVALID_TIME_FORMAT;

	while(curr != userInput.end()) {
		inputString.clear();

		while(curr != userInput.end()
			&& !Utilities::equalsIgnoreCase(*curr, FIELD_NAME)
			&& !Utilities::equalsIgnoreCase(*curr, FIELD_LABEL_ADD)
			&& !Utilities::equalsIgnoreCase(*curr, FIELD_LABEL_DELETE)
			&& !Utilities::equalsIgnoreCase(*curr, FIELD_PRIORITY_SET)
			&& !Utilities::equalsIgnoreCase(*curr, FIELD_PRIORITY_UNSET)
			&& !Utilities::equalsIgnoreCase(*curr, FIELD_DATE_ON)
			&& !Utilities::equalsIgnoreCase(*curr, FIELD_DATE_FROM)
			&& !Utilities::equalsIgnoreCase(*curr, FIELD_DATE_TO)
			&& !Utilities::equalsIgnoreCase(*curr, FIELD_DATE_BY)
			&& !Utilities::equalsIgnoreCase(*curr, FIELD_TIME_AT)) {
				inputString.push_back(*curr);
				curr++;
		}
		log(DEBUG,"Parsing string: " + Utilities::vecToString(inputString));

		if( (inputMode == START_DATE || inputMode == END_DATE)
			&& (newDate = parseDate(inputString)) == INVALID_DATE_FORMAT
			&& (newDate = parseDay(inputString)) == INVALID_DATE_FORMAT) {
				log(DEBUG,"Invalid date format: " + Utilities::vecToString(inputString));
				if(inputMode == START_DATE) {
					inputMode = START_TIME;
				} else if(inputMode == END_DATE) {
					inputMode = END_TIME;
				}
		}

		switch(inputMode) {
		case NAME:
			newTask->setName(Utilities::vecToString(Utilities::removeSlashKeywords(inputString)));
			break;
		case LABELS_ADD:
			newTask->addLabels(Utilities::removeSlashKeywords(inputString));
			break;
		case LABELS_DELETE:
			newTask->deleteLabels(Utilities::removeSlashKeywords(inputString));
			break;
		case PRIORITY_SET:
			newTask->setPriority();
			break;
		case PRIORITY_UNSET:
			newTask->unsetPriority();
			break;
		case START_DATE:
			newTask->setType(EVENT);
			newTask->setStartDate(newDate);
			break;
		case END_DATE:
			if(newTask->getType() == FLOATING) {
				newTask->setType(TODO);
			}
			newTask->setEndDate(newDate);
			break;
		case START_TIME:
			if((newTime = parseTime(inputString)) != INVALID_TIME_FORMAT) {
				if(newTask->getStartTime() == 0) {
					newTask->setStartTime(newTime);
				}
			} else {
				break;
			}
		case END_TIME:
			if((newTime = parseTime(inputString)) != INVALID_TIME_FORMAT) {
				if(newTask->getStartDate() == 0 || newTask->getStartDate() == INVALID_DATE_FORMAT) {
					newTask->setStartDate(parseDay(Utilities::stringToVec("today")));
				}
				if(newTask->getEndDate() == 0 || newTask->getEndDate() == INVALID_DATE_FORMAT) {
					newTask->setEndDate(newTask->getStartDate());
				}
				newTask->setEndTime(newTime);
				if(newTask->getStartTime()!=0 && newTask->getStartTime()!=newTask->getEndTime()) {
					newTask->setType(EVENT);
				} else {
					newTask->setType(TODO);
				}
			}
			break;
		case INVALID_FIELD:
			break;
		}

		if(curr != userInput.end()) {
			log(DEBUG,"Parsing field: " + *curr);
			inputMode = Utilities::stringToFieldType(*curr);
			curr++;
		} else {
			break;
		}
	}

	log(INFO,"Parsed task");
	return newTask;
}


//========== These support user methods ==========

std::vector<FieldType> Parser::extractFields(std::string restOfInput) {
	assert(restOfInput != "");
	std::vector<std::string> vecInput = Utilities::stringToVec(restOfInput);
	std::vector<std::string>::iterator curr = vecInput.begin();
	std::vector<FieldType> fields;

	if(Utilities::isInt(*curr)){
		curr++;
	}

	if(Utilities::stringToFieldType(*curr) == INVALID_FIELD) {
		fields.push_back(NAME);
	}

	FieldType newField;
	while(curr != vecInput.end()) {
		newField = Utilities::stringToFieldType(*curr);
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
		throw std::runtime_error("Invalid year string: " + yearString + "\n");
	}
	return year;
}


//========== These handle task parameters ==========

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
int Parser::parseDay(std::vector<std::string> dayString) {
	if(dayString.empty()) {
		return INVALID_DATE_FORMAT;
	}

	std::vector<std::string>::iterator curr;
	for(curr=dayString.begin(); curr!=dayString.end(); curr++) {
		*curr = Utilities::stringToLower(*curr);
	}
	curr = dayString.begin();

	int maxDays;
	int newDate = INVALID_DATE_FORMAT;

	time_t t = time(0); // get current time
	struct tm now;
	localtime_s(&now,&t);
	int year = now.tm_year - 100; // get current year, tm_year: years since 1990

	Month month =	(Month)(now.tm_mon + 1); // tm_mon:  Jan starts at 0
	Day day =		  (Day)(now.tm_wday);
	int date =				now.tm_mday;

	if(*curr == "today") {
		curr++;
	} else if(*curr == "tmr" || *curr == "tomorrow") {
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
