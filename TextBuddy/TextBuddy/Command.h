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

class Add: public Command {
private:
	Task newTask;
public:
	Task getNewTask();
	void setNewTask(Task aTask); //testing
};

class Delete: public Command {
private:
	int taskToDelete;
public:
	int getTaskToDelete();
	void setTaskToDelete(int index); //testing
};

class Modify: public Command {
private:
	std::vector<FieldType> fieldsToModify;
	Task tempTask;
public:
	std::vector<FieldType> getFieldsToModify();
	Task getTempTask();
};

class Search: public Command {
private:
	std::string searchPhrase;
public:
	std::string getSearchPhrase();
	void setSearchPhrase(std::string phr); //testing
};

#endif