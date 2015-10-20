// @@author A0126677U (Soon Hao Ye)

#ifndef FEEDBACK_H_
#define FEEDBACK_H_

#include "Task.h"
//#include "stdafx.h"

class Feedback {
private:
	bool exit;
	bool operationSucceeded;
	bool updateView;
	bool warning;
	std::string feedbackMessage;
	std::vector<Task> tasksToShow;

public:
	Feedback();
	Feedback(std::string feedbackMessage, std::vector<Task> tasksToShow);
	void pushTask(Task newTask);
	void setAddedMessage();
	void setErrorMessage(std::string message);
	void setExit();
	void setSearchMessage(std::string searchPhrase,bool isFound);
	void setTasksToShow(std::vector<Task> tasksToShow);
	void setUpdateView(bool truthValue);
	bool isExit();
	bool isSuccess();
	bool isWarning();
	bool needToUpdateDisplay();
	std::string getFeedbackMessage();
	std::vector<Task> getTaskToShow();
	std::vector<std::string> getTaskToShow_string();
};

#endif