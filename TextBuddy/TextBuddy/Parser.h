// @@author Aaron Chong Jun Hao

#ifndef PARSER_H_
#define PARSER_H_

#include"Command.h"
#include"Task.h"

class Parser {
private:
	// bool isActive;

	// This defines the file extension used by TextBuddy
	static const std::string FILE_EXTENSION;

	// These are the possible command types
	static const std::string COMMAND_ADD;
	static const std::string COMMAND_DELETE;
	static const std::string COMMAND_SEARCH;
	static const std::string COMMAND_CLEAR_ALL;
	static const std::string COMMAND_DISPLAY_ALL;
	static const std::string COMMAND_SORT_ALL;
	static const std::string COMMAND_EXIT;

	// These are the possible field types for tasks
	static const std::string FIELD_DATE_BY;
	static const std::string FIELD_DATE_ON;
	static const std::string FIELD_TIME_AT;
	static const std::string FIELD_TIME_FROM;
	static const std::string FIELD_TIME_TO;
	static const std::string FIELD_PRIORITY;
	static const std::string FIELD_LABEL;
	static int runningCount;

	// These are the locations at which various parameters appear
	// static const int PARAM_POSITION_TASK = 0;
	// static const int PARAM_POSITION_TASK_NUMBER = 0;

	// This is the return value for invalid numbers
	static const int INVALID_NUMBER_FORMAT = -1;

	// These functions support user methods
	// static std::string vecToString(std::vector<std::string> inputString);
	static bool containsAny(std::string targetWord, std::string searchWords);
	static int findMaxDays(Month month, int year=2015);
	static Month findMonth(std::string monthString);
	static Day findDay(std::string dayString);
	// Credits: Adapted from CityConnect.cpp (CS2103 Tutorial 2)
	// static std::vector<std::string> splitParameters(std::string commandParametersString);
	static bool isPositiveAndValidInt(std::string s);
	static std::string getFirstWord(std::string userCommand);
	static std::string removeFirstWord(std::string userCommand);

	static std::string removeSpaces(const std::string& s, const std::string& delimiters = " \f\n\r\t\v" );
	static bool equalsIgnoreCase(const std::string& str1, const std::string& str2);
	static int parseInt(std::string str);
	static std::string replace(std::string a, std::string b, std::string c);

public:
	Parser();
	~Parser();

	static std::string parseFileName(char* argv[]);
	Command parseCommand(std::string userCommand);
	Task parseTask(std::string restOfCommand);

	// These handle task parameters
	static int parseDate(std::vector<std::string> inputString);
	static int parseTime(std::vector<std::string> inputString);
	// Credits: Adapted from CityConnect.cpp (CS2103 Tutorial 2)
	static std::vector<std::string> splitParameters(std::string commandParametersString);

	// Temporary methods for unit testing
	static std::string vecToString(std::vector<std::string> inputString);
	static std::string taskToBuffer(Task taskToConvert);
};

#endif