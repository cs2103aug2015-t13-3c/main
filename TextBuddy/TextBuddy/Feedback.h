#include "stdafx.h"
#include "Utilities.h"

class Feedback {
public:
	Feedback();
	Feedback(std::string feedbackMessage, std::vector<Task> tasksToShow);
	void pushTask(Task newTask);
	void setTasksToShow(std::vector<Task> tasksToShow);
	void setAddedMessage();
	void setSearchMessage(std::string searchPhrase,bool isFound);
	std::string getFeedbackMessage();
	std::vector<Task> getTaskToShow();
	std::vector<std::string> getTaskToShow_string();
	void setUpdateView(bool);
	bool needToUpdateDisplay();
	bool isSuccess();

private:
	bool updateView;
	bool operationSucceeded;
	std::string feedbackMessage;
	std::vector<Task> tasksToShow;
};
