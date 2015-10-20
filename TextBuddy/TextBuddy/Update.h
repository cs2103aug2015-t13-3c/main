// @@author (Soon Hao Ye)

#include <vector>
//#include <assert.h>
#include "Command.h"
#include "Task.h"

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