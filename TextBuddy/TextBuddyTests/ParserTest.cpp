#include "stdafx.h"
#include "Parser.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TextBuddyTests
{
	TEST_CLASS(ParserTest)
	{
	public:

		// Initialise common variables to be used in tests
		std::string userInput;
		std::vector<std::string> inputString;
		std::string expectedString;
		int expectedInt;
		Task tempTask;

		TEST_METHOD(Parser_parseDate)
		{
			Parser p;

			// Invalid Input

			expectedInt = 0;
			/*
			userInput = "";
			inputString = p.splitParameters(userInput);
			Assert::AreEqual(expected,p.parseDate(inputString));
			*/
			userInput = "invalid";
			inputString = p.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseDate(inputString));

			userInput = "1 invalid";
			inputString = p.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseDate(inputString));

			userInput = "this invalid";
			inputString = p.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseDate(inputString));

			// Valid input

			expectedInt = 151231;
			userInput = "31 dec";
			inputString = p.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseDate(inputString));

			expectedInt = 150929;
			userInput = "tmr";
			inputString = p.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseDate(inputString));

			expectedInt = 151003;
			userInput = "sat";
			inputString = p.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseDate(inputString));

			expectedInt = 150928;
			userInput = "this mon";
			inputString = p.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseDate(inputString));

			expectedInt = 151004;
			userInput = "next sun";
			inputString = p.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseDate(inputString));
		}

		TEST_METHOD(Parser_vecToString)
		{
			Parser p;

			expectedString = "little brown fox";
			userInput = "little brown fox";
			inputString = p.splitParameters(userInput);
			Assert::AreEqual(expectedString,p.vecToString(inputString));
		}

		TEST_METHOD(Parser_parseTask)
		{
			Parser p;

			expectedString = "Name: little brown fox\n";
			userInput = "little brown fox";
			tempTask = p.parseTask(userInput);
			Assert::AreEqual(expectedString,p.taskToBuffer(tempTask));
		}

	};
}