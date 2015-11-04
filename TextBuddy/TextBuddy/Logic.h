// Chin Kiat Boon @@author A0096720A

#ifndef LOGIC_H_
#define LOGIC_H_

#include "History.h"
#include "IO.h"
#include "Parser.h"
#include "Update.h"

enum DisplayMode {
	ALL,
	TODAY,
	WEEK,
	EVENTS,
	DEADLINES,
	FLOATINGS,
	SEARCHES,
	PAST_,
	FREESLOTS
};

class Logic {
private:
	static Logic* theOne;
	Logic();

	std::vector<Task>* currentView; 
	History* history;
	Parser* parser;
	IO* io;
	Update* updater;

	const static std::string Logic::ERROR_NO_INPUT;
	const static std::string ERROR_INVALID_COMMAND;
	static DisplayMode mode;

public:
	static Logic* getInstance();
	~Logic();

	std::string processCommand(std::string userCommand);
	void subscribe(std::vector<DisplayedTask>* tasks);
	DisplayMode getMode();
	static void setTodayMode();
	static void setWeekMode();
	static void setAllMode();
	static void setEventsMode();
	static void setDeadlinesMode();
	static void setFloatingMode();
	static void setSearchMode();
	static void setPastMode();
	void resetUpdaterNULL();
};

#endif