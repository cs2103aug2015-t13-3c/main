#include "stdafx.h"
#include "Parser.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TextBuddyTests
{
	TEST_CLASS(ParserTest)
	{
	public:

		std::string userInput;
		std::vector<std::string> inputString;
		int expected;

		TEST_METHOD(Parser_getStatus)
		{
			Parser parser(ON);
			bool status = parser.getStatus();
			Assert::AreEqual(true,status);
		}

		TEST_METHOD(Parser_parseDate)
		{
			Parser p;

			// Invalid Input

			expected = 0;
			/*
			userInput = "";
			inputString = p.splitParameters(userInput);
			Assert::AreEqual(expected,p.parseDate(inputString));
			*/
			userInput = "invalid";
			inputString = p.splitParameters(userInput);
			Assert::AreEqual(expected,p.parseDate(inputString));
			
			userInput = "1 invalid";
			inputString = p.splitParameters(userInput);
			Assert::AreEqual(expected,p.parseDate(inputString));

			userInput = "this invalid";
			inputString = p.splitParameters(userInput);
			Assert::AreEqual(expected,p.parseDate(inputString));
			
			// Valid input

			expected = 151231;
			userInput = "31 dec";
			inputString = p.splitParameters(userInput);
			Assert::AreEqual(expected,p.parseDate(inputString));

			expected = 150926;
			userInput = "tmr";
			inputString = p.splitParameters(userInput);
			Assert::AreEqual(expected,p.parseDate(inputString));

			expected = 150926;
			userInput = "sat";
			inputString = p.splitParameters(userInput);
			Assert::AreEqual(expected,p.parseDate(inputString));

			expected = 150921;
			userInput = "this mon";
			inputString = p.splitParameters(userInput);
			Assert::AreEqual(expected,p.parseDate(inputString));

			expected = 150927;
			userInput = "next sun";
			inputString = p.splitParameters(userInput);
			Assert::AreEqual(expected,p.parseDate(inputString));
		}

	};
}