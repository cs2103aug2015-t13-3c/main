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
	};
}