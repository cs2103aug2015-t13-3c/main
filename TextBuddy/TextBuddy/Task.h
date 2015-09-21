// @@author Aaron Chong Jun Hao

#ifndef TASK_H_
#define TASK_H_

class Task {
private:
	std::string taskName;

public:
	Task();
	~Task();
	void setTaskName(std::string restOfCommand);
	std::string getTaskName();
};

#endif