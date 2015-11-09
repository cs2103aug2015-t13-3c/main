// Chin Kiat Boon @@author A0096720A

#ifndef POWERSEARCH_H_
#define POWERSEARCH_H_

class PowerSearch: public Command {
private:
	std::string msg;
	std::string searchPhrase;
	int startDate;
	int startTime;
	int endDate;
	int endTime;
	int daysNeeded;
	int hrsNeeded;
	int minsNeeded;

	// Tasks that are within the period of interest
	std::vector<Task> tasksWithinPeriod;	
	// Contains "tasks" that hold the start and end of each free period found 
	std::vector<Task> freePeriods;			

	void convertTime(std::vector<Task> &taskVector);
	void addPeriod(int startDate, int startTime, int endDate, int endTime);
	int daysInMth(int month, int year);
	int numOfMin(int date, int time);
	bool isWithinFreePeriod(Task freePeriod, int daysNeeded, int hrsNeeded, int minsNeeded);

public:
	PowerSearch();
	PowerSearch(std::vector<std::string> searchParameters);
	~PowerSearch();

	// Setters and Getters
	void setTasksWithinPeriod(int startDate, int startTime, int endDate, int endTime);
	void setFreePeriods(int startDate, int startTime, int endDate, int endTime);
	std::vector<Task> getTasksWithinPeriod();
	std::vector<Task> getFreePeriods();

	void searchInfo(std::string phr, int startDate, int startTime, int endDate, int endTime);
	void regexSearch(std::string phr, int startDate, int startTime, int endDate, int endTime);
	void searchFreeSlot(int startDate, int startTime, int endDate, int endTime, 
		int daysNeeded, int hrsNeeded, int minsNeeded); 


	void execute();

	std::string getMessage();
};

#endif