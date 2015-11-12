// Aaron Chong Jun Hao @@author A0110376N
// Parser converts flexible natural language into commands and parameters for TaskShark.

#include "stdafx.h"
#include "Parser.h"
#include "PowerSearch.h"

Parser* Parser::theOne = nullptr;

Parser::Parser() {
	logger = TsLogger::getInstance();
	logger->log(SYS,"Parser instantiated");
}

Parser::~Parser() {
	logger->log(SYS,"Parser destructed");
}

// This defines the file extension used by TaskShark
const std::string Parser::FILE_EXTENSION = ".txt";

// Warnings when throwing exceptions
const std::string Parser::WARNING_INVALID_INT_STRING = "Invalid integer string: ";
const std::string Parser::WARNING_NULL_FILE_PATH	 = "No file path specified!";
const std::string Parser::WARNING_NULL_MODIFY_STRING = "No fields to modify!";
const std::string Parser::WARNING_NULL_PICK_TASK	 = "No blocked task to pick!";
const std::string Parser::WARNING_NULL_SEARCH_STRING = "No search phrase!";
const std::string Parser::WARNING_NULL_TASK_STRING	 = "No tasks to add!";

void Parser::log(Level level, std::string message) {
	logger->log(level,message);
	return;
}

void Parser::logSetTaskType(TaskType type) {
	log(DEBUG,"Setting task type as: " + Utilities::taskTypeToString(type));
	return;
}

//==================================================
//                  PUBLIC METHODS
//==================================================

Parser* Parser::getInstance() {
	if (theOne == nullptr) {
		theOne = new Parser();
	}
	return theOne;
}

std::string Parser::parseFileName(char* argv[]) {
	char* charFilePath = argv[1];
	// Replace forwardslash '/' and single backslash '\' with double backslash '\\'
	std::string newFilePath = Utilities::replace(charFilePath,"\\","\\\\");
	newFilePath = Utilities::replace(newFilePath,"/","\\\\");

	size_t fileExtensionPos = newFilePath.size() - FILE_EXTENSION.size();
	charFilePath = &newFilePath[0u];
	if (charFilePath + fileExtensionPos == FILE_EXTENSION) 	{
		newFilePath = charFilePath;
	} else {
		newFilePath = charFilePath + FILE_EXTENSION;
	}
	return newFilePath;
}

std::string Parser::parseFileName(std::string stringFilePath) {
	char* charFilePath = &stringFilePath[0u];
	char* argvForm[2]={"",charFilePath};
	return parseFileName(argvForm);
}

