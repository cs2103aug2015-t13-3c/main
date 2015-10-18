// @@author Chin Kiat Boon
#include "PowerSearch.h"


PowerSearch::PowerSearch(void) {
}


PowerSearch::~PowerSearch(void) {
}

//private methods
void PowerSearch::setTasksWithinPeriod(int startDate, int startTime, int endDate, int endTime) {
	std::vector<Task>::iterator iter = taskStore.begin();
	
	tasksWithinPeriod.clear();

	while (iter->getStartDate() < startDate) {
		++iter;
	}

	//if date is the same, further filter using time in the date
	if (iter->getStartDate() == startDate) {
		while (iter->getStartTime() < startTime) {
			++iter;
		}
	}

	while (iter->getStartDate() < endDate) {
		tasksWithinPeriod.push_back(*iter);
		++iter;
	}

	if (iter->getStartDate() == endDate) {
		while (iter->getStartTime() < endTime) {
			tasksWithinPeriod.push_back(*iter);
			++iter;
		}
	}

	tasksWithinPeriod.pop_back();									//bring back one task to ensure tasks still lie within range

	return;
}



//public methods
//KB: will complete all 3 (and add more if possible) by tues morning
void PowerSearch::searchInfo(std::string phr, int startDate, int endDate) {
	std::vector<Task>::iterator iter;

	for (iter = taskStore.begin(); iter != taskStore.end(); ++iter) {
		/*
	for (iter = taskStore.begin(); iter != taskStore.end(); ++iter) {
		taskName = iter->getName();
		if(Utilities::isSubstring(searchPhrase,taskName)) {
			id = iter->getID();
			indexString << id << ",";
		}
	}
	returnString = indexString.str();

	if(!returnString.empty()) {
		returnString.pop_back();
	}

	return returnString;*/
	}
}

void PowerSearch::searchFreeSlot(int startDate, int endDate) {
	
}

void PowerSearch::searchLabel(std::string label) {
	
}

