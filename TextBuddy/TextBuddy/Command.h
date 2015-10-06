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
	// Command();
	Command(CommandType newCommand=INVALID, std::string restOfUserCommand="", std::string inputString="");
	~Command();

	CommandType getCommand();
	std::string getRestOfCommand();
	std::string getUserInput();
};

class Add: public Command {
private:
	Task newTask;
public:
	Add();
	Add(std::string userInput);
	~Add();
	Task getNewTask();
	void setNewTask(Task aTask);
};

class Delete: public Command {
private:
	int deleteID;
public:
	Delete();
	~Delete();
	int getDeleteID();
	void setDeleteID(int index);
};

class Modify: public Command {
private:
	int modifyID;
	std::vector<FieldType> fieldsToModify;
	Task tempTask;
public:
	Modify();
	Modify(int taskID, std::vector<FieldType> fields, Task task);
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
	void setSearchPhrase(std::string phr);
};

// Classes with no methods for CLEAR_ALL, DISPLAY_ALL, SORT_ALL, EXIT

class ClearAll: public Command {
public:
	ClearAll();
	~ClearAll();
};

class DisplayAll: public Command {
public:
	DisplayAll();
	~DisplayAll();
};

class SortAll: public Command {
public:
	SortAll();
	~SortAll();
};

class Exit: public Command {
public:
	Exit();
	~Exit();
};

#endif