void Parser::createCmd(CommandType cmdType, std::string restOfInput, Command*& cmd) {
	switch (cmdType) {
	case ADD: {
		if (restOfInput == "") {
			log(WARN,WARNING_NULL_TASK_STRING);
			throw std::runtime_error(WARNING_NULL_TASK_STRING);
		}
		Task* taskPtr = parseTask(restOfInput);
		taskPtr->setID(Task::incrementRunningCount());
		cmd = new Add(*taskPtr,restOfInput);
		break; }

	case DELETE: {
		if (!Utilities::isPositiveNonZeroInt(restOfInput)) {
			log(WARN,WARNING_INVALID_INT_STRING + restOfInput);
			throw std::runtime_error(WARNING_INVALID_INT_STRING + restOfInput);
		}
		int deleteID = Utilities::stringToInt(restOfInput);
		cmd = new Delete(deleteID);
		break; }

	case MODIFY: {
		std::string tempTaskString;
		if (restOfInput == "" ||
			((tempTaskString=Utilities::removeFirstWord(restOfInput)) == "") && Task::lastEditID == 0) {
				log(WARN,WARNING_NULL_MODIFY_STRING);
				throw std::runtime_error(WARNING_NULL_MODIFY_STRING);
		}

		int modifyID = Utilities::stringToInt(Utilities::getFirstWord(restOfInput));
		if (modifyID == 0 && Task::lastEditID != 0) {
			tempTaskString = restOfInput;
		}
		if (Utilities::containsAny(tempTaskString,"float floating")) {
			bool isSetFloating = true;
			cmd = new Modify(modifyID,isSetFloating);
		} else {
			std::vector<FieldType> fieldsToModify = extractFields(restOfInput);
			Task* tempTaskPtr = parseTask(tempTaskString);
			cmd = new Modify(modifyID,fieldsToModify,*tempTaskPtr,tempTaskString);
		}
		break; }

	case PICK: {
		bool isPick = false;
		if (restOfInput == "") {
			log(WARN,WARNING_NULL_PICK_TASK);
			throw std::runtime_error(WARNING_NULL_PICK_TASK);
		}
		int pickID = Utilities::stringToInt(Utilities::getFirstWord(restOfInput));
		std::string pickString = Utilities::removeFirstWord(restOfInput);
		if (!pickString.empty() && Utilities::containsAny(pickString,"r re reserve")) {
			isPick = true;
		}
		cmd = new Pick(pickID,isPick);
		break; }

	case POWERSEARCH:
	case SEARCH: {
		if (restOfInput == "") {
			log(WARN,WARNING_NULL_SEARCH_STRING + ": " + restOfInput);
			throw std::runtime_error(WARNING_NULL_SEARCH_STRING + "!");

		} else if (isPowerSearchFormat(restOfInput)) {
			std::vector<std::string> searchParameters = parseSearchParameters(restOfInput);
			cmd = new PowerSearch(searchParameters);

		} else {
			std::string searchPhrase = restOfInput;
			cmd = new Search(searchPhrase);
		}
		break; }

	case MARKDONE: {
		if (!Utilities::isPositiveNonZeroInt(restOfInput)) {
			log(WARN,WARNING_INVALID_INT_STRING + restOfInput);
			throw std::runtime_error(WARNING_INVALID_INT_STRING + restOfInput);
		}
		int doneID = Utilities::stringToInt(restOfInput);
		cmd = new Markdone(doneID);
		break; }

	case UNMARKDONE: {
		if (!Utilities::isPositiveNonZeroInt(restOfInput)) {
			log(WARN,WARNING_INVALID_INT_STRING + restOfInput);
			throw std::runtime_error(WARNING_INVALID_INT_STRING + restOfInput);
		}
		int notdoneID = Utilities::stringToInt(restOfInput);
		cmd = new UnmarkDone(notdoneID);
		break; }

	case UNDO:
		cmd = new Undo;
		break;

	case REDO:
		cmd = new Redo;
		break;

	case VIEW: {
		log(DEBUG,"View option: " + restOfInput);
		if (restOfInput!="" && isPowerSearchKeywords(restOfInput)) {
			std::vector<std::string> searchParameters = parseSearchParameters(restOfInput);
			cmd = new View(searchParameters,restOfInput);
		} else {
			ViewType newView = Utilities::stringToViewType(restOfInput);
			cmd = new View(newView,restOfInput);
		}
		break; }

	case VIEW_DEFAULT:
		cmd = new View(VIEWTYPE_HOME,restOfInput);
		break;

	case CLEAR_ALL:
		cmd = new ClearAll;
		break;

	case DISPLAY_ALL:
		cmd = new DisplayAll;
		break;

	case LOAD: {
		if (restOfInput == "") {
			log(WARN,WARNING_NULL_FILE_PATH);
			throw std::runtime_error(WARNING_NULL_FILE_PATH);
		}
		bool isOverwriteFile = true;
		if (Utilities::containsAny(Utilities::getFirstWord(restOfInput),"from")) {
			isOverwriteFile = false;
			restOfInput = Utilities::removeFirstWord(restOfInput);
		}
		cmd = new Load(parseFileName(restOfInput),isOverwriteFile);
		break; }

	case SAVE: {
		if (restOfInput == "") {
			log(WARN,WARNING_NULL_FILE_PATH);
			throw std::runtime_error(WARNING_NULL_FILE_PATH);
		}
		bool isDeletePrevFile = false;
		if (Utilities::equalsIgnoreCase(Utilities::getFirstWord(restOfInput),"to")) {
			isDeletePrevFile = true;
			restOfInput = Utilities::removeFirstWord(restOfInput);
		}
		cmd = new Save(parseFileName(restOfInput),isDeletePrevFile);
		break; }

	case SET: {
		if (restOfInput == "") {
			log(WARN,"Nothing to set!");
			throw std::runtime_error("Nothing to set!");
		}
		std::string keyword = Utilities::getFirstWord(restOfInput);
		std::string userString = Utilities::removeFirstWord(restOfInput);
		cmd = new Set(keyword,userString);
		break; }

	case EXIT:
		cmd = new Exit;
		break;

	case INVALID:
		log(WARN,"Invalid command: " + restOfInput);
		break;
	}
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
	Command*	cmd			= new Command(INVALID,userInput);
	createCmd(cmdType, restOfInput, cmd);
	return cmd;
}

