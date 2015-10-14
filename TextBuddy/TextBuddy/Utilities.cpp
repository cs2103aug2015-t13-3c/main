// @@author Aaron Chong Jun Hao
// Methods are listed in order of return type, then alphabetical order

#include "stdafx.h"

Utilities::Utilities() {}
Utilities::~Utilities() {}

// ==================================================
//                     CONVERTERS
// ==================================================

std::string Utilities::stringToLower(std::string str) {
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}

//========== String-to-Type Converters ==========

int Utilities::stringToInt(std::string str) {
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

CommandType Utilities::stringToCmdType(std::string cmdString) {
	CommandType cmd;

	if(equalsIgnoreCase(cmdString, COMMAND_ADD))	{
		cmd = ADD;
	} else if(equalsIgnoreCase(cmdString, COMMAND_DELETE)) {
		cmd = DELETE;
	} else if(equalsIgnoreCase(cmdString, COMMAND_MODIFY)) {
		cmd = MODIFY;
	} else if(equalsIgnoreCase(cmdString, COMMAND_SEARCH)) {
		cmd = SEARCH;
	} else if(equalsIgnoreCase(cmdString, COMMAND_CLEAR_ALL)) {
		cmd = CLEAR_ALL;
	} else if(equalsIgnoreCase(cmdString, COMMAND_DISPLAY_ALL)) {
		cmd = DISPLAY_ALL;
	} else if(equalsIgnoreCase(cmdString, COMMAND_SORT_ALL)) {
		cmd = SORT_ALL;
	} else if(equalsIgnoreCase(cmdString, COMMAND_SAVE)) {
		cmd = SAVE;
	} else if(equalsIgnoreCase(cmdString, COMMAND_EXIT)) {
		cmd = EXIT;
	} else {
		cmd = INVALID;
	}

	return cmd;
}

Day Utilities::stringToDay(std::string dayString) {
	Day day = INVALID_DAY;

	if(containsAny(dayString,"sun sunday")) {
		day = SUN;
	} else if(containsAny(dayString,"mon monday")) {
		day = MON;
	} else if(containsAny(dayString,"tue tues tuesday")) {
		day = TUE;
	} else if(containsAny(dayString,"wed wednesday")) {
		day = WED;
	} else if(containsAny(dayString,"thu thur thurs thursday")) {
		day = THU;
	} else if(containsAny(dayString,"fri friday")) {
		day = FRI;
	} else if(containsAny(dayString,"sat saturday")) {
		day = SAT;
	}

	return day;
}

Month Utilities::stringToMonth(std::string monthString) {
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

FieldType Utilities::stringToFieldType(std::string fieldString) {
	FieldType field;

	if(equalsIgnoreCase(fieldString,FIELD_LABEL_ADD)) {
		field = LABELS_ADD;
	} else if(equalsIgnoreCase(fieldString,FIELD_LABEL_DELETE)) {
		field = LABELS_DELETE;
	} else if(equalsIgnoreCase(fieldString,FIELD_PRIORITY_SET)) {
		field = PRIORITY_SET;
	} else if(equalsIgnoreCase(fieldString,FIELD_PRIORITY_UNSET)) {
		field = PRIORITY_UNSET;
	} else if(equalsIgnoreCase(fieldString,FIELD_DATE_ON)) {
		field = START_DATE;
	} else if(equalsIgnoreCase(fieldString,FIELD_DATE_FROM)) {
		field = START_DATE;
	} else if(equalsIgnoreCase(fieldString,FIELD_DATE_TO)) {
		field = END_DATE;
	} else if(equalsIgnoreCase(fieldString,FIELD_DATE_BY)) {
		field = END_DATE;
	} else if(equalsIgnoreCase(fieldString,FIELD_TIME_AT)) {
		field = START_TIME;
	} else {
		field = INVALID_FIELD;
	}
	return field;
}

TaskType Utilities::stringToTaskType(std::string line) {
	TaskType type = FLOATING;

	if(line == "FLOATING") {
		type = FLOATING;
	} else if (line == "EVENT") {
		type = EVENT;
	} else if (line == "TODO") {
		type = TODO;
	}

	return type;
}

// This converts std::string to std::vector<std::string> based on delimiter space
std::vector<std::string> Utilities::splitParameters(std::string commandParametersString) {
	std::vector<std::string> tokens;
	std::istringstream iss(commandParametersString);
	std::copy(std::istream_iterator<std::string>(iss),
		std::istream_iterator<std::string>(),
		std::back_inserter<std::vector<std::string>>(tokens));
	return tokens;
}

//========== Type-to-String Converters ==========

std::string Utilities::boolToString(bool boolean) {
	if(boolean) {
		return "true";
	} else {
		return "false";
	}
}

std::string Utilities::dayToString(Day day) {
	std::string dayString;
	switch(day) {
	case SUN:
		dayString = "Sunday";
		break;
	case MON:
		dayString = "Monday";
		break;
	case TUE:
		dayString = "Tuesday";
		break;
	case WED:
		dayString = "Wednesday";
		break;
	case THU:
		dayString = "Thursday";
		break;
	case FRI:
		dayString = "Friday";
		break;
	case SAT:
		dayString = "Saturday";
		break;
	case INVALID_DAY:
		dayString = "INVALID_DAY";
		break;
	}

	return dayString;
}

std::string Utilities::taskToString(Task task) {
	const int MAX_BYTES = 2550;
	char buffer[MAX_BYTES] = "";

	sprintf_s(buffer, "%s%s\n%s%d\n%s%d\n%s%d\n",
		"Name: ",		task.getName().c_str(),		// %s%s\n
		/*
		"Type: ",		task.getType(),				// %s%d\n
		"Label: ",		task.getLabel().c_str(),	// %s%s\n
		"Done: ",		task.getDoneStatus(),		// %s%d\n
		"Priority: ",	task.getPriorityStatus(),	// %s%d\n
		"Start Date: ",	task.getStartDate(),		// %s%d\n
		*/
		"Start Time: ",	task.getStartTime(),		// %s%d\n
		"End Date: ",	task.getEndDate(),			// %s%d\n
		"End Time: ",	task.getEndTime()			// %s%d\n
		);

	return buffer;
}

std::string Utilities::taskTypeToString(TaskType type) {
	std::string typeString;
	switch(type) {
	case FLOATING:
		typeString = "FLOATING";
		break;
	case EVENT:
		typeString = "EVENT";
		break;
	case TODO:
		typeString = "TODO";
		break;
	}

	return typeString;
}

// NOTE TO AARON: method doesn't work in IOTest.cpp
// REPLY: Please refer to UtilitiesTest.cpp (Aaron)
std::string Utilities::vecToString(std::vector<std::string> inputString) {
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

// ==================================================
//             COMPARISONS AND MODIFIERS
// ==================================================

bool Utilities::containsAny(std::string targetWord, std::string searchWords) {
	targetWord = stringToLower(targetWord);
	searchWords = stringToLower(searchWords);
	std::vector<std::string> vecSearchWords = splitParameters(searchWords);
	std::vector<std::string>::iterator curr;

	for(curr=vecSearchWords.begin(); curr!=vecSearchWords.end(); curr++) {
		if(targetWord == *curr) {
			return true;
		}
	}

	return false;
}

bool Utilities::isInt(std::string intString) {
	return !(intString.empty()) && intString.find_first_not_of("0123456789")==std::string::npos;
}

// Credits: Adapted from CityConnect.cpp (CS2103 Tutorial 2)

bool Utilities::isPositiveNonZeroInt(std::string s) {
	int i = stringToInt(s);

	if(i == INVALID_NUMBER_FORMAT || i <= 0 ) {
		return false;
	} else {
		return true;
	}
}

bool Utilities::equalsIgnoreCase(const std::string& str1, const std::string& str2) {
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

std::string Utilities::getFirstWord(std::string userCommand) {
	return splitParameters(userCommand)[0];
}

std::string Utilities::removeFirstWord(std::string userCommand) {
	std::string commandTypeString = getFirstWord(userCommand);
	std::string parameters = removeSpaces(replace(userCommand, commandTypeString, ""));
	return parameters;
}

std::string Utilities::removeSpaces(const std::string& s, const std::string& delimiters) {
	if(!s.empty()) {
		std::string trimEnd = s.substr(0, s.find_last_not_of(delimiters) + 1);
		std::string trimStart = trimEnd.substr(trimEnd.find_first_not_of(delimiters));
		return trimStart;
	} else {
		return s;
	}
}

std::string Utilities::replace(std::string str, std::string from, std::string to) {
	int pos;
	do {
		pos = str.find(from);
		if(pos != -1) str.replace(pos, from.length(), to);
	} while (pos != -1);
	return str;
}

bool Utilities::isSubstring(std::string phrase, std::string search) {
	bool isFound = false;
	std::string::iterator pos = std::search(search.begin(), search.end(), 
		phrase.begin(),phrase.end(),equalsIgnoreCase_char);
	if(pos != search.end()) {
		isFound = true;
	}
	return isFound;
}

bool Utilities::equalsIgnoreCase_char (char l, char r) {
	return (tolower(l) == tolower(r));
}

// ==================================================
//           STRING-FOR-DISPLAY FORMATTERS
// ==================================================
// @@author Soon Hao Ye

std::string Utilities::taskDateAndTimeToDisplayString(Task task) {
	std::string placeHolder;
	std::string start;
	std::string end;
	int startDate = task.getStartDate();
	int startTime = task.getStartTime();

	if(task.getType() == TODO) {
		end = end + intDateToDayString(task.getEndDate());
		return "by " + end ;
	} else if(task.getType() == EVENT) {
		if(startDate != 0) {
			start = intDateToDayString(startDate);		
		}
		if(startTime != 0) {
			start = start + " " + intTimeTo12HourString(startTime);		
		}
		if(task.getEndDate() != startDate) {
			end = end + intDateToDayString(task.getEndDate());
			placeHolder = " to ";
		}
		if(task.getEndTime() != 0 && task.getStartTime() != task.getEndTime()) {
			end = end + " " + intTimeTo12HourString(task.getEndTime());
			placeHolder = " to ";
		}
		return start + placeHolder +  end ;
	} else {
		return "";
	}
}

std::string Utilities::intTimeTo12HourString(int time) {
	double time2;
	std::stringstream stream;
	if(time < 1200) {
		time2 = time/100.0 ;
		stream << std::fixed << std::setprecision(2) << time2;
		return stream.str() + " am";
	} else {
		if(time > 1259) {
			time = time - 1200;
		}
		stream.clear();
		time2 = time/100.0 ;
		stream << std::fixed << std::setprecision(2) << time2;
		return stream.str() + " pm";
	}	
}

std::string Utilities::intDateToDayString(int taskDate) {
	time_t t = time(0); // get current time
	struct tm now;
	localtime_s(&now,&t);

	// int localYear = now.tm_year - 100;
	int localMonth = now.tm_mon + 1;
	int localDay = now.tm_mday;
	int localWDay = (now.tm_wday);

	std::string date_UI;

	int date = taskDate;
	int day = date % 100;
	int month = (date % 10000)/100;
	int year = date/10000; 
	int differenceInDays = day - localDay;
	if(month == localMonth && day >= localDay && differenceInDays < 8) {	
		int dayOfWeek = (localWDay + differenceInDays);			
		if(dayOfWeek > 6) {
			dayOfWeek = dayOfWeek % 7;
			date_UI = dayToString((Day)dayOfWeek);
			date_UI = "next " + date_UI;
		} else {
			date_UI = dayToString((Day)dayOfWeek);
		}
	} else {
		date_UI = std::to_string(day) + "/" + std::to_string(month) + "/" +
			std::to_string(year);
	}
	return date_UI;
}