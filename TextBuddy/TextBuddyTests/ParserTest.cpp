// Aaron Chong Jun Hao @@author A0110376N

#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TextBuddyTests {
	TEST_CLASS(ParserTest) {
public:
	// Declare common variables to be used in tests
	TbLogger* logger;
	Parser* p;
	std::string userInput;
	std::vector<std::string> inputString;
	std::string expectedString;
	int expectedInt;
	Task tempTask;

	TEST_CLASS_INITIALIZE(StartTestClass) {
		TbLogger::getInstance()->clearLog();
	}

	TEST_METHOD_INITIALIZE(InitialiseLoggerAndParser) {
		logger = TbLogger::getInstance();
		p = Parser::getInstance();
	}

	TEST_METHOD_CLEANUP(EndTestMethod) {
		TbLogger::getInstance()->log(SYS,"Test ended\n");
	}

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
		expectedString = "C:\\\\Users\\\\%USERNAME%\\\\Downloads\\TEXT.txt";
		userInput = "C:\\Users\\%USERNAME%\\Downloads\\TEXT.txt";
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

	void logicProcessCommandStub(Command* cmd,
		Task &task,
		int &taskID,
		std::vector<FieldType> &fieldsToModify,
		std::string &searchPhrase) {
			CommandType cmdType = cmd->getCommand();

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
				logger->log(DEBUG,"Fields to modify: " + Utilities::fieldVecToString(fieldsToModify));
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
	}

	TEST_METHOD(Parser_parse_Modify) {
		std::string expectedString2;
		/*
		expectedInt = 1;
		expectedString = "name : -: star unstar from to from to at"; // Expected behaviour
		expectedString2 = "Name: Two turtle doves\nType: 1\nStart Time: 800\nEnd Date: 151024\nEnd Time: 800\n";
		userInput = "modify 1 Two turtle doves : label1 -: unlabel1 star dummy unstar dummy from today to tmr on fri by sat at 8 am";
		*/
		expectedInt = 3;
		expectedString = ": star";
		userInput = "modify 3 : star";

		Command* cmd = p->parse(userInput);

		Task task;
		int taskID = 0;
		std::vector<FieldType> fieldsToModify;
		std::string searchPhrase;

		logicProcessCommandStub(cmd,task,taskID,fieldsToModify,searchPhrase);

		Assert::AreEqual(expectedInt,taskID);
		Assert::AreEqual(expectedString,Utilities::fieldVecToString(fieldsToModify));
		// Assert::AreEqual(expectedString2,Utilities::taskToString(task));
	}

	TEST_METHOD(Parser_parse) {
		
		// Test for ADD
		expectedString = "Name: A partridge in a pear tree\nType: TODO\nLabels: \nDone: 0\nPriority: 0\nStart Date: 151024\nStart Time: 0\nEnd Date: 151024\nEnd Time: 2000\n";
		userInput = "add A partridge in a pear tree on sat by 8 pm";
		

		/*
		// Test for DELETE
		expectedInt = 1;
		userInput = "delete 1";
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
		Task task;
		int taskID = 0;
		std::vector<FieldType> fieldsToModify;
		std::string searchPhrase;

		logicProcessCommandStub(cmd,task,taskID,fieldsToModify,searchPhrase);

		//==================================================
		//          TEST-ASSERTING OUTPUT OF PARSE()
		//==================================================
		CommandType cmdType = cmd->getCommand();
		switch(cmdType) {
		case ADD:
			Assert::AreEqual(expectedString,Utilities::taskToString(task));
			break;
		case DELETE:
			Assert::AreEqual(expectedInt,taskID);
			break;
		case MODIFY: // Use separate TEST_METHOD(Parser_parse_Modify)
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
		expectedString = "Name: Sing a song\nType: TODO\nLabels: \nDone: 0\nPriority: 0\nStart Date: 151231\nStart Time: 0\nEnd Date: 151231\nEnd Time: 0\n";
		userInput = "Sing a song by 31 dec";
		tempTask = *(p->parseTask(userInput));
		Assert::AreEqual(expectedString,Utilities::taskToString(tempTask));

		expectedString = "Name: Sing a song\nType: EVENT\nLabels: \nDone: 0\nPriority: 0\nStart Date: 151231\nStart Time: 0\nEnd Date: 160101\nEnd Time: 0\n";
		userInput = "Sing a song from 31 dec to 1 jan";
		tempTask = *(p->parseTask(userInput));
		Assert::AreEqual(expectedString,Utilities::taskToString(tempTask));

		expectedString = "Name: \nType: FLOATING\nLabels: \nDone: 0\nPriority: 1\nStart Date: 0\nStart Time: 0\nEnd Date: 0\nEnd Time: 0\n";
		userInput = ": star";
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