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

	int findMaxDays(Month month, int year=2015);
	int findYear(std::string yearString="");

	bool isPowerSearchKeywords(std::string str);
	bool isPowerSearchFormat(std::string searchStrings);
	bool isFieldKeyword(std::string str);
	bool isDateField(FieldType field);
	bool isTodoField(FieldType field);

	void placeInField(Task* newTask, bool &isTODO, bool &isTODOreserve, bool &isReservation,
		FieldType inputMode, std::vector<std::string> inputString);	

	FieldType convertFieldDateToTime(FieldType &inputMode);
	void convertFieldToReserve(FieldType &inputMode);
	std::vector<FieldType>		extractFields(std::string restOfInput);
	std::vector<std::string>	removeSlashKeywords(std::vector<std::string> vecString);

public:
	Task* parseTask(std::string restOfCommand);
	std::vector<std::string> parseSearchParameters(std::string restOfInput);
	int parseDate(std::vector<std::string> dateString);
	int parseByDate(std::vector<std::string> dateString);
	int parseByDay(std::vector<std::string> dayString);
	int parseTime(std::vector<std::string> timeString);

	// This is the API
	~Parser();
	static Parser* getInstance();
	std::string parseFileName(char* argv[]);
	std::string parseFileName(std::string stringFilePath);
	Command* parse(std::string userInput);
};

#endif