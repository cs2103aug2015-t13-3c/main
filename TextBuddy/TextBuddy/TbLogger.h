// @@author A0110376N (Aaron Chong Jun Hao)
//
//========== Setup for your Component Class ==========
// Declare in header file as private member of the component class:
//		TbLogger* logger;
//
// Initialise in cpp file in constructor, change ### to class name:
//		logger = TbLogger::getInstance();
//		logger->setLogLevel(DEBUG);
//		logger->log(SYS,"### instantiated");
//
// Include in cpp file in destructor, change ### to class name:
//		logger->log(SYS,"### destructed");
//
// Add in cpp file as private method, change ### to class name:
//		void ###::log(Level level, std::string message) {
//		logger->log(level,message);
//		return;
//		}
//
// Use in cpp file in methods, refer to enum for Level options:
//		log(Level,std::string);
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

class TbLogger {
private:
	static const std::string logFileName;
	static TbLogger* logger;
	Level logLevel;
	TbLogger();

public:
	char* getLocalTime();
	static TbLogger* getInstance();
	~TbLogger();
	
	void setLogLevel(Level level);
	void log(Level level, std::string message);
	int getDate();
	void clearLog();
	void close();

	// Currently disabled
	// bool setLogFileName(std::string fileName);
};

#endif