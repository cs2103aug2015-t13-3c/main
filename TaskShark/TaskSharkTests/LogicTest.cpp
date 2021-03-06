// Chin Kiat Boon @@author A0096720A

#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TaskSharkTests {
	TEST_CLASS(LogicTest) {
public:
	TEST_METHOD(Logic_intitiateHistory) {
		History *history = history->getInstance();
	}

	TEST_METHOD(Logic_processCommand) {
		Logic* logic = Logic::getInstance();
		logic->resetUpdaterNULL();
		Command cmd;
		cmd.clearTaskStore();	// Clear state

		// To be subscribed
		std::vector<DisplayedTask>* tasks = new std::vector<DisplayedTask>();
		logic->subscribe(tasks);

		// Add
		logic->processCommand(std::string("Add that from 14 Oct to 16 Oct"));
		logic->processCommand(std::string("Add then"));
		logic->processCommand(std::string("Add this from 13 Oct to 15 Oct"));
		logic->processCommand(std::string("Search from 10 Oct to 17 Oct for 1 h"));
		try {
		logic->processCommand(std::string("Delete 1"));
		} catch (std::exception e) {
		}

		std::vector<Task> copyTask;	
		std::vector<Task>::iterator iter;

		copyTask = cmd.getTaskStore();
		Assert::AreEqual((size_t)3,copyTask.size());
		iter = copyTask.begin();
		Assert::AreEqual(std::string("this"),iter->getName());
		Assert::AreEqual(151013,iter->getStartDate());
		Assert::AreEqual(151015,iter->getEndDate());
		Assert::AreEqual(TIME_NOT_SET,iter->getStartTime());
		Assert::AreEqual(TIME_NOT_SET,iter->getEndTime());

		++iter;
		Assert::AreEqual(std::string("that"), iter->getName());
		Assert::AreEqual(151014,iter->getStartDate());
		Assert::AreEqual(151016,iter->getEndDate());

		++iter;
		Assert::AreEqual(std::string("then"), iter->getName());

		// View
		logic->processCommand(std::string("View floating"));
		copyTask = cmd.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual(std::string("then"),iter->getName());

		// Display all
		logic->processCommand(std::string("display all"));

		// Delete
		logic->processCommand(std::string("Delete 1"));
		copyTask = cmd.getTaskStore();
		iter = copyTask.begin();
		Assert::AreEqual(std::string("that"), iter->getName());

		++iter;
		Assert::AreEqual(std::string("then"), iter->getName());

		
		// Modify
		logic->processCommand(std::string("Modify 1 changed."));
		copyTask = cmd.getTaskStore();
		iter = copyTask.begin();
		Assert::AreEqual(std::string("that"), iter->getName());
		++iter;
		Assert::AreEqual(std::string("changed."), iter->getName());

		logic->processCommand(std::string("Modify 1 then"));
		

		// Search
		logic->processCommand(std::string("Search he"));	
		copyTask = cmd.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual((size_t)1,copyTask.size());
		Assert::AreEqual(std::string("then"), iter->getName());

		// MarkDone
		logic->processCommand(std::string("Done 1"));
		logic->processCommand(std::string("view all"));
		copyTask = cmd.getCurrentView();
		Assert::AreEqual((size_t)1,copyTask.size());
		copyTask = cmd.getTaskStore();
		iter = copyTask.begin();
		++iter;
		Assert::AreEqual(true,iter->getDoneStatus());
		// delete logic;
	}

	TEST_METHOD(Logic_undo) {
		Logic* logic = Logic::getInstance();
		logic->resetUpdaterNULL();
		Command cmd;
		cmd.clearTaskStore();	// Clear state

		History* history = History::getInstance();

		// To be subscribed
		std::vector<DisplayedTask>* tasks = new std::vector<DisplayedTask>();
		logic->subscribe(tasks);

		// Add
		logic->processCommand(std::string("Add that from 14 Oct to 16 Oct"));
		logic->processCommand(std::string("Add then"));
		logic->processCommand(std::string("Add this from 13 Oct to 15 Oct"));

		std::vector<Task> copyTask;	
		std::vector<Task>::iterator iter;

		copyTask = cmd.getTaskStore();
		Assert::AreEqual((size_t)3,copyTask.size());
		iter = copyTask.begin();
		Assert::AreEqual(std::string("this"),iter->getName());
		Assert::AreEqual(151013,iter->getStartDate());
		Assert::AreEqual(151015,iter->getEndDate());
		Assert::AreEqual(TIME_NOT_SET,iter->getStartTime());
		Assert::AreEqual(TIME_NOT_SET,iter->getEndTime());

		++iter;
		Assert::AreEqual(std::string("that"), iter->getName());
		Assert::AreEqual(151014,iter->getStartDate());
		Assert::AreEqual(151016,iter->getEndDate());

		++iter;
		Assert::AreEqual(std::string("then"), iter->getName());
		Assert::AreEqual(0,iter->getStartDate());
		Assert::AreEqual(0,iter->getEndDate());
		Assert::AreEqual(TIME_NOT_SET,iter->getStartTime());
		Assert::AreEqual(TIME_NOT_SET,iter->getEndTime());

		Assert::AreEqual(3,history->getUndoSize());

		// Undo Add
		logic->processCommand(std::string("Undo"));

		copyTask = cmd.getTaskStore();
		Assert::AreEqual((size_t)2,copyTask.size());

		// Redo Add
		logic->processCommand(std::string("Redo"));

		copyTask = cmd.getTaskStore();
		Assert::AreEqual((size_t)3,copyTask.size());
		
		// Problem: "then" set as EVENT instead of FLOATING
		
		// View
		/*
		copyTask = cmd.getCurrentView();
		iter = copyTask.begin();
		iter++;
		iter++;
		if (EVENT != iter->getType()) {
			Assert::Fail();
		}*/
		// View Floating
		std::string msg = logic->processCommand(std::string("View floating"));
		Assert::AreEqual(std::string("Viewing: floating"),msg);

		copyTask = cmd.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual((size_t)1,copyTask.size());
		Assert::AreEqual(std::string("then"),iter->getName());
		
		//View Event
		msg = logic->processCommand(std::string("View events"));
		Assert::AreEqual(std::string("Viewing: events"),msg);
		
		copyTask = cmd.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual((size_t)2,copyTask.size());
		Assert::AreEqual(std::string("this"),iter->getName());
		++iter;
		Assert::AreEqual(std::string("that"),iter->getName());

		//View Todo
		msg = logic->processCommand(std::string("View todo"));
		Assert::AreEqual(std::string("Viewing: todo"),msg);
		
		copyTask = cmd.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual((size_t)0,copyTask.size());
		
		//View Event
		msg = logic->processCommand(std::string("View all"));
		Assert::AreEqual(std::string("Viewing: all"),msg);
		
		copyTask = cmd.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual((size_t)3,copyTask.size());
		Assert::AreEqual(std::string("this"),iter->getName());
		++iter;
		Assert::AreEqual(std::string("that"),iter->getName());
		++iter;
		Assert::AreEqual(std::string("then"),iter->getName());

		// Display all
		logic->processCommand(std::string("display all"));
			
		// Delete
		logic->processCommand(std::string("Delete 1"));
		logic->processCommand(std::string("view all"));
		copyTask = cmd.getTaskStore();
		iter = copyTask.begin();
		Assert::AreEqual((size_t)2,copyTask.size());
		Assert::AreEqual(std::string("that"), iter->getName());

		++iter;
		Assert::AreEqual(std::string("then"), iter->getName());

		// Undo delete
		logic->processCommand(std::string("Undo"));

		logic->processCommand(std::string("view all"));
		copyTask = cmd.getCurrentView();
		Assert::AreEqual((size_t)3,copyTask.size());

		// Redo delete
		logic->processCommand(std::string("view float"));
		logic->processCommand(std::string("Redo"));

		logic->processCommand(std::string("view all"));
		copyTask = cmd.getCurrentView();
		Assert::AreEqual((size_t)2,copyTask.size());

		
		// Modify
		copyTask = cmd.getTaskStore();
		iter = copyTask.begin();
		Assert::AreEqual(std::string("that"), iter->getName());

		logic->processCommand(std::string("Modify 1 changed."));
		copyTask = cmd.getTaskStore();
		iter = copyTask.begin();
		Assert::AreEqual(std::string("changed."), iter->getName());

		++iter;
		Assert::AreEqual(std::string("then"), iter->getName());

		// Undo Modify
		logic->processCommand(std::string("Undo"));

		copyTask = cmd.getTaskStore();
		iter = copyTask.begin();
		Assert::AreEqual(std::string("that"), iter->getName());
		
		// Redo Modify
		logic->processCommand(std::string("Redo"));

		copyTask = cmd.getTaskStore();
		iter = copyTask.begin();
		Assert::AreEqual(std::string("changed."), iter->getName());
		logic->processCommand(std::string("Undo"));

		// Search
		logic->processCommand(std::string("Search hi"));	
		copyTask = cmd.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual((size_t)0,copyTask.size());
		
		logic->processCommand(std::string("Search he"));	
		copyTask = cmd.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual((size_t)1,copyTask.size());
		Assert::AreEqual(std::string("then"), iter->getName());

		// MarkDone
		logic->processCommand(std::string("Done 1"));
		logic->processCommand(std::string("view all"));

		copyTask = cmd.getCurrentView();
		Assert::AreEqual((size_t)1,copyTask.size());
		copyTask = cmd.getTaskStore();
		iter = copyTask.begin();
		++iter;
		Assert::AreEqual(true,iter->getDoneStatus());

		// View past
		logic->processCommand(std::string("view past"));

		copyTask = cmd.getCurrentView();
		Assert::AreEqual((size_t)1,copyTask.size());

		// Undo MarkDone
		logic->processCommand(std::string("Undo"));
		logic->processCommand(std::string("view all"));

		copyTask = cmd.getCurrentView();
		Assert::AreEqual((size_t)2,copyTask.size());
		copyTask = cmd.getTaskStore();
		iter = copyTask.begin();
		++iter;
		Assert::AreEqual(false,iter->getDoneStatus());

		// Redo MarkDone
		logic->processCommand(std::string("Redo"));
		logic->processCommand(std::string("view all"));

		copyTask = cmd.getCurrentView();
		Assert::AreEqual((size_t)1,copyTask.size());
		copyTask = cmd.getTaskStore();
		iter = copyTask.begin();
		++iter;
		Assert::AreEqual(true,iter->getDoneStatus());

		// UnmarkDone
		logic->processCommand(std::string("view past"));
		logic->processCommand(std::string("notdone 1"));
		logic->processCommand(std::string("view all"));
		copyTask = cmd.getCurrentView();
		Assert::AreEqual((size_t)2,copyTask.size());
		copyTask = cmd.getTaskStore();
		iter = copyTask.begin();
		++iter;
		Assert::AreEqual(false,iter->getDoneStatus());

		// Undo UnmarkDone
		logic->processCommand(std::string("Undo"));
		logic->processCommand(std::string("view all"));

		copyTask = cmd.getCurrentView();
		Assert::AreEqual((size_t)1,copyTask.size());
		copyTask = cmd.getTaskStore();
		iter = copyTask.begin();
		++iter;
		Assert::AreEqual(true,iter->getDoneStatus());
		
		// Redo UnmarkDone
		logic->processCommand(std::string("Redo"));
		logic->processCommand(std::string("view all"));

		copyTask = cmd.getCurrentView();
		Assert::AreEqual((size_t)2,copyTask.size());
		copyTask = cmd.getTaskStore();
		iter = copyTask.begin();
		++iter;
		Assert::AreEqual(false,iter->getDoneStatus());

		// delete logic;
	}


	// Chin Kiat Boon @@author A0096720A-unused
	/*
	TEST_METHOD(Logic_addTaskModifyTask) {
	Logic logic;
	logic.clearTaskStore();							// Clear state (Aaron)

	// Add
	// Note: Add has no more public setter methods (Aaron)

	Task taskOne;
	taskOne.setID(Task::incrementRunningCount());	// Added to fix uniqueID (Aaron)
	taskOne.setName("Sentence one.");
	Add thisTask(taskOne);							// Adds taskOne into taskStore (Step 1/2)
	logic.addInfo(thisTask);						// Adds taskOne into taskStore (Step 2/2)

	std::vector<Task> copyTask;
	copyTask = logic.getTaskStore();

	std::vector<Task>::iterator iter;
	iter = copyTask.begin();
	Assert::AreEqual(std::string("Sentence one."), iter->getName());

	Task taskTwo;
	taskTwo.setName("Sentence two.");
	thisTask = *(new Add(taskTwo));
	logic.addInfo(thisTask);

	copyTask = logic.getTaskStore();
	iter = copyTask.begin();
	++iter;
	Assert::AreEqual(std::string("Sentence two."),iter->getName());

	Task taskThree;
	taskThree.setName("Sentence three.");
	thisTask = *(new Add(taskThree));
	logic.addInfo(thisTask);

	copyTask = logic.getTaskStore();
	iter = copyTask.begin();
	++iter;
	++iter;
	Assert::AreEqual(std::string("Sentence three."),iter->getName());

	// Modify
	// Note: Modify has no more public setter methods (Aaron)

	std::vector<FieldType> testVector;
	testVector.push_back(NAME);

	int modifyID = 2;
	std::vector<FieldType> fieldsToModify = testVector;
	taskThree.setName("Changed.");
	Task tempTask = taskThree;
	Modify modTask(modifyID,fieldsToModify,tempTask);

	logic.modifyInfo(modTask);
	copyTask = logic.getTaskStore();
	iter = copyTask.begin();
	Assert::AreEqual(std::string("Sentence one."), iter->getName());
	++iter;
	Assert::AreEqual(std::string("Changed."),iter->getName());
	++iter;
	Assert::AreEqual(std::string("Sentence three."),iter->getName());
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
	}
	*/
	};
}
