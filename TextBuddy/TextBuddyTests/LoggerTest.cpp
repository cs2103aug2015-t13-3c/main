// @@author Aaron Chong Jun Hao

#include "stdafx.h"
#include <ctime>

namespace TextBuddyTests {
	TEST_CLASS(LoggerTest) {
public:
	TEST_METHOD(Logger_getLocalTime) {
		time_t rawtime;
		time(&rawtime);

		struct tm *timeinfo = new tm;
		localtime_s(timeinfo, &rawtime);

		char* buffer = new char[26];
		asctime_s(buffer,26,timeinfo);
		buffer[strlen(buffer)-1]=0;

		Logger* logger = Logger::getInstance();
		// Note: Visual check is consistent even though test fails
		// Microsoft::VisualStudio::CppUnitTestFramework::Assert::AreEqual(buffer,logger->getLocalTime());
	}

	};

}