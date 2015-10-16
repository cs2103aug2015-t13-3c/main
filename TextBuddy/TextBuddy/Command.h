// @@author Aaron Chong Jun Hao

#ifndef COMMAND_H_
#define COMMAND_H_

// These are the possible command types
// const std::string COMMAND_SORT_ALL = "sort";
const std::string COMMAND_ADD = "add";
const std::string COMMAND_DELETE = "delete";
const std::string COMMAND_MODIFY = "modify";
const std::string COMMAND_SEARCH = "search";
const std::string COMMAND_MARKDONE = "done";
const std::string COMMAND_UNDO = "undo";
const std::string COMMAND_VIEW = "view";
const std::string COMMAND_CLEAR_ALL = "clear";
const std::string COMMAND_DISPLAY_ALL = "display";
const std::string COMMAND_LOAD = "load";
const std::string COMMAND_SAVE = "save";
const std::string COMMAND_EXIT = "exit";

// These are the possible views
const std::string VIEW_ALL = "all";
const std::string VIEW_FLOATING = "floating";
const std::string VIEW_PAST = "past";
const std::string VIEW_TODO = "todo";
const std::string VIEW_WEEK = "week";

enum CommandType {
	ADD,
	DELETE,
	MODIFY,
	SEARCH,
	MARKDONE,
	UNDO,
	VIEW,
	CLEAR_ALL,
	DISPLAY_ALL,
	LOAD,
	SAVE,
	EXIT,
	INVALID
};

enum ViewType {
	VIEWTYPE_ALL,
	VIEWTYPE_FLOATING,
	VIEWTYPE_PAST,
	VIEWTYPE_TODO,
	VIEWTYPE_WEEK,
	VIEWTYPE_LABEL,
	VIEWTYPE_INVALID
};

class Command {
private:
	CommandType cmd;
	std::string userInput;
public:
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
	Add(Task task);
	~Add();
	Task getNewTask();
};

class Delete: public Command {
private:
	int deleteID;
public:
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
	Search(std::string phraseString);
	~Search();
	std::string getSearchPhrase();
};

class Markdone: public Command {
private:
	int doneID;
public:
	Markdone(int taskID);
	~Markdone();
	int getDoneID();
};

class Undo: public Command {
public:
	Undo();
	~Undo();
};

class View: public Command {
private:
	ViewType view;
public:
	View(ViewType newView);
	~View();
	ViewType getViewType();
};

class DisplayAll: public Command {
public:
	DisplayAll();
	~DisplayAll();
};

class Load: public Command {
private:
	std::string filePath;
public:
	Load(std::string Load);
	~Load();
	std::string getFilePath();
};

class Save: public Command {
private:
	std::string filePath;
public:
	Save(std::string filePath);
	~Save();
	std::string getFilePath();
};

class Exit: public Command {
public:
	Exit();
	~Exit();
};

#endif