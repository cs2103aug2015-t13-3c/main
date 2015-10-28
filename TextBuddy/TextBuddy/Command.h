// Created and maintained by Aaron Chong Jun Hao @@author A0110376N
// Modified to Command Pattern by Ng Ren Zhi

#ifndef COMMAND_H_
#define COMMAND_H_

#include "IO.h"


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
// Count: 8
const std::string VIEW_ALL = "all";
const std::string VIEW_FLOATING = "floating";
const std::string VIEW_EVENT = "event";
const std::string VIEW_TODO = "todo";
const std::string VIEW_NOTDONE = "notdone";
const std::string VIEW_PAST = "past";
const std::string VIEW_WEEK = "week";
const std::string VIEW_LABEL = "label";

// These are the View enums
// Count: 8 + VIEWTYPE_INVALID
enum ViewType {
	VIEWTYPE_ALL,
	VIEWTYPE_FLOATING,
	VIEWTYPE_EVENT,
	VIEWTYPE_TODO,
	VIEWTYPE_NOTDONE,
	VIEWTYPE_PAST,
	VIEWTYPE_WEEK,
	VIEWTYPE_LABELS,
	VIEWTYPE_INVALID
};

class Command {
private:
	CommandType cmd;
	std::string userInput;

protected:
	static std::vector<Task> currentView;
	static std::vector<Task> taskStore;
	std::vector<Task> overlapPeriods;

	//===== FOR UNDO =====
	std::vector<Task>::iterator currViewIter;
	std::vector<Task>::iterator taskStoreIter;
	int currViewPos;
	int taskStorePos;

	static const std::string ERROR_INDEX_OUT_OF_BOUNDS;
	static const std::string ERROR_TASK_START_LATER_THAN_TASK_END;

	bool copyView();
	void updateView();

	void initialiseIterators(int taskID);
	void getIterator();

	// Returns false if start is later than end, by checking date then time
	bool isDateLogical(Task task);

	void sortFloating(std::vector<Task> &taskVector);
	void sortPriority(std::vector<Task> &taskVector);
	void sortDate(std::vector<Task> &taskVector);
	void removeDoneTasks(std::vector<Task> & taskVector); // Removes done tasks from currentView
	void findOverlapPeriods();
	void addPeriod(std::vector<Task> &taskVector, int startDate, int startTime, int endDate, int endTime);

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

	// To use virtual functions, declare and use a base class POINTER
	// to create a derived class object (Ren Zhi)
	virtual ~Command();
	virtual void execute();
	virtual void undo();
	virtual std::string getMessage();
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
	bool doAdd();
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

	void setUndoDeleteInfo();
	void doDelete();
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
	int prevCurrPos;

	void doModify();
	void moveToPrevPos();

	void updateTaskTypes();
	bool updateFLOATING();
	bool updateTODO();
	bool updateEVENT();

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

	std::string doSearch();
	bool amendView(std::string listOfIds);
public:
	Search(std::string phraseString);
	~Search();
	std::string getSearchPhrase();

	virtual void setTasksWithinPeriod(int startDate, int startTime, int endDate, int endTime);

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

	void markDone();
public:
	Markdone(int taskID);
	~Markdone();
	int getDoneID();

	void execute();
	void undo();

	std::string getMessage();
};

class UnmarkDone: public Command {
private:
	//== EXECUTE ==
	int undoneID;
	//==== UNDO ===
	bool successUnmarkDone;

	void unmarkDone();
public:
	UnmarkDone(int taskID);
	~UnmarkDone();
	int getUndoneID();

	void execute();
	void undo();

	std::string getMessage();
};

class View: public Command {
private:
	//== EXECUTE ==
	ViewType view;
	std::vector<std::string> viewLabels;
	//==== UNDO ===
	std::vector<Task> previousView;

	bool viewAll();
	bool viewTaskType(TaskType type);
	bool viewDone();
	bool viewNotdone();
	bool viewLabel(std::vector<std::string> label);
	void viewWeek(int startDate, int EndDate, int StartTime, int EndTime);
public:
	View(ViewType newView,std::string labels);
	~View();
	ViewType getViewType();

	void execute();
	void undo();

	std::string getMessage();
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

	std::string getMessage();
};

class DisplayAll: public Command {
private:
	//==== UNDO ===
	std::vector<Task> previousView;
	
	void formatDefaultView();
public:
	DisplayAll();
	~DisplayAll();

	void execute();
	void undo();

	std::string getMessage();
};

class Undo: public Command {
public:
	Undo();
	~Undo();

	void execute();
};

class Redo: public Command {
public:
	Redo();
	~Redo();

	void execute();
};

class Load: public Command {
private:
	IO* io;
	std::string filePath;
	bool loadSuccess;

public:
	Load();
	Load(std::string Load);
	~Load();
	std::string getFilePath();

	void execute();

	std::string getMessage();
};

class Save: public Command {
private:
	IO* io;
	std::string filePath;
	bool saveSuccess;

public:
	Save();
	Save(std::string filePath);	
	~Save();
	std::string getFilePath();

	void execute();

	std::string getMessage();
};

class Exit: public Command {
public:
	Exit();
	~Exit();

	void execute();
};

#endif