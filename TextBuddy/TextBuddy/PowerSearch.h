// Chin Kiat Boon @@author A0096720A

#ifndef POWERSEARCH_H_
#define POWERSEARCH_H_

#include "Command.h"

class PowerSearch: public Search {
private:
	std::vector<Task> tasksWithinPeriod;	// tasks in the periods of interest
	std::vector<Task> freePeriods;			// each task element stores the start and end of the free period

public:
	PowerSearch(void);
	PowerSearch(std::vector<Task> cmdTaskStore, std::vector<Task> cmdCurrentView);
	~PowerSearch(void);
	
	// To be private
	void addFreePeriod(int startDate, int startTime, int endDate, int endTime);
	void setTasksWithinPeriod(int startDate, int startTime, int endDate, int endTime);

	// To remain public
	void searchInfo(std::string phr, int startDate, int startTime, int endDate, int endTime);
	void searchFreeSlot(int startDate, int startTime, int endDate, int endTime); // start and end of the period of interest
	void searchLabel(std::string label);

	// Setters and Getters
	std::vector<Task> getTasksWithinPeriod();
	std::vector<Task> getFreePeriods();
};

#endif