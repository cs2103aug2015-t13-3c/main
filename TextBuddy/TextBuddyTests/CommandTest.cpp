// Ng Ren Zhi @@author A0130463R

#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

void addThreeSentences(std::vector<Task> copyTask);

namespace TextBuddyTests {
	TEST_CLASS(Command_Add) {
public:
	// Originally written by Kiat Boon
	TEST_METHOD(Command_Add_execute) {
		Task taskOne;
		taskOne.setID(Task::incrementRunningCount());	// Added to fix uniqueID (Aaron)
		taskOne.setName("Sentence one.");
		Add addOne(taskOne);							// Adds taskOne into taskStore (Step 1/2)
		addOne.clearTaskStore();
		addOne.execute();								// Adds taskOne into taskStore (Step 2/2)

		std::vector<Task> copyTask;
		copyTask = addOne.getTaskStore();

		std::vector<Task>::iterator iter;
		iter = copyTask.begin();
		Assert::AreEqual(std::string("Sentence one."), iter->getName());

		Task taskTwo;
		taskTwo.setName("Sentence two.");
		Add addTwo(taskTwo);
		addTwo.execute();

		copyTask = addTwo.getTaskStore();
		iter = copyTask.begin();
		++iter;
		Assert::AreEqual(std::string("Sentence two."),iter->getName());

		Task taskThree;
		taskThree.setName("Sentence three.");
		Add addThree(taskThree);
		addThree.execute();

		copyTask = addThree.getTaskStore();
		iter = copyTask.begin();
		++iter;
		++iter;
		Assert::AreEqual(std::string("Sentence three."),iter->getName());
	}

	TEST_METHOD(Command_Add_undo) {
		Task taskOne;
		int firstID = Task::incrementRunningCount();
		taskOne.setID(firstID);	// Added to fix uniqueID (Aaron)
		taskOne.setName("Sentence one.");
		Add addOne(taskOne);
		addOne.clearTaskStore();
		addOne.execute();

		std::vector<Task> copyTask;
		copyTask = addOne.getTaskStore();

		std::vector<Task>::iterator iter;
		iter = copyTask.begin();
		Assert::AreEqual(std::string("Sentence one."), iter->getName());

		Task taskTwo;
		taskTwo.setName("Sentence two.");
		int taskTwoID = Task::incrementRunningCount();
		taskTwo.setID(taskTwoID);
		Add addTwo(taskTwo);
		addTwo.execute();

		copyTask = addTwo.getTaskStore();
		iter = copyTask.begin();
		++iter;
		Assert::AreEqual(std::string("Sentence two."),iter->getName());

		Task taskThree;
		taskThree.setName("Sentence three.");
		int taskThreeID = Task::incrementRunningCount();
		taskThree.setID(taskThreeID);
		Add addThree(taskThree);
		addThree.execute();

		copyTask = addThree.getTaskStore();
		iter = copyTask.begin();
		++iter;
		++iter;
		Assert::AreEqual(std::string("Sentence three."),iter->getName());

		addTwo.undo();

		copyTask = addTwo.getTaskStore();
		iter = copyTask.begin();
		Assert::AreEqual(2,addTwo.getSize());
		Assert::AreEqual(firstID+1,taskTwoID);
		Assert::AreEqual(firstID+2,taskThreeID);
		Assert::AreEqual(std::string("Sentence one."),iter->getName());
		++iter;
		Assert::AreEqual(std::string("Sentence three."),iter->getName());

		addOne.undo();

		copyTask = addThree.getTaskStore();
		iter = copyTask.begin();
		Assert::AreEqual(1,addOne.getSize());
		Assert::AreEqual(std::string("Sentence three."),iter->getName());
	}

	TEST_METHOD(Command_Add_Sort) {
		Task taskOne;
		taskOne.setID(Task::incrementRunningCount());	// Added to fix uniqueID (Aaron)
		taskOne.setName("Sentence one.");
		taskOne.setStartDate(150910);
		taskOne.setEndDate(150910);
		taskOne.setPriority();							// Prioritised task
		Add addOne(taskOne);							// Adds taskOne into taskStore (Step 1/2)
		addOne.clearTaskStore();
		addOne.execute();								// Adds taskOne into taskStore (Step 2/2)

		std::vector<Task> copyTask;
		std::vector<Task>::iterator iter;

		Task taskTwo;
		taskTwo.setName("Sentence two.");
		taskTwo.setStartDate(151010);
		taskTwo.setEndDate(151010);
		taskTwo.setPriority();							// Prioritised task
		Add addTwo(taskTwo);
		addTwo.execute();

		Task taskThree;
		taskThree.setName("Sentence three.");
		taskThree.setStartDate(150809);
		taskThree.setEndDate(150809);
		Add addThree(taskThree);
		addThree.execute();

		Task taskFour;
		taskFour.setName("Sentence four.");
		taskFour.setStartDate(150910);
		taskFour.setEndDate(150910);
		Add addFour(taskFour);
		addFour.execute();

		copyTask = addFour.getTaskStore();
		iter = copyTask.begin();
		Assert::AreEqual(150910,iter->getStartDate());
		++iter;
		Assert::AreEqual(151010, iter->getStartDate());
		++iter;
		Assert::AreEqual(150809, iter->getStartDate());
		++iter;
		Assert::AreEqual(150910, iter->getStartDate());
	}
	};

