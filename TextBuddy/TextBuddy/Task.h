// @@author Aaron Chong Jun Hao

#ifndef TASK_H_
#define TASK_H_

enum TaskType {
	FLOATING,
	EVENT,
	TODO
};

enum FieldType {
	NAME,
	START_DAY,
	END_DAY,
	START_DATE,
	END_DATE,
	START_TIME,
	END_TIME,
	PRIORITY,
	LABEL
};

enum Month {
	JAN, FEB, MAR,
	APR, MAY, JUN,
	JUL, AUG, SEP,
	OCT, NOV, DEC
};

enum Day {
	SUN,
	MON, TUE, WED,
	THU, FRI, SAT,
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

	// Setters, return if successful
	bool setName(std::string newName);
	bool setType(TaskType newType);
	bool setID(int newID);
	bool setLabel(std::string newLabel);

	bool toggleDone();
	bool togglePriority();

	bool setStartDay(Day newStartDay);
	bool setStartDate(int newStartDate);
	bool setStartTime(int newStartTime);

<<<<<<< HEAD
	bool setEndDay(Day newEndDate);
=======
	bool setEndDay(Day newEndDay);
>>>>>>> 22c66777f9b6fd43e6328a41ae12b9422b1804ca
	bool setEndDate(int newEndDate);
	bool setEndTime(int newEndTime);
};

#endif