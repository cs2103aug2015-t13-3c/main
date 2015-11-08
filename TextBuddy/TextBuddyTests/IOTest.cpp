// Ng Ren Zhi @@author A0130463R

#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TaskSharkTests {
	//========== LoadFileTest ==========
	TEST_CLASS(LoadFileTest) {
public:
	IO* io;
	std::vector<Task> emptyVector;
	std::vector<Task> actualVector;

	TEST_METHOD_INITIALIZE(GetInstanceIO) {
		io = IO::getInstance();
	}

	TEST_METHOD(IO_loadFile_emptyFile) {
		// Empty file should load empty vector
		io->saveFile("TEXT.txt", emptyVector);

		actualVector = io->loadFile("TEXT.txt");
		Assert::AreEqual(emptyVector.size(), actualVector.size());
	}

	TEST_METHOD(IO_loadFile_fileDoesntExist) {
		// Non-existent file should load empty vector
		try {
			actualVector = io->loadFile(""); // Exception thrown if file does not exist
		} catch (std::exception e) {
			Assert::AreEqual("File does not exist",e.what());
			Assert::AreEqual(emptyVector.size(),actualVector.size());
		}
	}

	TEST_METHOD(IO_loadFile_loadGibberish) {
		// Gibberish file name should load empty vector
		try {
			actualVector = io->loadFile("~`!@#$%^&*()_-+="); // Exception thrown if file does not exist
		} catch (std::exception e) {
			Assert::AreEqual("File does not exist",e.what());
			Assert::AreEqual(emptyVector.size(),actualVector.size());
		}
	}

	TEST_METHOD(IO_loadFile_oneTask) {
		std::vector<Task> textVector;
		Task newTask;
		textVector.push_back(newTask);
		io->saveFile("TEXT.txt", textVector);

		actualVector = io->loadFile("TEXT.txt");
		Assert::AreEqual(textVector.size(), actualVector.size());

		for (unsigned int i = 0; i < actualVector.size(); i++) {
			Task task1 = textVector[i];
			Task task2 = actualVector[i];

			Assert::AreEqual(task1.getDoneStatus() , task2.getDoneStatus());
			Assert::AreEqual(task1.getEndDate() , task2.getEndDate());
			Assert::AreEqual(task1.getEndTime() , task2.getEndTime());
			Assert::AreEqual(task1.getID() , task2.getID());
			Assert::AreEqual(task1.getLabelString() , task2.getLabelString());
			Assert::AreEqual(task1.getName() , task2.getName());
			Assert::AreEqual(task1.getPriorityStatus() , task2.getPriorityStatus());
			Assert::AreEqual(task1.getStartDate() , task2.getStartDate());
			Assert::AreEqual(task1.getStartTime() , task2.getStartTime());
			Assert::AreEqual(Utilities::taskTypeToString(task1.getType()) , Utilities::taskTypeToString(task2.getType()));
			/*
			// Assert::AreEqual cannot compare vectors and Tasks
			if (!Task::tasksAreEqual(textVector[i], actualVector[i])) {
			Assert::Fail();
			// Assert::AreEqual(textVector[i], actualVector[i]);
			}
			*/
		}
	}

	TEST_METHOD(IO_loadFile_threeTasks) {
		std::vector<Task> textVector;
		Task newTask;
		textVector.push_back(newTask);
		textVector.push_back(newTask);
		textVector.push_back(newTask);
		io->saveFile("TEXT.txt", textVector);

		std::vector<Task> actualVector = io->loadFile("TEXT.txt");
		Assert::AreEqual(textVector.size(), actualVector.size());

		for (unsigned int i = 0; i < actualVector.size(); i++) {
			Task task1 = textVector[i];
			Task task2 = actualVector[i];

			Assert::AreEqual(task1.getDoneStatus() , task2.getDoneStatus());
			Assert::AreEqual(task1.getEndDate() , task2.getEndDate());
			Assert::AreEqual(task1.getEndTime() , task2.getEndTime());
			Assert::AreEqual(task1.getID() , task2.getID());
			Assert::AreEqual(task1.getLabelString() , task2.getLabelString());
			Assert::AreEqual(task1.getName() , task2.getName());
			Assert::AreEqual(task1.getPriorityStatus() , task2.getPriorityStatus());
			Assert::AreEqual(task1.getStartDate() , task2.getStartDate());
			Assert::AreEqual(task1.getStartTime() , task2.getStartTime());
			Assert::AreEqual(Utilities::taskTypeToString(task1.getType()) , Utilities::taskTypeToString(task2.getType()));
			/*
			// Assert::AreEqual cannot compare vectors and Tasks
			if (!Task::tasksAreEqual(textVector[i], actualVector[i])) {
			Assert::Fail();
			// Assert::AreEqual(textVector[i], actualVector[i]);
			}
			*/
		}
	}

	// Use only if incorrect input
	// TEST_METHOD(IO_loadFile_correctStringReadIn)	{
	/*
	// Test if what is read to rapidJSON is the correct file contents
	std::ifstream inputFile("JSONTEXT.txt");
	std::string hardstring = "\"TaskShark Items\":\n[\n]";
	std::string inputFileText((std::istreambuf_iterator<char>(inputFile)),

	std::istreambuf_iterator<char>());
	Assert::AreEqual(hardstring, inputFileText);
	}
	*/
	};

	//========== SaveFileTest ==========
	TEST_CLASS(SaveFileTest) {
public:
	IO* io;
	TEST_METHOD_INITIALIZE(GetInstanceForIO) {
		io = IO::getInstance();
	}

	TEST_METHOD(IO_saveFile_fileDoesntExist) {
		// Cannot open file to save
		std::vector<Task> emptyVector;
		bool success = io->saveFile("", emptyVector);

		Assert::AreEqual(false, success);
	}

	TEST_METHOD(IO_saveFile_oneLine) {
		std::vector<Task> textVector;
		Task newTask;
		textVector.push_back(newTask);

		bool success = io->saveFile("TEXT.txt", textVector);
		Assert::AreEqual(true,success);
		std::vector<std::string> actualText = io->getText("TEXT.txt");

		std::string expectedText[] = {
			"{",
			"\t\"TaskShark Items\":",
			"\t[",
			"\t\t{",
			"\t\t\t\"name\": \"\",",
			"\t\t\t\"type\": \"FLOATING\",",
			"\t\t\t\"uniqueID\": 0,",
			"\t\t\t\"label\": [],",
			"\t\t\t\"isDone\": false,",
			"\t\t\t\"isPriority\": false,",
			// "\t\t\t\"startDay\": \"SUN\",",
			"\t\t\t\"startDate\": 0,",
			"\t\t\t\"startTime\": -1,",
			// "\t\t\t\"endDay\": \"SUN\",",
			"\t\t\t\"endDate\": 0,",
			"\t\t\t\"endTime\": -1",
			"\t\t}",
			"\t]",
			"}"
		};

		// TODO: Assert::AreEqual for diff vector sizes
		Assert::AreEqual((size_t) 17, actualText.size());

		for (unsigned int i = 0; i < actualText.size(); i++) {
			Assert::AreEqual(expectedText[i], actualText[i]);
		}
	}

	TEST_METHOD(IO_saveFile_threeLines) {
		std::vector<Task> textVector;
		Task newTask;
		newTask.setID(Task::incrementRunningCount()); // Added to fix uniqueID (Aaron)
		textVector.push_back(newTask);
		textVector.push_back(newTask);
		textVector.push_back(newTask);

		bool success = io->saveFile("TEXT.txt", textVector);
		Assert::AreEqual(true,success);
		std::vector<std::string> actualText = io->getText("TEXT.txt");

		std::string expectedText[] = {
			"{",
			"\t\"TaskShark Items\":",
			"\t[",
			"\t\t{",
			"\t\t\t\"name\": \"\",",
			"\t\t\t\"type\": \"FLOATING\",",
			"\t\t\t\"uniqueID\": 1,",
			"\t\t\t\"label\": [],",
			"\t\t\t\"isDone\": false,",
			"\t\t\t\"isPriority\": false,",
			// "\t\t\t\"startDay\": \"SUN\",",
			"\t\t\t\"startDate\": 0,",
			"\t\t\t\"startTime\": -1,",
			// "\t\t\t\"endDay\": \"SUN\",",
			"\t\t\t\"endDate\": 0,",
			"\t\t\t\"endTime\": -1",
			"\t\t}",
			"\t\t,",
			"\t\t{",
			"\t\t\t\"name\": \"\",",
			"\t\t\t\"type\": \"FLOATING\",",
			"\t\t\t\"uniqueID\": 1,",
			"\t\t\t\"label\": [],",
			"\t\t\t\"isDone\": false,",
			"\t\t\t\"isPriority\": false,",
			// "\t\t\t\"startDay\": \"SUN\",",
			"\t\t\t\"startDate\": 0,",
			"\t\t\t\"startTime\": -1,",
			//"\t\t\t\"endDay\": \"SUN\",",
			"\t\t\t\"endDate\": 0,",
			"\t\t\t\"endTime\": -1",
			"\t\t}",
			"\t\t,",
			"\t\t{",
			"\t\t\t\"name\": \"\",",
			"\t\t\t\"type\": \"FLOATING\",",
			"\t\t\t\"uniqueID\": 1,",
			"\t\t\t\"label\": [],",
			"\t\t\t\"isDone\": false,",
			"\t\t\t\"isPriority\": false,",
			// "\t\t\t\"startDay\": \"SUN\",",
			"\t\t\t\"startDate\": 0,",
			"\t\t\t\"startTime\": -1,",
			// "\t\t\t\"endDay\": \"SUN\",",
			"\t\t\t\"endDate\": 0,",
			"\t\t\t\"endTime\": -1",
			"\t\t}",
			"\t]",
			"}"
		};

		for (unsigned int i = 0; i < actualText.size(); i++) {
			Assert::AreEqual(expectedText[i], actualText[i]);
		}
	}

	// TEST_METHOD(IO_saveFile_noText) {
	/*
	io = IO::getInstance();
	std::vector<Task> emptyVector;
	std::string expectedText[] = {"{","\t\"TaskShark Items\":", "\t[","\t]","}"};

	bool success = io->saveFile("TEXT.txt", emptyVector);

	std::vector<std::string> actualText = io->getText("TEXT.txt");

	// std::vector<std::string> actualTextVector = io->getText("TEXT.txt");
	// std::string actualText = Utilities::vecToString(actualTextVector);

	for (unsigned int i = 0; i < actualText.size(); i++) {
	Assert::AreEqual(expectedText[i], actualText[i]);
	}
	}
	*/
	};

	//========== SetFilePathTest ==========
	TEST_CLASS(SetFilePathTest) {
public:
	Parser* parser;
	IO* io;

	TEST_METHOD(IO_setFilePath_parentDirectory) {
		parser = Parser::getInstance();
		io = IO::getInstance();
		std::vector<Task> taskVector;
		Task task = *parser->parseTask("dummy");
		std::string userInput = "../test";
		std::string newFilePath = parser->parseFileName(userInput);
		Assert::AreEqual(true,io->setFilePath(newFilePath,taskVector));

		// Remove file created
		remove(newFilePath.c_str());
	}

	TEST_METHOD(IO_setFilePath_parentOfParentDirectory) {
		parser = Parser::getInstance();
		io = IO::getInstance();
		std::vector<Task> taskVector;
		Task task = *parser->parseTask("dummy");
		std::string userInput = "../../test";
		std::string newFilePath = parser->parseFileName(userInput);
		Assert::AreEqual(true,io->setFilePath(newFilePath,taskVector));

		// Remove file created
		remove(newFilePath.c_str());
	}

	TEST_METHOD(IO_setFilePath_userDirectory) {
		parser = Parser::getInstance();
		io = IO::getInstance();
		std::vector<Task> taskVector;
		Task task = *parser->parseTask("dummy");
		std::string userInput = "C:/Users/Public/TEXT";
		std::string newFilePath = parser->parseFileName(userInput);
		Assert::AreEqual(true,io->setFilePath(newFilePath,taskVector));

		// Remove file created
		remove(newFilePath.c_str());
	}

	};

	//========== ChangeDirectoryTest ==========
	/*
	TEST_CLASS(ChangeDirectoryTest)	{
	public:
	TEST_METHOD(IO_changeDirectory_invalidPath)	{
	io = IO::getInstance();
	// Cannot open file to save
	std::string pathName = "Desktop";
	bool success = io->changeSourceFileLocation(pathName);
	Assert::AreEqual(false, success);
	}
	*/
}