	TEST_CLASS(Command_Delete) {
public:
	TEST_METHOD(Command_Delete_Execute) {
		std::vector<Task> copyTask;
		std::vector<Task>::iterator iter;
		addThreeSentences(copyTask);

		Delete deleteTwo(2);
		deleteTwo.execute();

		copyTask = deleteTwo.getTaskStore();
		iter = copyTask.begin();
		Assert::AreEqual(2,deleteTwo.getSize());
		Assert::AreEqual(std::string("Sentence one."),iter->getName());
		++iter;
		Assert::AreEqual(std::string("Sentence three."),iter->getName());

		Delete deleteOne(1);
		deleteOne.execute();

		copyTask = deleteOne.getTaskStore();
		iter = copyTask.begin();
		Assert::AreEqual(1,deleteOne.getSize());
		Assert::AreEqual(std::string("Sentence three."),iter->getName());
	}

	TEST_METHOD(Command_Delete_Undo) {
		std::vector<Task> copyTask;
		std::vector<Task>::iterator iter;
		addThreeSentences(copyTask);

		Delete deleteTwo(2);
		deleteTwo.execute();

		copyTask = deleteTwo.getTaskStore();
		iter = copyTask.begin();
		Assert::AreEqual(2,deleteTwo.getSize());
		Assert::AreEqual(std::string("Sentence one."),iter->getName());
		++iter;
		Assert::AreEqual(std::string("Sentence three."),iter->getName());

		Delete deleteOne(1);
		deleteOne.execute();

		copyTask = deleteOne.getTaskStore();
		iter = copyTask.begin();
		Assert::AreEqual(1,deleteOne.getSize());
		Assert::AreEqual(std::string("Sentence three."),iter->getName());

		deleteOne.undo();

		copyTask = deleteOne.getTaskStore();
		iter = copyTask.begin();
		Assert::AreEqual(2,deleteOne.getSize());
		Assert::AreEqual(std::string("Sentence one."),iter->getName());
		++iter;
		Assert::AreEqual(std::string("Sentence three."),iter->getName());

		deleteTwo.undo();

		copyTask = deleteOne.getTaskStore();
		iter = copyTask.begin();
		Assert::AreEqual(3,deleteTwo.getSize());
		Assert::AreEqual(std::string("Sentence one."),iter->getName());
		++iter;
		Assert::AreEqual(std::string("Sentence two."),iter->getName());
		++iter;
		Assert::AreEqual(std::string("Sentence three."),iter->getName());
	}
	};

