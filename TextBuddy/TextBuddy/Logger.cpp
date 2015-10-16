// @@author Aaron Chong Jun Hao

#include "stdafx.h"

const std::string Logger::logFileName = ".tblog";
Logger* Logger::logger = new Logger;

Logger::Logger() {
	logLevel = INFO;
	log(SYS,"Logger initialised");
}

Logger::~Logger() {
	log(SYS,"Logger closed");
}

char* Logger::getLocalTime() {
	time_t rawtime;
	time(&rawtime);

	struct tm *timeinfo = new tm;
	localtime_s(timeinfo, &rawtime);

	char* buffer = new char[26];
	asctime_s(buffer,26,timeinfo);
	buffer[strlen(buffer)-1]=0;
	return buffer;
}

Logger* Logger::getInstance() {
	return logger;
}

void Logger::setLogLevel(Level level) {
	logLevel = level;
}

void Logger::log(Level level, std::string message) {
	if(level >= logLevel) {
		std::ofstream logfile;
		logfile.open(logFileName, std::ofstream::out | std::ofstream::app);
		logfile << getLocalTime() << " " << message << std::endl;
		logfile.close();
	}
}

void Logger::close() {
	delete logger;
	return;
}

// Currently disabled
/*
bool Logger::setLogFileName(std::string fileName) {
	std::ofstream newLogFile;
	newLogFile.open(fileName, std::ofstream::out | std::ofstream::app);

	if(newLogFile.is_open()) {
		newLogFile.close();

		log(FATAL,"Logger closed");

		logFileName = fileName;
		log(FATAL,"Logger initiated");
		return true;
	}
	return false;
}
*/