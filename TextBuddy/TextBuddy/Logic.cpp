// Chin Kiat Boon @@author A0096720A

#include "stdafx.h"
#include "Logic.h"

const std::string Logic::ERROR_NO_INPUT = "No input";
const std::string Logic::ERROR_INVALID_COMMAND = "Invalid command entered";

Logic* Logic::theOne = new Logic();

DisplayMode Logic::mode=TODAY;

Logic::Logic() {
	history = History::getInstance();
	parser = Parser::getInstance();
	io = IO::getInstance();
	Command temp;
	currentView = temp.getCurrentViewPtr();
	updater = nullptr;
}

Logic::~Logic() {
	updater->~Update();
	delete updater;
}

//==================================================
//                      METHODS
//==================================================

Logic* Logic::getInstance() {
	Load initialLoad(IO::getInstance()->getFilePath());
	try {
		initialLoad.execute();
	} catch (std::exception e) {
		TbLogger::getInstance()->log(WARN,"File not found: " + IO::getInstance()->getFilePath());
	}
	return theOne;
}

std::string Logic::processCommand(std::string userCommand) {
	if(userCommand.empty()) {
		throw std::runtime_error(ERROR_NO_INPUT);
	}
	std::string message;
	Command* command;
	command = parser->parse(userCommand);
	CommandType cmd = command->getCommand();

	switch (cmd) {
	case INVALID:
		throw std::runtime_error(ERROR_INVALID_COMMAND);
	case UNDO:
	case REDO:
		command->execute();
		break;
	default:
		command->execute();
		message = command->getMessage();
		history->add(command);
	}

	assert(updater != nullptr);
	updater->update();
	io->saveFile(io->getFilePath(),Command::getTaskStore());
	// Save saveFile;
	// saveFile.execute();
	return message;
}

void Logic::subscribe(std::vector<DisplayedTask>* tasks) {
	assert(updater == nullptr);
	updater = new Update(tasks,currentView);
	updater->update();
}

DisplayMode Logic::getMode() {
	return mode;
}

void Logic::setAllMode() {
	mode = ALL;
}

void Logic::setTodayMode() {
	mode = TODAY;
}

void Logic::setWeekMode() {
	mode = WEEK;
}

void Logic::setEventsMode() {
	mode = EVENTS;
}

void Logic::setDeadlinesMode() {
	mode = DEADLINES;
}

void Logic::setFloatingMode() {
	mode = FLOATINGS;
}

void Logic::setSearchMode() {
	mode = SEARCHES;
}

void Logic::setPastMode() {
	mode = PAST_;
}

void Logic::resetUpdaterNULL() {
	updater = nullptr;
}