	TEST_CLASS(Command_Modify) {
public:
	TEST_METHOD(Command_Modify_execute_FLOATtoTODO) {
		std::vector<Task> copyTask;
		std::vector<Task>::iterator iter;
		addThreeSentences(copyTask);

		std::vector<FieldType> fields;
		fields.push_back(NAME);
		Task modifiedTask;
		modifiedTask.setName("New Sentence Two");
		Modify modifyTwo(2, fields, modifiedTask);

		modifyTwo.execute();

		copyTask = modifyTwo.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual(std::string("Sentence one."),iter->getName());
		++iter;
		Assert::AreEqual(std::string("New Sentence Two"),iter->getName());
		++iter;
		Assert::AreEqual(std::string("Sentence three."),iter->getName());
		int IDthree = iter->getID();

		fields.push_back(TODO_DATE);
		Assert::AreEqual((size_t)2,fields.size());
		modifiedTask.setName("New Sentence Three");
		modifiedTask.setEndDate(150101);
		modifiedTask.setStartDate(150101);
		Modify modifyThree(3, fields, modifiedTask);

		modifyThree.execute();

		copyTask = modifyThree.getTaskStore();
		iter = copyTask.begin();
		if(TODO != iter->getType()) assert (false);
		Assert::AreEqual(IDthree,iter->getID());
		Assert::AreEqual(150101,iter->getEndDate());
		Assert::AreEqual(150101,iter->getStartDate());
		Assert::AreEqual(TIME_NOT_SET,iter->getStartTime());		
		Assert::AreEqual(std::string("New Sentence Three"),iter->getName());
		++iter;
		Assert::AreEqual(std::string("Sentence one."),iter->getName());
		++iter;
		Assert::AreEqual(std::string("New Sentence Two"),iter->getName());
	}

	TEST_METHOD(Command_Modify_execute_FLOATtoEVENT) {
		std::vector<Task> copyTask;
		std::vector<Task>::iterator iter;
		addThreeSentences(copyTask);

		std::vector<FieldType> fields;
		fields.push_back(NAME);
		Task modifiedTask;
		modifiedTask.setName("New Sentence Three");
		fields.push_back(END_DATE);
		fields.push_back(START_DATE);
		modifiedTask.setStartDate(150101);	
		modifiedTask.setEndDate(150201);
		Assert::AreEqual((size_t)3,fields.size());
		Modify modifyThree(3, fields, modifiedTask);

		copyTask = modifyThree.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual(std::string("Sentence one."),iter->getName());
		++iter;
		Assert::AreEqual(std::string("Sentence two."),iter->getName());
		++iter;
		Assert::AreEqual(std::string("Sentence three."),iter->getName());
		int IDthree = iter->getID();

		modifyThree.execute();

		copyTask = modifyThree.getTaskStore();
		iter = copyTask.begin();
		if(EVENT != iter->getType()) assert (false);
		Assert::AreEqual(IDthree,iter->getID());
		Assert::AreEqual(150101,iter->getStartDate());
		Assert::AreEqual(150201,iter->getEndDate());
		Assert::AreEqual(TIME_NOT_SET,iter->getStartTime());
		Assert::AreEqual(std::string("New Sentence Three"),iter->getName());
		++iter;
		Assert::AreEqual(std::string("Sentence one."),iter->getName());
		++iter;
		Assert::AreEqual(std::string("Sentence two."),iter->getName());
	}

	TEST_METHOD(Command_Modify_undo_TODOtoEVENT) {
		std::vector<Task> copyTask;
		std::vector<Task>::iterator iter;
		addThreeSentences(copyTask);

		std::vector<FieldType> fields;
		fields.push_back(NAME);
		Task modifiedTask;
		modifiedTask.setName("New Sentence Two");
		Modify modifyTwo(2, fields, modifiedTask);

		modifyTwo.execute();

		copyTask = modifyTwo.getTaskStore();
		iter = copyTask.begin();
		Assert::AreEqual(std::string("Sentence one."),iter->getName());
		++iter;
		Assert::AreEqual(std::string("New Sentence Two"),iter->getName());
		++iter;
		Assert::AreEqual(std::string("Sentence three."),iter->getName());

		fields.push_back(TODO_DATE);
		modifiedTask.setName("New Sentence Three");
		modifiedTask.setStartDate(150101);
		modifiedTask.setEndDate(150101);
		Modify modifyThree(3, fields, modifiedTask);

		modifyThree.execute();

		copyTask = modifyThree.getTaskStore();
		iter = copyTask.begin();
		Assert::AreEqual(std::string("New Sentence Three"),iter->getName());
		Assert::AreEqual(150101,iter->getEndDate());
		++iter;
		Assert::AreEqual(std::string("Sentence one."),iter->getName());
		++iter;
		Assert::AreEqual(std::string("New Sentence Two"),iter->getName());		

		modifyThree.undo();

		copyTask = modifyThree.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual(0,iter->getStartDate());
		Assert::AreEqual(0,iter->getEndDate());
		Assert::AreEqual(std::string("Sentence one."),iter->getName());
		++iter;
		Assert::AreEqual(std::string("New Sentence Two"),iter->getName());
		++iter;
		Assert::AreEqual(std::string("Sentence three."),iter->getName());
		Assert::AreEqual(0,iter->getStartDate());

		modifyTwo.undo();

		copyTask = modifyTwo.getTaskStore();
		iter = copyTask.begin();
		++iter;
		Assert::AreEqual(std::string("Sentence two."),iter->getName());
	}
	};

