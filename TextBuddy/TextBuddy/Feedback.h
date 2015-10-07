#include "stdafx.h"
#include "Utilities.h"

class Feedback
{
public:
	Feedback();
	Feedback(std::string feedbackMessage, std::vector<Task> tasksToShow);
	std::string getFeedbackMessage();
	std::vector<std::string> getTaskToShow();
	void setUpdateView(bool);

private:
	bool updateView;
	std::string feedbackMessage;
	std::vector<Task> tasksToShow;
};
