// Aaron Chong Jun Hao @@author A0110376N
// Methods are listed in order of return type, then alphabetical order

#ifndef UTILITIES_H_
#define UTILITIES_H_

// This is the return value for invalid numbers
const int INVALID_NUMBER_FORMAT = -1;
const int LAST_TASK_INDICATOR = 0;

class Utilities {
private:
	// No public constructors and destructors, since Utilities is not an object
	Utilities();
	~Utilities();

public:
	//========== Int-to-Type Converters ==========
	static std::set<int>	intToSet(int num);
	//========== String-to-Type Converters ==========
	static std::string	stringToLower(std::string str);
	static int			stringToInt(std::string str);
	static CommandType	stringToCmdType(std::string str);
	static Day			stringToDay(std::string dayString);
	static Month		stringToMonth(std::string monthString);
	static FieldType	stringToFieldType(std::string fieldString);
	static TaskType		stringToTaskType(std::string typeString);
	static ViewType		stringToViewType(std::string viewString);
	static std::vector<std::string> stringToVec(std::string str);
	//========== Type-to-String Converters ==========
	static std::string	boolToString(bool boolean);
	static std::string	intToString(int num);
	static std::string	dayToString(Day day);
	static std::string  monthToString(Month month);
	static std::string	taskToString(Task task);
	static std::string	taskTypeToString(TaskType type);
	static std::string	fieldVecToString(std::vector<FieldType> fields);
	static std::string	vecToString(std::vector<std::string> vecString);
	static std::string	viewTypeToString(ViewType view);

	//========== Comparisons and Modifiers ==========
	static bool			containsAny(std::string searchWord, std::string words);
	static bool			isInt(std::string str);
	static bool			isSubstring(std::string subString, std::string word);
	// Strict weak ordering function for isSubstring()
	static bool			equalsIgnoreCase_char(char l, char r); 
	// Credits: Adapted from CityConnect.cpp (CS2103 Tutorial 2)
	// @@author A0110376N-reused
	static bool			equalsIgnoreCase(const std::string& str1, const std::string& str2);
	static bool			isPositiveNonZeroInt(std::string s);
	static std::string	getFirstWord(std::string words);
	static std::string	removeFirstWord(std::string words);
	static std::string	removeSpaces(const std::string& s, const std::string& delimiters = " \f\n\r\t\v" );
	static std::string	replace(std::string str, std::string from, std::string to);
	static std::string addSlashForInvertedComma(std::string words);
	static std::string removeFirstAndLastInvertedCommas(std::string words);
	//========== String-for-Display Formatters ==========
	// Soon Hao Ye @@author A0126677U
	static std::string getDate(int date);
	static std::string getTime(int time);
	static int getLocalDay();
	static int getLocalMonth();
	static int getLocalYear(); // Returns last 2 digits of the year (Eg. 15 for 2015)
};

#endif