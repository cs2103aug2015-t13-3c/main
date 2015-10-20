// @@author A0096720A (Chin Kiat Boon)

#ifndef LOGIC_H_
#define LOGIC_H_

#include "Feedback.h"
#include "History.h"
#include "Observer.h"
#include "Parser.h"
#include "IO.h"

class Logic {
private:
	Logic();
	static Logic *theOne; // Singleton
	History* history;
	Parser parser;
	IO io;

	const static std::string ERROR_INVALID_COMMAND;
	/*
	const static std::string ERROR_INDEX_OUT_OF_BOUNDS;
	int getIdOfIndex(int userIndex);
	bool isValidIndex(int index);
	void matchIndex(int index, std::vector<Task>::iterator &currIter, 
	std::vector<Task>::iterator &taskIter);
	std::vector<Task>::iterator matchCurrentViewIndex(int index);
	std::vector<Task>::iterator matchTaskViewIndex(int index);
	void markDone(Markdone toMarkDone);
	// bool markPriority(Star toMarkStar);
	*/
public:
	static Logic* getInstance();
	~Logic();
	
	Feedback processCommand(std::string userCommand);
	void subscribe(Observer* ui);
	std::vector<Task> getFloatingTasks();
	/*
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
};

/* Keep for reference */
/*
//assume max storage of 1000 tasks for now
#define SIZE 1000
class Logic{
private:
bool isActive;
IO io;

//start and end dates to allow for "blocking" of slots for now
//if it is a normal "duedate", no endDate and endTime will be given
//for floating tasks, all the dates and times could be given impossible values, eg 999999 for date and 9999 for time 
/*class struct textInfo{							//change to class
std::string text;						//basic contents of task
int startDate;							//YYMMDD format (if year not parsed then can add 150000)
int startTime;							//24hr/HHMM format (or HMM if [12mn < time < 12nn])
int endDate;							//if no endDate, give it value of startDate
int endTime;							//if no endTime, give it value of startTime
//bool isPriority;						//add later when necessary					
//std::string label;					
//int ID;								//permanently tagged to a specific task
//bool isDone;

public:
std::string getText();
int getStartDate();
int getStartTime();
int getEndDate();
int getEndTime();
};

//found separate array
struct textInfo infoArray[SIZE];			//change to vector
int sizeOfArray;

public:
Logic();
Logic(bool status);
~Logic();

bool getStatus();
bool canCallIO();

//the four methods below serve as 'intermediate' methods
int getSize(void);
void copyArray(struct textInfo stringArray[]);
void swapElement(textInfo& textA, textInfo& textB);
int addTime(int time, int addValue);

void addInfo(std::string textInput, int startDateInput, int startTimeInput, int endDateInput, int endTimeInput);
void deleteInfo(int index);
std::string returnInfo(int index);

//void sortDateAndTime(struct textInfo textArray[]);
void sortDateAndTime(void);
void changeInfo(std::string textInput, int startDateInput, int startTimeInput, int endDateInput, int endTimeInput, int index);
std::string freeSlotSearch(int date, int time);
//void blockSlots;
//std::string returnHelpText(void);
};
*/

#endif