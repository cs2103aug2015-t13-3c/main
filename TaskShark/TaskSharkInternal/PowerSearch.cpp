// Chin Kiat Boon @@author A0096720A

#include "stdafx.h"
#include "Command.h"
#include "PowerSearch.h"

PowerSearch::PowerSearch(std::vector<std::string> searchParameters) : Command(POWERSEARCH) {
	msg = "";
	searchPhrase = searchParameters[0];
	startDate	= Utilities::stringToInt(searchParameters[1]);
	startTime	= Utilities::stringToInt(searchParameters[2]);
	endDate		= Utilities::stringToInt(searchParameters[3]);
	endTime		= Utilities::stringToInt(searchParameters[4]);
	daysNeeded	= Utilities::stringToInt(searchParameters[5]);
	hrsNeeded	= Utilities::stringToInt(searchParameters[6]);
	minsNeeded	= Utilities::stringToInt(searchParameters[7]);
}

PowerSearch::~PowerSearch() {}

//========== Private Methods ==========

// Converts time from parser into a suitable format for PowerSearch
void PowerSearch::convertTime(std::vector<Task> &taskVector) {
	std::vector<Task>::iterator iter;

	for (iter = taskVector.begin(); iter != taskVector.end(); ++iter) {
		if (iter->getStartTime() == TIME_NOT_SET) {
			iter->setStartTime(0);
		}
		if (iter->getEndTime() == TIME_NOT_SET) {
			iter->setEndTime(0);
		}
	}
}

// Adds a period where there are no undone tasks on hand to freeSlots
void PowerSearch::addPeriod(int startDate, int startTime, int endDate, int endTime) {
	Task freePeriod;

	freePeriod.setStartDate(startDate);
	freePeriod.setStartTime(startTime);
	freePeriod.setEndDate(endDate);
	if (endTime == 0) {
		freePeriod.setEndTime(2359);
	} else {
		freePeriod.setEndTime(endTime);
	}

	freePeriods.push_back(freePeriod);
}

int PowerSearch::daysInMth(int month, int year) {
	switch(month) {
	case 1:
		return 31;
	case 2:
		if (year%4 == 0) {
			return 29;
		} else {
			return 28;
		}
	case 3:
		return 31;
	case 4:
		return 30;
	case 5:
		return 31;
	case 6:
		return 30;
	case 7:
		return 31;
	case 8:
		return 31;
	case 9:
		return 30;
	case 10:
		return 31;
	case 11:
		return 30;
	case 12:
		return 31;
	default:
		assert(false);
	}
	return 0;
}

// Counts number of minutes from yr 2000, 1 Jan 0000
int PowerSearch::numOfMin(int date, int time) {
	int numOfYrs = date/10000;
	int numOfMths = (date%10000)/100;
	int numOfDays = date%100;
	int numOfHrs = time/100;
	int numOfMins = time%100;
	int totalDays;

	totalDays = numOfYrs*365 + numOfDays;

	for (int i = 1; i <= numOfMths; i++) {
		totalDays += daysInMth(numOfMths, numOfYrs);
	}

	return (totalDays*24*60 + numOfHrs*60 + numOfMins);
}

// Returns true if a particular period is longer than the time requested by the user
// Evaluates based on number of minutes
bool PowerSearch::isWithinFreePeriod(Task freePeriod, int daysNeeded, int hrsNeeded, int minsNeeded) {
	int numMinNeeded;			
	int numMinAvail;			// Number of minutes in the free period	

	numMinAvail = numOfMin(freePeriod.getEndDate(), freePeriod.getEndTime()) - numOfMin(freePeriod.getStartDate(), freePeriod.getStartTime());
	numMinNeeded = daysNeeded*24*60 + hrsNeeded*60 + minsNeeded;

	if (numMinAvail >= numMinNeeded) {
		return true;
	} else {
		return false;
	}
}

//========== Public Methods ==========

