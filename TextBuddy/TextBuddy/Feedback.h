// @@author Soon Hao Ye

#ifndef FEEDBACK_H_
#define FEEDBACK_H_

#include "stdafx.h"

class Feedback {
private:
	bool exit;
	bool warning;
	bool updateView;
	bool operationSucceeded;
	std::string feedbackMessage;
	std::vector<Task> tasksToShow;

public:
	Feedback();
	Feedback(std::string feedbackMessage, std::vector<Task> tasksToShow);
	void pushTask(Task newTask);
	void setTasksToShow(std::vector<Task> tasksToShow);
	void setAddedMessage();
	void setSearchMessage(std::string searchPhrase,bool isFound);
	void setErrorMessage(std::string message);
	std::string getFeedbackMessage();
	std::vector<Task> getTaskToShow();
	std::vector<std::string> getTaskToShow_string();
	void setUpdateView(bool);
	bool needToUpdateDisplay();
	bool isSuccess();
	bool isWarning();
	void setExit();
	bool isExit();
};

#endif