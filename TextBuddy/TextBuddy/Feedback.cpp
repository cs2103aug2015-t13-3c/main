#include "Feedback.h"


Feedback::Feedback(std::string feedbackMessage, std::vector<Task> tasksToShow) {
	this->feedbackMessage = feedbackMessage;
	this->tasksToShow = tasksToShow;
}

Feedback::Feedback() {
}

void Feedback::pushTask(Task newTask) {
	tasksToShow.push_back(newTask);
}

void Feedback::setAddedMessage() {
	Task addedTask = tasksToShow[0];
	feedbackMessage = "added " + addedTask.getName() + "\r\n";
}

void Feedback::setUpdateView(bool trueOrFalse) {
	this->updateView = trueOrFalse;
}

std::string Feedback::getFeedbackMessage() {
	return feedbackMessage;
}

std::vector<std::string> Feedback::getTaskToShow() {
	int size = tasksToShow.size();
	std::vector<std::string> tasksAsStrings;
	Utilities utility;
	for(int i=0 ; i< size ; ++i) {
		tasksAsStrings[i] = utility.taskToString(tasksToShow[i]);
	}
	return tasksAsStrings;
}
