// @@author Soon Hao Ye

#include "stdafx.h"
#include "UI.h"

UI::UI() {
	// isActive = false;
}

UI::UI(int argc, char* argv[]): p(), l() {
	checkStartCommand(argc, argv);
	std::string textBuddyFileName = p.parseFileName(argv);
	showToUser(MESSAGE_WELCOME, textBuddyFileName);
}

UI::~UI() {}

// These messages are shown to the user
const std::string UI::MESSAGE_WELCOME = "Welcome to TextBuddy. %s is ready for use\n";
const std::string UI::MESSAGE_PROGRAM_TERMINATION = "Press any key to terminate the program...\n";

// These indicate errors in user commands
const std::string UI::ERROR_STARTUP = "To start, enter: %s NAME_OF_FILE\n";

// ==================================================
//                      METHODS
// ==================================================

// This is run on startup
void UI::checkStartCommand(int argc, char* argv[]) {
	if(argc != 2) {
		showToUser(ERROR_STARTUP, argv[0]);
		terminateProgram();
	}

	Parser p;
	// Logic l;

	std::string newFileName = p.parseFileName(argv);
	/*
	bool canAccess = l.testSave(newFileName);

	if(!canAccess) {
		showToUser(ERROR_INVALID_FILE);
		terminateProgram();
	}
	*/
	return;
}

std::string UI::readUserCommand() {
	return "";
}

std::string UI::processCommand(std::string userCommand) {
	return "";
}
bool UI::showToUser(std::string formattedText, std::string param1) {
	return true;
}

void UI::terminateProgram() {
	showToUser(MESSAGE_PROGRAM_TERMINATION);
	getchar();
	exit(EXIT_FAILURE);
}

// ==================================================
//                      TO DELETE
// ==================================================

/*
UI::UI(bool status): isActive(status), parser(ON), logic(ON) {}

bool UI::getStatus() {
return isActive && canCallParser() && canCallLogic();
}

bool UI::canCallParser() {
return parser.getStatus();
}

bool UI::canCallLogic() {
return logic.getStatus();
}
*/
