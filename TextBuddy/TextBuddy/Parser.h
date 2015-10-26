// Aaron Chong Jun Hao @@author A0110376N
// Parser converts flexible natural language into commands and parameters for TextBuddy.

#ifndef PARSER_H_
#define PARSER_H_

class Parser {
private:
	static Parser* theOne;
	Parser();

	TbLogger* logger;
	void log(Level level, std::string message);
	void logSetTaskType(TaskType type);

	// This defines the file extension used by TextBuddy
	static const std::string FILE_EXTENSION;

	// These are the return values for invalid parameters
	static const int INVALID_DATE_FORMAT = -1;
	static const int INVALID_TIME_FORMAT = -1;

	void convertFieldDateToTime(FieldType &inputMode);
	void placeInField(Task* newTask, bool &isTODO,
		FieldType inputMode, std::vector<std::string> inputString);	

	std::vector<FieldType> extractFields(std::string restOfInput);

	int findMaxDays(Month month, int year=2015);
	int findYear(std::string yearString="");

public:
	Task* parseTask(std::string restOfCommand);
	int parseDate(std::vector<std::string> dateString);
	int parseDay(std::vector<std::string> dayString);
	int parseTime(std::vector<std::string> timeString);

	// This is the API
	~Parser();
	static Parser* getInstance();
	std::string parseFileName(char* argv[]);
	std::string parseFileName(std::string stringFilePath);
	Command* parse(std::string userInput);
};

#endif