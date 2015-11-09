// Aaron Chong Jun Hao @@author A0110376N
// Parser converts flexible natural language into commands and parameters for TaskShark.

#ifndef PARSER_H_
#define PARSER_H_

class Parser {
private:
	static Parser* theOne;
	Parser();

	TsLogger* logger;
	void log(Level level, std::string message);
	void logSetTaskType(TaskType type);

	// This defines the file extension used by TaskShark
	static const std::string FILE_EXTENSION;

	static const std::string WARNING_INVALID_INT_STRING;
	static const std::string WARNING_NULL_FILE_PATH;
	static const std::string WARNING_NULL_MODIFY_STRING;
	static const std::string WARNING_NULL_PICK_TASK;
	static const std::string WARNING_NULL_SEARCH_STRING;
	static const std::string WARNING_NULL_TASK_STRING;

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

	std::string placeInField(Task* newTask, bool &isTODO, bool &isTODOreserve, bool &isReservation,
		FieldType inputMode, std::vector<std::string> inputString);	

	FieldType convertFieldDateToTime(FieldType &inputMode);
	void convertFieldToReserve(FieldType &inputMode);
	void createCmd(CommandType cmdType, std::string restOfInput, Command*& cmd);
	std::vector<FieldType>		extractFields(std::string restOfInput);
	std::vector<std::string>	removeSlashKeywords(std::vector<std::string> vecString);

public:
	Task* parseTask(std::string &restOfCommand);
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