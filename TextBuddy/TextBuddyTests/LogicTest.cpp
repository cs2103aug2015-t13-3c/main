#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TextBuddyTests
{
	TEST_CLASS(LogicTest)
	{
	public:
		
		TEST_METHOD(Logic_getStatus)
		{
			Logic logic(ON);

			bool status = logic.getStatus();

			Assert::AreEqual(true,status);
		}

	};
}