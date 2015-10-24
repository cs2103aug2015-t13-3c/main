// @@author A0126677U (Soon Hao Ye)

#include "stdafx.h"
#include "Update.h"

Update::Update(std::vector<std::string>* labels,
			std::vector<std::string>* description,
			std::vector<std::string>* dateAndTime,
			std::vector<std::string>* floatingTasks,
			std::vector<bool>* priorityTasks,
			std::vector<Task>* currentView ) {
	this->labels = labels;
	this->description = description;
	this->dateAndTime = dateAndTime;
	this->floatingTasks = floatingTasks;
	this->priorityTasks = priorityTasks;
	this->currentView = currentView;
}

void Update::update() {
	
		labels->clear();
		description->clear();
		dateAndTime->clear();
		priorityTasks->clear();
		floatingTasks->clear();
		std::vector<Task>::iterator i = currentView->begin();
		while(i!=currentView->end()) {
			labels->push_back(i->getLabelString());
			description->push_back(i->getName());
			dateAndTime->push_back(i->getDateAndTime_UI());
			priorityTasks->push_back(i->getPriorityStatus());
			if(i->getType() == FLOATING) {
				floatingTasks->push_back(i->getName());
			}
			++i;
		}
	
}