	TEST_CLASS(Command_Search) {
public:
	TEST_METHOD(Command_Search_execute) {
		// Add tasks
		Task task;
		task.setID(Task::incrementRunningCount());
		task.setName("one two three");
		Add addOne(task);
		addOne.clearTaskStore();
		addOne.execute();

		task.setID(Task::incrementRunningCount());
		task.setName("one three five");
		Add addTwo(task);
		addTwo.execute();

		task.setID(Task::incrementRunningCount());
		task.setName("one five seven");
		Add addThree(task);
		addThree.execute();

		std::vector<Task> copyTask;
		copyTask = addThree.getTaskStore();

		// Start searching
		Search searchOne("one");
		searchOne.execute();

		copyTask = addThree.getCurrentView();
		std::vector<Task>::iterator iter;
		iter = copyTask.begin();
		Assert::AreEqual((size_t)3,copyTask.size());
		Assert::AreEqual(std::string("one two three"),iter->getName());
		++iter;
		Assert::AreEqual(std::string("one three five"),iter->getName());
		++iter;
		Assert::AreEqual(std::string("one five seven"),iter->getName());

		Search searchThree("three");
		searchThree.execute();

		copyTask = searchThree.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual((size_t)2,copyTask.size());
		Assert::AreEqual(std::string("one two three"),iter->getName());
		++iter;
		Assert::AreEqual(std::string("one three five"),iter->getName());

		Search searchFive("fiVe");
		searchFive.execute();

		copyTask = addThree.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual((size_t)2,copyTask.size());
		Assert::AreEqual(std::string("one three five"),iter->getName());
		++iter;
		Assert::AreEqual(std::string("one five seven"),iter->getName());

		Search searchSeven("sEveN");
		searchSeven.execute();

		copyTask = addThree.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual((size_t)1,copyTask.size());
		Assert::AreEqual(std::string("one five seven"),iter->getName());

		Search searchGibberish("51267");
		searchGibberish.execute();

		copyTask = addThree.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual((size_t)0,copyTask.size());
	}

	TEST_METHOD(Command_Search_undo) {
		// Add tasks
		Task task;
		task.setID(Task::incrementRunningCount());
		task.setName("one two three");
		Add addOne(task);
		addOne.clearTaskStore();
		addOne.execute();

		task.setID(Task::incrementRunningCount());
		task.setName("one three five");
		Add addTwo(task);
		addTwo.execute();

		task.setID(Task::incrementRunningCount());
		task.setName("one five seven");
		Add addThree(task);
		addThree.execute();

		std::vector<Task> copyTask;
		copyTask = addThree.getTaskStore();

		// Start searching
		Search searchOne("one");
		searchOne.execute();

		copyTask = addThree.getCurrentView();
		std::vector<Task>::iterator iter;
		iter = copyTask.begin();
		Assert::AreEqual((size_t)3,copyTask.size());
		Assert::AreEqual(std::string("one two three"),iter->getName());
		++iter;
		Assert::AreEqual(std::string("one three five"),iter->getName());
		++iter;
		Assert::AreEqual(std::string("one five seven"),iter->getName());

		Search searchThree("three");
		searchThree.execute();

		copyTask = searchThree.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual((size_t)2,copyTask.size());
		Assert::AreEqual(std::string("one two three"),iter->getName());
		++iter;
		Assert::AreEqual(std::string("one three five"),iter->getName());

		searchThree.undo();

		copyTask = addThree.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual((size_t)3,copyTask.size());
		Assert::AreEqual(std::string("one two three"),iter->getName());
		++iter;
		Assert::AreEqual(std::string("one three five"),iter->getName());
		++iter;
		Assert::AreEqual(std::string("one five seven"),iter->getName());

		Search searchFive("fiVe");
		searchFive.execute();

		copyTask = addThree.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual((size_t)2,copyTask.size());
		Assert::AreEqual(std::string("one three five"),iter->getName());
		++iter;
		Assert::AreEqual(std::string("one five seven"),iter->getName());

		Search searchSeven("sEveN");
		searchSeven.execute();

		copyTask = addThree.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual((size_t)1,copyTask.size());
		Assert::AreEqual(std::string("one five seven"),iter->getName());

		searchSeven.undo();

		copyTask = addThree.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual((size_t)2,copyTask.size());
		Assert::AreEqual(std::string("one three five"),iter->getName());
		++iter;
		Assert::AreEqual(std::string("one five seven"),iter->getName());

		searchFive.undo();

		copyTask = addThree.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual((size_t)3,copyTask.size());
		Assert::AreEqual(std::string("one two three"),iter->getName());
		++iter;
		Assert::AreEqual(std::string("one three five"),iter->getName());
		++iter;
		Assert::AreEqual(std::string("one five seven"),iter->getName());
	}
	};

