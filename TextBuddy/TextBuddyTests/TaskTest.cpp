#include "stdafx.h"
#include "CppUnitTest.h"
#include "Task.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TextBuddyTests {
	TEST_CLASS(UnitTest) {
	public:
		
		// Task::getDate should return a string in :
		// DD/MM format if the date is the present year
		// DD/MM/YY format if the date is next year onwards
		TEST_METHOD(Task_getDate_getTime_UI) {
			Task task;

			// Task with 1 single date 
			int startDate = 151120;
			int endDate = 151120;
			int startTime; 
			int endTime;
			std::string expectedDate = "20/11";
			std::string expectedTime = "";
			task.setStartDate(startDate);
			task.setEndDate(endDate);
			Assert::AreEqual(expectedDate,task.getDate_UI());
			Assert::AreEqual(expectedTime,task.getTime_UI());

			// Task with 2 different date
			endDate = 151130;
			expectedDate = "20/11 - 30/11";
			task.setEndDate(endDate);
			Assert::AreEqual(expectedDate,task.getDate_UI());
			Assert::AreEqual(expectedTime,task.getTime_UI());

			// Task with dates spanning different year
			endDate = 160120;
			expectedDate = "20/11 - 20/1/16";
			task.setEndDate(endDate);
			Assert::AreEqual(expectedDate,task.getDate_UI());
			Assert::AreEqual(expectedTime,task.getTime_UI());

			// Task with 1 single time
			startDate = 0;
			endDate = 0;
			startTime = -1;
			endTime = 1800;
			task.setStartDate(startDate);
			task.setEndDate(endDate);
			task.setStartTime(startTime);
			task.setEndTime(endTime);
			expectedDate = "";
			expectedTime = "6.00 pm";
			Assert::AreEqual(expectedDate,task.getDate_UI());
			Assert::AreEqual(expectedTime,task.getTime_UI());

			// Task with 2 different time
			startTime = 1500;
			task.setStartTime(startTime);
			expectedDate = "";
			expectedTime = "3.00 pm - 6.00 pm";
			Assert::AreEqual(expectedDate,task.getDate_UI());
			Assert::AreEqual(expectedTime,task.getTime_UI());

			// Task with 2 differnt date and time
			startDate = 151120;
			endDate = 151123;
			task.setStartDate(startDate);
			task.setEndDate(endDate);
			expectedDate = "20/11 - 23/11";
			Assert::AreEqual(expectedDate,task.getDate_UI());
			Assert::AreEqual(expectedTime,task.getTime_UI());
		}

	};
}