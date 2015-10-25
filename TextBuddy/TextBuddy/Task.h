// Aaron Chong Jun Hao @@author A0110376N

#ifndef TASK_H_
#define TASK_H_

// This is the default initialisation for time, since 0 represents "12 am"
const int TIME_NOT_SET = -1;

// These are the valid Field keywords
// Count: 10
const std::string FIELD_NAME = "name";
const std::string FIELD_LABEL_ADD = ":";
const std::string FIELD_LABEL_DELETE = "-:";
const std::string FIELD_PRIORITY_SET = "star";
const std::string FIELD_PRIORITY_UNSET = "unstar";
const std::string FIELD_DATE_FROM = "from";
const std::string FIELD_DATE_TO = "to";
const std::string FIELD_DATE_BY = "by";
const std::string FIELD_DATE_ON = "on";
const std::string FIELD_TIME_AT = "at";

// These are the Field enums
// Count: 10 + INVALID_FIELD
enum FieldType {
	NAME,
	LABELS_ADD,
	LABELS_DELETE,
	LABELS_CLEAR,
	PRIORITY_SET,
	PRIORITY_UNSET,
	START_DATE,
	START_TIME,
	END_DATE,
	END_TIME,
	TODO_DATE,
	TODO_TIME,
	INVALID_FIELD,
};

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

enum TaskType {
	FLOATING,
	EVENT,
	TODO
};

class Task {
private:
	static int runningCount;
	static Task tempTask;

	std::string name;
	TaskType type;
	int uniqueID;

	std::set<std::string> labels;
	std::vector<std::string> labelsToDelete;

	bool isDone;
	bool isPriority;

	int startDate; // YYMMDD, supports 2015-2099
	int startTime; // HHMM, 24-hour format

	int endDate;
	int endTime;

public:
	static int getRunningCount();
	static int incrementRunningCount();					// For new tasks
	static void setRunningCount(int lastCount);			// For startup
	static bool tasksAreEqual(Task task1, Task task2);	// For testing

	Task();
	~Task();

	// Getters
	std::string getName();
	TaskType getType();
	int getID();
	std::string getLabelString();
	std::vector<std::string> getLabels();
	std::vector<std::string> getLabelsToDelete();

	bool getDoneStatus();
	bool getPriorityStatus();

	int getStartDate();
	int getStartTime();

	int getEndDate();
	int getEndTime();

	std::string getDate_UI();
	std::string getTime_UI();

	// Setters, return true if successful
	bool setName(std::string newName);
	bool setType(TaskType newType);
	bool setID(int newID);

	bool addLabels(std::vector<std::string> newLabels);
	bool deleteLabels(std::vector<std::string> badLabels);
	bool setLabelsToDelete(std::vector<std::string> oldLabels);
	bool clearLabels();

	bool markDone();		// Returns false if already done
	bool unmarkDone();		// Returns false if already not done

	bool setPriority();		// Returns false if already priority
	bool unsetPriority();	// Returns false if already not priority

	bool setStartDate(int newStartDate);
	bool setStartTime(int newStartTime);

	bool setEndDate(int newEndDate);
	bool setEndTime(int newEndTime);

	bool isUrgent();
};

#endif