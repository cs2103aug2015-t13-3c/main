// @@author A0110376N (Aaron Chong Jun Hao)
// Methods are listed in order of return type, then alphabetical order

#ifndef UTILITIES_H_
#define UTILITIES_H_

#include "Task.h"
#include "Command.h"
#include <string>

// This is the return value for invalid numbers
const int INVALID_NUMBER_FORMAT = -1;
const int LAST_TASK_INDICATOR = 0;

class Utilities {
private:
	// No public constructors and destructors, since Utilities is not an object
	Utilities();
	~Utilities();

public:
	//========== String-to-Type Converters ==========
	static std::string	stringToLower(std::string str);
	static CommandType	stringToCmdType(std::string str);
	static int			stringToInt(std::string str);
	static Day			stringToDay(std::string dayString);
	static Month		stringToMonth(std::string monthString);
	static FieldType	stringToFieldType(std::string fieldString);
	static TaskType		stringToTaskType(std::string line);
	static ViewType		stringToViewType(std::string viewString);
	static std::vector<std::string> stringToVec(std::string commandParametersString);
	static std::vector<std::string> splitParameters(std::string commandParametersString); // To be removed
	//========== Type-to-String Converters ==========
	static std::string	boolToString(bool boolean);
	static std::string	dayToString(Day day);
	static std::string	taskToString(Task taskToConvert);
	static std::string	taskTypeToString(TaskType type);
	static std::string	fieldVecToString(std::vector<FieldType> fieldsToModify);
	static std::string	vecToString(std::vector<std::string> inputString);

	//========== Comparisons and Modifiers ==========
	static bool			containsAny(std::string searchWord, std::string words);
	static bool			isInt(std::string intString);
	static bool			isKeyword(std::string str);
	static bool			isSubstring(std::string subString, std::string wordString);
	static std::vector<std::string>	removeSlashKeywords(std::vector<std::string> inputString);
	// Strict weak ordering function for isSubstring()
	static bool			equalsIgnoreCase_char(char l, char r); 
	// Credits: Adapted from CityConnect.cpp (CS2103 Tutorial 2)
	// @@author A0110376N-reused
	static bool			equalsIgnoreCase(const std::string& str1, const std::string& str2);
	static bool			isPositiveNonZeroInt(std::string s);
	static std::string	getFirstWord(std::string userCommand);
	static std::string	removeFirstWord(std::string userCommand);
	static std::string	removeSpaces(const std::string& s, const std::string& delimiters = " \f\n\r\t\v" );
	static std::string	replace(std::string str, std::string from, std::string to);

	//========== String-for-Display Formatters ==========
	// @@author A0126677U (Soon Hao Ye)
	static std::string taskDateAndTimeToDisplayString(Task task);
	static std::string intDateToDayString(int taskDate);
	static std::string intTimeTo12HourString(int time);
};

#endif