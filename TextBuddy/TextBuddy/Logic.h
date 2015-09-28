// @@author Chin Kiat Boon

#ifndef LOGIC_H_
#define LOGIC_H_

#include "IO.h"
#include "Command.h"
#include "Task.h"
#include "Parser.h"
#include <string>
#include <stdlib.h>
#include <iostream>
#include <sstream>

class Logic {
private:
	// bool isActive;
	IO io;
	std::vector<Task> taskStore;
	std::vector<Task> currentView;

public:
	Logic();
	~Logic();
	// Logic(bool status);
	// bool getStatus();
	// bool canCallIO();
	int getSize();
	std::vector<Task> getTaskStore();
	std::vector<Task> getCurrentView();
	bool addInfo(Add taskName);
	bool deleteInfo(Delete idToDelete);
	bool modifyInfo(Modify toModify);
	std::string searchInfo(Search toSearch);

	bool matchPhrase(std::string phr, std::string str);

	void processCommand(std::string userCommand, Parser& parser);
};

/* Keep for reference*/
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