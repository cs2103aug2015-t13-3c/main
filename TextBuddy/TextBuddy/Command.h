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

	SAVE,
	EXIT,
	INVALID
};

class Command {
private:
	CommandType cmd;
	std::string userInput;
public:
	// Command();
	Command(CommandType newCmd=INVALID, std::string rawInput="");
	~Command();

	CommandType getCommand();
	std::string getUserInput();
};

// ==================================================
//                  DERIVED COMMANDS
// ==================================================

class Add: public Command {
private:
	Task newTask;
public:
	Add();
	Add(Task task, std::string rawInput="");
	~Add();
	Task getNewTask();

	// NOTE TO KIAT BOON: Public setter (used in LogicTest.cpp) to be removed (Aaron)
	void setNewTask(Task task);
};

class Delete: public Command {
private:
	int deleteID;
public:
	Delete();
	Delete(int taskID);
	~Delete();
	int getDeleteID();
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
};

class Search: public Command {
private:
	std::string searchPhrase;
public:
	Search();
	Search(std::string phraseString);
	~Search();
	std::string getSearchPhrase();
};

// Classes with no methods for CLEAR_ALL, DISPLAY_ALL, SORT_ALL, SAVE, EXIT

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

class Save: public Command {
public:
	Save();
	~Save();
};

class Exit: public Command {
public:
	Exit();
	~Exit();
};

#endif