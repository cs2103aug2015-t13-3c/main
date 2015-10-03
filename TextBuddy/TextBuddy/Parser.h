// @@author Aaron Chong Jun Hao

#ifndef PARSER_H_
#define PARSER_H_

class Parser {
private:
	Utilities u;

	// This defines the file extension used by TextBuddy
	static const std::string FILE_EXTENSION;

	// These are the possible field types for tasks
	static const std::string FIELD_DATE_BY;
	static const std::string FIELD_DATE_ON;
	static const std::string FIELD_TIME_AT;
	static const std::string FIELD_TIME_FROM;
	static const std::string FIELD_TIME_TO;
	static const std::string FIELD_PRIORITY;
	static const std::string FIELD_LABEL;
	static int runningCount;

	// This is the return value for invalid numbers
	static const int INVALID_DATE_FORMAT = -1;
	static const int INVALID_TIME_FORMAT = -1;

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

	// These handle task parameters
	static int parseDate(std::vector<std::string> inputString);
	static int parseTime(std::vector<std::string> inputString);
};

#endif