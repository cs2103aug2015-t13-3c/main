// @@author Aaron Chong Jun Hao

#ifndef COMMAND_H_
#define COMMAND_H_

enum CommandType {
	ADD,
	DELETE,
	MODIFY,
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
	Command(CommandType newCommand, std::string restOfUserCommand="", std::string inputString="");
	virtual ~Command();

	CommandType getCommand();
	std::string getRestOfCommand();
	std::string getUserInput();

	void setCmdType(CommandType cmdType);				// Used for CLEAR_ALL, DISPLAY_ALL, SORT_ALL
	virtual void setNewTask(Task task);					// Add
	virtual void setTaskToDelete(int index);			// Delete
	virtual std::vector<FieldType> getFieldsToModify();	// Modify
	virtual void setSearchPhrase(std::string phr);		// Search
};

class Add: public Command {
private:
	Task newTask;
public:
	Add();
	~Add();
	Task getNewTask();
	void setNewTask(Task aTask) override; //testing
};

class Delete: public Command {
private:
	int taskToDelete;
public:
	Delete();
	~Delete();
	int getTaskToDelete();
	void setTaskToDelete(int index) override; //testing
};

class Modify: public Command {
private:
	std::vector<FieldType> fieldsToModify;
	Task tempTask;
public:
	Modify();
	~Modify();
	std::vector<FieldType> getFieldsToModify() override;
	Task getTempTask();
};

class Search: public Command {
private:
	std::string searchPhrase;
public:
	Search();
	~Search();
	std::string getSearchPhrase();
	void setSearchPhrase(std::string phr) override; //testing
};

#endif