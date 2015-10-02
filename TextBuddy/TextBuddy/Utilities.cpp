// @@author Aaron Chong Jun Hao
// @@author Ng Ren Zhi

#include "stdafx.h"

Utilities::Utilities() {}
Utilities::~Utilities() {}

// ==================================================
//  CONVERTERS FOR MULTIPLE ARCHITECTURE COMPONENTS
// ==================================================

std::string Utilities::stringToLower(std::string str) {
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}

// String to other types

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

Day Utilities::stringToEnumDay(std::string dayString) {
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

Month Utilities::stringToEnumMonth(std::string monthString) {
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

TaskType Utilities::stringToEnumType(std::string line) {
	TaskType type;

	if(line == "FLOATING") {
		type = FLOATING;
	} else if (line == "EVENT") {
		type = EVENT;
	} else if (line == "TODO") {
		type = TODO;
	}

	return type;
}

// Other types to string

std::string Utilities::boolToIntString(bool boolean) {
	if(boolean) {
		return "1";
	} else {
		return "0";
	}
}

std::string Utilities::enumDayToString(Day day) {
	std::string dayString;
	switch(day) {
	case SUN:
		dayString = "SUN";
		break;
	case MON:
		dayString = "MON";
		break;
	case TUE:
		dayString = "TUE";
		break;
	case WED:
		dayString = "WED";
		break;
	case THU:
		dayString = "THU";
		break;
	case FRI:
		dayString = "FRI";
		break;
	case SAT:
		dayString = "SAT";
		break;
	case INVALID_DAY:
		dayString = "INVALID_DAY";
		break;

	}

	return dayString;
}

std::string Utilities::enumTypeToString(TaskType type) {
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

std::string Utilities::taskToString(Task task) {
	const int MAX_BYTES = 2550;
	char buffer[MAX_BYTES] = "";

	sprintf_s(buffer, "%s%s\n"/*"%s%d\n%s%s\n%s%d\n%s%d\n%s%d\n%s%d\n%s%d\n%s%d\n%s%d\n%s%d\n"/*11 outputs*/,
		"Name: ",		task.getName().c_str()/*,
		"Type: ",		task.getType(),
		"Label: ",		task.getLabel().c_str(),
		"Done: ",		task.getDoneStatus(),
		"Priority: ",	task.getPriorityStatus(),
		"Start Day: ",	task.getStartDay(),
		"Start Date: ",	task.getStartDate(),
		"Start Time: ",	task.getStartTime(),
		"End Day: ",	task.getEndDay(),
		"End Date: ",	task.getEndDate(),
		"End Time: ",	task.getEndTime()*/
		);

	return buffer;
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


// ==================================================
//                USED IN PARSER ONLY
// ==================================================

bool Utilities::containsAny(std::string targetWord, std::string searchWords) {
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

std::string Utilities::replace(std::string a, std::string b, std::string c) {
	int pos;
	do {
		pos = a.find(b);
		if(pos != -1) a.replace(pos, b.length(), c);
	} while (pos != -1);
	return a;
}

// This method only splits strings based on delimiter space
std::vector<std::string> Utilities::splitParameters(std::string commandParametersString) {
	std::vector<std::string> tokens;
	std::istringstream iss(commandParametersString);
	std::copy(std::istream_iterator<std::string>(iss),
		std::istream_iterator<std::string>(),
		std::back_inserter<std::vector<std::string>>(tokens));
	return tokens;
}
