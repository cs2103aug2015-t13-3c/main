// @@author Aaron Chong Jun Hao

#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TextBuddyTests {
	TEST_CLASS(ParserTest) {
public:
	// Initialise common variables to be used in tests
	Parser p;
	std::string userInput;
	std::vector<std::string> inputString;
	std::string expectedString;
	int expectedInt;
	Task tempTask;

	TEST_METHOD(Parser_parseFileName) {
		userInput = "test.txt";
		Assert::AreEqual(userInput,p.parseFileName(userInput));

		expectedString = "\\\\test.txt";
		userInput = "/test.txt";
		Assert::AreEqual(expectedString,p.parseFileName(userInput));

		expectedString = "test.txt";
		userInput = "test";
		Assert::AreEqual(expectedString,p.parseFileName(userInput));
	}

	TEST_METHOD(Parser_parse) {
		/*
		// Test for ADD
		expectedString = "Name: A partridge in a pear tree\nStart Time: 1900\nEnd Date: 151016\nEnd Time: 2000\n";
		userInput = "add A partridge in a pear tree by fri from 7 pm to 8 pm";
		*/

		/*
		// Test for DELETE
		expectedInt = 1;
		userInput = "delete 1";
		*/

		
		// Test for MODIFY
		expectedInt = 1;
		userInput = "modify 1 Two turtle doves";
		expectedString = "name";
		std::string expectedString2 = "Name: Two turtle doves\n";
		

		/*
		// Test for SEARCH
		expectedString = "answerToLife";
		userInput = "search answerToLife";
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
		case MARKDONE:		// Mark task as done
		case UNDO:			// Undo last command if ADD, DELETE or MODIFY or MARKDONE
		case VIEW:
		case DISPLAY_ALL:	// Display all tasks
		case LOAD:
		case SAVE:			// Save to new file path
		case EXIT:			// Exit program
		case INVALID:		// Return error message to UI
			break;
		}

		// ==================================================
		//          TEST-ASSERTING OUTPUT OF PARSE()
		// ==================================================
		switch(cmdType) {
		case ADD:
			// Assert::AreEqual(expectedString,cmd->getUserInput());
			Assert::AreEqual(expectedString,Utilities::taskToString(task));
			break;
		case DELETE:
			Assert::AreEqual(expectedInt,taskID);
			break;
		case MODIFY:
			Assert::AreEqual(expectedInt,taskID);
			Assert::AreEqual(expectedString,Utilities::fieldVecToString(fieldsToModify));
			Assert::AreEqual(expectedString2,Utilities::taskToString(task));
			break;
		case SEARCH:
			Assert::AreEqual(expectedString,searchPhrase);
			break;
		case MARKDONE:
		case UNDO:
		case VIEW:
		case DISPLAY_ALL:
		case LOAD:
		case SAVE:
		case EXIT:
		case INVALID:
			break;
		}
	}

	// Note: As parseDate() takes in regex like "this Monday",
	//       test cases are only valid for ONE week each time!
	TEST_METHOD(Parser_parseDate) {
		// Invalid date formats
		const int INVALID_DATE_FORMAT = -1;
		expectedInt = INVALID_DATE_FORMAT;

		userInput = "";
		inputString = Utilities::splitParameters(userInput);
		Assert::AreEqual(expectedInt,p.parseDate(inputString));

		userInput = "invalid";
		inputString = Utilities::splitParameters(userInput);
		Assert::AreEqual(expectedInt,p.parseDate(inputString));

		userInput = "1 invalid";
		inputString = Utilities::splitParameters(userInput);
		Assert::AreEqual(expectedInt,p.parseDate(inputString));

		userInput = "7 pm";
		inputString = Utilities::splitParameters(userInput);
		Assert::AreEqual(expectedInt,p.parseDate(inputString));

		// Valid date formats
		expectedInt = 151231;
		userInput = "31 dec";
		inputString = Utilities::splitParameters(userInput);
		Assert::AreEqual(expectedInt,p.parseDate(inputString));

		userInput = "31 dec 15";
		inputString = Utilities::splitParameters(userInput);
		Assert::AreEqual(expectedInt,p.parseDate(inputString));
	}

	TEST_METHOD(Parser_parseDay) {
		// Invalid date formats
		const int INVALID_DATE_FORMAT = -1;
		expectedInt = INVALID_DATE_FORMAT;

		userInput = "7 pm";
		inputString = Utilities::splitParameters(userInput);
		Assert::AreEqual(expectedInt,p.parseDate(inputString));

		// Change to appropriate date for today/tmr before running this test
		/*
		expectedInt = 151010;
		userInput = "today";
		inputString = Utilities::splitParameters(userInput);
		Assert::AreEqual(expectedInt,p.parseDay(inputString));

		expectedInt = 151011;
		userInput = "tmr";
		inputString = Utilities::splitParameters(userInput);
		Assert::AreEqual(expectedInt,p.parseDay(inputString));
		*/

		expectedInt = 151012;
		userInput = "mon";
		inputString = Utilities::splitParameters(userInput);
		Assert::AreEqual(expectedInt,p.parseDay(inputString));

		expectedInt = 151017;
		userInput = "this sat";
		inputString = Utilities::splitParameters(userInput);
		Assert::AreEqual(expectedInt,p.parseDay(inputString));

		expectedInt = 151018;
		userInput = "next sun";
		inputString = Utilities::splitParameters(userInput);
		Assert::AreEqual(expectedInt,p.parseDay(inputString));
	}

	TEST_METHOD(Parser_parseTask) {
		expectedString = "Name: Sing a song\nStart Time: 0\nEnd Date: 151231\nEnd Time: 0\n";
		userInput = "Sing a song by 31 dec";
		tempTask = *(p.parseTask(userInput));
		Assert::AreEqual(expectedString,Utilities::taskToString(tempTask));
	}

	TEST_METHOD(Parser_parseTime) {
		// Invalid time formats
		expectedInt = -1;

		userInput = "invalid";
		inputString = Utilities::splitParameters(userInput);
		Assert::AreEqual(expectedInt,p.parseTime(inputString));

		userInput = "0";
		inputString = Utilities::splitParameters(userInput);
		Assert::AreEqual(expectedInt,p.parseTime(inputString));

		userInput = "0.";
		inputString = Utilities::splitParameters(userInput);
		Assert::AreEqual(expectedInt,p.parseTime(inputString));

		userInput = ".0";
		inputString = Utilities::splitParameters(userInput);
		Assert::AreEqual(expectedInt,p.parseTime(inputString));

		userInput = "0.0";
		inputString = Utilities::splitParameters(userInput);
		Assert::AreEqual(expectedInt,p.parseTime(inputString));

		userInput = "0.1";
		inputString = Utilities::splitParameters(userInput);
		Assert::AreEqual(expectedInt,p.parseTime(inputString));

		userInput = "2400";
		inputString = Utilities::splitParameters(userInput);
		Assert::AreEqual(expectedInt,p.parseTime(inputString));

		userInput = "2360";
		inputString = Utilities::splitParameters(userInput);
		Assert::AreEqual(expectedInt,p.parseTime(inputString));

		userInput = "24.00";
		inputString = Utilities::splitParameters(userInput);
		Assert::AreEqual(expectedInt,p.parseTime(inputString));

		userInput = "23.60";
		inputString = Utilities::splitParameters(userInput);
		Assert::AreEqual(expectedInt,p.parseTime(inputString));

		userInput = "23.59 am";
		inputString = Utilities::splitParameters(userInput);
		Assert::AreEqual(expectedInt,p.parseTime(inputString));

		userInput = "23.59 pm";
		inputString = Utilities::splitParameters(userInput);
		Assert::AreEqual(expectedInt,p.parseTime(inputString));

		// Valid time formats

		expectedInt = 100;
		userInput = "100";
		inputString = Utilities::splitParameters(userInput);
		Assert::AreEqual(expectedInt,p.parseTime(inputString));

		expectedInt = 2359;
		userInput = "23.59";
		inputString = Utilities::splitParameters(userInput);
		Assert::AreEqual(expectedInt,p.parseTime(inputString));

		expectedInt = 0;
		userInput = "12 am";
		inputString = Utilities::splitParameters(userInput);
		Assert::AreEqual(expectedInt,p.parseTime(inputString));

		expectedInt = 0;
		userInput = "12.00 am";
		inputString = Utilities::splitParameters(userInput);
		Assert::AreEqual(expectedInt,p.parseTime(inputString));

		expectedInt = 100;
		userInput = "1.00 am";
		inputString = Utilities::splitParameters(userInput);
		Assert::AreEqual(expectedInt,p.parseTime(inputString));

		expectedInt = 1159;
		userInput = "11.59 am";
		inputString = Utilities::splitParameters(userInput);
		Assert::AreEqual(expectedInt,p.parseTime(inputString));

		expectedInt = 1200;
		userInput = "12 pm";
		inputString = Utilities::splitParameters(userInput);
		Assert::AreEqual(expectedInt,p.parseTime(inputString));

		expectedInt = 1200;
		userInput = "12.00 pm";
		inputString = Utilities::splitParameters(userInput);
		Assert::AreEqual(expectedInt,p.parseTime(inputString));
	}

	};
}