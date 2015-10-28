// Aaron Chong Jun Hao @@author A0110376N
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

CommandType Utilities::stringToCmdType(std::string str) {
	CommandType cmd;

	if(equalsIgnoreCase(str, COMMAND_ADD))	{
		cmd = ADD;
	} else if(equalsIgnoreCase(str, COMMAND_DELETE)) {
		cmd = DELETE;
	} else if(equalsIgnoreCase(str, COMMAND_MODIFY)) {
		cmd = MODIFY;
	} else if(equalsIgnoreCase(str, COMMAND_SEARCH)) {
		cmd = SEARCH;
	} else if(equalsIgnoreCase(str, COMMAND_MARKDONE)) {
		cmd = MARKDONE;
	} else if(equalsIgnoreCase(str, COMMAND_UNDO)) {
		cmd = UNDO;
	} else if(equalsIgnoreCase(str, COMMAND_REDO)) {
		cmd = REDO;
	} else if(equalsIgnoreCase(str, COMMAND_VIEW)) {
		cmd = VIEW;
	} else if(equalsIgnoreCase(str, COMMAND_CLEAR_ALL)) {
		cmd = CLEAR_ALL;
	} else if(equalsIgnoreCase(str, COMMAND_DISPLAY_ALL)) {
		cmd = DISPLAY_ALL;
	} else if(equalsIgnoreCase(str, COMMAND_LOAD)) {
		cmd = LOAD;
	} else if(equalsIgnoreCase(str, COMMAND_SAVE)) {
		cmd = SAVE;
	} else if(equalsIgnoreCase(str, COMMAND_EXIT)) {
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
		field = TODO_DATE;
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

std::vector<std::string> Utilities::stringToVec(std::string str) {
	std::vector<std::string> tokens;
	std::istringstream iss(str);
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

std::string Utilities::fieldVecToString(std::vector<FieldType> fields) {
	std::vector<FieldType>::iterator curr = fields.begin();
	std::string newString;

	while(curr != fields.end()) {
		switch(*curr) {
		case NAME:
			newString += FIELD_NAME;
			break;
		case LABELS_ADD:
			newString += FIELD_LABEL_ADD;
			break;
		case LABELS_DELETE:
			newString += FIELD_LABEL_DELETE;
			break;
		case LABELS_CLEAR:
			newString += "LABELS_CLEAR";
			break;
		case PRIORITY_SET:
			newString += FIELD_PRIORITY_SET;
			break;
		case PRIORITY_UNSET:
			newString += FIELD_PRIORITY_UNSET;
			break;
		case START_DATE:
			newString += FIELD_DATE_FROM;
			break;
		case START_TIME:
			newString += FIELD_TIME_AT;
			break;
		case END_DATE:
			newString += FIELD_DATE_TO;
			break;
		case END_TIME:
			newString += "FIELD_END_TIME";
			break;
		case TODO_DATE:
		case TODO_TIME:
			newString += FIELD_DATE_BY;
			break;
		case INVALID_FIELD:
			newString += "FIELD_INVALID";
			break;
		}

		TbLogger::getInstance(); // Somehow removing this crashes the program (Aaron)
		if(++curr != fields.end()) {
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

std::string Utilities::vecToString(std::vector<std::string> vecString) {
	std::string newString;
	std::vector<std::string>::iterator curr;
	for(curr=vecString.begin(); curr!=vecString.end(); ) {
		newString += *curr;
		if(++curr != vecString.end()) {
			newString += " ";
		}
	}
	return newString;
}

//==================================================
//             COMPARISONS AND MODIFIERS
//==================================================

bool Utilities::containsAny(std::string words1, std::string words2) {
	words1 = stringToLower(words1);
	words2 = stringToLower(words2);
	std::vector<std::string> vecWords1 = stringToVec(words1);
	std::vector<std::string> vecWords2 = stringToVec(words2);
	std::vector<std::string>::iterator curr1;
	std::vector<std::string>::iterator curr2;

	for(curr1=vecWords1.begin(); curr1!=vecWords1.end(); curr1++) {
		for(curr2=vecWords2.begin(); curr2!=vecWords2.end(); curr2++) {
			if(*curr1 == *curr2) {
				TbLogger::getInstance()->log(DEBUG,"Checking that " + *curr1 + " = " + *curr2);
				return true;
			}
		}
	}
	return false;
}

bool Utilities::isInt(std::string str) {
	return !(str.empty()) && str.find_first_not_of("0123456789")==std::string::npos;
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

std::string Utilities::getFirstWord(std::string words) {
	return stringToVec(words)[0];
}

std::string Utilities::removeFirstWord(std::string words) {
	std::string commandTypeString = getFirstWord(words);
	std::string parameters = removeSpaces(replace(words, commandTypeString, ""));
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

bool Utilities::isSubstring(std::string subString, std::string words) {
	bool isFound = false;
	std::string::iterator pos = std::search(words.begin(), words.end(), 
		subString.begin(),subString.end(),equalsIgnoreCase_char);
	if(pos != words.end()) {
		isFound = true;
	}
	return isFound;
}

bool Utilities::equalsIgnoreCase_char (char l, char r) {
	return (tolower(l) == tolower(r));
}

// Soon Hao Ye @@author A0126677U
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
