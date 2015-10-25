// Chin Kiat Boon @@author A0096720A 

#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TextBuddyTests {
	TEST_CLASS(PowerSearchTest) {
public:
	TEST_METHOD(PowerSearch_setTasksWithinPeriod) {
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

	TEST_METHOD(PowerSearch_searchFreeSlot) {
		PowerSearch testPwrSearch;
		std::vector<Task>::iterator iter;

		//Create method to "fit in" an event that may or may not exceed the range of free period
		//Or can set parameter x such that free period mentioned must exceed x
		Task testTask;
		testTask.setStartDate(151010);
		testTask.setStartTime(0);
		testTask.setEndDate(151010);					//is endDate set to be the same as startDate if no range?
		testTask.setEndTime(1500);						//note: throw exception if endtime < starttime
		(testPwrSearch.taskStore).push_back(testTask);
		testTask.setStartDate(151010);
		testTask.setStartTime(0);
		testTask.setEndDate(151010);
		testTask.setEndTime(1600);
		(testPwrSearch.taskStore).push_back(testTask);
		testTask.setStartDate(151015);
		testTask.setStartTime(2100);
		testTask.setEndDate(151016);
		testTask.setEndTime(2000);
		(testPwrSearch.taskStore).push_back(testTask);
		testTask.setStartDate(151015);
		testTask.setStartTime(2300);
		testTask.setEndDate(151015);
		testTask.setEndTime(2350);
		(testPwrSearch.taskStore).push_back(testTask);
		testTask.setStartDate(151016);
		testTask.setStartTime(1500);
		testTask.setEndDate(151016);
		testTask.setEndTime(1500);
		(testPwrSearch.taskStore).push_back(testTask);
		testTask.setStartDate(151017);
		testTask.setStartTime(800);
		testTask.setEndDate(151017);
		testTask.setEndTime(800);
		(testPwrSearch.taskStore).push_back(testTask);

		testPwrSearch.searchFreeSlot(151009,2300,151017,900);

		iter = (testPwrSearch.freeDates).begin();
		Assert::AreEqual(151009, iter->getStartDate());
		Assert::AreEqual(2300, iter->getStartTime());
		Assert::AreEqual(151010, iter->getEndDate());
		Assert::AreEqual(0, iter->getEndTime());

		++iter;
		Assert::AreEqual(151010, iter->getStartDate());
		Assert::AreEqual(1600, iter->getStartTime());
		Assert::AreEqual(151015, iter->getEndDate());
		Assert::AreEqual(2100, iter->getEndTime());

		++iter;
		Assert::AreEqual(151016, iter->getStartDate());
		Assert::AreEqual(2000, iter->getStartTime());
		Assert::AreEqual(151017, iter->getEndDate());
		Assert::AreEqual(800, iter->getEndTime());

		++iter;
		Assert::AreEqual(151017, iter->getStartDate());
		Assert::AreEqual(800, iter->getStartTime());
		Assert::AreEqual(151017, iter->getEndDate());
		Assert::AreEqual(900, iter->getEndTime());
	}

	};
}