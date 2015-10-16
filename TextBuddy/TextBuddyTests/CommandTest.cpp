// @@author Ng Ren Zhi

#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TextBuddyTests
{
	TEST_CLASS(Command_Add)
	{
	public:

		//originally written by Kiat Boon
		TEST_METHOD(Command_Add_execute)
		{
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

		TEST_METHOD(Command_Add_undo)
		{
			Task taskOne;
			taskOne.setID(Task::incrementRunningCount());	// Added to fix uniqueID (Aaron)
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
			Assert::AreEqual(3,taskTwoID);
			Assert::AreEqual(4,taskThreeID);
			Assert::AreEqual(std::string("Sentence one."),iter->getName());
			++iter;
			Assert::AreEqual(std::string("Sentence three."),iter->getName());

			addOne.undo();

			copyTask = addThree.getTaskStore();
			iter = copyTask.begin();
			Assert::AreEqual(1,addOne.getSize());
			Assert::AreEqual(std::string("Sentence three."),iter->getName());

		}
	};

	TEST_CLASS(Command_Delete)
	{
	public:

		TEST_METHOD(Command_Delete_Execute)
		{
			Task taskOne;
			taskOne.setID(Task::incrementRunningCount());
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

			Delete deleteTwo(2);
			deleteTwo.execute();

			copyTask = deleteTwo.getTaskStore();
			iter = copyTask.begin();
			Assert::AreEqual(2,addTwo.getSize());
			Assert::AreEqual(std::string("Sentence one."),iter->getName());
			++iter;
			Assert::AreEqual(std::string("Sentence three."),iter->getName());

			Delete deleteOne(1);
			deleteOne.execute();

			copyTask = deleteOne.getTaskStore();
			iter = copyTask.begin();
			Assert::AreEqual(1,addOne.getSize());
			Assert::AreEqual(std::string("Sentence three."),iter->getName());
		}

		TEST_METHOD(Command_Delete_Undo)
		{
			Task taskOne;
			taskOne.setID(Task::incrementRunningCount());
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

			Delete deleteTwo(2);
			deleteTwo.execute();

			copyTask = deleteTwo.getTaskStore();
			iter = copyTask.begin();
			Assert::AreEqual(2,addTwo.getSize());
			Assert::AreEqual(std::string("Sentence one."),iter->getName());
			++iter;
			Assert::AreEqual(std::string("Sentence three."),iter->getName());

			Delete deleteOne(1);
			deleteOne.execute();

			copyTask = deleteOne.getTaskStore();
			iter = copyTask.begin();
			Assert::AreEqual(1,addOne.getSize());
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

	TEST_CLASS(Command_Modify)
	{
	public:

		TEST_METHOD(Command_Modify_execute)
		{
			Task taskOne;
			taskOne.setID(Task::incrementRunningCount());
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

			std::vector<FieldType> fields;
			fields.push_back(NAME);
			Task modifiedTaskTwo;
			modifiedTaskTwo.setName("New Sentence Two");
			Modify modifyTwo(2, fields, modifiedTaskTwo);

			copyTask = modifyTwo.getTaskStore();
			iter = copyTask.begin();
			++iter;
			Assert::AreEqual(std::string("New Sentence Two"),iter->getName());
		}

	};
}