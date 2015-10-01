// @@author Aaron Chong Jun Hao
// @@author Ng Ren Zhi

#include "stdafx.h"
#include "Utilities.h"

Utilities::Utilities() {}

Utilities::~Utilities() {}

// ==================================================
//      USED IN MULTIPLE ARCHITECTURE COMPONENTS
// ==================================================

// Empty


// ==================================================
//                   USED IN PARSER
// ==================================================

std::string Utilities::taskToBuffer(Task task) {
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

// Credits: Adapted from CityConnect.cpp (CS2103 Tutorial 2)

bool Utilities::isPositiveAndValidInt(std::string s) {
	int i = parseInt(s);

	if(i == INVALID_NUMBER_FORMAT || i <= 0 ) {
		return false;
	} else {
		return true;
	}
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

int Utilities::parseInt(std::string str) {
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

std::string Utilities::replace(std::string a, std::string b, std::string c) {
	int pos;
	do {
		pos = a.find(b);
		if(pos != -1) a.replace(pos, b.length(), c);
	} while (pos != -1);
	return a;
}


// ==================================================
//                     USED IN IO
// ==================================================

