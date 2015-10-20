// @@author A0130463R (Ng Ren Zhi)

#include "stdafx.h"
#include "IO.h"

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
		addOne.execute();						// Adds taskOne into taskStore (Step 2/2)

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

	// Added by Kiat Boon 20/10/15
	TEST_METHOD(Command_Add_Sort) {
		Task taskOne;
		taskOne.setID(Task::incrementRunningCount());	// Added to fix uniqueID (Aaron)
		taskOne.setName("Sentence one.");
		taskOne.setStartDate(150910);
		taskOne.setPriority();							//prioritised task
		Add addOne(taskOne);							// Adds taskOne into taskStore (Step 1/2)
		addOne.clearTaskStore();
		addOne.execute();						// Adds taskOne into taskStore (Step 2/2)

		std::vector<Task> copyTask;
		std::vector<Task>::iterator iter;

		Task taskTwo;
		taskTwo.setName("Sentence two.");
		taskTwo.setStartDate(151010);
		taskTwo.setPriority();							//prioritised task
		Add addTwo(taskTwo);
		addTwo.execute();

		Task taskThree;
		taskThree.setName("Sentence three.");
		taskThree.setStartDate(150809);
		Add addThree(taskThree);
		addThree.execute();

		Task taskFour;
		taskFour.setName("Sentence four.");
		taskFour.setStartDate(150910);
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
	TEST_METHOD(Command_Modify_execute) {
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

		fields.push_back(START_DATE);
		modifiedTask.setName("New Sentence Three");
		modifiedTask.setStartDate(150101);
		Modify modifyThree(3, fields, modifiedTask);

		modifyThree.execute();

		copyTask = modifyThree.getTaskStore();
		iter = copyTask.begin();
		++iter;
		++iter;
		Assert::AreEqual(std::string("New Sentence Three"),iter->getName());
		Assert::AreEqual(150101,iter->getStartDate());
	}

	TEST_METHOD(Command_Modify_undo) {
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

		fields.push_back(START_DATE);
		modifiedTask.setName("New Sentence Three");
		modifiedTask.setStartDate(150101);
		Modify modifyThree(3, fields, modifiedTask);

		modifyThree.execute();

		copyTask = modifyThree.getTaskStore();
		iter = copyTask.begin();
		++iter;
		++iter;
		Assert::AreEqual(std::string("New Sentence Three"),iter->getName());
		Assert::AreEqual(150101,iter->getStartDate());

		modifyTwo.undo();

		copyTask = modifyTwo.getTaskStore();
		iter = copyTask.begin();
		++iter;
		Assert::AreEqual(std::string("Sentence two."),iter->getName());

		modifyThree.undo();

		copyTask = modifyThree.getTaskStore();
		iter = copyTask.begin();
		++iter;
		++iter;
		Assert::AreEqual(std::string("Sentence three."),iter->getName());
		Assert::AreEqual(0,iter->getStartDate());
	}
	};

	TEST_CLASS(Command_Search) {
public:
	TEST_METHOD(Command_Search_execute) {
		//Add tasks
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

		//Start searching
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
		//Add tasks
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

		//Start Searching
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
		//Add tasks
		Task task;
		Add addOne(task);
		addOne.clearTaskStore();
		addOne.execute();

		task.setID(task.incrementRunningCount());
		task.markDone();
		Add addTwo(task);
		addTwo.execute();

		//Start markDone
		Markdone markdoneOne(1);
		markdoneOne.execute();

		std::vector<Task> copyTask;
		copyTask = addTwo.getCurrentView();
		std::vector<Task>::iterator iter;
		iter = copyTask.begin();
		Assert::AreEqual((size_t)1,copyTask.size());
		copyTask = addTwo.getTaskStore();
		iter = copyTask.begin();
		Assert::AreEqual(true,iter->getDoneStatus());

		Markdone markdoneTwo(1);
		markdoneTwo.execute();

		copyTask = addTwo.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual((size_t)1,copyTask.size());
		Assert::AreEqual(true,iter->getDoneStatus());
	}

	TEST_METHOD(Command_MarkDone_undo) {
		//Add tasks
		Task task;
		Add addOne(task);
		addOne.clearTaskStore();
		addOne.execute();

		task.setID(task.incrementRunningCount());
		task.markDone();
		Add addTwo(task);
		addTwo.execute();

		//Start markDone
		Markdone markdoneOne(1);
		markdoneOne.execute();

		std::vector<Task> copyTask;
		copyTask = addTwo.getCurrentView();
		std::vector<Task>::iterator iter;
		iter = copyTask.begin();
		Assert::AreEqual((size_t)1,copyTask.size());
		copyTask = addTwo.getTaskStore();
		iter = copyTask.begin();
		Assert::AreEqual(true,iter->getDoneStatus());

		Markdone markdoneTwo(1);
		markdoneTwo.execute();

		copyTask = addTwo.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual((size_t)1,copyTask.size());
		Assert::AreEqual(true,iter->getDoneStatus());

		markdoneTwo.undo();

		copyTask = addTwo.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual((size_t)1,copyTask.size());
		Assert::AreEqual(true,iter->getDoneStatus());

		markdoneOne.undo();

		copyTask = addTwo.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual((size_t)2,copyTask.size());
		Assert::AreEqual(false,iter->getDoneStatus());
	}
	};

	TEST_CLASS(Command_UnmarkDone) {
public:
	TEST_METHOD(Command_UnmarkDone_execute) {
		//Add tasks
		Task task;
		Add addOne(task);
		addOne.clearTaskStore();
		addOne.execute();

		task.setID(task.incrementRunningCount());
		task.markDone();
		Add addTwo(task);
		addTwo.execute();

		//Start unmarkDone
		UnmarkDone unmarkdoneOne(1);
		unmarkdoneOne.execute();

		std::vector<Task> copyTask;
		copyTask = addTwo.getCurrentView();
		std::vector<Task>::iterator iter;
		iter = copyTask.begin();
		Assert::AreEqual((size_t)2,copyTask.size());

		UnmarkDone unmarkdoneTwo(2);
		unmarkdoneTwo.execute();

		copyTask = addTwo.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual((size_t)1,copyTask.size());
		copyTask = addTwo.getTaskStore();
		iter = copyTask.begin();
		++iter;
		Assert::AreEqual(false,iter->getDoneStatus());
	}

	TEST_METHOD(Command_UnmarkDone_undo) {
		//Add tasks
		Task task;
		Add addOne(task);
		addOne.clearTaskStore();
		addOne.execute();

		task.setID(task.incrementRunningCount());
		task.markDone();
		Add addTwo(task);
		addTwo.execute();

		//Start unmarkDone
		UnmarkDone unmarkdoneOne(1);
		unmarkdoneOne.execute();

		std::vector<Task> copyTask;
		copyTask = addTwo.getCurrentView();
		std::vector<Task>::iterator iter;
		iter = copyTask.begin();
		Assert::AreEqual((size_t)2,copyTask.size());

		UnmarkDone unmarkdoneTwo(2);
		unmarkdoneTwo.execute();

		copyTask = addTwo.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual((size_t)1,copyTask.size());
		copyTask = addTwo.getTaskStore();
		iter = copyTask.begin();
		++iter;
		Assert::AreEqual(false,iter->getDoneStatus());

		unmarkdoneTwo.undo();

		copyTask = addTwo.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual((size_t)2,copyTask.size());
		++iter;
		Assert::AreEqual(true,iter->getDoneStatus());

		unmarkdoneOne.undo();

		copyTask = addTwo.getCurrentView();
		iter = copyTask.begin();
		Assert::AreEqual((size_t)2,copyTask.size());
	}
	};

	TEST_CLASS(Command_Load) {
public:
	TEST_METHOD(Command_Load_Execute_empty) {
		IO* io = IO::getInstance();
		Load load(io->getFilePath());
		load.execute();
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
