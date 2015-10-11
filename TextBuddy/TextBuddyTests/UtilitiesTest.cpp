// @@author Aaron Chong Jun Hao

#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TextBuddyTests {
	TEST_CLASS(UtilitiesTest) {
public:
	TEST_METHOD(Utilities_vecToString) {
		std::string expectedString = "little brown fox";
		std::string userInput = "little brown fox";
		std::vector<std::string> inputString = Utilities::splitParameters(userInput);
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
	};
}