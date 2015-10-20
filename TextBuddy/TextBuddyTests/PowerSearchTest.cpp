// @@author Chin Kiat Boon 

#include "stdafx.h"
#include "PowerSearch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TextBuddyTests {
	TEST_CLASS(PowerSearchTest) {
	public:
		
		TEST_METHOD(setTasksWithinPeriodTest) {
			PowerSearch testPwrSearch;
			std::vector<Task>::iterator iter;

			Task testTask;
			testTask.setStartDate(151010);
			testTask.setStartTime(0);
			(testPwrSearch.taskStore).push_back(testTask);
			testTask.setStartDate(151010);
			testTask.setStartTime(0);
			(testPwrSearch.taskStore).push_back(testTask);
			testTask.setStartDate(151015);
			testTask.setStartTime(2100);
			(testPwrSearch.taskStore).push_back(testTask);
			testTask.setStartDate(151015);
			testTask.setStartTime(2300);
			(testPwrSearch.taskStore).push_back(testTask);
			testTask.setStartDate(151016);
			testTask.setStartTime(1500);
			(testPwrSearch.taskStore).push_back(testTask);
			testTask.setStartDate(151017);
			testTask.setStartTime(800);
			(testPwrSearch.taskStore).push_back(testTask);

			testPwrSearch.setTasksWithinPeriod(151015,2200,151016,1500);


			iter = (testPwrSearch.tasksWithinPeriod).begin();
			Assert::AreEqual(iter->getStartDate(), 151015);
			Assert::AreEqual(iter->getStartTime(), 2300);

			++iter;
			Assert::AreEqual(iter->getStartDate(), 151016);

		}

	};
}