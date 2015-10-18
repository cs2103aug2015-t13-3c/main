// @@author Ng Ren Zhi

#include "History.h"
#include "stdafx.h"

History::History(void) {
}


History::~History(void) {
}

History * History::theOne;

//=========== PUBLIC METHODS ===================
History* History::getInstance() {
	if (theOne == NULL) {
		theOne = new History();
	}
	return theOne;
}

//add new command into history stack
void History::add(Command cmd) {
	commandHistory.push_back(cmd);
}

// undo most recent command in history stack
void History::undo() {
	Command cmdToUndo = commandHistory.back();
	cmdToUndo.undo();
	commandHistory.pop_back(); // Haven't implement redo yet
	return;
}