//==================================================
//                 PRIVATE METHODS
//==================================================

Task* Parser::parseTask(std::string &restOfCommand) {
	log(DEBUG,"Parsing task");
	log(DEBUG,"Parsing field keyword: name");
	FieldType inputMode = NAME;

	std::vector<std::string> userInput = Utilities::stringToVec(restOfCommand);
	std::vector<std::string>::iterator curr = userInput.begin();
	std::vector<std::string> inputString;
	Task* newTask = new Task;
	bool isTODO = false;
	bool isTODOreserve = false;
	bool isReservation = false;
	std::string invalidDateTimeString = "";
	std::string errorString;

	while (curr != userInput.end() || inputMode == PRIORITY_SET) {
		inputString.clear();

		while (curr!=userInput.end() && !isFieldKeyword(*curr)) {
			inputString.push_back(*curr);
			++curr;
		}

		errorString = placeInField(newTask,isTODO,isTODOreserve,isReservation,inputMode,inputString);
		if (errorString != "") {
			if (invalidDateTimeString != "") {
				invalidDateTimeString += ", " + errorString;
			}
		}

		if (curr != userInput.end()) {
			log(DEBUG,"Parsing field keyword: " + *curr);
			inputMode = Utilities::stringToFieldType(*curr);
			++curr;
		} else {
			break;
		}
	}

	int endDate;
	if ((endDate=newTask->getEndDate()) < newTask->getStartDate()) {
		newTask->setEndDate(endDate+10000); // Set end date as next year
	}

	if (isTODO) {
		newTask->setType(TODO);
		newTask->setStartDate(newTask->getEndDate());
		newTask->setStartTime(newTask->getEndTime());
	}
	if (isTODOreserve) {
		newTask->setReserveType(TODO);
		newTask->addReserveStartDate(newTask->getReserveEndDate());
		newTask->addReserveStartTime(newTask->getReserveEndTime());
	}

	log(DEBUG,"Parsed task of type: " + Utilities::taskTypeToString(newTask->getType()));
	restOfCommand = invalidDateTimeString;
	return newTask;
}