void PowerSearch::setTasksWithinPeriod(int startDate, int startTime, int endDate, int endTime) {
	std::vector<Task> taskVector = taskStore;
	std::vector<Task>::iterator iter;

	tasksWithinPeriod.clear();

	if (endDate == -1) {
		tasksWithinPeriod = taskVector;
	} else {
		removeDoneTasks(taskVector);
		sortDate(taskVector);
		convertTime(taskVector);

		for (iter = taskVector.begin(); iter != taskVector.end(); ++iter) {
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
}

void PowerSearch::setFreePeriods(int startDate, int startTime, int endDate, int endTime) {
	std::vector<Task> taskVector = taskStore;
	std::vector<Task>::iterator iter;
	Task freeDateTask;
	int freeDateStart = startDate;
	int freeTimeStart = startTime;

	removeDoneTasks(taskVector);
	removeTaskType(taskVector, FLOATING);
	removeTaskType(taskVector, TODO);
	sortDate(taskVector);
	convertTime(taskVector);

	iter = taskVector.begin();

	// Need to take into account the period before start of first task
	for (iter = taskVector.begin(); iter != taskVector.end(); ++iter) {
		// Do not consider 'events' input that do not actually contain a period eg. from 5 nov at 11 pm to 5 nov at 11 pm 
		if (!((iter->getStartDate() == iter->getEndDate()) && (iter->getStartTime() == iter->getEndTime()))) {
			if ((iter->getStartDate() > freeDateStart) || ((iter->getStartDate() == freeDateStart) && (iter->getStartTime() >= freeTimeStart))) {
				addPeriod(freeDateStart, freeTimeStart, iter->getStartDate(), iter->getStartTime()); 
			}
			// Condition set to prevent freeDateStart from "going back"
			if ((freeDateStart < iter->getEndDate()) || ((freeDateStart == iter->getEndDate()) && (freeTimeStart < iter->getEndTime()))) {
				freeDateStart = iter->getEndDate();			
				freeTimeStart = iter->getEndTime();
			}
		}
	}

	// Account for period between: after the end of all tasks, and still within the period of interest 
	if ((freeDateStart < endDate) || ((freeDateStart == endDate) && (freeTimeStart < endTime))) {
		addPeriod(freeDateStart, freeTimeStart, endDate, endTime);
	}
}

std::vector<Task> PowerSearch::getTasksWithinPeriod() {
	return tasksWithinPeriod;
}

std::vector<Task> PowerSearch::getFreePeriods() {
	return freePeriods;
}

// Searches for a phrase within a particular time period, stores the output in currentView
void PowerSearch::searchInfo(std::string phr, int startDate, int startTime, int endDate, int endTime) {
	std::vector<Task>::iterator iter;
	std::string taskName;

	setTasksWithinPeriod(startDate, startTime, endDate, endTime);
	currentView.clear();

	for (iter = tasksWithinPeriod.begin(); iter != tasksWithinPeriod.end(); ++iter) {
		taskName = iter->getName();
		if (Utilities::isSubstring(phr,taskName)) {
			currentView.push_back(*iter);
		}
	}
}

// Takes in input of regex format and allows UI to display matching searches
// Supports "*", "+", "?"
// If no time boundary, all time-related parameters to be set to -1
// If time boundary present, floating tasks will not be added into the search
void PowerSearch::regexSearch(std::string regPhr, int startDate, int startTime, int endDate, int endTime) {
	std::vector<Task> taskVector;
	std::vector<Task>::iterator iter;

	setTasksWithinPeriod(startDate, startTime, endDate, endTime);
	currentView.clear();

	for (iter = tasksWithinPeriod.begin(); iter != tasksWithinPeriod.end(); ++iter) {
		if (std::regex_match(iter->getName(), std::regex(regPhr))) {
			currentView.push_back(*iter);
		}
	}
}


// Input: First 4 variables refers to the period of interest, last 3 variables refer to the time required for the free slot
// Output: Modifies currentView into a list of free dates
void PowerSearch::searchFreeSlot(int startDate, int startTime, int endDate, int endTime,
								 int daysNeeded, int hrsNeeded, int minsNeeded) {
									 int count = 1;
									 std::vector<Task>::iterator iter;

									 setFreePeriods(startDate, startTime, endDate, endTime);
									 currentView.clear();

									 for (iter = freePeriods.begin(); iter != freePeriods.end(); ++iter) {
										 if (isWithinFreePeriod(*iter,daysNeeded,hrsNeeded,minsNeeded)) {
											 iter->setName("Free period " + Utilities::intToString(count));
											 count++;

											 currentView.push_back(*iter);
										 }
									 }
}

void PowerSearch::execute() {
	if (searchPhrase == "") {
		searchFreeSlot(startDate,startTime, endDate, endTime, daysNeeded, hrsNeeded, minsNeeded);
		msg = "Here is the list of free slots available";
	} else if (Utilities::isSubstring("*", searchPhrase) || Utilities::isSubstring("+", searchPhrase) ||
		Utilities::isSubstring("?", searchPhrase)) { 
			regexSearch(searchPhrase, startDate, startTime, endDate, endTime);
			msg = "Results for \""+ searchPhrase + "\"";
	} else {
		searchInfo(searchPhrase, startDate, startTime, endDate, endTime);
		msg = "Results for \"" + searchPhrase + "\"";
	}
}

std::string PowerSearch::getMessage() {
	return msg;
}