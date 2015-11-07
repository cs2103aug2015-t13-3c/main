// Soon Hao Ye @@author A0126677U

#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TextBuddyTests {
	TEST_CLASS(UnitTest) {
public:

	// Task::getDate_UI should return a string in:
	// DD/MMM format if the date is the present year
	// DD/MMM/YY format if the date is next year onwards
	TEST_METHOD(Task_getDate_getDisplayTime) {
		Task task;

		// Task with 1 single date 
		int startDate = 151120;
		int endDate = 151120;
		int startTime; 
		int endTime;
		std::string expectedDate = "20/Nov";
		std::string expectedTime = "";
		task.setStartDate(startDate);
		task.setEndDate(endDate);
		Assert::AreEqual(expectedDate,task.getDate_UI());
		Assert::AreEqual(expectedTime,task.getDisplayTime());

		// Task with 2 different date
		endDate = 151130;
		expectedDate = "20/Nov - 30/Nov";
		task.setEndDate(endDate);
		Assert::AreEqual(expectedDate,task.getDate_UI());
		Assert::AreEqual(expectedTime,task.getDisplayTime());

		// Task with dates spanning different year
		endDate = 160120;
		expectedDate = "20/Nov - 20/Jan/16";
		task.setEndDate(endDate);
		Assert::AreEqual(expectedDate,task.getDate_UI());
		Assert::AreEqual(expectedTime,task.getDisplayTime());

		// Task with 1 single time
		startDate = DATE_NOT_SET;
		endDate = DATE_NOT_SET;
		startTime = TIME_NOT_SET;
		endTime = 1800;
		task.setStartDate(startDate);
		task.setEndDate(endDate);
		task.setStartTime(startTime);
		task.setEndTime(endTime);
		expectedDate = "";
		expectedTime = "6.00 pm";
		Assert::AreEqual(expectedDate,task.getDate_UI());
		Assert::AreEqual(expectedTime,task.getDisplayTime());

		// Task with 2 different time
		startTime = 1500;
		task.setStartTime(startTime);
		expectedDate = "";
		expectedTime = "3.00 pm - 6.00 pm";
		Assert::AreEqual(expectedDate,task.getDate_UI());
		Assert::AreEqual(expectedTime,task.getDisplayTime());

		// Task with 2 differnt date and time
		startDate = 151120;
		endDate = 151123;
		task.setStartDate(startDate);
		task.setEndDate(endDate);
		expectedDate = "20/Nov - 23/Nov";
		Assert::AreEqual(expectedDate,task.getDate_UI());
		Assert::AreEqual(expectedTime,task.getDisplayTime());
	}

	};
}