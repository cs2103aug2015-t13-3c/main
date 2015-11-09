// Soon Hao Ye @@author A0126677U

#include "stdafx.h"
#include "Update.h"

Update::Update(std::vector<DisplayedTask>* tasksToDisplay,
			   std::vector<Task>* currentView ) {
				   this->tasksToDisplay = tasksToDisplay;
				   this->currentView = currentView;
}

Update::~Update() {
	if (tasksToDisplay != nullptr) {
		delete tasksToDisplay;
	}
}

void Update::update() {
	tasksToDisplay->clear();
	std::vector<Task>::iterator i = currentView->begin();
	while (i!=currentView->end()) {
		DisplayedTask task;
		task.label = i->getLabelString();
		task.description = i->getName();
		task.date = i->getDisplayDate();
		task.time = i->getDisplayTime();
		task.status = NORMAL;
		if (i->getPriorityStatus()) {
			task.status = PRIORITY;
		}
		if (i->isUrgent()) {
			task.status = URGENT;
		}
		if (i->getDoneStatus()) {
			task.status = PAST;
		}
		task.type = i->getType();
		tasksToDisplay->push_back(task);
		++i;
	}
}
