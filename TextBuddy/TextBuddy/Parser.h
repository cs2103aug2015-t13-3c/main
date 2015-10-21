// @@author A0110376N (Aaron Chong Jun Hao)
// Parser converts flexible natural language into commands and parameters for TextBuddy.

#ifndef PARSER_H_
#define PARSER_H_

#include "stdafx.h"

class Parser {
private:
	static Parser* theOne;
	Parser();
	TbLogger* logger;

	// This defines the file extension used by TextBuddy
	static const std::string FILE_EXTENSION;

	// These are the return values for invalid parameters
	static const int INVALID_DATE_FORMAT = -1;
	static const int INVALID_TIME_FORMAT = -1;

	void log(Level level, std::string message);

public:
	~Parser();

	// This is the API
	static Parser* getInstance();
	std::string parseFileName(char* argv[]);
	std::string parseFileName(std::string stringFilePath);
	Command* parse(std::string userInput);
	Task* parseTask(std::string restOfCommand);

	// These support user methods
	std::vector<FieldType> extractFields(std::string restOfInput);
	int findMaxDays(Month month, int year=2015);
	int findYear(std::string yearString="");

	// These handle task parameters
	int parseDate(std::vector<std::string> dateString);
	int parseDay(std::vector<std::string> dayString);
	int parseTime(std::vector<std::string> timeString);
};

#endif