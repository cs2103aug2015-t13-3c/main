// @@author Aaron Chong Jun Hao
// Modified to Command pattern (Ren Zhi)

#include "Feedback.h"
#include "IO.h"
#include <vector>

#ifndef COMMAND_H_
#define COMMAND_H_

// These are the possible command types
// const std::string COMMAND_CLEAR_ALL = "clear";
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
	REDO,
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

protected:
	static std::vector<Task> taskStore;
	static std::vector<Task> currentView;
	Feedback feedback;

	const static std::string ERROR_INDEX_OUT_OF_BOUNDS;

	bool sortDate(std::vector<Task> &taskVector);
	bool copyView();

	//added by haoye
	void matchIndex(int index, std::vector<Task>::iterator &currIter, 
	std::vector<Task>::iterator &taskIter);
	std::vector<Task>::iterator matchCurrentViewIndex(int index);
	std::vector<Task>::iterator matchTaskViewIndex(int index);
	bool isValidIndex(int index);

public:
	Command(CommandType newCmd=INVALID, std::string rawInput="");
	~Command();
	
	CommandType getCommand();
	std::string getUserInput();
	static std::vector<Task> getTaskStore();
	static std::vector<Task> getCurrentView();
	static int getSize();
	static void clearTaskStore();

	/*virtual*/ void execute();
	/*virtual*/ void undo();
};

// ==================================================
//                  DERIVED COMMANDS
// ==================================================

class Add: public Command {
private:
	// == EXECUTE ==
	Task newTask;
	// ==== UNDO ===
	int currViewID;

	bool addInfo();

public:
	Add(Task task);
	~Add();
	Task getNewTask();

	void execute();
	void undo();
};

class Delete: public Command {
private:
	// == EXECUTE ==
	int deleteID; //ID on GUI, not Task ID
	// ==== UNDO ===
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
};

class Modify: public Command {
private:
	// == EXECUTE ==
	int modifyID; // GUI ID, not task ID
	std::vector<FieldType> fieldsToModify;
	Task tempTask;
	// ==== UNDO ===
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
};

class Search: public Command {
private:
	// == EXECUTE ==
	std::string searchPhrase;
	// ==== UNDO ===
	std::vector<Task> currentViewBeforeSearch;

	std::string searchInfo();
	bool amendView(std::string listOfIds);
public:
	Search(std::string phraseString);
	~Search();
	std::string getSearchPhrase();

	void execute();
	void undo();
};

class Markdone: public Command {
private:
	// == EXECUTE ==
	int doneID;
	// ==== UNDO ===
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
	// == EXECUTE ==
	int undoneID;
	// ==== UNDO ===
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
	ViewType view;
public:
	View(ViewType newView);
	~View();
	ViewType getViewType();

	void execute();
	void undo();
};

class DisplayAll: public Command {
private:
	// ==== UNDO ===
	std::vector<Task> previousView;
public:
	DisplayAll();
	~DisplayAll();

	void execute();
	void undo();
};

class Load: public Command {
private:
	std::string filePath;
public:
	Load(std::string Load);
	~Load();
	std::string getFilePath();

	void execute();
};

class Save: public Command {
private:
	//TODO: make IO singleton
	IO io;
	std::string filePath;
public:
	Save();
	Save(std::string filePath);	
	~Save();
	std::string getFilePath();

	void execute();
	// no undo for save
};

class Exit: public Command {
public:
	Exit();
	~Exit();

	void execute();
};

#endif