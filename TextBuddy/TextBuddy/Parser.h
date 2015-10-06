// @@author Aaron Chong Jun Hao

#ifndef PARSER_H_
#define PARSER_H_

class Parser {
private:
	static Utilities u;

	// This defines the file extension used by TextBuddy
	static const std::string FILE_EXTENSION;

	// This is the running counter to set uniqueID for tasks
	static int runningCount;

	// This is the return value for invalid numbers
	static const int INVALID_DATE_FORMAT = -1;
	static const int INVALID_TIME_FORMAT = -1;

public:
	Parser();
	~Parser();

	// This is the API
	static std::string parseFileName(char* argv[]);
	static Command* parse(std::string userInput);
	static Command parseCommand(std::string userCommand);
	static Task parseTask(std::string restOfCommand);

	// These support user methods
	static std::vector<FieldType> extractFields(std::string restOfInput);
	static int findMaxDays(Month month, int year=2015);

	// These handle task parameters
	static int parseDate(std::vector<std::string> inputString);
	static int parseTime(std::vector<std::string> inputString);
};

#endif