// @@author A0130463R (Ng Ren Zhi)

// #include "stdafx.h"
#include "History.h"

History* History::theOne = new History();

History::History() {}

History::~History() {}

//========== PUBLIC METHODS ==========
History* History::getInstance() {
	return theOne;
}

// Add new command into History stack
void History::add(Command cmd) {
	// cmd.execute();
	commandHistory.push_back(cmd);
	redoHistory.clear();
}

// Undo most recent command in History stack
void History::undo() {
	if(commandHistory.size() == 0) {
		throw std::runtime_error("Nothing to undo");
	} else {
		Command cmdToUndo = commandHistory.back();
		cmdToUndo.undo();
		redoHistory.push_back(cmdToUndo);
		commandHistory.pop_back();
	}
	return;
}

// Redo most recent undone action in redo stack
void History::redo() {
	if(redoHistory.size() == 0) {
		throw std::runtime_error("Nothing to redo");
	} else {
		Command cmdToRedo = redoHistory.back();
		cmdToRedo.execute();
		commandHistory.push_back(cmdToRedo);
		redoHistory.pop_back();
	}
	return;
}
