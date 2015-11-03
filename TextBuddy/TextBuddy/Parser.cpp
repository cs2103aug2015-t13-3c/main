// Aaron Chong Jun Hao @@author A0110376N
// Parser converts flexible natural language into commands and parameters for TextBuddy.

#include "stdafx.h"
#include "Parser.h"
#include "PowerSearch.h"

Parser* Parser::theOne = nullptr;

Parser::Parser() {
	logger = TbLogger::getInstance();
	logger->log(SYS,"Parser instantiated");
	logger->setLogLevel(DEBUG_INTERNAL);
	// logger->setLogLevel(DEBUG);
}

Parser::~Parser() {
	logger->log(SYS,"Parser destructed");
}

// This defines the file extension used by TextBuddy
const std::string Parser::FILE_EXTENSION = ".txt";

void Parser::log(Level level, std::string message) {
	logger->log(level,message);
	return;
}

void Parser::logSetTaskType(TaskType type) {
	log(DEBUG_INTERNAL,"Setting task type as: " + Utilities::taskTypeToString(type));
	return;
}

//==================================================
//                  PUBLIC METHODS
//==================================================

Parser* Parser::getInstance() {
	if(theOne == nullptr) {
		theOne = new Parser();
	}
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

	case PICK: {
		bool isPick = false;
		if(restOfInput == "") {
			log(WARN,"No blocked task to pick: " + restOfInput);
			throw std::runtime_error("No blocked task to pick!");
		}
		int pickID = Utilities::stringToInt(Utilities::getFirstWord(restOfInput));
		std::string pickString = Utilities::removeFirstWord(restOfInput);
		if(!pickString.empty() && Utilities::containsAny(pickString,"r re reserve")) {
			isPick = true;
		}
		cmd = new Pick(pickID,isPick);
		break;}

	case POWERSEARCH:
	case SEARCH: {
		if(restOfInput=="") {
			log(WARN,"No search phrase: " + restOfInput);
			throw std::runtime_error("No search phrase!");

		} else if(isPowerSearchFormat(restOfInput)) {
			std::vector<std::string> searchParameters = parseSearchParameters(restOfInput);
			cmd = new PowerSearch(searchParameters);

		} else {
			std::string searchPhrase = restOfInput;
			cmd = new Search(searchPhrase);
		}
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

//==================================================
//                 PRIVATE METHODS
//==================================================

Task* Parser::parseTask(std::string restOfCommand) {
	log(INFO,"Parsing task");
	log(DEBUG_INTERNAL,"Parsing field keyword: name");
	FieldType inputMode = NAME;

	std::vector<std::string> userInput = Utilities::stringToVec(restOfCommand);
	std::vector<std::string>::iterator curr = userInput.begin();
	std::vector<std::string> inputString;
	Task* newTask = new Task;
	bool isTODO = false;
	bool isTODOreserve = false;
	bool isReservation = false;

	while(curr != userInput.end() || inputMode == PRIORITY_SET) {
		inputString.clear();

		while(curr!=userInput.end() && !isFieldKeyword(*curr)) {
			inputString.push_back(*curr);
			curr++;
		}

		placeInField(newTask,isTODO,isTODOreserve,isReservation,inputMode,inputString);

		if(curr != userInput.end()) {
			log(DEBUG_INTERNAL,"Parsing field keyword: " + *curr);
			inputMode = Utilities::stringToFieldType(*curr);
			curr++;
		} else {
			break;
		}
	}

	int endDate;
	if((endDate=newTask->getEndDate()) < newTask->getStartDate()) {
		newTask->setEndDate(endDate+10000); // Set end date as next year
	}

	if(isTODO) {
		newTask->setType(TODO);
		newTask->setStartDate(newTask->getEndDate());
		newTask->setStartTime(newTask->getEndTime());
	}
	if(isTODOreserve) {
		newTask->setReserveType(TODO);
		newTask->addReserveStartDate(newTask->getReserveEndDate());
		newTask->addReserveStartTime(newTask->getReserveEndTime());
	}

	log(INFO,"Parsed task of type: " + Utilities::taskTypeToString(newTask->getType()));
	return newTask;
}

std::vector<std::string> Parser::parseSearchParameters(std::string restOfInput) {
	std::vector<std::string> searchParameters(8);
	int newDate;
	int newTime;
	int digit;
	// Powersearch keywords: from, to, on, at, before, after, for
	std::string searchPhrase;
	int startDate = DATE_NOT_SET;
	int startTime = TIME_NOT_SET;
	int endDate = DATE_NOT_SET;
	int endTime = TIME_NOT_SET;
	int daysNeeded = 0;
	int hoursNeeded = 1; // Default duration
	int minutesNeeded = 0;

	std::vector<std::string> userInput = Utilities::stringToVec(restOfInput);
	std::vector<std::string>::iterator curr = userInput.begin();
	std::vector<std::string> inputString;
	std::string inputMode = "searchPhrase";

	while(curr != userInput.end()) {
		inputString.clear();

		log(DEBUG_INTERNAL,"Processing PowerSearch phrase: " + *curr);

		while(curr!=userInput.end() && !isPowerSearchKeywords(*curr)) {
			inputString.push_back(*curr);
			curr++;
		}

		if(Utilities::equalsIgnoreCase(inputMode,"searchPhrase")) {
			searchPhrase = Utilities::vecToString(inputString);
			if(searchPhrase != "") {
				hoursNeeded = 0;
			}

		} else if(Utilities::containsAny(inputMode,"from after")) {
			if((newDate=parseDate(inputString)) != INVALID_DATE_FORMAT) {
				startDate = newDate;
				log(DEBUG_INTERNAL,"Parsed " + Utilities::vecToString(inputString) + " to be date: " + std::to_string(newDate));
			} else if((newTime=parseTime(inputString)) != INVALID_TIME_FORMAT) {
				startTime = newTime;
				log(DEBUG_INTERNAL,"Parsed " + Utilities::vecToString(inputString) + " to be time: " + std::to_string(newTime));
			}

		} else if(Utilities::containsAny(inputMode,"to before")) {
			if((newDate=parseDate(inputString)) != INVALID_DATE_FORMAT) {
				endDate = newDate;
			} else if((newTime=parseTime(inputString)) != INVALID_TIME_FORMAT) {
				endTime = newTime;
			}

		} else if(Utilities::equalsIgnoreCase(inputMode,"on")) {
			if((newDate=parseDate(inputString)) != INVALID_DATE_FORMAT) {
				startDate = newDate;
				endDate = newDate;
			}

		} else if(Utilities::equalsIgnoreCase(inputMode,"at")) {
			if((newTime=parseTime(inputString)) != INVALID_TIME_FORMAT) {
				if(startDate==DATE_NOT_SET) {
					startDate = parseByDay(Utilities::stringToVec("today"));
				}
				if(endDate==DATE_NOT_SET) {
					endDate = startDate;
				}
				if(startTime==TIME_NOT_SET) {
					startTime = newTime;
				} else {
					endTime = newTime;
				}
			}

		} else if(searchPhrase=="" && Utilities::equalsIgnoreCase(inputMode,"for")) {
			// Freeslot PowerSearch
			while(curr!=userInput.end() && ++curr!=userInput.end() && Utilities::isInt(*curr)) {
				digit = Utilities::stringToInt(*curr);
				if(++curr != userInput.end()) {
					if(Utilities::equalsIgnoreCase(*curr,"d")) {
						daysNeeded = digit;
						hoursNeeded = 0;
					} else if(Utilities::equalsIgnoreCase(*curr,"h")) {
						hoursNeeded = digit;
					} else if(Utilities::equalsIgnoreCase(*curr,"m")) {
						minutesNeeded = digit;
					}
				}
			}
		} else {
			break;
		}

		if(curr != userInput.end()) {
			log(DEBUG_INTERNAL,"Parsing PowerSearch inputMode: " + *curr);
			inputMode = *curr;
			if(!Utilities::equalsIgnoreCase(inputMode,"for")) {
				curr++;
			}
		} else {
			break;
		}
	}

	if(endDate < startDate) {
		endDate = endDate + 10000; // Set end date as next year
	}

	if(startDate == DATE_NOT_SET) {
		startDate = parseByDay(Utilities::stringToVec("today"));
	}
	if(endDate == DATE_NOT_SET) {
		endDate = parseByDay(Utilities::stringToVec("today"));
	}
	if(startTime == TIME_NOT_SET) {
		startTime = 0;
	}
	if(endTime == TIME_NOT_SET) {
		endTime = 2359;
	}

	searchParameters[0] = searchPhrase;
	searchParameters[1] = std::to_string(startDate);
	searchParameters[2] = std::to_string(startTime);
	searchParameters[3] = std::to_string(endDate);
	searchParameters[4] = std::to_string(endTime);
	searchParameters[5] = std::to_string(daysNeeded);
	searchParameters[6] = std::to_string(hoursNeeded);
	searchParameters[7] = std::to_string(minutesNeeded);
	log(INFO,"Parsed PowerSearch");
	return searchParameters;
}

int Parser::parseDate(std::vector<std::string> dateString) {
	int newDate;
	if(    ((newDate=parseByDate(dateString)) != INVALID_DATE_FORMAT)   // Try parseByDate first
		|| ((newDate=parseByDay(dateString))  != INVALID_DATE_FORMAT)) { // Try parseByDay if previous is invalid
			return newDate;
	} else if(dateString.size()==1) {
		// Convert DD/MM(/YY) to DD MM( YY)
		dateString = Utilities::stringToVec(Utilities::replace(Utilities::vecToString(dateString),"/"," "));
		// Try both parseByDate and parseByDay again
		if(    ((newDate=parseByDate(dateString)) != INVALID_DATE_FORMAT)
			|| ((newDate=parseByDay(dateString))  != INVALID_DATE_FORMAT)) {
				return newDate;
		}

	}
	return INVALID_DATE_FORMAT;
}

// Processes dates in these formats:
// - DD MM/MMM/MMMM
int Parser::parseByDate(std::vector<std::string> dateString) {
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
				yearInput = findYear();				// Get current year
			} else if(Utilities::isInt(*curr)) {
				yearInput = findYear(*curr);		// Exception thrown if year is not current or next
				curr++;
			}
			assert(yearInput>=0 && yearInput<=99);
		}
	}

	if(curr == dateString.end()
		&& yearInput != 0
		&& monthInput != INVALID_MONTH
		&& (1<=dateInput && dateInput<=maxDays)) {
			newDate = yearInput*10000 + (int)monthInput*100 + dateInput;
	}

	log(DEBUG_INTERNAL,"Parsed by date: " + std::to_string(newDate));
	return newDate;
}

