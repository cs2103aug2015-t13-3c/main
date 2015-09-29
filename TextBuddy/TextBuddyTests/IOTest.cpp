#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TextBuddyTests
{
	//============= LoadFileTest ====================
	TEST_CLASS(LoadFileTest)
	{
	public:
		
		/*TEST_METHOD(IO_getStatus)
		{
			IO io(ON);

			bool status = io.getStatus();

			Assert::AreEqual(true,status);
		}*/

		TEST_METHOD(IO_loadFile_emptyFile)
		{
			// Empty file should load empty vector
			std::vector<Task> actualVector = IO::loadFile("TEXT.txt");
			std::vector<Task> emptyVector;

			Assert::AreEqual(emptyVector.size(), actualVector.size());
		}
		
		TEST_METHOD(IO_loadFile_fileDoesntExist)
		{
			// Empty file should load empty vector
			std::vector<Task> actualVector = IO::loadFile("");
			std::vector<Task> emptyVector;

			Assert::AreEqual(emptyVector.size(), actualVector.size());
		}

		TEST_METHOD(IO_loadFile_loadGibberish)
		{
			// Empty file should load empty vector
			std::vector<Task> actualVector = IO::loadFile("");
			std::vector<Task> emptyVector;

			Assert::AreEqual(emptyVector.size(), actualVector.size());
		}
	};

	//============= SaveFileTest ====================

	TEST_CLASS(SaveFileTest)
	{
	public:

		TEST_METHOD(IO_saveFile_fileDoesntExist)
		{
			// Cannot open file to save
			std::vector<Task> emptyVector;
			bool success = IO::saveFile("", emptyVector);

			Assert::AreEqual(false, success);
		}

		TEST_METHOD(IO_saveFile_noText)
		{
			std::vector<Task> emptyVector;
			bool success = IO::saveFile("Text.txt", emptyVector);
			std::vector<std::string> actualText = IO::getText("Text.txt");

			Assert::AreEqual(emptyVector.size(), actualText.size());
		}
	};
}