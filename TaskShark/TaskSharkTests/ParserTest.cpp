// Aaron Chong Jun Hao @@author A0110376N

#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TaskSharkTests {
	TEST_CLASS(ParserTest) {
public:
	// Declare common variables to be used in tests
	TsLogger* logger;
	Parser* p;
	Command* cmd;
	Task task;

	std::string				 userInput;
	std::vector<std::string> inputVec;
	std::string				 expectedString;
	int						 expectedInt;
	static const int		 INVALID_DATE_FORMAT = -1;

	TEST_CLASS_INITIALIZE(StartTestClass) {
		TsLogger::getInstance()->clearLog();
	}

	TEST_METHOD_INITIALIZE(GetInstanceLoggerAndParser) {
		logger = TsLogger::getInstance();
		p = Parser::getInstance();
	}

	TEST_METHOD_CLEANUP(EndTestMethod) {
		TsLogger::getInstance()->log(SYS,"Test ended\n");
	}

	TEST_METHOD(Parser_parseFileName) {
		//===== Normal =====
		userInput = "test.txt";
		Assert::AreEqual(userInput,p->parseFileName(userInput));

		//===== No file extension =====
		expectedString = "test.txt";
		userInput = "test";
		Assert::AreEqual(expectedString,p->parseFileName(userInput));

		//===== Single forward slash =====
		expectedString = "\\\\test.txt";
		userInput = "/test.txt";
		Assert::AreEqual(expectedString,p->parseFileName(userInput));

		//===== Single backslash =====
		// Note: Seen as double here because '\' is a reserved character
		expectedString = "C:\\\\Users\\\\%USERNAME%\\\\Downloads\\\\TEXT.txt";
		userInput = "C:\\Users\\%USERNAME%\\Downloads\\TEXT.txt";
		Assert::AreEqual(expectedString,p->parseFileName(userInput));
	}

	TEST_METHOD(Parser_parse_Add) {
		cmd->clearTaskStore();
		Add* add;
		size_t storeSizeBeforeAdd;

		//===== EVENT with multipleWordsName, endDate and endTime =====
		userInput = "add A partridge in a pear tree on sat by 8 pm";
		cmd = p->parse(userInput);
		add = (Add*)cmd;
		storeSizeBeforeAdd = add->getTaskStore().size();
		add->execute();
		task = add->getTaskStore().back();
		Assert::AreEqual(storeSizeBeforeAdd+1, add->getTaskStore().size());
		Assert::AreEqual(std::string("A partridge in a pear tree"),task.getName());
		Assert::AreEqual(std::string("TODO"),Utilities::taskTypeToString(task.getType()));
		// Assert::AreEqual(std::string(""),task.getLabelString());
		// Assert::AreEqual(false,task.getDoneStatus());
		// Assert::AreEqual(false,task.getPriorityStatus());
		Assert::AreEqual(151114, task.getStartDate());
		Assert::AreEqual(2000, task.getStartTime());
		Assert::AreEqual(151114, task.getEndDate());
		Assert::AreEqual(2000, task.getEndTime());

		//===== EVENT with singleWordName, startDate and endDate =====
		userInput = "Add that from 14 Oct to 16 Oct";
		cmd = p->parse(userInput);
		add = (Add*)cmd;
		storeSizeBeforeAdd = add->getTaskStore().size();
		add->execute();
		task = add->getNewTask();
		Assert::AreEqual(storeSizeBeforeAdd+1, add->getTaskStore().size());
		Assert::AreEqual(std::string("that"),task.getName());
		Assert::AreEqual(std::string("EVENT"),Utilities::taskTypeToString(task.getType()));
		// Assert::AreEqual(std::string(""),task.getLabelString());
		// Assert::AreEqual(false,task.getDoneStatus());
		// Assert::AreEqual(false,task.getPriorityStatus());
		Assert::AreEqual(151014, task.getStartDate());
		// Assert::AreEqual(TIME_NOT_SET, task.getStartTime());
		Assert::AreEqual(151016, task.getEndDate());
		// Assert::AreEqual(TIME_NOT_SET, task.getEndTime());

		//===== EVENT with singleWordName, startTime and endTime =====
		userInput = "Add fishing from 12 am to 11.59 pm";
		cmd = p->parse(userInput);
		add = (Add*)cmd;
		storeSizeBeforeAdd = add->getTaskStore().size();
		add->execute();
		task = add->getNewTask();
		Assert::AreEqual(storeSizeBeforeAdd+1, add->getTaskStore().size());
		Assert::AreEqual(std::string("fishing"),task.getName());
		Assert::AreEqual(std::string("EVENT"),Utilities::taskTypeToString(task.getType()));
		// Assert::AreEqual(std::string(""),task.getLabelString());
		// Assert::AreEqual(false,task.getDoneStatus());
		// Assert::AreEqual(false,task.getPriorityStatus());
		// Assert::AreEqual(0, task.getStartDate());
		Assert::AreEqual(0, task.getStartTime());
		// Assert::AreEqual(0, task.getEndDate());
		Assert::AreEqual(2359, task.getEndTime());
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
			case POWERSEARCH:
			case PICK:
			case MARKDONE:		// Mark task as done
			case UNMARKDONE:	// Mark task as not done
			case UNDO:			// Undo last command if ADD, DELETE or MODIFY or MARKDONE
			case REDO:			// Redo last undo, if any
			case VIEW:			// Process view
			case CLEAR_ALL:		// Clear all tasks
			case DISPLAY_ALL:	// Display all tasks
			case LOAD:			// Load file
			case SAVE:			// Save to new file path
			case EXIT:			// Exit program
			case INVALID:		// Return error message to UI
				break;
			}
	}

	TEST_METHOD(Parser_parse_Delete) {
		expectedInt = 1;
		userInput = "delete 1";
		cmd = p->parse(userInput);
		Delete* deleteCmd = (Delete*)cmd;
		Assert::AreEqual(expectedInt,deleteCmd->getDeleteID());
	}

	TEST_METHOD(Parser_parse_Modify) {
		Task task;
		Add add(task);
		add.execute();
		add.execute();
		add.execute();
		/*
		expectedInt = 1;
		expectedString = "Name: Two turtle doves\nType: 1\nStart Time: 800\nEnd Date: 151024\nEnd Time: 800\n";
		userInput = "modify 1 Two turtle doves : label1 -: unlabel1 star dummy unstar dummy from today to tmr on fri by sat at 8 am";
		*/
		expectedInt = 3;
		expectedString = ": star";
		userInput = "modify 3 : star";

		Command* cmd = p->parse(userInput);
				
		int taskID = 0;
		std::vector<FieldType> fieldsToModify;
		std::string searchPhrase;

		logicProcessCommandStub(cmd,task,taskID,fieldsToModify,searchPhrase);

		Assert::AreEqual(expectedInt,taskID);
		Assert::AreEqual(expectedString,Utilities::fieldVecToString(fieldsToModify));
		// Assert::AreEqual(expectedString2,Utilities::taskToString(task));
	}

	TEST_METHOD(Parser_parse_nonCRUD) {
		//===== Test for SEARCH =====
		expectedString = "answerToLife";
		userInput = "search answerToLife";

		cmd = p->parse(userInput);
		int taskID = 0;
		std::vector<FieldType> fieldsToModify;
		std::string searchPhrase;

		logicProcessCommandStub(cmd,task,taskID,fieldsToModify,searchPhrase);

		//==================================================
		//          TEST-ASSERTING OUTPUT OF PARSE()
		//==================================================
		CommandType cmdType = cmd->getCommand();
		switch(cmdType) {
		case ADD:		// Use TEST_METHOD(Parser_parse_Add)
		case DELETE:	// Use TEST_METHOD(Parser_parse_Delete)
		case MODIFY:	// Use TEST_METHOD(Parser_parse_Modify)
			break;
		case SEARCH:
			Assert::AreEqual(expectedString,searchPhrase);
			break;
		case POWERSEARCH:
		case PICK:
		case MARKDONE:
		case UNMARKDONE:
		case UNDO:
		case REDO:
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

	// Note: As parseByDate() takes in regex like "this Monday",
	//       test cases are only valid for ONE week each time!
	TEST_METHOD(Parser_parseByDate) {
		// Invalid date formats
		expectedInt = INVALID_DATE_FORMAT;

		userInput = "";
		inputVec = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseByDate(inputVec));

		userInput = "invalid";
		inputVec = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseByDate(inputVec));

		userInput = "1 invalid";
		inputVec = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseByDate(inputVec));

		userInput = "7 pm";
		inputVec = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseByDate(inputVec));

		// Valid date formats
		expectedInt = 151231;
		userInput = "31 dec";
		inputVec = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseByDate(inputVec));

		userInput = "31 dec 15";
		inputVec = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseByDate(inputVec));
	}

	TEST_METHOD(Parser_parseByDay) {
		// Invalid date formats
		expectedInt = INVALID_DATE_FORMAT;

		userInput = "7 pm";
		inputVec = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseByDate(inputVec));

		// Change to appropriate date for today/tmr before running this test
		/*
		expectedInt = 151018;
		userInput = "today";
		inputVec = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseByDay(inputVec));

		expectedInt = 151019;
		userInput = "tmr";
		inputVec = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseByDay(inputVec));
		*/

		expectedInt = 151109;
		userInput = "mon";
		inputVec = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseByDay(inputVec));

		expectedInt = 151114;
		userInput = "this sat";
		inputVec = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseByDay(inputVec));

		expectedInt = 151115;
		userInput = "next sun";
		inputVec = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseByDay(inputVec));
	}

	TEST_METHOD(Parser_parseTask) {
		//===== TODO with endDate =====
		expectedString = "Name: Sing a song\nType: TODO\nLabels: \nDone: 0\nPriority: 0\nStart Date: 151231\nStart Time: -1\nEnd Date: 151231\nEnd Time: -1\n";
		userInput = "Sing a song by 31 dec";
		task = *(p->parseTask(userInput));
		Assert::AreEqual(expectedString,Utilities::taskToString(task));
		Assert::AreEqual(std::string("Sing a song"),task.getName());
		Assert::AreEqual(std::string("TODO"),Utilities::taskTypeToString(task.getType()));
		// Assert::AreEqual(std::string(""),task.getLabelString());
		// Assert::AreEqual(false,task.getDoneStatus());
		// Assert::AreEqual(false,task.getPriorityStatus());
		Assert::AreEqual(151231, task.getStartDate());
		Assert::AreEqual(TIME_NOT_SET, task.getStartTime());
		Assert::AreEqual(151231, task.getEndDate());
		Assert::AreEqual(TIME_NOT_SET, task.getEndTime());

		//===== EVENT with startDate and endDate =====
		expectedString = "Name: Sing a song\nType: EVENT\nLabels: \nDone: 0\nPriority: 0\nStart Date: 151231\nStart Time: -1\nEnd Date: 160101\nEnd Time: -1\n";
		userInput = "Sing a song from 31 dec to 1 jan";
		task = *(p->parseTask(userInput));
		Assert::AreEqual(expectedString,Utilities::taskToString(task));
		Assert::AreEqual(std::string("Sing a song"),task.getName());
		Assert::AreEqual(std::string("EVENT"),Utilities::taskTypeToString(task.getType()));
		// Assert::AreEqual(std::string(""),task.getLabelString());
		// Assert::AreEqual(false,task.getDoneStatus());
		// Assert::AreEqual(false,task.getPriorityStatus());
		Assert::AreEqual(151231, task.getStartDate());
		// ::AreEqual(TIME_NOT_SET, task.getStartTime());
		Assert::AreEqual(160101, task.getEndDate());
		// Assert::AreEqual(TIME_NOT_SET, task.getEndTime());

		//===== EVENT with startDate, startTime, endDate, endTime =====
		userInput = "Camp from fri at 6 pm to next sun at 4 pm";
		task = *(p->parseTask(userInput));
		Assert::AreEqual(std::string("Camp"),task.getName());
		Assert::AreEqual(std::string("EVENT"),Utilities::taskTypeToString(task.getType()));
		// Assert::AreEqual(std::string(""),task.getLabelString());
		// Assert::AreEqual(false,task.getDoneStatus());
		// Assert::AreEqual(false,task.getPriorityStatus());
		Assert::AreEqual(151113, task.getStartDate());
		Assert::AreEqual(1800, task.getStartTime());
		Assert::AreEqual(151115, task.getEndDate());
		Assert::AreEqual(1600, task.getEndTime());

		//===== (Modify) FLOATING with emptyAddLabels and setPriority =====
		expectedString = "Name: \nType: FLOATING\nLabels: \nDone: 0\nPriority: 1\nStart Date: 0\nStart Time: -1\nEnd Date: 0\nEnd Time: -1\n";
		userInput = ": star";
		task = *(p->parseTask(userInput));
		Assert::AreEqual(expectedString,Utilities::taskToString(task));
		Assert::AreEqual(std::string(""),task.getName());
		Assert::AreEqual(std::string("FLOATING"),Utilities::taskTypeToString(task.getType()));
		// Assert::AreEqual(std::string(""),task.getLabelString());
		// Assert::AreEqual(false,task.getDoneStatus());
		// Assert::AreEqual(false,task.getPriorityStatus());
		Assert::AreEqual(DATE_NOT_SET, task.getStartDate());
		Assert::AreEqual(TIME_NOT_SET, task.getStartTime());
		Assert::AreEqual(DATE_NOT_SET, task.getEndDate());
		Assert::AreEqual(TIME_NOT_SET, task.getEndTime());
	}

	TEST_METHOD(Parser_parseTime) {
		// Invalid time formats
		expectedInt = -1;

		userInput = "invalid";
		inputVec = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputVec));

		userInput = "0";
		inputVec = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputVec));

		userInput = "0.";
		inputVec = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputVec));

		userInput = ".0";
		inputVec = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputVec));

		userInput = "0.0";
		inputVec = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputVec));

		userInput = "0.1";
		inputVec = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputVec));

		userInput = "2400";
		inputVec = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputVec));

		userInput = "2360";
		inputVec = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputVec));

		userInput = "24.00";
		inputVec = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputVec));

		userInput = "23.60";
		inputVec = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputVec));

		userInput = "23.59 am";
		inputVec = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputVec));

		userInput = "23.59 pm";
		inputVec = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputVec));

		// Valid time formats

		expectedInt = 100;
		userInput = "100";
		inputVec = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputVec));

		expectedInt = 2359;
		userInput = "23.59";
		inputVec = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputVec));

		expectedInt = 0;
		userInput = "12 am";
		inputVec = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputVec));

		expectedInt = 0;
		userInput = "12.00 am";
		inputVec = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputVec));

		expectedInt = 100;
		userInput = "1.00 am";
		inputVec = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputVec));

		expectedInt = 1159;
		userInput = "11.59 am";
		inputVec = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputVec));

		expectedInt = 1200;
		userInput = "12 pm";
		inputVec = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputVec));

		expectedInt = 1200;
		userInput = "12.00 pm";
		inputVec = Utilities::stringToVec(userInput);
		Assert::AreEqual(expectedInt,p->parseTime(inputVec));
	}

	// - Searchphrase
	//		e.g. search lab      from mon at     1 pm to tue at 2 pm
	//		e.g. search lecture  on   wed before 3 pm
	//		e.g. search tutorial on   thu after  4 pm
	// - Freeslot  (keyword: for)
	//		e.g. search                   from   8 am        to 2 pm for 1 h 
	TEST_METHOD(Parser_parseSearchParameters) {
		Command::clearTaskStore();
		int today = p->parseByDay(Utilities::stringToVec("today"));
		int defaultEndDate = p->parseByDate(Utilities::stringToVec("31 dec")) + 10000;
		std::vector<std::string> searchParameters;
		std::vector<std::string>::iterator param;

		std::vector<std::string> holder(8);
		std::vector<std::string>::iterator curr = holder.begin();
		std::vector<std::string>::iterator searchPhrase = curr++;
		std::vector<std::string>::iterator startDate = curr++;
		std::vector<std::string>::iterator startTime = curr++;
		std::vector<std::string>::iterator endDate = curr++;
		std::vector<std::string>::iterator endTime = curr++;
		std::vector<std::string>::iterator daysNeeded = curr++;
		std::vector<std::string>::iterator hrsNeeded = curr++;
		std::vector<std::string>::iterator minsNeeded = curr;

		// Searchphrase PowerSearch with startDate, startTime, endDate,endTime
		// Tested: from, at, to
		userInput = "lab from mon at 1 pm to tue at 2 pm";
		searchParameters = p->parseSearchParameters(userInput);
		for (curr=holder.begin(),param=searchParameters.begin(); curr!=holder.end(); curr++,param++) {
			*curr = *param;
		}
		Assert::AreEqual(std::string("lab"),*searchPhrase);
		Assert::AreEqual(151109,Utilities::stringToInt(*startDate));
		Assert::AreEqual(1300,Utilities::stringToInt(*startTime));
		Assert::AreEqual(151110,Utilities::stringToInt(*endDate));
		Assert::AreEqual(1400,Utilities::stringToInt(*endTime));
		Assert::AreEqual(0,Utilities::stringToInt(*daysNeeded));
		Assert::AreEqual(0,Utilities::stringToInt(*hrsNeeded));
		Assert::AreEqual(0,Utilities::stringToInt(*minsNeeded));

		// Searchphrase PowerSearch with startDate, startTime, endDate,endTime
		// Tested: after, before
		userInput = "lecture on wed after 3 pm before 4 pm";
		searchParameters = p->parseSearchParameters(userInput);
		for (curr=holder.begin(),param=searchParameters.begin(); curr!=holder.end(); curr++,param++) {
			*curr = *param;
		}
		Assert::AreEqual(std::string("lecture"),*searchPhrase);
		Assert::AreEqual(151111,Utilities::stringToInt(*startDate));
		Assert::AreEqual(1500,Utilities::stringToInt(*startTime));
		Assert::AreEqual(151111,Utilities::stringToInt(*endDate));
		Assert::AreEqual(1600,Utilities::stringToInt(*endTime));
		Assert::AreEqual(0,Utilities::stringToInt(*daysNeeded));
		Assert::AreEqual(0,Utilities::stringToInt(*hrsNeeded));
		Assert::AreEqual(0,Utilities::stringToInt(*minsNeeded));

		// Freeslot Powersearch with daysNeeded and minsNeeded, so hrs corrected to 0
		// Tested: for, d, m
		userInput = "for 1 d 1 m";
		searchParameters = p->parseSearchParameters(userInput);
		for (curr=holder.begin(),param=searchParameters.begin(); curr!=holder.end(); curr++,param++) {
			*curr = *param;
		}
		Assert::AreEqual(std::string(""),*searchPhrase);
		Assert::AreEqual(today,Utilities::stringToInt(*startDate));
		Assert::AreEqual(TIME_NOT_SET,Utilities::stringToInt(*startTime));
		Assert::AreEqual(defaultEndDate,Utilities::stringToInt(*endDate));
		Assert::AreEqual(2359,Utilities::stringToInt(*endTime));
		Assert::AreEqual(1,Utilities::stringToInt(*daysNeeded));
		Assert::AreEqual(0,Utilities::stringToInt(*hrsNeeded));
		Assert::AreEqual(1,Utilities::stringToInt(*minsNeeded));

		// Freeslot Powersearch with no parameters
		// Tested: h defaults to 1 hour
		userInput = "for";
		searchParameters = p->parseSearchParameters(userInput);
		for (curr=holder.begin(),param=searchParameters.begin(); curr!=holder.end(); curr++,param++) {
			*curr = *param;
		}
		Assert::AreEqual(std::string(""),*searchPhrase);
		Assert::AreEqual(today,Utilities::stringToInt(*startDate));
		Assert::AreEqual(TIME_NOT_SET,Utilities::stringToInt(*startTime));
		Assert::AreEqual(defaultEndDate,Utilities::stringToInt(*endDate));
		Assert::AreEqual(2359,Utilities::stringToInt(*endTime));
		Assert::AreEqual(0,Utilities::stringToInt(*daysNeeded));
		Assert::AreEqual(1,Utilities::stringToInt(*hrsNeeded));
		Assert::AreEqual(0,Utilities::stringToInt(*minsNeeded));

		// Searchphrase PowerSearch with attempted Freeslot Powersearch
		// Tested: Searchphrase PowerSearch takes precedence over Freeslot Powersearch
		userInput = "lecture for 1 d 1 m";
		searchParameters = p->parseSearchParameters(userInput);
		for (curr=holder.begin(),param=searchParameters.begin(); curr!=holder.end(); curr++,param++) {
			*curr = *param;
		}
		Assert::AreEqual(std::string("lecture"),*searchPhrase);
		Assert::AreEqual(today,Utilities::stringToInt(*startDate));
		Assert::AreEqual(TIME_NOT_SET,Utilities::stringToInt(*startTime));
		Assert::AreEqual(defaultEndDate,Utilities::stringToInt(*endDate));
		Assert::AreEqual(2359,Utilities::stringToInt(*endTime));
		Assert::AreEqual(0,Utilities::stringToInt(*daysNeeded));
		Assert::AreEqual(0,Utilities::stringToInt(*hrsNeeded));
		Assert::AreEqual(0,Utilities::stringToInt(*minsNeeded));
	}

	};
}