// Processes dates in these formats:
// - (this/next) DDD/DDDD
int Parser::parseByDay(std::vector<std::string> dayString) {
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

	time_t currTime = time(0);
	struct tm localTime;
	localtime_s(&localTime,&currTime);
	int year = localTime.tm_year - 100;				// tm_year: years since 1990

	Month month =	(Month)(localTime.tm_mon + 1);	// tm_mon:  Jan starts at 0
	Day day =		  (Day)(localTime.tm_wday);
	int date =				localTime.tm_mday;

	if(*curr == "today" || *curr == "later") {
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

	log(DEBUG_INTERNAL,"Parsed by day: " + std::to_string(newDate));
	return newDate;
}

// Processes times in these formats:
// - HH    AM/PM (default: assume AM)
// - HH.MM AM/PM (default: assume AM) // TODO: Does not read minutes yet
// - HHMM        (24-hour)
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

	log(DEBUG_INTERNAL,"Parsed time: " + std::to_string(time));
	return time;
}

FieldType Parser::convertFieldDateToTime(FieldType &inputMode) {
	if(inputMode == START_DATE) {
		inputMode = START_TIME;
	} else if(inputMode == END_DATE) {
		inputMode = END_TIME;
	} else if(inputMode == TODO_DATE) {
		inputMode = TODO_TIME;
	}
	return inputMode;
}

