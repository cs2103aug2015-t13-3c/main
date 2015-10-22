// @@author A0126677U (Soon Hao Ye)

class Update {
protected:
	std::vector<std::string>* labels;
	std::vector<std::string>* description;
	std::vector<std::string>* dateAndTime;
	std::vector<std::string>* floatingTasks;
	std::vector<bool>* priotiryTasks;
	std::vector<Task>* currentView;

public:
	void update();
	Update(std::vector<std::string>* labels,
			std::vector<std::string>* description,
			std::vector<std::string>* dateAndTime,
			std::vector<std::string>* floatingTasks,
			std::vector<bool> *priotiryTasks,
			std::vector<Task>* currentView);
};