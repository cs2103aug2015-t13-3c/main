// @@author Soon Hao Ye

#include "Utilities.h"
#include "Feedback.h"

Feedback::Feedback(std::string feedbackMessage, std::vector<Task> tasksToShow) {
	this->feedbackMessage = feedbackMessage;
	this->tasksToShow = tasksToShow;
}

Feedback::Feedback() {
	warning = false;
	exit = false;
}

void Feedback::pushTask(Task newTask) {
	tasksToShow.push_back(newTask);
}

void Feedback::setTasksToShow(std::vector<Task> tasksToShow) {
	this->tasksToShow = tasksToShow;
	this->updateView = true;
}

void Feedback::setSearchMessage(std::string searchPhrase, bool isFound) {
	if(isFound) {
		operationSucceeded = true;
		feedbackMessage = "results for \"" + searchPhrase + "\"";
	} else {
		operationSucceeded = false;
		feedbackMessage = "no results found for \"" + searchPhrase + "\"";
	}
}

void Feedback::setAddedMessage() {
	operationSucceeded = true;
	Task addedTask = tasksToShow[0];
	feedbackMessage = "added " + addedTask.getName() ;
}

void Feedback::setErrorMessage(std::string message) {
	this->feedbackMessage = message;
	this->operationSucceeded = false;
	this->updateView = false;
}

void Feedback::setUpdateView(bool trueOrFalse) {
	this->updateView = trueOrFalse;
}

std::string Feedback::getFeedbackMessage() {
	return feedbackMessage;
}

std::vector<std::string> Feedback::getTaskToShow_string() {
	int size = tasksToShow.size();
	std::vector<std::string> tasksAsStrings;
	Utilities utility;
	for(int i=0 ; i< size ; ++i) {
		tasksAsStrings[i] = utility.taskToString(tasksToShow[i]);
	}
	return tasksAsStrings;
}

std::vector<Task> Feedback::getTaskToShow() {
	return tasksToShow;
}

bool Feedback::needToUpdateDisplay() {
	return updateView;
}

bool Feedback::isSuccess() {
	return operationSucceeded;
}

bool Feedback::isWarning() {
	return warning;
}

void Feedback::setExit() {
	exit = true;
}

bool Feedback::isExit() {
	return exit;
}
