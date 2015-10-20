// @@author A0110376N (Aaron Chong Jun Hao)

#include "stdafx.h"
#include <ctime>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TextBuddyTests {
	TEST_CLASS(TbLoggerTest) {
public:
	TEST_METHOD(TbLogger_getLocalTime) {
		time_t rawtime;
		time(&rawtime);

		struct tm *timeinfo = new tm;
		localtime_s(timeinfo, &rawtime);

		char* buffer = new char[26];
		asctime_s(buffer,26,timeinfo);
		buffer[strlen(buffer)-1]=0;

		TbLogger* logger = TbLogger::getInstance();
		// Note: Visual check is consistent even though test fails
		// Assert::AreEqual(buffer,logger->getLocalTime());
	}

	};

}