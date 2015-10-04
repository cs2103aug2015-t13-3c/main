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

	// Used for CLEAR_ALL, DISPLAY_ALL, SORT_ALL
	void setCmdType(CommandType cmdType);

};

class Add: public Command {
private:
	Task newTask;
public:
	Add();
	~Add();
	Task getNewTask();
	void setNewTask(Task aTask); //testing
};

class Delete: public Command {
private:
	int deleteID;
public:
	Delete();
	~Delete();
	int getDeleteID();
	void setDeleteID(int index); //testing
};

class Modify: public Command {
private:
	int modifyID;
	std::vector<FieldType> fieldsToModify;
	Task tempTask;
public:
	Modify();
	~Modify();
	int getModifyID();
	std::vector<FieldType> getFieldsToModify();
	Task getTempTask();
	void setModifyID(int index);
	void setFieldsToModify(std::vector<FieldType> fields);
	void setTempTask(Task task);
	
	
};

class Search: public Command {
private:
	std::string searchPhrase;
public:
	Search();
	~Search();
	std::string getSearchPhrase();
	void setSearchPhrase(std::string phr); //testing
};

#endif