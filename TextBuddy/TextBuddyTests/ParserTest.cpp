// @@author A0110376N (Aaron Chong Jun Hao)

#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TextBuddyTests {
	TEST_CLASS(ParserTest) {
public:
	// Initialise common variables to be used in tests
	Parser* p;
	std::string userInput;
	std::vector<std::string> inputString;
	std::string expectedString;
	int expectedInt;
	Task tempTask;

	TEST_METHOD(Parser_parseFileName) {
		userInput = "test.txt";
		Assert::AreEqual(userInput,p->parseFileName(userInput));

		expectedString = "\\\\test.txt";
		userInput = "/test.txt";
		Assert::AreEqual(expectedString,p->parseFileName(userInput));

		expectedString = "test.txt";
		userInput = "test";
		Assert::AreEqual(expectedString,p->parseFileName(userInput));

		// Added by Ren Zhi 19/10/15
		expectedString = "C:\\\\Users\\\\Public\\\\test.txt";
		userInput = "C:\\Users\\Public\\test.txt";
		Assert::AreEqual(expectedString,p->parseFileName(userInput));
	}

	// Added by Ren Zhi 19/10/15
	TEST_METHOD(Parser_parse_Add) {
		Add *add;
		Command *cmd;
		cmd = p->parse("Add that from 14 Oct to 16 Oct");
		add = (Add*)cmd;

		// Assert::AreEqual(ADD,cmd->getCommand());
		
		Task task = add->getNewTask();
		// task.setStartDate(141015);
		// task.setEndDate(161015);
		// task.setName("that");
		// task.setID(task.getRunningCount());
		Assert::AreEqual(151014, task.getStartDate());
		Assert::AreEqual(151016, task.getEndDate());
		Assert::AreEqual(std::string("that"),task.getName());

		add->clearTaskStore();
		add->execute();
		std::vector<Task> store = add->getTaskStore();
		Assert::AreEqual((size_t)1, store.size());
	}

	TEST_METHOD(Parser_parse) {
		// Clear before start
		TbLogger::getInstance()->clear();
		std::string expectedString2;
		
		// Test for ADD
		expectedString = "Name: A partridge in a pear tree\nType: 2\nStart Time: 0\nEnd Date: 151024\nEnd Time: 2000\n";
		userInput = "add A partridge in a pear tree on sat by 8 pm";
		

		/*
		// Test for DELETE
		expectedInt = 1;
		userInput = "delete 1";
		*/

		/*
		// Test for MODIFY
		expectedInt = 1;
		userInput = "modify 1 Two turtle doves : label1 -: unlabel1 star dummy unstar dummy from today to tmr on fri by sat at 8 am";
		expectedString = "name : -: star unstar from to from to at"; // Expected behaviour
		expectedString2 = "Name: Two turtle doves\nType: 1\nStart Time: 800\nEnd Date: 151024\nEnd Time: 800\n";
		*/

		/*
		// Test for SEARCH
		expectedString = "answerToLife";
		userInput = "search answerToLife";
		*/

		//==================================================
		//         IMPLEMENTING PARSE() TO BE TESTED
		//==================================================
		Command* cmd = p->parse(userInput);

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

		//==================================================
		//          TEST-ASSERTING OUTPUT OF PARSE()
		//==================================================
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
		case CLEAR_ALL:
		case DISPLAY_ALL:
		case LOAD:
		case SAVE:
		case EXIT:
		case INVALID:
			break;
		}

		// Clear if successful
		TbLogger::getInstance()->clear();
	}

	// Note: As parseDate() takes in regex like "this Monday",
	//       test cases are only valid for ONE week each time!
	TEST_METHOD(Parser_parseDate) {
		// Invalid date formats
		const int INVALID_DATE_FORMAT = -1;
		expectedInt = INVALID_DATE_FORMAT;

		userInput = "";
		inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseDate(inputString));

		userInput = "invalid";
		inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseDate(inputString));

		userInput = "1 invalid";
		inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseDate(inputString));

		userInput = "7 pm";
		inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseDate(inputString));

		// Valid date formats
		expectedInt = 151231;
		userInput = "31 dec";
		inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseDate(inputString));

		userInput = "31 dec 15";
		inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseDate(inputString));
	}

	TEST_METHOD(Parser_parseDay) {
		// Invalid date formats
		const int INVALID_DATE_FORMAT = -1;
		expectedInt = INVALID_DATE_FORMAT;

		userInput = "7 pm";
		inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseDate(inputString));

		// Change to appropriate date for today/tmr before running this test
		/*
		expectedInt = 151018;
		userInput = "today";
		inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseDay(inputString));

		expectedInt = 151019;
		userInput = "tmr";
		inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseDay(inputString));
		*/

		expectedInt = 151019;
		userInput = "mon";
		inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseDay(inputString));

		expectedInt = 151024;
		userInput = "this sat";
		inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseDay(inputString));

		expectedInt = 151025;
		userInput = "next sun";
		inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseDay(inputString));
	}

	TEST_METHOD(Parser_parseTask) {
		expectedString = "Name: Sing a song\nType: 2\nStart Time: 0\nEnd Date: 151231\nEnd Time: 0\n";
		userInput = "Sing a song by 31 dec";
		tempTask = *(p->parseTask(userInput));
		Assert::AreEqual(expectedString,Utilities::taskToString(tempTask));
	}

	TEST_METHOD(Parser_parseTime) {
		// Invalid time formats
		expectedInt = -1;

		userInput = "invalid";
		inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputString));

		userInput = "0";
		inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputString));

		userInput = "0.";
		inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputString));

		userInput = ".0";
		inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputString));

		userInput = "0.0";
		inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputString));

		userInput = "0.1";
		inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputString));

		userInput = "2400";
		inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputString));

		userInput = "2360";
		inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputString));

		userInput = "24.00";
		inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputString));

		userInput = "23.60";
		inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputString));

		userInput = "23.59 am";
		inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputString));

		userInput = "23.59 pm";
		inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputString));

		// Valid time formats

		expectedInt = 100;
		userInput = "100";
		inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputString));

		expectedInt = 2359;
		userInput = "23.59";
		inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputString));

		expectedInt = 0;
		userInput = "12 am";
		inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputString));

		expectedInt = 0;
		userInput = "12.00 am";
		inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputString));

		expectedInt = 100;
		userInput = "1.00 am";
		inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputString));

		expectedInt = 1159;
		userInput = "11.59 am";
		inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputString));

		expectedInt = 1200;
		userInput = "12 pm";
		inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputString));

		expectedInt = 1200;
		userInput = "12.00 pm";
		inputString = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputString));
	}

	};
}