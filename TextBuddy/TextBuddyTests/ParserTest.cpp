#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TextBuddyTests
{
	TEST_CLASS(ParserTest)
	{
	public:
		
		TEST_METHOD(Parser_getStatus)
		{
			Parser parser(ON);

			bool status = parser.getStatus();

			Assert::AreEqual(true,status);
		}

	};
}