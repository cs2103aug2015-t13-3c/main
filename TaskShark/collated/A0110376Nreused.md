# A0110376Nreused
###### TaskSharkInternal\Utilities.cpp
``` cpp

bool Utilities::isPositiveNonZeroInt(std::string s) {
	int i = stringToInt(s);

	if (i == INVALID_NUMBER_FORMAT || i <= 0 ) {
		return false;
	} else {
		return true;
	}
}

bool Utilities::equalsIgnoreCase(const std::string& str1, const std::string& str2) {
	if (str1.size() != str2.size()) {
		return false;
	} else {
		for (std::string::const_iterator c1 = str1.begin(), c2 = str2.begin(); c1 != str1.end(); ++c1 , ++c2) {
			if (tolower(*c1) != tolower(*c2)) {
				return false;
			}
		}
	}
	return true;
}

std::string Utilities::getFirstWord(std::string words) {
	return stringToVec(words)[0];
}

std::string Utilities::removeFirstWord(std::string words) {
	std::string commandTypeString = getFirstWord(words);
	int strSize = commandTypeString.size();
	std::string parameters = removeSpaces(words.substr(strSize));
	return parameters;
}

std::string Utilities::removeSpaces(const std::string& s, const std::string& delimiters) {
	if (!s.empty()) {
		std::string trimEnd = s.substr(0, s.find_last_not_of(delimiters) + 1);
		std::string trimStart = trimEnd.substr(trimEnd.find_first_not_of(delimiters));
		return trimStart;
	} else {
		return s;
	}
}

std::string Utilities::replace(std::string str, std::string from, std::string to) {
	int pos;
	int fromLength = from.length();
	std::string newString = "";
	do {
		pos = str.find(from);
		if (pos != -1) {
			newString += str.substr(0,pos) + to;
			str = str.substr(pos+fromLength); // Bugfix: infinite loop if str not updated (Ren Zhi)
		}
	} while (pos != -1);
	newString += str;
	return newString;
}

bool Utilities::isSubstring(std::string subString, std::string words) {
	bool isFound = false;
	std::string::iterator pos = std::search(words.begin(), words.end(), 
		subString.begin(),subString.end(),equalsIgnoreCase_char);
	if (pos != words.end()) {
		isFound = true;
	}
	return isFound;
}

bool Utilities::equalsIgnoreCase_char (char l, char r) {
	return (tolower(l) == tolower(r));
}

std::string Utilities::addSlashForInvertedComma(std::string words) {
	for (unsigned int i = 0; i < words.size(); i++) {
		if (words[i] == '\"') {
			words.insert(i,"\\\\\\");
			i++;
			i++;
			i++;
		}
	}
	return words;
}

std::string Utilities::removeFirstAndLastInvertedCommas(std::string words) {
	// Erase first inverted comma
	std::string::iterator iter = words.begin();
	if (*iter == '\"') {
		words.erase(iter);
	}

	// Erase last inverted comma
	iter = words.end()-1;
	if (*iter == '\"') {
		words.erase(iter);
	}
	return words;
}

```
###### TaskSharkInternal\Utilities.h
``` h
	static bool			equalsIgnoreCase(const std::string& str1, const std::string& str2);
	static bool			isPositiveNonZeroInt(std::string s);
	static std::string	getFirstWord(std::string words);
	static std::string	removeFirstWord(std::string words);
	static std::string	removeSpaces(const std::string& s, const std::string& delimiters = " \f\n\r\t\v" );
	static std::string	replace(std::string str, std::string from, std::string to);
	static std::string	addSlashForInvertedComma(std::string words);
	static std::string	removeFirstAndLastInvertedCommas(std::string words);

	//========== String-for-Display Formatters ==========
```