	TEST_CLASS(Command_MarkDone) {
public:
	TEST_METHOD(Command_MarkDone_execute) {
		// Add tasks
		Task task;
		Add addOne(task);
		addOne.clearTaskStore();
		addOne.execute();

		task.setID(task.incrementRunningCount());
		task.markDone();
		Add addTwo(task);
		addTwo.execute();

		// KB: to include done tasks
		View viewAll(VIEWTYPE_ALL, "");
		viewAll.execute();

		// Start markDone
		Markdone markdoneOne(1);	
		markdoneOne.execute();		

		std::vector<Task> copyTask;
		copyTask = addTwo.getCurrentView();
		std::vector<Task>::iterator iter;
		iter = copyTask.begin();
		Assert::AreEqual((size_t)0,copyTask.size());
		copyTask = addTwo.getTaskStore();
		iter = copyTask.begin();
		Assert::AreEqual(true,iter->getDoneStatus());

		View viewPast(VIEWTYPE_PAST, "");
		viewPast.execute();

		Markdone markdoneTwo(1);
		markdoneTwo.execute();

		copyTask = addTwo.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual((size_t)0,copyTask.size());		
	}

	TEST_METHOD(Command_MarkDone_undo) {
		// Add tasks
		Task task;
		Add addOne(task);
		addOne.clearTaskStore();
		addOne.execute();

		task.setID(task.incrementRunningCount());
		task.markDone();
		Add addTwo(task);
		addTwo.execute();

		View viewAll(VIEWTYPE_ALL, "");
		viewAll.execute();

		// Start markDone
		Markdone markdoneOne(1);
		markdoneOne.execute();

		std::vector<Task> copyTask;
		copyTask = addTwo.getCurrentView();
		std::vector<Task>::iterator iter;
		iter = copyTask.begin();
		Assert::AreEqual((size_t)0,copyTask.size());
		copyTask = addTwo.getTaskStore();
		iter = copyTask.begin();
		Assert::AreEqual(true,iter->getDoneStatus());

		View viewPast(VIEWTYPE_PAST, "");
		viewPast.execute();

		Markdone markdoneTwo(1);
		markdoneTwo.execute();

		copyTask = addTwo.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual((size_t)0,copyTask.size()); // Goes back to deafult view

		markdoneTwo.undo();

		copyTask = addTwo.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual((size_t)0,copyTask.size());
		copyTask = addTwo.getTaskStore();
		iter = copyTask.begin();
		++iter;
		Assert::AreEqual(true,iter->getDoneStatus());

		markdoneOne.undo();

		copyTask = addTwo.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual((size_t)1,copyTask.size());
		Assert::AreEqual(false,iter->getDoneStatus());
	}
	};

	TEST_CLASS(Command_UnmarkDone) {
public:
	TEST_METHOD(Command_UnmarkDone_execute) {
		// Add tasks
		Task task;
		Add addOne(task);
		addOne.clearTaskStore();
		addOne.execute();

		task.setID(task.incrementRunningCount());
		task.markDone();
		Add addTwo(task);
		addTwo.execute();

		View viewAll(VIEWTYPE_ALL, "");
		viewAll.execute();

		// Start unmarkDone
		UnmarkDone unmarkdoneOne(1);
		unmarkdoneOne.execute();

		std::vector<Task> copyTask;
		copyTask = addTwo.getCurrentView();
		std::vector<Task>::iterator iter;
		iter = copyTask.begin();
		Assert::AreEqual((size_t)1,copyTask.size());

		View viewPast(VIEWTYPE_PAST, "");
		viewPast.execute();

		UnmarkDone unmarkdoneTwo(1);
		unmarkdoneTwo.execute();

		copyTask = addTwo.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual((size_t)2,copyTask.size());
		++iter;
		Assert::AreEqual(false,iter->getDoneStatus());
	}

	TEST_METHOD(Command_UnmarkDone_undo) {
		// Add tasks
		Task task;
		Add addOne(task);
		addOne.clearTaskStore();
		addOne.execute();

		task.setID(task.incrementRunningCount());
		task.markDone();
		Add addTwo(task);
		addTwo.execute();

		View viewAll(VIEWTYPE_ALL, "");
		viewAll.execute();

		// Start unmarkDone
		UnmarkDone unmarkdoneOne(1);
		unmarkdoneOne.execute();

		std::vector<Task> copyTask;
		copyTask = addTwo.getCurrentView();
		std::vector<Task>::iterator iter;
		iter = copyTask.begin();
		Assert::AreEqual((size_t)1,copyTask.size());

		View viewPast(VIEWTYPE_PAST, "");
		viewPast.execute();

		UnmarkDone unmarkdoneTwo(1);
		unmarkdoneTwo.execute();

		copyTask = addTwo.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual((size_t)2,copyTask.size());
		copyTask = addTwo.getTaskStore();
		iter = copyTask.begin();
		++iter;
		Assert::AreEqual(false,iter->getDoneStatus());

		unmarkdoneTwo.undo();

		copyTask = addTwo.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual((size_t)1,copyTask.size());
		copyTask = addTwo.getTaskStore();
		iter = copyTask.begin();
		++iter;
		Assert::AreEqual(true,iter->getDoneStatus());

		unmarkdoneOne.undo();

		copyTask = addTwo.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual((size_t)1,copyTask.size());
	}
	};

