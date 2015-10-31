// Chin Kiat Boon @@author A0096720A 

#include "stdafx.h"
#include "PowerSearch.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TextBuddyTests {
	TEST_CLASS(PowerSearchTest) {
public:
	TEST_METHOD(PowerSearch_setTasksWithinPeriod) {
		std::vector<std::string> paraVec; 
		paraVec.push_back("");
		paraVec.push_back("151010");
		paraVec.push_back("1015");
		paraVec.push_back("151212");
		paraVec.push_back("2330");
		paraVec.push_back("0");
		paraVec.push_back("1");
		paraVec.push_back("0");

		PowerSearch testPwrSearch(paraVec);
		testPwrSearch.execute();
		Task testTask;
		
		testTask.setStartDate(151010);
		testTask.setStartTime(0);
		testTask.setEndDate(151212);
		testTask.setEndTime(0);
		Add addOne(testTask);
		addOne.execute();
		testTask.setStartDate(151010);
		testTask.setStartTime(0);
		testTask.setEndDate(151212);
		testTask.setEndTime(0);
		Add addTwo(testTask);
		addTwo.execute();
		testTask.setStartDate(151015);
		testTask.setStartTime(2100);
		testTask.setEndDate(151212);
		testTask.setEndTime(2000);				
		Add addThree(testTask);
		addThree.execute();
		testTask.setStartDate(151015);
		testTask.setStartTime(2300);
		testTask.setEndDate(151212);
		testTask.setEndTime(2300);
		Add addFour(testTask);
		addFour.execute();
		testTask.setStartDate(151016);
		testTask.setStartTime(1500);
		testTask.setEndDate(151212);
		testTask.setEndTime(1500);
		Add addFive(testTask);
		addFive.execute();
		testTask.setStartDate(151017);
		testTask.setStartTime(800);
		testTask.setEndDate(151212);
		testTask.setEndTime(800);
		Add addSix(testTask);
		addSix.execute();
		
		testPwrSearch.setTasksWithinPeriod(151015,2200,151016,1500);

		std::vector<Task> copyTask = testPwrSearch.getTasksWithinPeriod();
		std::vector<Task>::iterator iter;

		iter = copyTask.begin();
		Assert::AreEqual(iter->getStartDate(), 151015);
		Assert::AreEqual(iter->getStartTime(), 2300);

		++iter;
		Assert::AreEqual(iter->getStartDate(), 151016);
	}
	
	TEST_METHOD(PowerSearch_searchFreeSlot) {
		std::vector<std::string> paraVec; 
		paraVec.push_back("");
		paraVec.push_back("151010");
		paraVec.push_back("1015");
		paraVec.push_back("151212");
		paraVec.push_back("2330");
		paraVec.push_back("0");
		paraVec.push_back("1");
		paraVec.push_back("0");

		PowerSearch testPwrSearch(paraVec);
		//Create method to "fit in" an event that may or may not exceed the range of free period
		//Or can set parameter x such that free period mentioned must exceed x
		Task testTask;
		testTask.setStartDate(151010);
		testTask.setStartTime(0);
		testTask.setEndDate(151010);					//is endDate set to be the same as startDate if no range?
		testTask.setEndTime(1500);						//note: throw exception if endtime < starttime
		Add addOne(testTask);
		addOne.clearTaskStore();
		addOne.execute();

		testTask.setStartDate(151010);
		testTask.setStartTime(0);
		testTask.setEndDate(151010);
		testTask.setEndTime(1600);
		Add addTwo(testTask);
		addTwo.execute();

		testTask.setStartDate(151015);
		testTask.setStartTime(2100);
		testTask.setEndDate(151016);
		testTask.setEndTime(2000);
		Add addThree(testTask);
		addThree.execute();
		
		testTask.setStartDate(151015);
		testTask.setStartTime(2300);
		testTask.setEndDate(151015);
		testTask.setEndTime(2350);
		Add addFour(testTask);
		addFour.execute();
		testTask.setStartDate(151016);
		testTask.setStartTime(1500);
		testTask.setEndDate(151016);
		testTask.setEndTime(1500);
		Add addFive(testTask);
		addFive.execute();

		testTask.setStartDate(151017);
		testTask.setStartTime(800);
		testTask.setEndDate(151017);
		testTask.setEndTime(800);
		Add addSix(testTask);
		addSix.execute();

		testPwrSearch.setFreePeriods(151009,2300,151017,900);

		std::vector<Task> copyTask = testPwrSearch.getFreePeriods();
		std::vector<Task>::iterator iter;

		iter = copyTask.begin();
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