// @@author Aaron Chong Jun Hao
// @@author Ng Ren Zhi

// Sort methods by return type, then alphabetical order

#ifndef UTILITIES_H_
#define UTILITIES_H_

class Utilities {
private:
	// This is the return value for invalid numbers
	static const int INVALID_NUMBER_FORMAT = -1;

public:
	Utilities();
	~Utilities();

	// Converters used in multiple architecture components
	static int			stringToInt(std::string str);
	static std::string	stringToLower(std::string str);
	static std::string	vecToString(std::vector<std::string> inputString);
	static std::string	taskToBuffer(Task taskToConvert);
	
	static Day			stringToEnumDay(std::string line);
	static TaskType		stringToEnumType(std::string line);
	static std::string	boolToIntString(bool boolean);
	static std::string	enumDayToString(Day day);
	static std::string	enumTypeToString(TaskType type);
	

	// Used in Parser only
	static bool			containsAny(std::string targetWord, std::string searchWords);
	// Credits: Adapted from CityConnect.cpp (CS2103 Tutorial 2)
	static bool			isPositiveNonZeroInt(std::string s);
	static bool			equalsIgnoreCase(const std::string& str1, const std::string& str2);
	static std::string	getFirstWord(std::string userCommand);
	static std::string	removeFirstWord(std::string userCommand);
	static std::string	removeSpaces(const std::string& s, const std::string& delimiters = " \f\n\r\t\v" );
	static std::string	replace(std::string a, std::string b, std::string c);
	static std::vector<std::string> splitParameters(std::string commandParametersString);

};

#endif