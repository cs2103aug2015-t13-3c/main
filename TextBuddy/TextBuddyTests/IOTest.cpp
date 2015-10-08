// @@author Ng Ren Zhi

#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TextBuddyTests
{
	//============= LoadFileTest ====================
	TEST_CLASS(LoadFileTest)
	{
	public:

		//Use only if incorrect input
		/*
		TEST_METHOD(IO_loadFile_correctStringReadIn)
		{
		//Test if what is read to rapidJSON is the correct file contents
		std::ifstream inputFile("JSONText.txt");
		std::string hardstring = "\"TextBuddy Items\":\n[\n]";

		std::string inputFileText((std::istreambuf_iterator<char>(inputFile)),
		std::istreambuf_iterator<char>());

		Assert::AreEqual(hardstring, inputFileText);
		}
		*/

		TEST_METHOD(IO_loadFile_emptyFile)
		{
			// Empty file should load empty vector
			IO io;

			std::vector<Task> emptyVector;
			io.saveFile("TEXT.txt", emptyVector);
			std::vector<Task> actualVector = io.loadFile("TEXT.txt");

			Assert::AreEqual(emptyVector.size(), actualVector.size());

		}

		TEST_METHOD(IO_loadFile_fileDoesntExist)
		{
			IO io;
			// Empty file should load empty vector
			std::vector<Task> actualVector = io.loadFile("");
			std::vector<Task> emptyVector;

			Assert::AreEqual(emptyVector.size(), actualVector.size());

		}

		TEST_METHOD(IO_loadFile_loadGibberish)
		{
			IO io;
			// Empty file should load empty vector
			std::vector<Task> actualVector = io.loadFile("");
			std::vector<Task> emptyVector;

			Assert::AreEqual(emptyVector.size(), actualVector.size());

		}

		TEST_METHOD(IO_loadFile_oneTask)
		{
			IO io;
			std::vector<Task> textVector;
			Task newTask;
			textVector.push_back(newTask);
			io.saveFile("TEXT.txt", textVector);

			std::vector<Task> actualVector = io.loadFile("TEXT.txt");

			Assert::AreEqual(textVector.size(), actualVector.size());

			for(unsigned int i = 0; i < actualVector.size(); i++) {
				Task task1 = textVector[i];
				Task task2 = actualVector[i];

				Assert::AreEqual(task1.getDoneStatus() , task2.getDoneStatus());
				Assert::AreEqual(Utilities::dayToString(task1.getEndDay()) , Utilities::dayToString(task2.getEndDay()));
				Assert::AreEqual(task1.getEndDate() , task2.getEndDate());
				Assert::AreEqual(task1.getEndTime() , task2.getEndTime());
				Assert::AreEqual(task1.getID() , task2.getID());
				Assert::AreEqual(task1.getLabel() , task2.getLabel());
				Assert::AreEqual(task1.getName() , task2.getName());
				Assert::AreEqual(task1.getPriorityStatus() , task2.getPriorityStatus());
				Assert::AreEqual(task1.getStartDate() , task2.getStartDate());
				Assert::AreEqual(Utilities::dayToString(task1.getStartDay()) , Utilities::dayToString(task2.getStartDay()));
				Assert::AreEqual(task1.getStartTime() , task2.getStartTime());
				Assert::AreEqual(Utilities::taskTypeToString(task1.getType()) , Utilities::taskTypeToString(task2.getType()));
				/*
				//Assert::AreEqual cannot compare vectors and Tasks
				if(!Task::tasksAreEqual(textVector[i], actualVector[i])) {
				Assert::Fail();
				//Assert::AreEqual(textVector[i], actualVector[i]);
				}
				*/
			}
		}

		TEST_METHOD(IO_loadFile_threeTasks)
		{
			IO io;
			std::vector<Task> textVector;
			Task newTask;
			textVector.push_back(newTask);
			textVector.push_back(newTask);
			textVector.push_back(newTask);
			io.saveFile("TEXT.txt", textVector);

			std::vector<Task> actualVector = io.loadFile("TEXT.txt");

			Assert::AreEqual(textVector.size(), actualVector.size());

			for(unsigned int i = 0; i < actualVector.size(); i++) {
				Task task1 = textVector[i];
				Task task2 = actualVector[i];

				Assert::AreEqual(task1.getDoneStatus() , task2.getDoneStatus());
				Assert::AreEqual(Utilities::dayToString(task1.getEndDay()) , Utilities::dayToString(task2.getEndDay()));
				Assert::AreEqual(task1.getEndDate() , task2.getEndDate());
				Assert::AreEqual(task1.getEndTime() , task2.getEndTime());
				Assert::AreEqual(task1.getID() , task2.getID());
				Assert::AreEqual(task1.getLabel() , task2.getLabel());
				Assert::AreEqual(task1.getName() , task2.getName());
				Assert::AreEqual(task1.getPriorityStatus() , task2.getPriorityStatus());
				Assert::AreEqual(task1.getStartDate() , task2.getStartDate());
				Assert::AreEqual(Utilities::dayToString(task1.getStartDay()) , Utilities::dayToString(task2.getStartDay()));
				Assert::AreEqual(task1.getStartTime() , task2.getStartTime());
				Assert::AreEqual(Utilities::taskTypeToString(task1.getType()) , Utilities::taskTypeToString(task2.getType()));
				/*
				//Assert::AreEqual cannot compare vectors and Tasks
				if(!Task::tasksAreEqual(textVector[i], actualVector[i])) {
				Assert::Fail();
				//Assert::AreEqual(textVector[i], actualVector[i]);
				}
				*/
			}
		}
	};

	//============= SaveFileTest ====================

	TEST_CLASS(SaveFileTest)
	{
	public:

		TEST_METHOD(IO_saveFile_fileDoesntExist)
		{
			IO io;
			// Cannot open file to save
			std::vector<Task> emptyVector;
			bool success = io.saveFile("", emptyVector);

			Assert::AreEqual(false, success);
		}

		TEST_METHOD(IO_saveFile_noText)
		{
			IO io;
			std::vector<Task> emptyVector;
			std::string expectedText[] = {"{","\t\"TextBuddy Items\":", "\t[","\t]","}"};

			bool success = io.saveFile("TEXT.txt", emptyVector);

			std::vector<std::string> actualText = io.getText("TEXT.txt");

			// NOTE: vecToString method doesnt work
			// REPLY: Please see UtilitiesTest.cpp (Aaron)

			// std::vector<std::string> actualTextVector = IO::getText("Text.txt");
			// std::string actualText = Utilities::vecToString(actualTextVector);

			for(unsigned int i = 0; i < actualText.size(); i++) {
				Assert::AreEqual(expectedText[i], actualText[i]);
			}
		}

		TEST_METHOD(IO_saveFile_oneLine)
		{
			IO io;
			std::vector<Task> textVector;
			Task newTask;
			textVector.push_back(newTask);

			bool success = io.saveFile("TEXT.txt", textVector);
			std::vector<std::string> actualText = io.getText("TEXT.txt");

			std::string expectedText[] = {
				"{",
				"\t\"TextBuddy Items\":",
				"\t[",
				"\t\t{",
				"\t\t\t\"name\": \"\",",
				"\t\t\t\"type\": \"FLOATING\",",
				"\t\t\t\"uniqueID\": 6,",
				"\t\t\t\"label\": \"\",",
				"\t\t\t\"isDone\": false,",
				"\t\t\t\"isPriority\": false,",
				"\t\t\t\"startDay\": \"SUN\",",
				"\t\t\t\"startDate\": 0,",
				"\t\t\t\"startTime\": 0,",
				"\t\t\t\"endDay\": \"SUN\",",
				"\t\t\t\"endDate\": 0,",
				"\t\t\t\"endTime\": 0",
				"\t\t}",
				"\t]",
				"}"
			};

			// TODO: assert areEqual for diff vector sizes
			Assert::AreEqual((size_t) 19, actualText.size());

			for(unsigned int i = 0; i < actualText.size(); i++) {
				Assert::AreEqual(expectedText[i], actualText[i]);
			}

		}

		TEST_METHOD(IO_saveFile_threeLines)
		{
			IO io;
			std::vector<Task> textVector;
			Task newTask;
			textVector.push_back(newTask);
			textVector.push_back(newTask);
			textVector.push_back(newTask);

			bool success = io.saveFile("Text.txt", textVector);
			std::vector<std::string> actualText = io.getText("Text.txt");

			std::string expectedText[] = {
				"{",
				"\t\"TextBuddy Items\":",
				"\t[",
				"\t\t{",
				"\t\t\t\"name\": \"\",",
				"\t\t\t\"type\": \"FLOATING\",",
				"\t\t\t\"uniqueID\": 7,",
				"\t\t\t\"label\": \"\",",
				"\t\t\t\"isDone\": false,",
				"\t\t\t\"isPriority\": false,",
				"\t\t\t\"startDay\": \"SUN\",",
				"\t\t\t\"startDate\": 0,",
				"\t\t\t\"startTime\": 0,",
				"\t\t\t\"endDay\": \"SUN\",",
				"\t\t\t\"endDate\": 0,",
				"\t\t\t\"endTime\": 0",
				"\t\t}",
				"\t\t,",
				"\t\t{",
				"\t\t\t\"name\": \"\",",
				"\t\t\t\"type\": \"FLOATING\",",
				"\t\t\t\"uniqueID\": 7,",
				"\t\t\t\"label\": \"\",",
				"\t\t\t\"isDone\": false,",
				"\t\t\t\"isPriority\": false,",
				"\t\t\t\"startDay\": \"SUN\",",
				"\t\t\t\"startDate\": 0,",
				"\t\t\t\"startTime\": 0,",
				"\t\t\t\"endDay\": \"SUN\",",
				"\t\t\t\"endDate\": 0,",
				"\t\t\t\"endTime\": 0",
				"\t\t}",
				"\t\t,",
				"\t\t{",
				"\t\t\t\"name\": \"\",",
				"\t\t\t\"type\": \"FLOATING\",",
				"\t\t\t\"uniqueID\": 7,",
				"\t\t\t\"label\": \"\",",
				"\t\t\t\"isDone\": false,",
				"\t\t\t\"isPriority\": false,",
				"\t\t\t\"startDay\": \"SUN\",",
				"\t\t\t\"startDate\": 0,",
				"\t\t\t\"startTime\": 0,",
				"\t\t\t\"endDay\": \"SUN\",",
				"\t\t\t\"endDate\": 0,",
				"\t\t\t\"endTime\": 0",
				"\t\t}",
				"\t]",
				"}"
			};

			for(unsigned int i = 0; i < actualText.size(); i++) {
				Assert::AreEqual(expectedText[i], actualText[i]);
			}

		}
	};

}