void Parser::convertFieldToReserve(FieldType &inputMode) {
	if(inputMode == START_DATE) {
		inputMode = RESERVE_START_DATE;
	} else if(inputMode == START_TIME) {
		inputMode = RESERVE_START_TIME;
	} else if(inputMode == END_DATE) {
		inputMode = RESERVE_END_DATE;
	} else if(inputMode == END_TIME) {
		inputMode = RESERVE_END_TIME;
	} else if(inputMode == TODO_DATE) {
		inputMode = RESERVE_TODO_DATE;
	} else if(inputMode == TODO_TIME) {
		inputMode = RESERVE_TODO_TIME;
	}
	return;
}

void Parser::placeInField(Task* newTask, bool &isTODO, bool &isTODOreserve, bool &isReservation, FieldType inputMode, std::vector<std::string> inputString) {
	if(inputMode == RESERVE) {
		isReservation = true;
		return;
	}

	int newDate = DATE_NOT_SET;
	int newTime = TIME_NOT_SET;

	log(DEBUG_INTERNAL,"Parsing string: " + Utilities::vecToString(inputString));

	if(isDateField(inputMode)) {
		newDate = parseDate(inputString);
		if(newDate == INVALID_DATE_FORMAT) {
			convertFieldDateToTime(inputMode);
		}

		if(!isReservation) {
			if(/*newTask->getType()==FLOATING &&*/ isTodoField(inputMode)) {
				isTODO = true;
			} else if(isTODO && inputMode==START_TIME) {
				inputMode = TODO_TIME;
			}
		} else if(isReservation) {
			log(DEBUG,"Parsing reservation: " + Utilities::vecToString(inputString));
			if(newTask->getReserveType()==FLOATING && isTodoField(inputMode)) {
				isTODOreserve = true;
			} else if(isTODOreserve && inputMode==START_TIME) {
				inputMode = TODO_TIME;
			}
			convertFieldToReserve(inputMode);
		}
	}

	switch(inputMode) {
	case NAME:
		newTask->setName(Utilities::vecToString(removeSlashKeywords(inputString)));
		break;
	case LABELS_ADD:
		newTask->addLabels(removeSlashKeywords(inputString));
		break;
	case LABELS_DELETE:
		newTask->setLabelsToDelete(removeSlashKeywords(inputString));
		break;
	case LABELS_CLEAR:
		// Nothing to set
		break;
	case PRIORITY_SET:
		newTask->setPriority();
		break;
	case PRIORITY_UNSET:
		// Unset by default
		break;
	case START_DATE:
		logSetTaskType(EVENT);
		newTask->setType(EVENT);
		newTask->setStartDate(newDate);
		if(newTask->getEndDate() == DATE_NOT_SET) {
			newTask->setEndDate(newDate);
		}
		break;
	case TODO_DATE:
	case END_DATE:
		if(newTask->getType() == FLOATING) {
			logSetTaskType(TODO);
			isTODO = true;
		} else if(isTODO
			|| (newTask->getType()==EVENT && newTask->getStartDate()==DATE_NOT_SET)) {
				newTask->setStartDate(newDate);
		}
		newTask->setEndDate(newDate);
		break;
	case START_TIME:
		logSetTaskType(EVENT);
		newTask->setType(EVENT);
		if((newTime = parseTime(inputString)) != INVALID_TIME_FORMAT) {
			if(newTask->getStartTime() == TIME_NOT_SET) {
				newTask->setStartTime(newTime);
			}
		} else {
			break;
		}
	case TODO_TIME:
	case END_TIME:
		if((newTime = parseTime(inputString)) != INVALID_TIME_FORMAT) {
			if(newTask->getStartDate() == DATE_NOT_SET) {
				if(newTask->getEndDate() == DATE_NOT_SET) {
					newTask->setStartDate(parseByDay(Utilities::stringToVec("today")));
				} else {
					newTask->setStartDate(newTask->getEndDate());
				}
			}
			if(newTask->getEndDate() == DATE_NOT_SET) {
				newTask->setEndDate(newTask->getStartDate());
			}

			newTask->setEndTime(newTime);
			if(newTask->getStartTime()!=TIME_NOT_SET && newTask->getStartTime()!=newTask->getEndTime()) {
				logSetTaskType(EVENT);
				newTask->setType(EVENT);
			} else if(newTask->getType()==EVENT && newTask->getStartTime()==TIME_NOT_SET) {
				newTask->setStartTime(newTime);
			} else if(newTask->getType()==FLOATING) {
				logSetTaskType(TODO);
				isTODO = true;
			}
		}
		break;
		//===== Reservation: Follow above and modify accordingly =====
	case RESERVE_START_DATE:
		log(DEBUG_INTERNAL,"Placing in reserveStartDate");
		newTask->setReserveType(EVENT);
		newTask->addReserveStartDate(newDate);
		break;
	case RESERVE_TODO_DATE:
	case RESERVE_END_DATE:
		log(DEBUG_INTERNAL,"Placing in reserveEndDate");
		if(newTask->getReserveType() == FLOATING) {
			isTODOreserve = true;
		} else if(isTODOreserve
			|| (newTask->getReserveType()==EVENT && newTask->getReserveStartDate()==DATE_NOT_SET)) {
				newTask->addReserveStartDate(newDate);
		}
		newTask->addReserveEndDate(newDate);
		break;
	case RESERVE_START_TIME:
		log(DEBUG_INTERNAL,"Placing in reserveStartTime");
		newTask->setReserveType(EVENT);
		if((newTime = parseTime(inputString)) != INVALID_TIME_FORMAT) {
			if(newTask->getReserveStartTime() == TIME_NOT_SET) {
				newTask->addReserveStartTime(newTime);
			}
		} else {
			break;
		}
	case RESERVE_TODO_TIME:
	case RESERVE_END_TIME:
		log(DEBUG_INTERNAL,"Placing in reserveEndTime");
		if((newTime = parseTime(inputString)) != INVALID_TIME_FORMAT) {
			if(newTask->getReserveStartDate() == DATE_NOT_SET) {
				if(newTask->getReserveEndDate() == DATE_NOT_SET) {
					newTask->addReserveStartDate(parseByDay(Utilities::stringToVec("today")));
				} else {
					newTask->addReserveStartDate(newTask->getReserveEndDate());
				}
			}
			if(newTask->getReserveEndDate() == DATE_NOT_SET) {
				newTask->addReserveEndDate(newTask->getReserveStartDate());
			}
			newTask->addReserveEndTime(newTime);
			if(newTask->getReserveStartTime()!=TIME_NOT_SET && newTask->getReserveStartTime()!=newTask->getReserveEndTime()) {
				newTask->setReserveType(EVENT);
			} else if(newTask->getReserveType()==EVENT && newTask->getReserveStartTime()==TIME_NOT_SET) {
				newTask->addReserveStartTime(newTime);
			} else if(newTask->getReserveType()==FLOATING) {
				isTODOreserve = true;
			}
		}
		break;
	case RESERVE:
		//===== Reservation: Follow end =====
	case INVALID_FIELD:
		break;
	}
	return;
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
	time_t t = time(0);				// Get current time
	struct tm now;
	localtime_s(&now,&t);
	int year = now.tm_year - 100;	// Get current year, tm_year: years since 1900

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

// Powersearch (keywords: from, to, at, on, before, after, for)
bool Parser::isPowerSearchKeywords(std::string str) {
	assert(str!="");
	std::string powerSearchKeywords = "from to at on before after for";
	return Utilities::containsAny(str,powerSearchKeywords);
}

// - Searchphrase
//		e.g. search lab      from mon at     1 pm to tue at 2 pm
//		e.g. search lecture  on   tue wed 3 pm
//		e.g. search tutorial on   wed thu  4 pm
// - Freeslot  (keyword: for)
//		e.g. search                   from   8 am        to 2 pm for 1 h 
bool Parser::isPowerSearchFormat(std::string searchStrings) {
	assert(searchStrings!="");
	std::vector<std::string> vecWords = Utilities::stringToVec(searchStrings);
	std::string powerSearchKeywords = "from to at on before after for";
	std::string searchPhrase = *(vecWords.begin());

	if(isPowerSearchKeywords(searchStrings)) {
		if(!isPowerSearchKeywords(searchPhrase)) {
			return true;	// Searchphrase (takes precedence over Freeslot powersearch)
		} else if(Utilities::containsAny(searchStrings,"for")) {
			return true;	// Freeslot
		}
	}
	return false;
}

bool Parser::isFieldKeyword(std::string str) {
	return Utilities::stringToFieldType(str)!=INVALID_FIELD;
}

bool Parser::isDateField(FieldType field) {
	return (field==START_DATE || field==END_DATE || field==TODO_DATE);
}

bool Parser::isTodoField(FieldType field) {
	return (field==TODO_DATE || field==TODO_TIME);
}

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
		if(newField == LABELS_DELETE
			&& curr+1 != vecInput.end()
			&& Utilities::stringToFieldType(*(curr+1)) != INVALID_FIELD) {
				fields.push_back(LABELS_CLEAR);
		} else if(isDateField(newField)
			&& curr+1 != vecInput.end()
			&& curr+2 != vecInput.end()
			&& parseDate(std::vector<std::string>(curr+1,curr+3)) == INVALID_DATE_FORMAT) {
				fields.push_back(convertFieldDateToTime(newField));
		} else if(newField != INVALID_FIELD) {
			fields.push_back(newField);
		}
		curr++;
	}

	log(DEBUG,"Fields extracted: " + Utilities::fieldVecToString(fields));
	return fields;
}

// TODO: Remove actual keywords
std::vector<std::string> Parser::removeSlashKeywords(std::vector<std::string> vecString) {
	std::vector<std::string>::iterator curr;
	std::string subString;
	for(curr=vecString.begin(); curr!=vecString.end(); curr++) {
		if( ((*curr)[0] == '/' || (*curr)[0] == '\\') && isFieldKeyword(subString = (*curr).substr(1)) ) {
			*curr = subString;
		}
	}
	return vecString;
}
