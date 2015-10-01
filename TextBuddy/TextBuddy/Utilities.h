// @@author Aaron Chong Jun Hao
// @@author Ng Ren Zhi

#ifndef UTILITIES_H_
#define UTILITIES_H_

#include "Task.h"

class Utilities {
private:
	// This is the return value for invalid numbers
	static const int INVALID_NUMBER_FORMAT = -1;

public:
	Utilities();
	~Utilities();

	// Used in multiple architecture components
	// Empty


	// Used in Parser
	static std::string vecToString(std::vector<std::string> inputString);
	static std::string taskToBuffer(Task taskToConvert);
	// Credits: Adapted from CityConnect.cpp (CS2103 Tutorial 2)
	static bool isPositiveAndValidInt(std::string s);
	static bool equalsIgnoreCase(const std::string& str1, const std::string& str2);
	static int parseInt(std::string str);
	static std::string getFirstWord(std::string userCommand);
	static std::string removeFirstWord(std::string userCommand);
	static std::string removeSpaces(const std::string& s, const std::string& delimiters = " \f\n\r\t\v" );
	static std::string replace(std::string a, std::string b, std::string c);
	static std::vector<std::string> splitParameters(std::string commandParametersString);


	// Used in IO
	//Empty

};
#endif
