// Chin Kiat Boon @@author A0096720A

#ifndef POWERSEARCH_H_
#define POWERSEARCH_H_

#include "Command.h"

class PowerSearch: public Search {
private:
	std::vector<Task> tasksWithinPeriod;	// tasks in the periods of interest
	std::vector<Task> freePeriods;			// each task element stores the start and end of the free period

	void addPeriod(int startDate, int startTime, int endDate, int endTime);
	int daysInMth(int month, int year);
	int numOfMin(int date, int time);
	bool isWithinFreePeriod(Task freePeriod, int daysNeeded, int hrsNeeded, int minsNeeded);
public:
	PowerSearch(void);
	~PowerSearch(void);
	
	// Setters and Getters
	void setTasksWithinPeriod(int startDate, int startTime, int endDate, int endTime);
	void setFreePeriods(int startDate, int startTime, int endDate, int endTime);
	std::vector<Task> getTasksWithinPeriod();
	std::vector<Task> getFreePeriods();

	void searchInfo(std::string phr, int startDate, int startTime, int endDate, int endTime);
	void searchFreeSlot(int startDate, int startTime, int endDate, int endTime, 
		int daysNeeded, int hrsNeeded, int minsNeeded); // Start and end of the period of interest
	void searchLabel(std::string label);

};

#endif