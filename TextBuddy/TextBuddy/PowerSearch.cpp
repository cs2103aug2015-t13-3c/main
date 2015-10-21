// @@author A0096720A (Chin Kiat Boon)

#include <string>
#include "Utilities.h"
#include "PowerSearch.h"

PowerSearch::PowerSearch(void) {}

PowerSearch::PowerSearch(std::vector<Task> cmdTaskStore, std::vector<Task> cmdCurrentView) {
	taskStore = cmdTaskStore;
	currentView = cmdCurrentView;
}

PowerSearch::~PowerSearch(void) {}

//========== Private Methods ==========
void PowerSearch::setTasksWithinPeriod(int startDate, int startTime, int endDate, int endTime) {
	std::vector<Task>::iterator iter = taskStore.begin();
	
	tasksWithinPeriod.clear();

	for (iter = taskStore.begin(); iter != taskStore.end(); ++iter) {
		if ((iter->getStartDate() > startDate) && (iter->getStartDate() < endDate)) {
			tasksWithinPeriod.push_back(*iter);
		}

		// If date is the same, further filter using time in the date
		if (iter->getStartDate() == startDate) {
			if (iter->getStartTime() >= startTime) {
				tasksWithinPeriod.push_back(*iter);
			}
		}

		if (iter->getStartDate() == endDate) {
			if (iter->getStartTime() <= endTime) {
				tasksWithinPeriod.push_back(*iter);				
			}
		}
	}
}

void PowerSearch::addFreeDate(int startDate, int startTime, int endDate, int endTime) {
	Task freeDateTask;

	freeDateTask.setStartDate(startDate);
	freeDateTask.setStartTime(startTime);
	freeDateTask.setEndDate(endDate);
	freeDateTask.setEndTime(endTime);

	freeDates.push_back(freeDateTask);
}


// Searches for a phrase within a particular time period, stores the output in currentView
void PowerSearch::searchInfo(std::string phr, int startDate, int startTime, int endDate, int endTime) {
	std::vector<Task>::iterator iter;
	std::string taskName;

	currentView.clear();
	setTasksWithinPeriod(startDate, startTime, endDate, endTime);

	for (iter = tasksWithinPeriod.begin(); iter != tasksWithinPeriod.end(); ++iter) {
		taskName = iter->getName();
		if (Utilities::isSubstring(phr,taskName)) {
			currentView.push_back(*iter);
		}
	}
}

void PowerSearch::searchFreeSlot(int startDate, int startTime, int endDate, int endTime) {
	std::vector<Task>::iterator iter;
	Task freeDateTask;
	int freeDateStart = startDate;
	int freeTimeStart = startTime;

	setTasksWithinPeriod(startDate, startTime, endDate, endTime);
	iter = tasksWithinPeriod.begin();

	//need to take into account the period before start of first task
	
	for (iter = tasksWithinPeriod.begin(); iter != tasksWithinPeriod.end(); ++iter) {
		if ((iter->getStartDate() > freeDateStart) || ((iter->getStartDate() == freeDateStart) && (iter->getStartTime() >= freeTimeStart))) {
			addFreeDate(freeDateStart, freeTimeStart, iter->getStartDate(), iter->getStartTime()); 
		}
		//condition set to prevent freeDateStart from "going back"
		if ((freeDateStart < iter->getEndDate()) || ((freeDateStart == iter->getEndDate()) && (freeTimeStart < iter->getEndTime()))) {
			freeDateStart = iter->getEndDate();			
			freeTimeStart = iter->getEndTime();
		}
	}

	//account for period between: after the end of all tasks, and still within the period of interest 
	if ((freeDateStart < endDate) || ((freeDateStart == endDate) && (freeTimeStart < endTime))) {
		addFreeDate(freeDateStart, freeTimeStart, endDate, endTime);
	}
}

void PowerSearch::searchLabel(std::string label) {
	std::vector<std::string> searchVector;

	std::vector<Task>::iterator taskIter;
	std::vector<std::string>::iterator setIter;

	currentView.clear();

	for (taskIter == taskStore.begin(); taskIter != taskStore.end(); ++taskIter) {
		searchVector = taskIter->getLabels();

		for (setIter == searchVector.begin(); setIter != searchVector.end(); ++setIter) {
			if (*setIter == label) {
				currentView.push_back(*taskIter);
				break;
			}
		}
	}
}
