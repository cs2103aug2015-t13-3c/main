// @@author A0126677U (Soon Hao Ye)

#include <string>
#include "Utilities.h"
#include "Feedback.h"

Feedback::Feedback() {
	warning = false;
	exit = false;
}

Feedback::Feedback(std::string feedbackMessage, std::vector<Task> tasksToShow) {
	this->feedbackMessage = feedbackMessage;
	this->tasksToShow = tasksToShow;
}

void Feedback::pushTask(Task newTask) {
	tasksToShow.push_back(newTask);
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

void Feedback::setExit() {
	exit = true;
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

void Feedback::setTasksToShow(std::vector<Task> tasksToShow) {
	this->tasksToShow = tasksToShow;
	this->updateView = true;
}

void Feedback::setUpdateView(bool truthValue) {
	this->updateView = truthValue;
}

std::string Feedback::getFeedbackMessage() {
	return feedbackMessage;
}

std::vector<Task> Feedback::getTaskToShow() {
	return tasksToShow;
}

std::vector<std::string> Feedback::getTaskToShow_string() {
	int size = tasksToShow.size();
	std::vector<std::string> tasksAsStrings;
	for(int i=0 ; i< size ; ++i) {
		tasksAsStrings[i] = Utilities::taskToString(tasksToShow[i]);
	}
	return tasksAsStrings;
}

bool Feedback::needToUpdateDisplay() {
	return updateView;
}

bool Feedback::isExit() {
	return exit;
}

bool Feedback::isSuccess() {
	return operationSucceeded;
}

bool Feedback::isWarning() {
	return warning;
}
