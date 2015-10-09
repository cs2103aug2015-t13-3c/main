// @@author Aaron Chong Jun Hao

#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TextBuddyTests
{
	TEST_CLASS(ParserTest)
	{
	public:
		// Initialise common variables to be used in tests
		Utilities u;
		Parser p;

		std::string userInput;
		std::vector<std::string> inputString;
		std::string expectedString;
		int expectedInt;
		Task tempTask;

		TEST_METHOD(Parser_parse) {

			// Test for ADD
		//	expectedString = "Name: A partridge in a pear tree\nEnd Date: 151009\n";
		//	userInput = "add A partridge in a pear tree by fri";
			expectedString = "";
			userInput = "add ";
			/*
			// Test for DELETE
			expectedInt = 1;
			userInput = "delete 1";
			*/

			/*
			// Test for MODIFY
			expectedInt = 1;
			userInput = "modify 1 name Two turtle doves";
			expectedString = "Name: Two turtle doves\n";
			*/

			/*
			// Test for SEARCH
			expectedString = "answerToLife";
			userInput = "search answerToLife";
			*/

			/*
			// Test for CLEAR_ALL
			userInput = "clear";
			*/

			/*
			// Test for DISPLAY_ALL
			userInput = "display";
			*/

			/*
			// Test for SORT_ALL
			userInput = "sort";
			*/

			/*
			// Test for EXIT
			userInput = "exit";
			*/

			/*
			// Test for INVALID
			userInput = "invalid";
			*/

			// ==================================================
			//         IMPLEMENTING PARSE() TO BE TESTED
			// ==================================================
			Command* cmd = p.parse(userInput);

			// For switch()
			CommandType cmdType = cmd->getCommand();
			// Declare objects outside switch()
			Task task;
			int taskID;
			std::vector<FieldType> fieldsToModify;
			std::string searchPhrase;

			// This shows a possible implementation for Logic::processCommand()
			switch(cmdType) {
			case ADD:
				task = ((Add*)cmd)->getNewTask();
				break;
			case DELETE:
				taskID = ((Delete*)cmd)->getDeleteID();
				break;
			case MODIFY:
				taskID = ((Modify*)cmd)->getModifyID();
				fieldsToModify = ((Modify*)cmd)->getFieldsToModify();
				task = ((Modify*)cmd)->getTempTask();
				break;
			case SEARCH:
				searchPhrase = ((Search*)cmd)->getSearchPhrase();
				break;
			case CLEAR_ALL:
				// Clear tasks
				break;
			case DISPLAY_ALL:
				// Display tasks
				break;
			case SORT_ALL:
				// Sort tasks
				break;
			case EXIT:
				// Exit program
				break;
			case INVALID:
				// Return error message to UI
				break;
			}

			// ==================================================
			//          TEST-ASSERTING OUTPUT OF PARSE()
			// ==================================================
			switch(cmdType) {
			case ADD:
				// Assert::AreEqual(expectedString,cmd->getUserInput());
				Assert::AreEqual(expectedString,u.taskToString(task));
				break;
			case DELETE:
				Assert::AreEqual(expectedInt,taskID);
				break;
			case MODIFY:
				Assert::AreEqual(expectedInt,taskID);
				// Assert::AreEqual(expectedString,fieldVecToString(fieldsToModify);
				Assert::AreEqual(expectedString,u.taskToString(task));
				break;
			case SEARCH:
				Assert::AreEqual(expectedString,searchPhrase);
				break;

				// Methods with nothing to test
			case CLEAR_ALL:
			case DISPLAY_ALL:
			case SORT_ALL:
			case EXIT:
			case INVALID:
				break;
			}
		}

		// Note: As parseDate() takes in regex like "this Monday",
		//       test cases are only valid for ONE week each time!
		TEST_METHOD(Parser_parseDate)
		{
			// Invalid date formats
			const int INVALID_DATE_FORMAT = -1;
			expectedInt = INVALID_DATE_FORMAT;

			userInput = "";
			inputString = u.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseDate(inputString));

			userInput = "invalid";
			inputString = u.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseDate(inputString));

			userInput = "1 invalid";
			inputString = u.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseDate(inputString));
			
			// Valid date formats
			expectedInt = 151231;
			userInput = "31 dec";
			inputString = u.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseDate(inputString));

			userInput = "31 dec 15";
			inputString = u.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseDate(inputString));

		}

		TEST_METHOD(Parser_parseDay)
		{
			// Change to appropriate date for tomorrow before running this test
			/*
			expectedInt = 151009;
			userInput = "tmr";
			inputString = u.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseDay(inputString));
			*/

			expectedInt = 151010;
			userInput = "sat";
			inputString = u.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseDay(inputString));

			expectedInt = 151005;
			userInput = "this mon";
			inputString = u.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseDay(inputString));

			expectedInt = 151011;
			userInput = "next sun";
			inputString = u.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseDay(inputString));
		}

		TEST_METHOD(Parser_parseTask)
		{
			expectedString = "Name: Sing a song\nEnd Date: 151231\n";
			userInput = "Sing a song by 31 dec";
			tempTask = *(p.parseTask(userInput));
			Assert::AreEqual(expectedString,u.taskToString(tempTask));
		}

		TEST_METHOD(Parser_parseTime) {

			// Invalid time formats
			expectedInt = -1;

			userInput = "invalid";
			inputString = u.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseTime(inputString));

			userInput = "0";
			inputString = u.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseTime(inputString));

			userInput = "0.";
			inputString = u.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseTime(inputString));

			userInput = ".0";
			inputString = u.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseTime(inputString));

			userInput = "0.0";
			inputString = u.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseTime(inputString));

			userInput = "0.1";
			inputString = u.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseTime(inputString));

			userInput = "2400";
			inputString = u.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseTime(inputString));

			userInput = "2360";
			inputString = u.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseTime(inputString));

			userInput = "24.00";
			inputString = u.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseTime(inputString));

			userInput = "23.60";
			inputString = u.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseTime(inputString));

			userInput = "23.59 am";
			inputString = u.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseTime(inputString));

			userInput = "23.59 pm";
			inputString = u.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseTime(inputString));

			// Valid time formats

			expectedInt = 100;
			userInput = "100";
			inputString = u.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseTime(inputString));

			expectedInt = 2359;
			userInput = "23.59";
			inputString = u.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseTime(inputString));

			expectedInt = 0;
			userInput = "12 am";
			inputString = u.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseTime(inputString));

			expectedInt = 0;
			userInput = "12.00 am";
			inputString = u.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseTime(inputString));

			expectedInt = 100;
			userInput = "1.00 am";
			inputString = u.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseTime(inputString));

			expectedInt = 1159;
			userInput = "11.59 am";
			inputString = u.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseTime(inputString));

			expectedInt = 1200;
			userInput = "12 pm";
			inputString = u.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseTime(inputString));

			expectedInt = 1200;
			userInput = "12.00 pm";
			inputString = u.splitParameters(userInput);
			Assert::AreEqual(expectedInt,p.parseTime(inputString));
		}

	};
}