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

			expected = 150924;
			userInput = "tmr";
			inputString = p.splitParameters(userInput);
			Assert::AreEqual(expected,p.parseDate(inputString));
			
			expected = 150922;
			userInput = "this tue";
			inputString = p.splitParameters(userInput);
			Assert::AreEqual(expected,p.parseDate(inputString));

			expected = 150924;
			userInput = "this thu";
			inputString = p.splitParameters(userInput);
			Assert::AreEqual(expected,p.parseDate(inputString));
			
			expected = 150927;
			userInput = "next sun";
			inputString = p.splitParameters(userInput);
			Assert::AreEqual(expected,p.parseDate(inputString));

			expected = 151003;
			userInput = "next sat";
			inputString = p.splitParameters(userInput);
			Assert::AreEqual(expected,p.parseDate(inputString));
		}

	};
}