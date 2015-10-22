// @@author A0110376N (Aaron Chong Jun Hao)
// Modified to Command Pattern by Ng Ren Zhi

#ifndef COMMAND_H_
#define COMMAND_H_

#include <vector>
#include "IO.h"
#include "Task.h"
#include "PowerSearch.h"

// These are the valid Command keywords
// Count: 14
const std::string COMMAND_ADD = "add";
const std::string COMMAND_DELETE = "delete";
const std::string COMMAND_MODIFY = "modify";
const std::string COMMAND_SEARCH = "search";
const std::string COMMAND_MARKDONE = "done";
const std::string COMMAND_UNMARKDONE = "notdone";
const std::string COMMAND_UNDO = "undo";
const std::string COMMAND_REDO = "redo";
const std::string COMMAND_VIEW = "view";
const std::string COMMAND_CLEAR_ALL = "clear";
const std::string COMMAND_DISPLAY_ALL = "display";
const std::string COMMAND_LOAD = "load";
const std::string COMMAND_SAVE = "save";
const std::string COMMAND_EXIT = "exit";

// These are the Command enums
// Count: 14 + INVALID
enum CommandType {
	ADD,
	DELETE,
	MODIFY,
	SEARCH,
	MARKDONE,
	UNMARKDONE,
	UNDO,
	REDO,
	VIEW,
	CLEAR_ALL,
	DISPLAY_ALL,
	LOAD,
	SAVE,
	EXIT,
	INVALID
};

// These are the valid View keywords
// Count: 7
const std::string VIEW_ALL = "all";
const std::string VIEW_FLOATING = "floating";
const std::string VIEW_PAST = "past";
const std::string VIEW_TODO = "todo";
const std::string VIEW_WEEK = "week";
const std::string VIEW_LABEL = "label";
const std::string VIEW_NOTDONE = "notdone";

// These are the View enums
// Count: 6 + VIEWTYPE_INVALID
enum ViewType {
	VIEWTYPE_ALL,
	VIEWTYPE_FLOATING,
	VIEWTYPE_PAST,
	VIEWTYPE_TODO,
	VIEWTYPE_WEEK,
	VIEWTYPE_LABELS,
	VIEWTYPE_NOTDONE,
	VIEWTYPE_INVALID
};

class Command {
private:
	CommandType cmd;
	std::string userInput;

protected:
	static std::vector<Task> currentView;
	static std::vector<Task> taskStore;

	static const std::string ERROR_INDEX_OUT_OF_BOUNDS;

	bool copyView();
	bool sortDate(std::vector<Task> &taskVector);

	void matchIndex(int index, std::vector<Task>::iterator &currIter, 
		std::vector<Task>::iterator &taskIter);
	bool isValidIndex(int index);
	std::vector<Task>::iterator matchCurrentViewIndex(int index);
	std::vector<Task>::iterator matchTaskStoreIndex(int index);

public:
	Command(CommandType newCmd=INVALID, std::string rawInput="");
	CommandType getCommand();
	std::string getUserInput();

	static std::vector<Task> getCurrentView();
	static std::vector<Task>* getCurrentViewPtr();
	static std::vector<Task> getTaskStore();
	static int getSize();
	static void clearTaskStore();

	virtual ~Command();
	virtual void execute();
	virtual std::string getMessage();
	virtual void undo();
};

//==================================================
//                  DERIVED COMMANDS
//==================================================

class Add: public Command {
private:
	//== EXECUTE ==
	Task newTask;
	//==== UNDO ===
	int currViewID;

	bool addInfo();
public:
	Add(Task task);
	~Add();
	Task getNewTask();

	void execute();
	void undo();
	std::string getMessage();
};

class Delete: public Command {
private:
	//== EXECUTE ==
	int deleteID; // ID on GUI, not taskID
	//==== UNDO ===
	Task taskToBeDeleted;
	std::vector<Task>::iterator currViewIter;
	std::vector<Task>::iterator taskStoreIter;

	void deleteInfo();
public:
	Delete(int taskID);
	~Delete();
	int getDeleteID();

	void execute();
	void undo();
	std::string getMessage();
};

class Modify: public Command {
private:
	//== EXECUTE ==
	int modifyID; // ID on GUI, not taskID
	std::vector<FieldType> fieldsToModify;
	Task tempTask;
	//==== UNDO ===
	Task originalTask;

	void modifyInfo();
public:
	Modify(int taskID, std::vector<FieldType> fields, Task task);
	~Modify();
	int getModifyID();
	std::vector<FieldType> getFieldsToModify();
	Task getTempTask();

	void execute();
	void undo();
	std::string getMessage();
};

class Search: public Command {
private:
	//== EXECUTE ==
	std::string searchPhrase;
	//==== UNDO ===
	std::vector<Task> currentViewBeforeSearch;

	std::string searchInfo();
	bool amendView(std::string listOfIds);
public:
	Search(std::string phraseString);
	~Search();
	std::string getSearchPhrase();

	void execute();
	void undo();
	std::string getMessage();
};

class Markdone: public Command {
private:
	//== EXECUTE ==
	int doneID;
	//==== UNDO ===
	bool successMarkDone;
	std::vector<Task>::iterator currIter;
	std::vector<Task>::iterator taskIter;

	void markDone();
public:
	Markdone(int taskID);
	~Markdone();
	int getDoneID();

	void execute();
	void undo();
};

class UnmarkDone: public Command {
private:
	//== EXECUTE ==
	int undoneID;
	//==== UNDO ===
	bool successUnmarkDone;
	std::vector<Task>::iterator currIter;
	std::vector<Task>::iterator taskIter;

	void unmarkDone();
public:
	UnmarkDone(int taskID);
	~UnmarkDone();
	int getUndoneID();

	void execute();
	void undo();
};

class Undo: public Command {
public:
	Undo();
	~Undo();
};

class View: public Command {
private:
	//== EXECUTE ==
	ViewType view;
	std::vector<std::string> viewLabels;
	PowerSearch pwrSearch;
	//==== UNDO ===
	std::vector<Task> previousView;

	bool viewAll();
	bool viewTaskType(TaskType type);
	bool viewDone();
	bool viewNotdone();
	bool viewLabel(std::string label);

public:
	View(ViewType newView,std::string labels);
	~View();
	ViewType getViewType();

	void execute();
	void undo();
};

class ClearAll: public Command {
private:
	//==== UNDO ===
	std::vector<Task> previousView;
public:
	ClearAll();
	~ClearAll();

	void execute();
	void undo();
};

class DisplayAll: public Command {
private:
	//==== UNDO ===
	std::vector<Task> previousView;
public:
	DisplayAll();
	~DisplayAll();

	void execute();
	void undo();
};

class Load: public Command {
private:
	IO* io;
	std::string filePath;
public:
	Load();
	Load(std::string Load);
	~Load();
	std::string getFilePath();

	void execute();
	void formatDefaultView();
};

class Save: public Command {
private:
	IO* io;
	std::string filePath;
public:
	Save();
	Save(std::string filePath);	
	~Save();
	std::string getFilePath();

	void execute();
	// No undo() for Save
};

class Exit: public Command {
public:
	Exit();
	~Exit();

	void execute();
};

#endif