	TEST_CLASS(Command_View) {
public:

	TEST_METHOD(Command_View_TaskType) {
		// TODO: Implement for other task types too
		Task taskOne;
		taskOne.setID(Task::incrementRunningCount());	// Added to fix uniqueID (Aaron)
		taskOne.setName("Sentence one.");
		taskOne.setType(FLOATING);
		taskOne.setStartDate(150910);
		taskOne.setEndDate(150910);
		Add addOne(taskOne);							// Adds taskOne into taskStore (Step 1/2)
		addOne.clearTaskStore();
		addOne.execute();								// Adds taskOne into taskStore (Step 2/2)

		std::vector<Task> copyTask;
		std::vector<Task>::iterator iter;

		Task taskTwo;
		taskTwo.setName("Sentence two.");
		taskTwo.setType(TODO);
		taskTwo.setStartDate(151010);
		taskTwo.setEndDate(151010);
		Add addTwo(taskTwo);
		addTwo.execute();

		Task taskThree;
		taskThree.setName("Sentence three.");
		taskThree.setType(TODO);
		taskThree.setStartDate(150809);
		taskThree.setEndDate(150809);
		Add addThree(taskThree);
		addThree.execute();

		Task taskFour;
		taskFour.setName("Sentence four.");
		taskFour.setType(FLOATING);
		taskFour.setStartDate(150910);
		taskFour.setEndDate(150910);
		Add addFour(taskFour);
		addFour.execute();

		View viewFloating(VIEWTYPE_FLOATING, "");
		viewFloating.execute();

		copyTask = viewFloating.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual(std::string("Sentence one."), iter->getName());
		++iter;
		Assert::AreEqual(std::string("Sentence four."), iter->getName());
	}

	TEST_METHOD(Command_View_All) {
		// Three: 150809
		// One: 150910
		// Four: 150910	
		// Two: 151010
		// View all only views done tasks

		Task taskOne;
		taskOne.setID(Task::incrementRunningCount());	// Added to fix uniqueID (Aaron)
		taskOne.setName("Sentence one.");
		taskOne.setStartDate(150910);
		taskOne.setEndDate(150910);
		Add addOne(taskOne);							// Adds taskOne into taskStore (Step 1/2)
		addOne.clearTaskStore();
		addOne.execute();								// Adds taskOne into taskStore (Step 2/2)

		std::vector<Task> copyTask;
		std::vector<Task>::iterator iter;

		Task taskTwo;
		taskTwo.setName("Sentence two.");
		taskTwo.setStartDate(151010);
		taskTwo.setEndDate(151010);
		Add addTwo(taskTwo);
		addTwo.execute();

		Task taskThree;
		taskThree.setName("Sentence three.");
		taskThree.setStartDate(150809);
		taskThree.setEndDate(150809);
		taskThree.markDone();
		Add addThree(taskThree);
		addThree.execute();

		Task taskFour;
		taskFour.setName("Sentence four.");
		taskFour.setStartDate(150910);
		taskFour.setEndDate(150910);
		Add addFour(taskFour);
		addFour.execute();

		View viewAll(VIEWTYPE_ALL, "");
		viewAll.execute();

		copyTask = viewAll.getCurrentView();
		iter = copyTask.begin();
		//Assert::AreEqual(std::string("Sentence three."), iter->getName());
		//++iter;
		Assert::AreEqual(std::string("Sentence one."), iter->getName());
		++iter;
		Assert::AreEqual(std::string("Sentence four."), iter->getName());
		++iter;
		Assert::AreEqual(std::string("Sentence two."), iter->getName());
		//Assert::AreEqual(150809, iter->getStartDate());	
	}

	TEST_METHOD(Command_View_Done) {
		Task taskOne;
		taskOne.setID(Task::incrementRunningCount());	// Added to fix uniqueID (Aaron)
		taskOne.setName("Sentence one.");
		taskOne.setStartDate(150910);
		taskOne.setEndDate(150910);
		Add addOne(taskOne);							// Adds taskOne into taskStore (Step 1/2)
		addOne.clearTaskStore();
		addOne.execute();								// Adds taskOne into taskStore (Step 2/2)

		std::vector<Task> copyTask;
		std::vector<Task>::iterator iter;

		Task taskTwo;
		taskTwo.setName("Sentence two.");
		taskTwo.setStartDate(151010);
		taskTwo.setEndDate(151010);
		Add addTwo(taskTwo);
		addTwo.execute();

		Task taskThree;
		taskThree.setName("Sentence three.");
		taskThree.setStartDate(150809);
		taskThree.setEndDate(150809);
		taskThree.markDone();
		Add addThree(taskThree);
		addThree.execute();

		Task taskFour;
		taskFour.setName("Sentence four.");
		taskFour.setStartDate(150910);
		taskFour.setEndDate(150910);
		Add addFour(taskFour);
		addFour.execute();

		View viewPast(VIEWTYPE_PAST, "");
		viewPast.execute();

		copyTask = viewPast.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual(std::string("Sentence three."), iter->getName());
	}

	TEST_METHOD(Command_View_Notdone) {
		Task taskOne;
		taskOne.setID(Task::incrementRunningCount());	// Added to fix uniqueID (Aaron)
		taskOne.setName("Sentence one.");
		taskOne.setStartDate(150910);
		taskOne.setEndDate(150910);
		Add addOne(taskOne);							// Adds taskOne into taskStore (Step 1/2)
		addOne.clearTaskStore();
		addOne.execute();								// Adds taskOne into taskStore (Step 2/2)

		std::vector<Task> copyTask;
		std::vector<Task>::iterator iter;

		Task taskTwo;
		taskTwo.setName("Sentence two.");
		taskTwo.setStartDate(151010);
		taskTwo.setEndDate(151010);
		Add addTwo(taskTwo);
		addTwo.execute();

		Task taskThree;
		taskThree.setName("Sentence three.");
		taskThree.setStartDate(150809);
		taskThree.setEndDate(150809);
		taskThree.markDone();
		Add addThree(taskThree);
		addThree.execute();

		Task taskFour;
		taskFour.setName("Sentence four.");
		taskFour.setStartDate(150910);
		taskFour.setEndDate(150910);
		Add addFour(taskFour);
		addFour.execute();

		/*
		View viewNotDone(VIEWTYPE_NOTDONE, "");
		viewNotDone.execute();

		copyTask = viewNotDone.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual(std::string("Sentence one."), iter->getName());
		++iter;
		Assert::AreEqual(std::string("Sentence four."), iter->getName());
		++iter;
		Assert::AreEqual(std::string("Sentence two."), iter->getName());
		*/
	}
	};

