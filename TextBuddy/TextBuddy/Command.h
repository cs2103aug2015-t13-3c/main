// @@author Aaron Chong Jun Hao

#ifndef COMMAND_H_
#define COMMAND_H_

// These are the possible command types
const std::string COMMAND_ADD = "add";
const std::string COMMAND_DELETE = "delete";
const std::string COMMAND_MODIFY = "modify";
const std::string COMMAND_SEARCH = "search";
const std::string COMMAND_CLEAR_ALL = "clear";
const std::string COMMAND_DISPLAY_ALL = "display";
const std::string COMMAND_SORT_ALL = "sort";
const std::string COMMAND_SAVE = "save";
const std::string COMMAND_EXIT = "exit";

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
	// Add();
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
	// Delete();
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
	// Modify();
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
	// Search();
	Search(std::string phraseString);
	~Search();
	std::string getSearchPhrase();
};

class Save: public Command {
private:
	std::string newFilePath;
public:
	Save(std::string filePath);
	~Save();

	std::string getFilePath();
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

class Exit: public Command {
public:
	Exit();
	~Exit();
};

#endif