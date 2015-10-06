// @@author Aaron Chong Jun Hao

// Methods are listed in order of return type, then alphabetical order

#ifndef UTILITIES_H_
#define UTILITIES_H_

#include "Task.h"

// These are the possible command types
const std::string COMMAND_ADD = "add";
const std::string COMMAND_DELETE = "delete";
const std::string COMMAND_MODIFY = "modify";
const std::string COMMAND_SEARCH = "search";
const std::string COMMAND_CLEAR_ALL = "clear";
const std::string COMMAND_DISPLAY_ALL = "display";
const std::string COMMAND_SORT_ALL = "sort";
const std::string COMMAND_EXIT = "exit";

class Utilities {
private:
	// This is the return value for invalid numbers
	static const int INVALID_NUMBER_FORMAT = -1;

public:
	Utilities();
	~Utilities();

	// Converters used in multiple architecture components
	static std::string	stringToLower(std::string str);

	static CommandType	stringToCmdType(std::string str);
	static int			stringToInt(std::string str);
	static Day			stringToDay(std::string dayString);
	static Month		stringToMonth(std::string monthString);
	static FieldType	stringToFieldType(std::string fieldString);
	static TaskType		stringToTaskType(std::string line);
	static std::vector<std::string> splitParameters(std::string commandParametersString);

	static std::string	boolToString(bool boolean);
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

};

#endif