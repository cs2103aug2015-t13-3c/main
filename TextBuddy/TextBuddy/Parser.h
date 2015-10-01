// @@author Aaron Chong Jun Hao

#ifndef PARSER_H_
#define PARSER_H_

class Parser {
private:
	Utilities u;

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

public:
	Parser();
	~Parser();

	// This is the API
	static std::string parseFileName(char* argv[]);
	Command parse(std::string userInput);
	Command parseCommand(std::string userCommand);
	Task parseTask(std::string restOfCommand);

	// These functions support user methods
	static int findMaxDays(Month month, int year=2015);
	static Month findMonth(std::string monthString);
	static Day findDay(std::string dayString);

	// These handle task parameters
	static int parseDate(std::vector<std::string> inputString);
	static int parseTime(std::vector<std::string> inputString);
};

#endif