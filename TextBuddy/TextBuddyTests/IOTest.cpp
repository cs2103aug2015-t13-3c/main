#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TextBuddyTests
{
	TEST_CLASS(IOTest)
	{
	public:
		
		TEST_METHOD(IO_getStatus)
		{
			IO io(ON);

			bool status = io.getStatus();

			Assert::AreEqual(true,status);
		}

		TEST_METHOD(IO_loadFile_emptyFile)
		{
			// Empty file should load empty vector
			vector<string> actualText = IO::loadFile("");
			vector<string> emptyVector;

			Assert::AreEqual(emptyVector.size(), actualText.size());
		}
	};
}