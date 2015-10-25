// @@author A0126677U (Soon Hao Ye)

class Update {
protected:
	std::vector<std::string>* labels;
	std::vector<std::string>* description;
	std::vector<std::string>* taskDate;
	std::vector<std::string>* taskTime;
	std::vector<std::string>* floatingTasks;
	std::vector<int>* color;
	std::vector<Task>* currentView;

public:
	Update(std::vector<std::string>* labels,
			std::vector<std::string>* description,
			std::vector<std::string>* taskDate,
			std::vector<std::string>* taskTime,
			std::vector<std::string>* floatingTasks,
			std::vector<int>* color,
			std::vector<Task>* currentView);
	~Update();
	void update();
};