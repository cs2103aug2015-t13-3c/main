#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TextBuddyTests
{
	TEST_CLASS(LogicTest)
	{
	public:
		/*
		TEST_METHOD(Logic_getStatus)
		{
			Logic logic(ON);

			bool status = logic.getStatus();

			Assert::AreEqual(true,status);
		}

		TEST_METHOD(Logic_AddAndReturnInfoTest) {
			Logic testLogic;
			
			Assert::AreEqual(std::string("Nothing to return."), testLogic.returnInfo(2));

			testLogic.addInfo(std::string("abc"), 150718, 1157, 150818, 1257);
			testLogic.addInfo(std::string("def"), 150818, 1257, 150918, 1357);
			testLogic.addInfo(std::string("Hello World!"), 150918, 1357, 151018, 1457);

			Assert::AreEqual(std::string("Hello World!|150918|1357|151018|1457"), testLogic.returnInfo(2));
		}

		TEST_METHOD(Logic_DeleteInfoTest) {
			Logic testLogic;

			testLogic.addInfo(std::string("abc"), 150718, 1157, 150818, 1257);
			testLogic.addInfo(std::string("def"), 150818, 1257, 150918, 1357);
			testLogic.addInfo(std::string("Hello World!"), 150918, 1357, 151018, 1457);

			testLogic.deleteInfo(1);
			Assert::AreEqual(std::string("Hello World!|150918|1357|151018|1457"), testLogic.returnInfo(1));

		}

		TEST_METHOD(Logic_SortTest) {
			Logic testLogic;

			testLogic.addInfo(std::string("abc"), 150718, 1357, 150818, 1257);
			testLogic.addInfo(std::string("def"), 150818, 957, 150918, 1357);
			testLogic.addInfo(std::string("Hello World!"), 150808, 1057, 151018, 1457);

			testLogic.sortDateAndTime();

			Assert::AreEqual(std::string("Hello World!|150808|1057|151018|1457"), testLogic.returnInfo(1));
		}

		TEST_METHOD(Logic_freeSlotSearch) {
			Logic testLogic;
			std::string freeSlot;

			freeSlot = testLogic.freeSlotSearch(150918,2143);

			Assert::AreEqual(std::string("150918|2143"), freeSlot);

			testLogic.addInfo(std::string("abc"), 150718, 1357, 150818, 1256);
			testLogic.addInfo(std::string("def"), 150818, 1257, 150918, 1355);
			testLogic.addInfo(std::string("Hello World!"), 150918, 1357, 151018, 1457);

			freeSlot = testLogic.freeSlotSearch(150718,1355);

			Assert::AreEqual(std::string("150718|1355"), freeSlot);

			freeSlot = testLogic.freeSlotSearch(150718,1357);

			Assert::AreEqual(std::string("150918|1356"), freeSlot);

			freeSlot = testLogic.freeSlotSearch(140718,1357);

			Assert::AreEqual(std::string("140718|1357"), freeSlot);
		}

		TEST_METHOD(Logic_AddTimeTest) {
			Logic testLogic;
			int number;

			number = testLogic.addTime(2259, 1);
			Assert::AreEqual(2300, number);
		}

		TEST_METHOD(Logic_ChangeInfoTest) {
			Logic testLogic;

			testLogic.addInfo(std::string("abc"), 150718, 1157, 150818, 1257);
			testLogic.addInfo(std::string("def"), 150818, 1257, 150918, 1357);
			testLogic.addInfo(std::string("Hello World!"), 150918, 1357, 151018, 1457);
			testLogic.addInfo(std::string("Hello World!"), 150918, 1357, 151018, 1457);

			testLogic.changeInfo(std::string("Bye World!"), 151126, 1300, 151126, 1500, 3);
			Assert::AreEqual(std::string("Bye World!|151126|1300|151126|1500"), testLogic.returnInfo(3));
		}*/
	};
}