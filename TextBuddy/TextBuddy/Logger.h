// @@author Aaron Chong Jun Hao
//
//========== Setup for your Component Class ==========
// Declare in header file as private member of the component class:
//		Logger* logger;
//
// Initialise in cpp file in constructor, change ### to class name:
//		logger = Logger::getInstance();
//		logger->setLogLevel(DEBUG);
//		logger->log(SYS,"### instantiated");
//
// Include in cpp file in destructor, change ### to class name:
//		logger->log(SYS,"### destructed");
//
// Use in cpp file in methods, refer to enum for Level options:
//		logger->log(Level,std::string);
//
//========== Note for Logic ==========
// Include the following upon exit command:
//		delete parser;
//		delete IO;
//
//========== Note for UI ==========
// Include the following upon exit command:
//		delete logic;
//		logger->close();
//

#ifndef LOGGER_H_
#define LOGGER_H_

enum Level {
	DEBUG,	// set this when debugging
	INFO,	// default
	SYS,	// system information for when components are instantiated and destructed
	WARN,	// error that will not stop the application from running
	FATAL	// very severe error that will presumably lead the application to abort
};

class Logger {
private:
	static const std::string logFileName;
	static Logger* logger;
	Level logLevel;
	Logger();

public:
	char* getLocalTime();

	~Logger();
	static Logger* getInstance();
	void setLogLevel(Level level);
	void log(Level level, std::string message);
	void close();

	// Currently disabled
	// bool setLogFileName(std::string fileName);
};

#endif