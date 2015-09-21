// @@author Aaron Chong Jun Hao

#ifndef COMMAND_H_
#define COMMAND_H_

enum CommandType {
	ADD,
	DELETE,
	SEARCH,

	CLEAR_ALL,
	DISPLAY_ALL,
	SORT_ALL,

	EXIT,
	INVALID
};

class Command {
private:
	CommandType cmd;
	std::string restOfCommand;
	std::string userInput;

public:
	Command();
	Command(CommandType newCommand, std::string restOfUserCommand, std::string inputString);
	~Command();

	CommandType getCommand();
	std::string getRestOfCommand();
	std::string getUserInput();

};

#endif