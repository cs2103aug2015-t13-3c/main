// @@author Soon Hao Ye

#ifndef UI_H_
#define UI_H_

#include "Parser.h"
#include "Logic.h"

class UI {
private:
	Parser p;
	Logic l;
	// bool isActive;
	// Parser parser;
	// Logic logic;

	// These messages are shown to the user
	static const std::string MESSAGE_WELCOME;
	static const std::string MESSAGE_PROGRAM_TERMINATION;

	// These indicate errors in user commands
	static const std::string ERROR_STARTUP;

public:
	UI();
	UI(int argc, char* argv[]);
	~UI();
	// UI(bool status);
	// bool getStatus();
	// bool canCallParser();
	// bool canCallLogic();

	// This is run on startup
	static void checkStartCommand(int argc, char* argv[]);

	// These are UI methods
	static std::string readUserCommand();
	static std::string processCommand(std::string userCommand);
	static bool showToUser(std::string formattedText, std::string param1="");
	static void terminateProgram();

};

#endif