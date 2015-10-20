// @@author A0096720A (Chin Kiat Boon)

#include "Command.h"

#ifndef POWERSEARCH_H_
#define POWERSEARCH_H_

// Make it child class of Search when ready
// Since it is not made child class yet, assume all dates are sorted in increasing order first
// All made public for testing first
class PowerSearch {
public:
	std::vector<Task> taskStore;			// will remove upon making it child class
	std::vector<Task> currentView;			// will remove upon making it child class
	std::vector<Task> tasksWithinPeriod;	// tasks in the periods of interest
	std::vector<Task> freeDates;			// each task element stores the start and end of the free period

	PowerSearch(void);
	PowerSearch(std::vector<Task> cmdTaskStore, std::vector<Task> cmdCurrentView);
	~PowerSearch(void);

	// To be private
	void setTasksWithinPeriod(int startDate, int startTime, int endDate, int endTime);		
	void addFreeDate(int startDate, int startTime, int endDate, int endTime);

	// To remain public
	void searchInfo(std::string phr, int startDate, int startTime, int endDate, int endTime);
	void searchFreeSlot(int startDate, int startTime, int endDate, int endTime); // start and end of the period of interest
	void searchLabel(std::string label);

	// Setters and Getters
};

#endif