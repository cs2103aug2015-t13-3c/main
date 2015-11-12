// Chin Kiat Boon @@author A0096720A 

#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TaskSharkTests {
	TEST_CLASS(PowerSearchTest) {
public:
	TEST_METHOD(PowerSearch_searchFreeSlot) {
		// Vector created to allow creation of testPwrSearch
		std::vector<std::string> paraVec; 
		paraVec.push_back("");
		paraVec.push_back("0");
		paraVec.push_back("0");
		paraVec.push_back("0");
		paraVec.push_back("0");
		paraVec.push_back("0");
		paraVec.push_back("0");
		paraVec.push_back("0");

		PowerSearch testPwrSearch(paraVec);
		Task testTask;
		testTask.setStartDate(151010);
		testTask.setStartTime(0);
		testTask.setEndDate(151011);					
		testTask.setEndTime(0);						
		testTask.setType(EVENT);
		Add addOne(testTask);
		addOne.clearTaskStore();
		addOne.execute();

		testTask.setStartDate(151010);
		testTask.setStartTime(0);
		testTask.setEndDate(151010);
		testTask.setEndTime(1600);
		testTask.setType(EVENT);
		Add addTwo(testTask);
		addTwo.execute();

		testTask.setStartDate(151015);
		testTask.setStartTime(2100);
		testTask.setEndDate(151016);
		testTask.setEndTime(2000);
		testTask.setType(EVENT);
		Add addThree(testTask);
		addThree.execute();

		testTask.setStartDate(151015);
		testTask.setStartTime(2300);
		testTask.setEndDate(151015);
		testTask.setEndTime(2350);
		testTask.setType(EVENT);
		Add addFour(testTask);
		addFour.execute();

		testTask.setStartDate(151016);
		testTask.setStartTime(1500);
		testTask.setEndDate(151016);
		testTask.setEndTime(1500);
		testTask.setType(TODO);
		Add addFive(testTask);
		addFive.execute();

		testTask.setStartDate(151017);
		testTask.setStartTime(800);
		testTask.setEndDate(151017);
		testTask.setEndTime(800);
		testTask.setType(TODO);
		Add addSix(testTask);
		addSix.execute();

		// Equivalent to user input "Search from 9 Oct at 11 pm to 19 Oct at 9 am"
		testPwrSearch.setFreePeriods(151009,2300,151019,900);

		std::vector<Task> copyTask = testPwrSearch.getFreePeriods();
		std::vector<Task>::iterator iter;

		// Below are the expected results of the free periods to be found within the period
		iter = copyTask.begin();
		Assert::AreEqual(151009, iter->getStartDate());
		Assert::AreEqual(2300, iter->getStartTime());
		Assert::AreEqual(151009, iter->getEndDate());
		Assert::AreEqual(2359, iter->getEndTime());

		++iter;
		Assert::AreEqual(151011, iter->getStartDate());
		Assert::AreEqual(0, iter->getStartTime());
		Assert::AreEqual(151015, iter->getEndDate());
		Assert::AreEqual(2100, iter->getEndTime());

		++iter;
		Assert::AreEqual(151016, iter->getStartDate());
		Assert::AreEqual(2000, iter->getStartTime());
		Assert::AreEqual(151019, iter->getEndDate());
		Assert::AreEqual(900, iter->getEndTime());
	}

	};
}