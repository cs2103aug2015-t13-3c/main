// @@author A0110376N (Aaron Chong Jun Hao)

#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TextBuddyTests {
	TEST_CLASS(UtilitiesTest) {
public:

	TEST_METHOD(Utilities_fieldVecToString) {
		TbLogger::getInstance()->clear();
		Parser p;
		std::string expectedString = "from from to"; // Expected behaviour
		std::vector<FieldType> fields = p.extractFields("from on by");
		Assert::AreEqual(expectedString,Utilities::fieldVecToString(fields));
	}

	TEST_METHOD(Utilities_vecToString) {
		std::string expectedString = "little brown fox";
		std::string userInput = "little brown fox";
		std::vector<std::string> inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedString,Utilities::vecToString(inputString));
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

	//added @haoye 20/10/15
	/*
	TEST_METHOD(Utilities_taskDateAndTimeToDisplayString) {
		Task test;
		TaskType types[15] = {TODO,TODO,TODO,TODO,TODO,EVENT,EVENT,
			EVENT,EVENT,EVENT,EVENT,EVENT,EVENT,EVENT,EVENT};
		int startDates[15] = {
			0,
			151023,
			151201,
			151027,
			151131
		};
		int endDates[15] = {
			0,
			151023,
			151201,
			151027,
			151131
		};
		int startTimes[15] = {
			1200,
			900,
			1800,
			0,
			0
		};
		int endTimes[15] = {
			1200,
			900,
			1800,
			0,
			0
		};
		std::string expected[15] = {
			"by 12.00 pm",
			"by Friday 9.00 am",
			"by 1/12/2015 6.00 pm",
			"by next Tuesday",
			"by 31/11/2015"
		};

		for(int i=0 ; i< 5 ; ++i) {
			test.setType(types[i]);
			test.setStartDate(startDates[i]);
			test.setEndDate(endDates[i]);
			test.setStartTime(startTimes[i]);
			test.setEndTime(endTimes[i]);
			std::string result = Utilities::taskDateAndTimeToDisplayString(test);
			Assert::AreEqual(result,expected[i]);
		}
	}
	*/
	};
}