	TEST_CLASS(Command_Load) {
public:
	TEST_METHOD(Command_Load_Execute_empty) {
		try {
			IO* io = IO::getInstance();
			Load load(io->getFilePath());
			load.execute();
		} catch (std::exception e) {
			Assert::AreEqual("File does not exist",e.what());
		}
	}
	};

	TEST_CLASS(Command_Pick) {
public:
	Command* cmd;
	Parser* parser;
	History* history;
	Task task;
	std::string userInput;

	TEST_METHOD_INITIALIZE(ClearTaskStoreAndGetParser) {
		cmd->clearTaskStore();
		parser = Parser::getInstance();
		history = History::getInstance();
		history->clearHistory();
	}

	TEST_METHOD(Command_Pick_reserve_empty) {
		userInput = "add Recursion Lecture on 1 dec by 2 pm";
		cmd = parser->parse(userInput);
		cmd->execute();
		task = cmd->getTaskStore().back();
		Assert::AreEqual(std::string("Recursion Lecture"),task.getName());
		Assert::AreEqual(std::string("TODO"),Utilities::taskTypeToString(task.getType()));
		// Assert::AreEqual(std::string(""),task.getLabelString());
		// Assert::AreEqual(false,task.getDoneStatus());
		// Assert::AreEqual(false,task.getPriorityStatus());
		Assert::AreEqual(151201, task.getStartDate());
		Assert::AreEqual(1400, task.getStartTime());
		Assert::AreEqual(151201, task.getEndDate());
		Assert::AreEqual(1400, task.getEndTime());

		userInput = "pick 1 reserve";
		cmd = parser->parse(userInput);
		cmd->execute();
		task = cmd->getTaskStore().back();
		Assert::AreEqual(std::string("Recursion Lecture"),task.getName());
		Assert::AreEqual(std::string("TODO"),Utilities::taskTypeToString(task.getType()));
		// Assert::AreEqual(std::string(""),task.getLabelString());
		// Assert::AreEqual(false,task.getDoneStatus());
		// Assert::AreEqual(false,task.getPriorityStatus());
		Assert::AreEqual(151201, task.getStartDate());
		Assert::AreEqual(1400, task.getStartTime());
		Assert::AreEqual(151201, task.getEndDate());
		Assert::AreEqual(1400, task.getEndTime());
	}

	TEST_METHOD(Command_Pick_reserve_TODOtoEVENT) {
		userInput = "add Recursion Lecture on 1 dec by 2 pm reserve on 3 dec from 4 pm to 5 pm";
		cmd = parser->parse(userInput);
		cmd->execute();
		task = cmd->getTaskStore().back();
		Assert::AreEqual(std::string("Recursion Lecture"),task.getName());
		Assert::AreEqual(std::string("TODO"),Utilities::taskTypeToString(task.getType()));
		// Assert::AreEqual(std::string(""),task.getLabelString());
		// Assert::AreEqual(false,task.getDoneStatus());
		// Assert::AreEqual(false,task.getPriorityStatus());
		Assert::AreEqual(151201, task.getStartDate());
		Assert::AreEqual(1400, task.getStartTime());
		Assert::AreEqual(151201, task.getEndDate());
		Assert::AreEqual(1400, task.getEndTime());

		history->add(cmd);
		Assert::AreEqual(1,history->getUndoSize());

		userInput = "pick 1 reserve"; // 'r' and 're' are alternatives to 'reserve'
		cmd = parser->parse(userInput);
		cmd->execute();
		task = cmd->getTaskStore().back();
		Assert::AreEqual(std::string("Recursion Lecture"),task.getName());
		Assert::AreEqual(std::string("EVENT"),Utilities::taskTypeToString(task.getType()));
		// Assert::AreEqual(std::string(""),task.getLabelString());
		// Assert::AreEqual(false,task.getDoneStatus());
		// Assert::AreEqual(false,task.getPriorityStatus());
		Assert::AreEqual(151203, task.getStartDate());
		Assert::AreEqual(1600, task.getStartTime());
		Assert::AreEqual(151203, task.getEndDate());
		Assert::AreEqual(1700, task.getEndTime());

		history->add(cmd);
		Assert::AreEqual(2,history->getUndoSize());
	}

	TEST_METHOD(Command_Pick_reserve_Undo) {
		Command_Pick_reserve_TODOtoEVENT();

		userInput = "undo";
		cmd = parser->parse(userInput);
		cmd->execute();
		task = cmd->getTaskStore().back();
		Assert::AreEqual(std::string("Recursion Lecture"),task.getName());
		Assert::AreEqual(std::string("TODO"),Utilities::taskTypeToString(task.getType()));
		// Assert::AreEqual(std::string(""),task.getLabelString());
		// Assert::AreEqual(false,task.getDoneStatus());
		// Assert::AreEqual(false,task.getPriorityStatus());
		Assert::AreEqual(151201, task.getStartDate());
		Assert::AreEqual(1400, task.getStartTime());
		Assert::AreEqual(151201, task.getEndDate());
		Assert::AreEqual(1400, task.getEndTime());
	}
	};
}

void addThreeSentences(std::vector<Task> copyTask) {
	Task taskOne;
	taskOne.setID(Task::incrementRunningCount());
	taskOne.setName("Sentence one.");
	Add addOne(taskOne);
	addOne.clearTaskStore();
	addOne.execute();

	copyTask = addOne.getTaskStore();

	std::vector<Task>::iterator iter;
	iter = copyTask.begin();
	Assert::AreEqual(std::string("Sentence one."), iter->getName());

	Task taskTwo;
	taskTwo.setName("Sentence two.");
	taskTwo.setID(Task::incrementRunningCount());
	Add addTwo(taskTwo);
	addTwo.execute();

	copyTask = addTwo.getTaskStore();
	iter = copyTask.begin();
	++iter;
	Assert::AreEqual(std::string("Sentence two."),iter->getName());

	Task taskThree;
	taskThree.setName("Sentence three.");
	taskThree.setID(Task::incrementRunningCount());
	Add addThree(taskThree);
	addThree.execute();

	copyTask = addThree.getTaskStore();
	iter = copyTask.begin();
	++iter;
	++iter;
	Assert::AreEqual(std::string("Sentence three."),iter->getName());
}
