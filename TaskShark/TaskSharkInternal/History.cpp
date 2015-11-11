// Ng Ren Zhi @@author A0130463R

#include "stdafx.h"
#include "History.h"

History* History::theOne = nullptr;

std::vector<Command*> History::commandHistory;
std::vector<Command*> History::redoHistory;

History::History() {
	commandHistory.clear();
	redoHistory.clear();
}

History::~History() {}

//========== PUBLIC METHODS ==========

History* History::getInstance() {
	if (theOne == nullptr) {
		theOne = new History();
	}
	return theOne;
}

int History::getUndoSize() {
	return commandHistory.size();
}

// Add new command into History stack
void History::add(Command *cmd) {
	// cmd.execute();

	if(checkUndoableCommand(cmd)) {
		commandHistory.push_back(cmd);
		redoHistory.clear();
	}
}

// Undo most recent command in History stack
void History::undo() {
	if (commandHistory.size() == 0) {
		throw std::runtime_error("Nothing to undo");
	} else {
		Command* cmdToUndo = commandHistory.back();
		cmdToUndo->undo();
		redoHistory.push_back(cmdToUndo);
		commandHistory.pop_back();
	}
	return;
}

// Redo most recent undone action in redo stack
void History::redo() {
	if (redoHistory.size() == 0) {
		throw std::runtime_error("Nothing to redo");
	} else {
		Command* cmdToRedo = redoHistory.back();
		cmdToRedo->execute();
		commandHistory.push_back(cmdToRedo);
		redoHistory.pop_back();
	}
	return;
}

void History::clearHistory() {
	commandHistory.clear();
	redoHistory.clear();
}

bool History::checkUndoableCommand(Command* cmd) {
	CommandType type = cmd->getCommand();
	if(type == VIEW || type == SEARCH || type == POWERSEARCH || type == DISPLAY_ALL) {
		return false;
	} else {
		return true;
	}
}