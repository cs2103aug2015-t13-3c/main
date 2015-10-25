// @@author A0110376N (Aaron Chong Jun Hao)
// Methods are listed in order of return type, then alphabetical order

#include "stdafx.h"

Utilities::Utilities() {}
Utilities::~Utilities() {}

//==================================================
//                     CONVERTERS
//==================================================

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
		return LAST_TASK_INDICATOR;
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
	} else if(equalsIgnoreCase(cmdString, COMMAND_MARKDONE)) {
		cmd = MARKDONE;
	} else if(equalsIgnoreCase(cmdString, COMMAND_UNDO)) {
		cmd = UNDO;
	} else if(equalsIgnoreCase(cmdString, COMMAND_REDO)) {
		cmd = REDO;
	} else if(equalsIgnoreCase(cmdString, COMMAND_VIEW)) {
		cmd = VIEW;
	} else if(equalsIgnoreCase(cmdString, COMMAND_CLEAR_ALL)) {
		cmd = CLEAR_ALL;
	} else if(equalsIgnoreCase(cmdString, COMMAND_DISPLAY_ALL)) {
		cmd = DISPLAY_ALL;
	} else if(equalsIgnoreCase(cmdString, COMMAND_LOAD)) {
		cmd = LOAD;
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

	if(equalsIgnoreCase(fieldString,FIELD_NAME)) {
		field = NAME;
	} else if(equalsIgnoreCase(fieldString,FIELD_LABEL_ADD)) {
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

TaskType Utilities::stringToTaskType(std::string taskString) {
	TaskType type = FLOATING;

	if(taskString == "FLOATING") {
		type = FLOATING;
	} else if(taskString == "EVENT") {
		type = EVENT;
	} else if(taskString == "TODO") {
		type = TODO;
	}
	return type;
}

ViewType Utilities::stringToViewType(std::string viewString) {
	ViewType view;

	if(viewString == "") {
		view = VIEWTYPE_INVALID;
	} else if(equalsIgnoreCase(viewString,VIEW_ALL)) {
		view = VIEWTYPE_ALL;
	} else if(equalsIgnoreCase(viewString,VIEW_FLOATING)) {
		view = VIEWTYPE_FLOATING;
	} else if(equalsIgnoreCase(viewString,VIEW_EVENT)) {
		view = VIEWTYPE_EVENT;
	} else if(equalsIgnoreCase(viewString,VIEW_TODO)) {
		view = VIEWTYPE_TODO;
	} else if(equalsIgnoreCase(viewString,VIEW_NOTDONE)) {
		view = VIEWTYPE_NOTDONE;
	} else if(equalsIgnoreCase(viewString,VIEW_PAST)) {
		view = VIEWTYPE_PAST;
	} else if(equalsIgnoreCase(viewString,VIEW_WEEK)) {
		view = VIEWTYPE_WEEK;
	} else {
		view = VIEWTYPE_LABELS;
	}
	return view;

	// Check that all viewtypes are covered (never run because of return statement above)
	switch(view) {
	case VIEWTYPE_INVALID:
	case VIEWTYPE_ALL:
	case VIEWTYPE_FLOATING:
	case VIEWTYPE_EVENT:
	case VIEWTYPE_TODO:
	case VIEWTYPE_NOTDONE:
	case VIEWTYPE_PAST:
	case VIEWTYPE_WEEK:
	case VIEWTYPE_LABELS:
		break;
	}
}

// This converts std::string to std::vector<std::string> based on delimiter space
std::vector<std::string> Utilities::stringToVec(std::string commandParametersString) {
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

std::string Utilities::fieldVecToString(std::vector<FieldType> fieldsToModify) {
	std::vector<FieldType>::iterator curr = fieldsToModify.begin();
	std::string newString;

	while(curr != fieldsToModify.end()) {
		switch(*curr) {
		case NAME: newString += FIELD_NAME;
			break;
		case LABELS_ADD: newString += FIELD_LABEL_ADD;
			break;
		case LABELS_DELETE: newString += FIELD_LABEL_DELETE;
			break;
		case LABELS_CLEAR: newString += "LABELS_CLEAR";
			break;
		case PRIORITY_SET: newString += FIELD_PRIORITY_SET;
			break;
		case PRIORITY_UNSET: newString += FIELD_PRIORITY_UNSET;
			break;
		case START_DATE: newString += FIELD_DATE_FROM;
			break;
		case END_DATE: newString += FIELD_DATE_TO;
			break;
		case START_TIME: newString += FIELD_TIME_AT;
			break;
		case END_TIME: newString += "FIELD_END_TIME";
			break;
		case INVALID_FIELD:
			newString += "FIELD_INVALID";
			break;
		}

		TbLogger::getInstance(); // Somehow removing this crashes the program (Aaron)
		if(++curr != fieldsToModify.end()) {
			newString += " ";
		}
	}
	return newString;
}

std::string Utilities::taskToString(Task task) {
	const int MAX_BYTES = 2550;
	char buffer[MAX_BYTES] = "";

	sprintf_s(buffer, "%s%s\n%s%s\n%s%s\n%s%d\n%s%d\n%s%d\n%s%d\n%s%d\n%s%d\n",
		"Name: ",		task.getName().c_str(),						// %s%s\n
		"Type: ",		taskTypeToString(task.getType()).c_str(),	// %s%s\n
		"Labels: ",		task.getLabelString().c_str(),				// %s%s\n
		"Done: ",		task.getDoneStatus(),						// %s%d\n
		"Priority: ",	task.getPriorityStatus(),					// %s%d\n
		"Start Date: ",	task.getStartDate(),						// %s%d\n
		"Start Time: ",	task.getStartTime(),						// %s%d\n
		"End Date: ",	task.getEndDate(),							// %s%d\n
		"End Time: ",	task.getEndTime()							// %s%d\n
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

//==================================================
//             COMPARISONS AND MODIFIERS
//==================================================

bool Utilities::containsAny(std::string searchWord, std::string words) {
	searchWord = stringToLower(searchWord);
	words = stringToLower(words);
	std::vector<std::string> vecWords = stringToVec(words);
	std::vector<std::string>::iterator curr;

	for(curr=vecWords.begin(); curr!=vecWords.end(); curr++) {
		if(searchWord == *curr) {
			return true;
		}
	}
	return false;
}

bool Utilities::isInt(std::string intString) {
	return !(intString.empty()) && intString.find_first_not_of("0123456789")==std::string::npos;
}

bool Utilities::isKeyword(std::string str) {
	return stringToFieldType(str)!=INVALID_FIELD;
}

std::vector<std::string> Utilities::removeSlashKeywords(std::vector<std::string> inputString) {
	std::vector<std::string>::iterator curr;
	std::string subString;
	for(curr=inputString.begin(); curr!=inputString.end(); curr++) {
		if( ((*curr)[0] == '/' || (*curr)[0] == '\\') && isKeyword(subString = (*curr).substr(1)) ) {
			*curr = subString;
		}
	}
	return inputString;
}

// Credits: Adapted from CityConnect.cpp (CS2103 Tutorial 2)
// @@author A0110376N-reused

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
	return stringToVec(userCommand)[0];
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
	int fromLength = from.length();
	std::string newString = "";
	do {
		pos = str.find(from);
		if(pos != -1) {
			newString += str.substr(0,pos) + to;
			str = str.substr(pos+fromLength); // Bugfix: infinite loop if str not updated (Ren Zhi)
		}
	} while (pos != -1);
	newString += str;
	return newString;
}

bool Utilities::isSubstring(std::string subString, std::string wordString) {
	bool isFound = false;
	std::string::iterator pos = std::search(wordString.begin(), wordString.end(), 
		subString.begin(),subString.end(),equalsIgnoreCase_char);
	if(pos != wordString.end()) {
		isFound = true;
	}
	return isFound;
}

bool Utilities::equalsIgnoreCase_char (char l, char r) {
	return (tolower(l) == tolower(r));
}


// @@author A0126677U (Soon Hao Ye)

int Utilities::getLocalDay() {
	time_t t = time(0); 
	struct tm now;
	localtime_s(&now,&t);  
	return now.tm_mday;
}

int Utilities::getLocalMonth() {
	time_t t = time(0); 
	struct tm now;
	localtime_s(&now,&t);  
	return now.tm_mon + 1;
}

int Utilities::getLocalYear() {
	time_t t = time(0); 
	struct tm now;
	localtime_s(&now,&t);  
	return now.tm_year - 100 ;
}

//==================================================
//           STRING-FOR-DISPLAY FORMATTERS
//==================================================
std::string Utilities::getDate(int date) {
	assert(time != 0);
	int localYear = getLocalYear(); 
	int day = date % 100;
	int month = (date % 10000)/100;
	int year = date/10000;

	std::string d = std::to_string(day) + "/" + std::to_string(month);
	if(year != localYear) {
		d = d + "/" + std::to_string(year);
	}
	return d;
}

std::string Utilities::getTime(int time) {
	assert(time >= 0);
	double time2;
	std::stringstream stream;
	std::string amOrPm;
	if(time < 1200) {
		if(time == 0) {
			time = 1200;
		}
		amOrPm = " am";
	} else {
		if(time > 1259) {
			time = time - 1200;
		}
		amOrPm = " pm";
	}	
	time2 = time/100.0 ;
	stream << std::fixed << std::setprecision(2) << time2;
	return stream.str() + amOrPm ;
}
