// @@author Aaron Chong Jun Hao

#ifndef TASK_H_
#define TASK_H_

enum TaskType {
	FLOATING,
	EVENT,
	TODO
};

enum Month {
	JAN, FEB, MAR,
	APR, MAY, JUN,
	JUL, AUG, SEP,
	OCT, NOV, DEC
};

enum Day {
	MON, TUE, WED,
	THU, FRI, SAT,
	SUN
};

class Task {
private:
	static int runningCount;

	std::string name;
	TaskType type;
	int uniqueID;
	std::string label;

	bool isDone;
	bool isPriority;
	
	Day startDay;
	int startDate; // YYMMDD, supports 2015-2099
	int startTime; // HHMM, 24-hour format

	Day endDay;
	int endDate;
	int endTime;    // HHMM, 24-hour format

public:
	static int getRunningCount();
	
	Task();
	~Task();
	
	// Getters
	std::string getName();
	TaskType getType();
	int getID();
	std::string getLabel();

	bool getDoneStatus();
	bool getPriorityStatus();

	Day getStartDay();
	int getStartDate();
	int getStartTime();

	Day getEndDay();
	int getEndDate();
	int getEndTime();

	// Setters
	std::string setName();
	TaskType setType();
	int setID();
	std::string setLabel();

	bool toggleDone();
	bool togglePriority();

	Day setStartDay();
	int setStartDate();
	int setStartTime();

	Day setEndDay();
	int setEndDate();
	int setEndTime();
};

#endif