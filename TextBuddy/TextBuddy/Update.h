// Soon Hao Ye @@author A0126677U
enum TaskStatus {
	PRIORITY,
	URGENT,
	NORMAL
};

struct DisplayedTask {
	std::string description;
	std::string label;
	std::string date;
	std::string time;
	TaskStatus status;
	TaskType type;
}; 

class Update {
protected:
	std::vector<Task>* currentView;
	std::vector<DisplayedTask>* tasksToDisplay;

public:
	Update(std::vector<DisplayedTask>* tasksToDisplay,std::vector<Task>* currentView);
	~Update();
	void update();
};

