// @@author A0110376N (Aaron Chong Jun Hao)

#include "stdafx.h"

const std::string TbLogger::logFileName = ".tblog";

TbLogger* TbLogger::logger = new TbLogger;

TbLogger::TbLogger() {
	logLevel = INFO;
	log(SYS,"Logger initialised");
}

TbLogger::~TbLogger() {
	log(SYS,"Logger closed");
}

char* TbLogger::getLocalTime() {
	time_t rawtime;
	time(&rawtime);

	struct tm *timeinfo = new tm;
	localtime_s(timeinfo, &rawtime);

	char* buffer = new char[26];
	asctime_s(buffer,26,timeinfo);
	buffer[strlen(buffer)-1]=0;
	return buffer;
}

int TbLogger::getDate() {
	time_t rawtime = time(0);			// get current time
	struct tm dayinfo;
	localtime_s(&dayinfo,&rawtime);

	int year =	dayinfo.tm_year - 100;	// tm_year: years since 1990
	int month =	dayinfo.tm_mon + 1;		// tm_mon:  Jan starts at 0
	int day =	dayinfo.tm_mday;
	int date = year*10000 + month*100 + day;
	return date;
}

TbLogger* TbLogger::getInstance() {
	return logger;
}

void TbLogger::setLogLevel(Level level) {
	logLevel = level;
}

void TbLogger::log(Level level, std::string message) {
	if(level >= logLevel) {
		std::ofstream logfile;
		logfile.open(logFileName, std::ofstream::out | std::ofstream::app);
		logfile << getLocalTime() << " " << message << std::endl;
		logfile.close();
	}
}

void TbLogger::clearLog() {
	remove(logFileName.c_str());
	return;
}

void TbLogger::close() {
	delete logger;
	return;
}

// Currently disabled
/*
bool TbLogger::setLogFileName(std::string fileName) {
	std::ofstream newLogFile;
	newLogFile.open(fileName, std::ofstream::out | std::ofstream::app);

	if(newLogFile.is_open()) {
		newLogFile.close();

		log(FATAL,"TbLogger closed");

		logFileName = fileName;
		log(FATAL,"TbLogger initiated");
		return true;
	}
	return false;
}
*/