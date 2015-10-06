#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TextBuddyTests {
	TEST_CLASS(LogicTest) {
	public:
		
		/*TEST_METHOD(Logic_getStatus) {
			Logic logic(ON);

			bool status = logic.getStatus();

			Assert::AreEqual(true,status);
		}*/
		
		TEST_METHOD(Logic_addTaskDeleteTask) {
			Logic logic;

			//add

			Task taskOne;
			taskOne.setName("Sentence one.");
			Add thisTask;
			thisTask.setNewTask(taskOne);	//adds taskOne onto taskStore
			logic.addInfo(thisTask);

			std::vector<Task> copyTask;

			copyTask = logic.getTaskStore();

			std::vector<Task>::iterator iter;

			iter = copyTask.begin();

			Assert::AreEqual(std::string("Sentence one."), iter->getName());
			
			Task taskTwo;
			taskTwo.setName("Sentence two.");
			thisTask.setNewTask(taskTwo);
			logic.addInfo(thisTask);

			copyTask = logic.getTaskStore();
			iter = copyTask.begin();
			++iter;

			Assert::AreEqual(std::string("Sentence two."),iter->getName());

			Task taskThree;
			taskThree.setName("Sentence three.");
			thisTask.setNewTask(taskThree);
			logic.addInfo(thisTask);

			copyTask = logic.getTaskStore();
			iter = copyTask.begin();
			++iter;
			++iter;
			Assert::AreEqual(std::string("Sentence three."),iter->getName());
			
			//modify

			Modify modTask;
			std::vector<FieldType> testVector;
			testVector.push_back(NAME);
			modTask.setModifyID(2);
			modTask.setFieldsToModify(testVector);

			taskThree.setName("Changed.");
			modTask.setTempTask(taskThree);

			logic.modifyInfo(modTask);
			copyTask = logic.getTaskStore();
			iter = copyTask.begin();
			Assert::AreEqual(std::string("Sentence one."), iter->getName());
			++iter;
			Assert::AreEqual(std::string("Changed."),iter->getName());
			++iter;
			Assert::AreEqual(std::string("Sentence three."),iter->getName());
		}

		TEST_METHOD(Logic_matchPhrase) {
			Logic logic;
			bool isTrue;

			isTrue = logic.matchPhrase("Hi", "high");
			Assert::AreEqual(true, isTrue);
			
			isTrue = logic.matchPhrase("hi", "thIgh");
			Assert::AreEqual(true, isTrue);
			
			isTrue = logic.matchPhrase("high", "high");
			Assert::AreEqual(true, isTrue);
			
			isTrue = logic.matchPhrase("hi", "h1gh");
			Assert::AreEqual(false, isTrue);
			
			isTrue = logic.matchPhrase("thigh", "high");
			Assert::AreEqual(false, isTrue);
			
		}
		
		TEST_METHOD(Logic_processInfo) {
			Logic logic;
			Parser parser;

			//add
			logic.processCommand(std::string("Add this"));
			logic.processCommand(std::string("Add that"));
			logic.processCommand(std::string("Add then"));
			std::vector<Task> copyTask;

			copyTask = logic.getTaskStore();

			std::vector<Task>::iterator iter;

			iter = copyTask.begin();

			Assert::AreEqual(std::string("this"), iter->getName());
			++iter;

			Assert::AreEqual(std::string("that"),iter->getName());
			++iter;

			Assert::AreEqual(std::string("then"), iter->getName());

			//delete
			logic.processCommand(std::string("Delete 1"));
			copyTask = logic.getTaskStore();
			iter = copyTask.begin();
			Assert::AreEqual(std::string("that"), iter->getName());
			++iter;

			Assert::AreEqual(std::string("then"), iter->getName());
			
			//modify
			logic.processCommand(std::string("Modify 1 changed."));
			copyTask = logic.getTaskStore();
			iter = copyTask.begin();
			Assert::AreEqual(std::string("changed."), iter->getName());
			++iter;

			Assert::AreEqual(std::string("then"), iter->getName());

			logic.processCommand(std::string("Modify 1 that"));

			//search
			std::string output = logic.processCommand(std::string("Search th"));	
			Assert::AreEqual(std::string("that,then"), output);
			

			copyTask = logic.getCurrentView();
			iter = copyTask.begin();
			Assert::AreEqual(std::string("that"), iter->getName());

			++iter;
			Assert::AreEqual(std::string("then"), iter->getName());

		}
		
		/*
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