// @@author Aaron Chong Jun Hao

#ifndef TASK_H_
#define TASK_H_

class Task {
private:
	std::string taskName;
	bool isDone;
	bool isFloating;
	bool isPriority;
	std::string date;     // YYMMDD, supports 2015-2099
	std::string time;     // HHMM, 24-hour format
	std::string timeFrom; // HHMM, 24-hour format
	std::string timeTo;   // HHMM, 24-hour format
	std::string label;

public:
	Task();
	~Task();
	void setTaskName(std::string restOfCommand="");
	std::string getTaskName();
};

#endif