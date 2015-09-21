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
	
	Day day;

	int dateStart;// YYMMDD, supports 2015-2099
	int dateEnd;
	int timeFrom; // HHMM, 24-hour format
	int timeTo;   // HHMM, 24-hour format

public:
	Task();
	~Task();
	void setName(std::string restOfCommand="");
	std::string getName();
	std::string getRunningCount();
};

#endif