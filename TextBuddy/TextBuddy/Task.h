// @@author Aaron Chong Jun Hao

#ifndef TASK_H_
#define TASK_H_

enum Day {
	SUN,
	MON, TUE, WED,
	THU, FRI, SAT,
	INVALID_DAY
};

enum Month {
	INVALID_MONTH,
	JAN, FEB, MAR,
	APR, MAY, JUN,
	JUL, AUG, SEP,
	OCT, NOV, DEC
};

enum FieldType {
	NAME,
	START_DAY,
	START_DATE,
	START_TIME,
	END_DAY,
	END_DATE,
	END_TIME,
	LABEL,
	PRIORITY,
	INVALID_FIELD,
};

enum TaskType {
	FLOATING,
	EVENT,
	TODO
};

// These are the possible field types for tasks
const std::string FIELD_LABEL = ":";
const std::string FIELD_PRIORITY = "star";
const std::string FIELD_DATE_BY = "by";
const std::string FIELD_DATE_ON = "on";
const std::string FIELD_TIME_AT = "at";
const std::string FIELD_TIME_FROM = "from";
const std::string FIELD_TIME_TO = "to";

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
	int endTime;   // HHMM, 24-hour format

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

	// Setters, return true if successful
	bool setName(std::string newName);
	bool setType(TaskType newType);
	bool setID(int newID);
	bool setLabel(std::string newLabel);

	bool toggleDone();
	bool togglePriority();

	bool setStartDay(Day newStartDay);
	bool setStartDate(int newStartDate);
	bool setStartTime(int newStartTime);

	bool setEndDay(Day newEndDay);
	bool setEndDate(int newEndDate);
	bool setEndTime(int newEndTime);
};

#endif