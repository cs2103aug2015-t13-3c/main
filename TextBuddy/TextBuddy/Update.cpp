// Soon Hao Ye @@author A0126677U

#include "stdafx.h"
#include "Update.h"

Update::Update(std::vector<std::string>* labels,
			std::vector<std::string>* description,
			std::vector<std::string>* taskDate,
			std::vector<std::string>* taskTime,
			std::vector<std::string>* floatingTasks,
			std::vector<int>* color,
			std::vector<Task>* currentView ) {
	this->labels = labels;
	this->description = description;
	this->taskDate = taskDate;
	this->taskTime = taskTime;
	this->floatingTasks = floatingTasks;
	this->color = color;
	this->currentView = currentView;
}

Update::~Update() {}

void Update::update() {
		labels->clear();
		description->clear();
		taskDate->clear();
		taskTime->clear();
		color->clear();
		floatingTasks->clear();
		std::vector<Task>::iterator i = currentView->begin();
		while(i!=currentView->end()) {
			labels->push_back(i->getLabelString());
			description->push_back(i->getName());
			taskDate->push_back(i->getDate_UI());
			taskTime->push_back(i->getTime_UI());
			// 0 : grey
			// 1 : blue
			// 2 : red
			// 3 : black
			if(i->getDoneStatus()) {
				color->push_back(0);
			} else if(i->getPriorityStatus()) {
				color->push_back(1);
			} else if(i->isUrgent()) {
				color->push_back(2);
			} else {
				color->push_back(3);
			}
			if(i->getType() == FLOATING) {
				floatingTasks->push_back(i->getName());
			}
			++i;
		}
}