std::vector<std::string> Parser::parseSearchParameters(std::string restOfInput) {
	std::vector<std::string> searchParameters(8);
	int newDate;
	int newTime;
	int digit;
	// Powersearch keywords: from, to, on, at, before, after, for
	std::string searchPhrase;
	int startDate = parseByDay(Utilities::stringToVec("today"));		// Allow "before"
	int startTime = TIME_NOT_SET;
	int endDate = parseByDate(Utilities::stringToVec("31 dec")) + 10000; // Allow "after"
	int endTime = TIME_NOT_SET;
	int daysNeeded = 0;
	int hoursNeeded = 1; // Default duration
	int minutesNeeded = 0;

	std::vector<std::string> userInput = Utilities::stringToVec(restOfInput);
	std::vector<std::string>::iterator curr = userInput.begin();
	std::vector<std::string> inputString;
	std::string inputMode = "searchPhrase";

	while (curr != userInput.end()) {
		inputString.clear();

		log(DEBUG,"Processing PowerSearch phrase: " + *curr);

		while (curr!=userInput.end() && !isPowerSearchKeywords(*curr)) {
			inputString.push_back(*curr);
			++curr;
		}

		if (Utilities::equalsIgnoreCase(inputMode,"searchPhrase")) {
			searchPhrase = Utilities::vecToString(inputString);
			if (searchPhrase != "") {
				hoursNeeded = 0;
			}

		} else if (Utilities::containsAny(inputMode,"from after")) {
			if ((newDate = parseDate(inputString)) != INVALID_DATE_FORMAT) {
				startDate = newDate;
			} else if ((newTime = parseTime(inputString)) != INVALID_TIME_FORMAT) {
				startTime = newTime;
			}

		} else if (Utilities::containsAny(inputMode,"to before")) {
			if ((newDate = parseDate(inputString)) != INVALID_DATE_FORMAT) {
				endDate = newDate;
			} else if ((newTime = parseTime(inputString)) != INVALID_TIME_FORMAT) {
				if (endDate == DATE_NOT_SET) {
					endDate = startDate;
				}
				endTime = newTime;
			}

		} else if (Utilities::equalsIgnoreCase(inputMode,"on")) {
			if ((newDate = parseDate(inputString)) != INVALID_DATE_FORMAT) {
				startDate = newDate;
				endDate = newDate;
			}

		} else if (Utilities::equalsIgnoreCase(inputMode,"at")) {
			if ((newTime = parseTime(inputString)) != INVALID_TIME_FORMAT) {
				if (startDate == DATE_NOT_SET) {
					startDate = parseByDay(Utilities::stringToVec("today"));
				}
				if (endDate == DATE_NOT_SET) {
					endDate = startDate;
				}
				if (startTime == TIME_NOT_SET) {
					startTime = newTime;
				} else {
					endTime = newTime;
				}
			}

		} else if (searchPhrase=="" && Utilities::equalsIgnoreCase(inputMode,"for")) {
			// Freeslot PowerSearch
			while (curr!=userInput.end() && ++curr!=userInput.end() && Utilities::isInt(*curr)) {
				digit = Utilities::stringToInt(*curr);
				if (++curr != userInput.end()) {
					if (Utilities::equalsIgnoreCase(*curr,"d")) {
						daysNeeded = digit;
						hoursNeeded = 0;
					} else if (Utilities::equalsIgnoreCase(*curr,"h")) {
						hoursNeeded = digit;
					} else if (Utilities::equalsIgnoreCase(*curr,"m")) {
						minutesNeeded = digit;
					}
				}
			}
		} else {
			break;
		}

		if (curr != userInput.end()) {
			log(DEBUG,"Parsing PowerSearch inputMode: " + *curr);
			inputMode = *curr;
			if (!Utilities::equalsIgnoreCase(inputMode,"for")) {
				++curr;
			}
		} else {
			break;
		}
	}

	if (endDate < startDate) {
		// Set end date as next year
		endDate = endDate + 10000;
	}

	if (startDate == DATE_NOT_SET) {
		startDate = parseByDay(Utilities::stringToVec("today"));
	}
	if (endDate == DATE_NOT_SET) {
		endDate = parseByDay(Utilities::stringToVec("today"));
	}
	if (startTime == TIME_NOT_SET) {
		// startTime = 0;
	}
	if (endTime == TIME_NOT_SET) {
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
	if (   ((newDate=parseByDate(dateString)) != INVALID_DATE_FORMAT)
		|| ((newDate=parseByDay(dateString))  != INVALID_DATE_FORMAT)) {
			return newDate;
	} else if (dateString.size()==1 && parseTime(dateString)==INVALID_TIME_FORMAT) {
		// Convert DD/MM(/YY) to DD MM( YY)
		dateString = Utilities::stringToVec(Utilities::replace(Utilities::vecToString(dateString),"/"," "));
		// Try both parseByDate and parseByDay again
		if (   ((newDate=parseByDate(dateString)) != INVALID_DATE_FORMAT)
			|| ((newDate=parseByDay(dateString))  != INVALID_DATE_FORMAT)) {
				return newDate;
		}
		// Convert DD.MM(.YY) to DD MM( YY)
		dateString = Utilities::stringToVec(Utilities::replace(Utilities::vecToString(dateString),"."," "));
		// Try both parseByDate and parseByDay again
		if (   ((newDate=parseByDate(dateString)) != INVALID_DATE_FORMAT)
			|| ((newDate=parseByDay(dateString))  != INVALID_DATE_FORMAT)) {
				return newDate;
		}
	}
	return INVALID_DATE_FORMAT;
}

// Processes dates in these formats:
// - DD MM/MMM/MMMM
int Parser::parseByDate(std::vector<std::string> dateString) {
	if (dateString.empty()) {
		return INVALID_DATE_FORMAT;
	}

	std::vector<std::string>::iterator curr;
	for (curr=dateString.begin(); curr!=dateString.end(); ++curr) {
		*curr = Utilities::stringToLower(*curr);
	}
	curr = dateString.begin();

	int dateInput = 0;
	Month monthInput = INVALID_MONTH;
	int yearInput = 0;

	int maxDays = 0;
	int newDate = INVALID_DATE_FORMAT;

	// Get dateInput
	if (curr!=dateString.end() && Utilities::isInt(*curr)) {

		dateInput = Utilities::stringToInt(*curr);
		++curr;

		// Get monthInput
		if (curr!=dateString.end()) {
			monthInput = Utilities::stringToMonth(*curr);
			if (monthInput != INVALID_MONTH) {
				maxDays = findMaxDays(monthInput);
			}
			++curr;

			// Get yearInput
			if (curr==dateString.end()) {
				yearInput = findYear();				// Get current year
			} else if (Utilities::isInt(*curr)) {
				yearInput = findYear(*curr);		// Exception thrown if year is not current or next
				++curr;
			}
			assert(yearInput>=0 && yearInput<=99);
		}
	}

	if (curr == dateString.end()
		&& yearInput != 0
		&& monthInput != INVALID_MONTH
		&& (1<=dateInput && dateInput<=maxDays)) {
			newDate = yearInput*10000 + (int)monthInput*100 + dateInput;
	}

	log(DEBUG,"Parsed by date: " + std::to_string(newDate));
	return newDate;
}

// Processes dates in these formats:
// - (this/next) DDD/DDDD
int Parser::parseByDay(std::vector<std::string> dayString) {
	if (dayString.empty()) {
		return INVALID_DATE_FORMAT;
	}

	std::vector<std::string>::iterator curr;
	for (curr=dayString.begin(); curr!=dayString.end(); ++curr) {
		*curr = Utilities::stringToLower(*curr);
	}
	curr = dayString.begin();

	int maxDays;
	int newDate = INVALID_DATE_FORMAT;

	time_t currTime = time(nullptr);
	struct tm localTime;
	localtime_s(&localTime,&currTime);
	int year = localTime.tm_year - 100;				// tm_year: years since 1990

	Month month =	(Month)(localTime.tm_mon + 1);	// tm_mon:  Jan starts at 0
	Day day =		  (Day)(localTime.tm_wday);
	int date =				localTime.tm_mday;

	if (*curr == "today" || *curr == "later") {
		++curr;
	} else if (*curr == "tmr" || *curr == "tomorrow") {
		++date;
		++curr;
	} else if (*curr == "this") {
		++curr;
	} else if (*curr == "next") {
		date += 7;
		++curr;
	}

	if (curr!=dayString.end()) {
		Day newDay = Utilities::stringToDay(*curr);
		if (newDay != INVALID_DAY) {
			date += (int)newDay - (int)day;
			++curr;
		}
	}

	if (curr == dayString.end()) {
		maxDays = findMaxDays(month,year);
		if (date>maxDays) {
			month = (Month)(((int)month)+1);
			date-=maxDays;
		}

		Month maxMonth=DEC;
		if (month>maxMonth) {
			year++;
			month = (Month)((int)month-(int)maxMonth);
		}

		// Check that valid date
		if (1<=date && date<=maxDays) {
			newDate = year*10000 + (int)month*100 + date;
		}
	}

	log(DEBUG,"Parsed by day: " + std::to_string(newDate));
	return newDate;
}

// Processes times in these formats:
// - HH    AM/PM (default: assume AM)
// - HH.MM AM/PM (default: assume AM)
// - HHMM        (24-hour)
int Parser::parseTime(std::vector<std::string> timeString) {
	if (timeString.empty() || std::count(timeString.at(0).begin(),timeString.at(0).end(),'.')>1) {
		return INVALID_TIME_FORMAT;
	}

	int time;
	int hour;
	int min = 0;
	std::string hourString;
	std::string minString;

	// Add space between joined am/pm
	timeString = Utilities::stringToVec(Utilities::replace(Utilities::vecToString(timeString),"am"," am"));
	timeString = Utilities::stringToVec(Utilities::replace(Utilities::vecToString(timeString),"pm"," pm"));

	std::vector<std::string>::iterator curr = timeString.begin();
	if (!Utilities::isPositiveNonZeroInt(*curr)) {
		size_t iSemiColon = (*curr).find('.');
		if (iSemiColon == std::string::npos) {
			return INVALID_TIME_FORMAT;
		} else {
			// HH.MM AM/PM
			hourString = (*curr).substr(0, iSemiColon);
			minString = (*curr).substr(iSemiColon + 1);
			hour = Utilities::stringToInt(hourString);
			min = Utilities::stringToInt(minString);
			if (!Utilities::isPositiveNonZeroInt(hourString) || hour >= 24
				|| !Utilities::isInt(minString) || min >= 60) {
					return INVALID_TIME_FORMAT;
			}
		}
	} else {
		time = Utilities::stringToInt(*curr);
		if (time <= 12) {
			// HH (AM/PM)
			hour = time;
		} else if (time>12 && time<24 && timeString.size()==1) {
			// HH (24-hour)
			hour = time-12;
		} else if (time <= 2359) {
			// HHMM
			hour = time/100;
			min  = time%100;
			if (hour >= 24 || min >= 60) {
				return INVALID_TIME_FORMAT;
			}
		} else {
			return INVALID_TIME_FORMAT;
		}
	}

	++curr;
	if (curr != timeString.end()) {
		if (Utilities::containsAny(*curr,"am") && hour <= 12) {
			if (hour == 12) {
				hour -= 12;
			}
		} else if (Utilities::containsAny(*curr,"pm") && hour <= 12) {
			if (hour < 12) {
				hour += 12;
			}
		} else {
			return INVALID_TIME_FORMAT;
		}
		++curr;
	}

	if (curr == timeString.end()) {
		time = hour*100 + min;
	} else {
		return INVALID_TIME_FORMAT;
	}

	log(DEBUG,"Parsed time: " + std::to_string(time));
	return time;
}

FieldType Parser::convertFieldDateToTime(FieldType &inputMode) {
	if (inputMode == START_DATE) {
		inputMode = START_TIME;
	} else if (inputMode == END_DATE) {
		inputMode = END_TIME;
	} else if (inputMode == TODO_DATE) {
		inputMode = TODO_TIME;
	}
	return inputMode;
}

void Parser::convertFieldToReserve(FieldType &inputMode) {
	if (inputMode == START_DATE) {
		inputMode = RESERVE_START_DATE;
	} else if (inputMode == START_TIME) {
		inputMode = RESERVE_START_TIME;
	} else if (inputMode == END_DATE) {
		inputMode = RESERVE_END_DATE;
	} else if (inputMode == END_TIME) {
		inputMode = RESERVE_END_TIME;
	} else if (inputMode == TODO_DATE) {
		inputMode = RESERVE_TODO_DATE;
	} else if (inputMode == TODO_TIME) {
		inputMode = RESERVE_TODO_TIME;
	}
	return;
}

std::string Parser::placeInField(Task* newTask,bool &isTODO,bool &isTODOreserve,bool &isReservation,
								 FieldType inputMode,std::vector<std::string> inputString) {
	if (inputMode == RESERVE) {
		isReservation = true;
		return "";
	}

	log(DEBUG,"Parsing string: " + Utilities::vecToString(inputString));
	int newDate = DATE_NOT_SET;
	int newTime = TIME_NOT_SET;

	if (isDateField(inputMode)) {
		newDate = parseDate(inputString);
		if (newDate == INVALID_DATE_FORMAT) {
			newTime = parseTime(inputString);
			if(newTime == INVALID_DATE_FORMAT) {
				std::string invalidDateTimeString = Utilities::vecToString(inputString);
				return invalidDateTimeString;
			} else {
				convertFieldDateToTime(inputMode);
			}
		}

		if (!isReservation) {
			if (/*newTask->getType()==FLOATING &&*/ isTodoField(inputMode)) {
				isTODO = true;
			} else if (isTODO && inputMode==START_TIME) {
				inputMode = TODO_TIME;
			}
		} else if (isReservation) {
			log(DEBUG,"Parsing reservation: " + Utilities::vecToString(inputString));
			if (newTask->getReserveType()==FLOATING && isTodoField(inputMode)) {
				isTODOreserve = true;
			} else if (isTODOreserve && inputMode==START_TIME) {
				inputMode = TODO_TIME;
			}
			convertFieldToReserve(inputMode);
		}
	}

	switch (inputMode) {
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
		if (newTask->getEndDate() == DATE_NOT_SET) {
			newTask->setEndDate(newDate);
		}
		break;
	case TODO_DATE:
	case END_DATE:
		if (newTask->getType() == FLOATING) {
			logSetTaskType(TODO);
			isTODO = true;
		} else if (isTODO
			|| (newTask->getType()==EVENT && newTask->getStartDate()==DATE_NOT_SET)) {
				newTask->setStartDate(newDate);
		}
		newTask->setEndDate(newDate);
		break;
	case START_TIME:
		logSetTaskType(EVENT);
		newTask->setType(EVENT);
		if ((newTime = parseTime(inputString)) != INVALID_TIME_FORMAT) {
			if (newTask->getStartTime() == TIME_NOT_SET) {
				newTask->setStartTime(newTime);
			}
		} else {
			break;
		}
	case TODO_TIME:
	case END_TIME:
		if ((newTime = parseTime(inputString)) != INVALID_TIME_FORMAT) {
			if (newTask->getStartDate() == DATE_NOT_SET) {
				if (newTask->getEndDate() == DATE_NOT_SET) {
					newTask->setStartDate(parseByDay(Utilities::stringToVec("today")));
				} else {
					newTask->setStartDate(newTask->getEndDate());
				}
			}
			if (newTask->getEndDate() == DATE_NOT_SET) {
				newTask->setEndDate(newTask->getStartDate());
			}

			newTask->setEndTime(newTime);
			if (newTask->getStartTime()!=TIME_NOT_SET && newTask->getStartTime()!=newTask->getEndTime()) {
				logSetTaskType(EVENT);
				newTask->setType(EVENT);
			} else if (newTask->getType()==EVENT && newTask->getStartTime()==TIME_NOT_SET) {
				newTask->setStartTime(newTime);
			} else if (newTask->getType()==FLOATING) {
				logSetTaskType(TODO);
				isTODO = true;
			}
		}
		break;
		//===== Reservation: Follow above and modify accordingly =====
	case RESERVE_START_DATE:
		log(DEBUG,"Placing in reserveStartDate");
		newTask->setReserveType(EVENT);
		newTask->addReserveStartDate(newDate);
		break;
	case RESERVE_TODO_DATE:
	case RESERVE_END_DATE:
		log(DEBUG,"Placing in reserveEndDate");
		if (newTask->getReserveType() == FLOATING) {
			isTODOreserve = true;
		} else if (isTODOreserve
			|| (newTask->getReserveType()==EVENT && newTask->getReserveStartDate()==DATE_NOT_SET)) {
				newTask->addReserveStartDate(newDate);
		}
		newTask->addReserveEndDate(newDate);
		break;
	case RESERVE_START_TIME:
		log(DEBUG,"Placing in reserveStartTime");
		newTask->setReserveType(EVENT);
		if ((newTime = parseTime(inputString)) != INVALID_TIME_FORMAT) {
			if (newTask->getReserveStartTime() == TIME_NOT_SET) {
				newTask->addReserveStartTime(newTime);
			}
		} else {
			break;
		}
	case RESERVE_TODO_TIME:
	case RESERVE_END_TIME:
		log(DEBUG,"Placing in reserveEndTime");
		if ((newTime = parseTime(inputString)) != INVALID_TIME_FORMAT) {
			if (newTask->getReserveStartDate() == DATE_NOT_SET) {
				if (newTask->getReserveEndDate() == DATE_NOT_SET) {
					newTask->addReserveStartDate(parseByDay(Utilities::stringToVec("today")));
				} else {
					newTask->addReserveStartDate(newTask->getReserveEndDate());
				}
			}
			if (newTask->getReserveEndDate() == DATE_NOT_SET) {
				newTask->addReserveEndDate(newTask->getReserveStartDate());
			}
			newTask->addReserveEndTime(newTime);
			if (   newTask->getReserveStartTime() != TIME_NOT_SET
				&& newTask->getReserveStartTime() != newTask->getReserveEndTime()) {
					newTask->setReserveType(EVENT);
			} else if (newTask->getReserveType()==EVENT && newTask->getReserveStartTime()==TIME_NOT_SET) {
				newTask->addReserveStartTime(newTime);
			} else if (newTask->getReserveType()==FLOATING) {
				isTODOreserve = true;
			}
		}
		break;
	case RESERVE:
		//===== Reservation: Follow end =====
	case INVALID_FIELD:
		break;
	}
	return "";
}

int Parser::findMaxDays(Month month, int year) { // default year is 2015
	int maxDays = 0;
	bool isLeap;
	switch (month) {
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
		if (isLeap) {
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
	time_t t = time(nullptr);		// Get current time
	struct tm now;
	localtime_s(&now,&t);
	int year = now.tm_year - 100;	// Get current year, tm_year: years since 1900

	if (yearString == "") {
		return year;
	} else if (Utilities::isPositiveNonZeroInt(yearString)) {
		if (Utilities::stringToInt(yearString) == year
			|| Utilities::stringToInt(yearString) == 2000+year) {
				return year;
		} else if (Utilities::stringToInt(yearString) == year+1
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
	std::string searchPhrase = Utilities::getFirstWord(searchStrings);

	if (isPowerSearchKeywords(searchStrings)) {
		if (!isPowerSearchKeywords(searchPhrase)) {
			return true;	// Searchphrase (takes precedence over Freeslot powersearch)
		} else if (Utilities::containsAny(searchStrings,"for")) {
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

	if (Utilities::isInt(*curr)){
		++curr;
	}

	if (Utilities::stringToFieldType(*curr) == INVALID_FIELD) {
		fields.push_back(NAME);
	}

	FieldType newField;
	std::vector<std::string>::iterator end;
	while (curr != vecInput.end()) {
		newField = Utilities::stringToFieldType(*curr);
		if (newField == LABELS_DELETE
			&& curr+1 != vecInput.end()
			&& Utilities::stringToFieldType(*(curr+1)) != INVALID_FIELD) {
				fields.push_back(LABELS_CLEAR);
		} else if (isDateField(newField) && curr+1 != vecInput.end()) {
			end = curr+2;
			if (curr+2 != vecInput.end()) {
				end = curr+3;
			}
			if (parseDate(std::vector<std::string>(curr+1,end)) == INVALID_DATE_FORMAT
				&& parseTime(std::vector<std::string>(curr+1,end)) != INVALID_TIME_FORMAT) {
					fields.push_back(convertFieldDateToTime(newField));
			} else if (parseDate(std::vector<std::string>(curr+1,end)) != INVALID_DATE_FORMAT) {
				fields.push_back(newField);
			}
		} else if (newField != INVALID_FIELD) {
			fields.push_back(newField);
		}
		++curr;
	}

	log(DEBUG,"Fields extracted: " + Utilities::fieldVecToString(fields));
	return fields;
}

std::vector<std::string> Parser::removeSlashKeywords(std::vector<std::string> vecString) {
	std::vector<std::string>::iterator curr;
	std::string subString;
	for (curr=vecString.begin(); curr!=vecString.end(); ++curr) {
		if ( ((*curr)[0] == '/' || (*curr)[0] == '\\') && isFieldKeyword(subString = (*curr).substr(1)) ) {
			*curr = subString;
		}
	}
	return vecString;
}
