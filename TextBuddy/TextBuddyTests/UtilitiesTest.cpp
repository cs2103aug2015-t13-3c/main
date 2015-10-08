// @@author Aaron Chong Jun Hao

#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TextBuddyTests
{
	TEST_CLASS(UtilitiesTest)
	{
	public:

		TEST_METHOD(Utilities_vecToString)
		{
			std::string expectedString = "little brown fox";
			std::string userInput = "little brown fox";
			std::vector<std::string> inputString = Utilities::splitParameters(userInput);
			Assert::AreEqual(expectedString,Utilities::vecToString(inputString));
		}
	};
}