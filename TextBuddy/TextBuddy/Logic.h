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
	PAST_
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

public:
	static Logic* getInstance();
	static DisplayMode mode;
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

// Chin Kiat Boon @@author A0096720A-unused
/*
private:
int getIdOfIndex(int userIndex);
bool isValidIndex(int index);
void matchIndex(int index, std::vector<Task>::iterator &currIter, 
std::vector<Task>::iterator &taskIter);
std::vector<Task>::iterator matchCurrentViewIndex(int index);
std::vector<Task>::iterator matchTaskViewIndex(int index);
void markDone(Markdone toMarkDone);
// bool markPriority(Star toMarkStar);

public:
int getSize();
std::vector<Task> loadFile(std::string fileName);
std::vector<Task> getCurrentView();
std::vector<Task> getTaskStore();
std::vector<Task> getFloatingTasks();
void amendView(std::string listOfIds); // Amends currentView to store searched elements
void clearTaskStore();
void copyView();
void sortDate(std::vector<Task> &taskVector);
void viewTaskType(TaskType type); // Allows for viewing of a particular task type
void addInfo(Add taskName);
void deleteInfo(Delete idToDelete);
void modifyInfo(Modify toModify);
void saveFile(std::string fileName);
std::string searchInfo(Search toSearch);
*/

/* Keep for reference */
/*
// Assume max storage of 1000 tasks for now
#define SIZE 1000
class Logic{
private:
bool isActive;
IO* io;

// Start and end dates to allow for "blocking" of slots for now
// If it is a normal "duedate", no endDate and endTime will be given
// For floating tasks, all the dates and times could be given impossible values, eg 999999 for date and 9999 for time 
/*class struct textInfo{				// Change to class
std::string text;						// Basic contents of task
int startDate;							// YYMMDD format (if year not parsed then can add 150000)
int startTime;							// 24hr/HHMM format (or HMM if [12mn < time < 12nn])
int endDate;							// If no endDate, give it value of startDate
int endTime;							// If no endTime, give it value of startTime
// bool isPriority;						// Add later when necessary					
// std::string label;					
// int ID;								// Permanently tagged to a specific task
// bool isDone;

public:
std::string getText();
int getStartDate();
int getStartTime();
int getEndDate();
int getEndTime();
};

// Found separate array
struct textInfo infoArray[SIZE];		// Change to vector
int sizeOfArray;

public:
Logic();
Logic(bool status);
~Logic();

bool getStatus();
bool canCallIO();

// The four methods below serve as 'intermediate' methods
int getSize(void);
void copyArray(struct textInfo stringArray[]);
void swapElement(textInfo& textA, textInfo& textB);
int addTime(int time, int addValue);

void addInfo(std::string textInput, int startDateInput, int startTimeInput, int endDateInput, int endTimeInput);
void deleteInfo(int index);
std::string returnInfo(int index);

// void sortDateAndTime(struct textInfo textArray[]);
void sortDateAndTime(void);
void changeInfo(std::string textInput, int startDateInput, int startTimeInput, int endDateInput, int endTimeInput, int index);
std::string freeSlotSearch(int date, int time);
// void blockSlots;
// std::string returnHelpText(void);
};
*/

#endif