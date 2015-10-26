// Aaron Chong Jun Hao @@author A0110376N

#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TextBuddyTests {
	TEST_CLASS(UtilitiesTest) {
public:
	TEST_CLASS_INITIALIZE(StartTestClass) {
		TbLogger::getInstance()->clearLog();
	}

	TEST_METHOD_CLEANUP(EndTestMethod) {
		TbLogger::getInstance()->log(SYS,"Test ended\n");
	}

	TEST_METHOD(Utilities_fieldVecToString) {
		std::string expectedString = "name : -: LABELS_CLEAR star unstar from at to FIELD_END_TIME FIELD_INVALID";
		std::vector<FieldType> fields;
		fields.push_back(NAME);
		fields.push_back(LABELS_ADD);
		fields.push_back(LABELS_DELETE);
		fields.push_back(LABELS_CLEAR);
		fields.push_back(PRIORITY_SET);
		fields.push_back(PRIORITY_UNSET);
		fields.push_back(START_DATE);
		fields.push_back(START_TIME);
		fields.push_back(END_DATE);
		fields.push_back(END_TIME);
		fields.push_back(INVALID_FIELD);
		Assert::AreEqual(expectedString,Utilities::fieldVecToString(fields));
	}

	TEST_METHOD(Utilities_vecToString) {
		std::string expectedString = "little brown fox";
		std::string userInput = "little brown fox";
		std::vector<std::string> inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedString,Utilities::vecToString(inputString));
	}

	TEST_METHOD(Utilities_isFieldKeyword) {
		Assert::AreEqual(false,Utilities::isFieldKeyword("add"));
		Assert::AreEqual(true,Utilities::isFieldKeyword("at"));
	}

	TEST_METHOD(Utilities_removeSlashKeywords) {
		std::string expectedString = "/add";
		std::string userInput = "/add";
		std::vector<std::string> inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedString,Utilities::vecToString(Utilities::removeSlashKeywords(inputString)));

		expectedString = "take money from drawer";
		userInput = "take money /from drawer";
		inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedString,Utilities::vecToString(Utilities::removeSlashKeywords(inputString)));
	}

	TEST_METHOD(Utilities_equalsIgnoreCase) {
		bool isTrue;

		isTrue = Utilities::equalsIgnoreCase("Hi", "high");
		Assert::AreEqual(false, isTrue);

		isTrue = Utilities::equalsIgnoreCase("hi", "thIgh");
		Assert::AreEqual(false, isTrue);

		isTrue = Utilities::equalsIgnoreCase("high", "high");
		Assert::AreEqual(true, isTrue);

		isTrue = Utilities::equalsIgnoreCase("hi", "h1gh");
		Assert::AreEqual(false, isTrue);

		isTrue = Utilities::equalsIgnoreCase("thigh", "high");
		Assert::AreEqual(false, isTrue);
	}

	TEST_METHOD(Utilities_removeSpaces) {
		std::string expectedString = "add";
		std::string userInput = " add ";
		Assert::AreEqual(expectedString,Utilities::removeSpaces(userInput));
	}

	TEST_METHOD(Utilities_replace) {
		std::string expectedString = "substring";
		std::string a = "parentstring";
		std::string b = "parent";
		std::string c = "sub";
		Assert::AreEqual(expectedString,Utilities::replace(a,b,c));

		expectedString = "\\\\string";
		a = "/string";
		b = "/";
		c = "\\\\";
		Assert::AreEqual(expectedString,Utilities::replace(a,b,c));
	}

	TEST_METHOD(Utilities_getDate) {
		// year removed if same as present year
		int date = 151101;
		std::string expectedDate = "1/11";
		Assert::AreEqual(expectedDate,Utilities::getDate(date));

		// Boundary value : end of year and next year
		date = 151231;
		expectedDate = "31/12";
		Assert::AreEqual(expectedDate,Utilities::getDate(date));

		date = 160101;
		expectedDate = "1/1/16";
		Assert::AreEqual(expectedDate,Utilities::getDate(date));
	}

	TEST_METHOD(Utilities_getTime) {
		int time = 900;
		std::string expectedTime = "9.00 am";
		Assert::AreEqual(expectedTime,Utilities::getTime(time));

		time = 1800;
		expectedTime = "6.00 pm";
		Assert::AreEqual(expectedTime,Utilities::getTime(time));

		// Boundary value : transition between am and pm
		time = 1200;
		expectedTime = "12.00 pm";
		Assert::AreEqual(expectedTime,Utilities::getTime(time));

		time = 0;
		expectedTime = "12.00 am";
		Assert::AreEqual(expectedTime,Utilities::getTime(time));

	}

	};
}
