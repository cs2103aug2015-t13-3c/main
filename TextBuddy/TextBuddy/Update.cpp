// @@author (Soon Hao Ye)

#include "Update.h"

Update::Update(std::vector<std::string>* labels,
			std::vector<std::string>* description,
			std::vector<std::string>* dateAndTime,
			std::vector<std::string>* floatingTasks,
			std::vector<bool>* priotiryTasks,
			std::vector<Task>* currentView ) {
	this->labels = labels;
	this->description = description;
	this->dateAndTime = dateAndTime;
	this->floatingTasks = floatingTasks;
	this->priotiryTasks = priotiryTasks;
	this->currentView = currentView;
}

void Update::update() {
	
		labels->clear();
		description->clear();
		dateAndTime->clear();
		priotiryTasks->clear();
		floatingTasks->clear();
		std::vector<Task>::iterator i = currentView->begin();
		while(i!=currentView->end()) {
			labels->push_back(i->getLabel());
			description->push_back(i->getName());
			dateAndTime->push_back(i->getDateAndTime_UI());
			priotiryTasks->push_back(i->getPriorityStatus());
			if(i->getType() == FLOATING) {
				floatingTasks->push_back(i->getName());
			}
			++i;
		}
	
}