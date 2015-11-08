// Aaron Chong Jun Hao @@author A0110376N

#include "stdafx.h"

const std::string TsLogger::logFileName = ".tslog";

TsLogger* TsLogger::theOne = new TsLogger();

TsLogger::TsLogger() {
	logLevel = INFO;
	log(SYS,"Logger initialised");
	log(SYS,"Log level default: " + std::to_string(logLevel));
}

TsLogger::~TsLogger() {
	log(SYS,"Logger terminated\n");
}

char* TsLogger::getLocalTime() {
	time_t rawtime;
	time(&rawtime);

	struct tm *timeinfo = new tm;
	localtime_s(timeinfo, &rawtime);

	char* buffer = new char[26];
	asctime_s(buffer,26,timeinfo);
	buffer[strlen(buffer)-1]=0;
	return buffer;
}

int TsLogger::getDate() {
	time_t rawtime = time(nullptr);		// Get current time
	struct tm dayinfo;
	localtime_s(&dayinfo,&rawtime);

	int year =	dayinfo.tm_year - 100;	// tm_year: years since 1990
	int month =	dayinfo.tm_mon + 1;		// tm_mon:  Jan starts at 0
	int day =	dayinfo.tm_mday;
	int date = year*10000 + month*100 + day;
	return date;
}

TsLogger* TsLogger::getInstance() {
	return theOne;
}

void TsLogger::setLogLevel(Level level) {
	if (logLevel != level) {
		log(SYS,"Log level changed from " + std::to_string(logLevel) + " to "+ std::to_string(level));
		logLevel = level;
	}
	return;
}

void TsLogger::log(Level level, std::string message) {
	if (level >= logLevel) {
		std::ofstream logfile;
		logfile.open(logFileName, std::ofstream::out | std::ofstream::app);
		logfile << getLocalTime() << " " << message << std::endl;
		logfile.close();
	}
	return;
}

void TsLogger::clearLog() {
	remove(logFileName.c_str());
	log(SYS,"Log cleared");
	log(SYS,"Logger initialised");
	log(SYS,"Log level default: " + std::to_string(logLevel));
	return;
}

void TsLogger::close() {
	log(SYS,"Logger closed");
	delete theOne;
	return;
}

// Currently disabled
/*
bool TsLogger::setLogFileName(std::string fileName) {
std::ofstream newLogFile;
newLogFile.open(fileName, std::ofstream::out | std::ofstream::app);

if (newLogFile.is_open()) {
newLogFile.close();

log(FATAL,"TsLogger closed");

logFileName = fileName;
log(FATAL,"TsLogger initiated");
return true;
}
return false;
}
*/