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

void TbLogger::clear() {
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