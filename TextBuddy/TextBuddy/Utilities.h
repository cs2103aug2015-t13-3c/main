// @@author Aaron Chong Jun Hao
// @@author Ng Ren Zhi

// Sort methods by return type, then alphabetical order

#ifndef UTILITIES_H_
#define UTILITIES_H_

#include "Task.h"

class Utilities {
private:
	// This is the return value for invalid numbers
	static const int INVALID_NUMBER_FORMAT = -1;

	// These are the possible command types
	static const std::string COMMAND_ADD;
	static const std::string COMMAND_DELETE;
	static const std::string COMMAND_MODIFY;
	static const std::string COMMAND_SEARCH;
	static const std::string COMMAND_CLEAR_ALL;
	static const std::string COMMAND_DISPLAY_ALL;
	static const std::string COMMAND_SORT_ALL;
	static const std::string COMMAND_EXIT;

public:
	Utilities();
	~Utilities();

	// Converters used in multiple architecture components
	static std::string	stringToLower(std::string str);

	static CommandType	stringToCmdType(std::string str);
	static int			stringToInt(std::string str);
	static Day			stringToDay(std::string dayString);
	static Month		stringToMonth(std::string monthString);
	static TaskType		stringToTaskType(std::string line);

	static std::string	boolToIntString(bool boolean);
	static std::string	dayToString(Day day);
	static std::string	taskToString(Task taskToConvert);
	static std::string	taskTypeToString(TaskType type);
	static std::string	vecToString(std::vector<std::string> inputString);

	// Useful methods
	static bool			containsAny(std::string targetWord, std::string searchWords);

	// Credits: Adapted from CityConnect.cpp (CS2103 Tutorial 2)
	static bool			isPositiveNonZeroInt(std::string s);
	static bool			equalsIgnoreCase(const std::string& str1, const std::string& str2);
	static std::string	getFirstWord(std::string userCommand);
	static std::string	removeFirstWord(std::string userCommand);
	static std::string	removeSpaces(const std::string& s, const std::string& delimiters = " \f\n\r\t\v" );
	static std::string	replace(std::string a, std::string b, std::string c);
	static std::vector<std::string> splitParameters(std::string commandParametersString);

};

#endif