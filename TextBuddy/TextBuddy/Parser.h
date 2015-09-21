// @@author Aaron Chong Jun Hao

#ifndef PARSER_H_
#define PARSER_H_

class Parser{
private:
	bool isActive;
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

	// These are the locations at which various parameters appear
	static const int PARAM_POSITION_TASK = 0;
	static const int PARAM_POSITION_TASK_NUMBER = 0;

	// This is the return value for invalid numbers
	static const int INVALID_NUMBER_FORMAT = -1;

	// These functions support user methods
	// Credits: Adapted from CityConnect.cpp (CS2103 Tutorial 2)
	static bool isPositiveAndValidInt(std::string s);
	static std::vector<std::string> splitParameters(std::string commandParametersString);
	static std::string getFirstWord(std::string userCommand);
	static std::string removeFirstWord(std::string userCommand);

	static std::string removeSpaces(const std::string& s, const std::string& delimiters = " \f\n\r\t\v" );
	static bool equalsIgnoreCase(const std::string& str1, const std::string& str2);
	static int parseInt(std::string str);
	static std::string replace(std::string a, std::string b, std::string c);

public:
	Parser();
	Parser(bool status);
	~Parser();

	static bool getStatus();
	static std::string parseFileName(char* argv[]);
	static Command parseCommand(std::string userCommand);
	static Task parseTask(std::string restOfCommand);
};

#endif