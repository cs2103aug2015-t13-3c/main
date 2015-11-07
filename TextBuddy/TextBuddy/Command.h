// Created and maintained by Aaron Chong Jun Hao @@author A0110376N
// Modified to Command Pattern by Ng Ren Zhi

#ifndef COMMAND_H_
#define COMMAND_H_

#include "IO.h"

class Tb {
public:
	static bool firstLoad;
	static std::string MESSAGE_WELCOME;

	// These are the valid Command keywords
	// Count: 14
	static std::string COMMAND_ADD;
	static std::string COMMAND_DELETE;
	static std::string COMMAND_MODIFY;
	static std::string COMMAND_MODIFY_EDIT;		// Alternative keyword
	static std::string COMMAND_PICK_RESERVE;
	static std::string COMMAND_SEARCH;
	static std::string COMMAND_MARKDONE;
	static std::string COMMAND_UNMARKDONE;
	static std::string COMMAND_UNDO;
	static std::string COMMAND_REDO;
	static std::string COMMAND_VIEW;
	static std::string COMMAND_CLEAR_ALL;
	static std::string COMMAND_DISPLAY_ALL;
	static std::string COMMAND_LOAD;
	static std::string COMMAND_SAVE;
	static std::string COMMAND_EXIT;
};

// These are the Command enums
// Count: 14 + INVALID
enum CommandType {
	ADD,
	DELETE,
	MODIFY,
	PICK,
	SEARCH,
	POWERSEARCH,
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
const std::string VIEW_EVENT = "events";
const std::string VIEW_TODO = "todo";
const std::string VIEW_TODAY = "today";
const std::string VIEW_PAST = "past";
const std::string VIEW_WEEK = "week";
// const std::string VIEW_LABEL = "label";

// These are the View enums
// Count: 8 + VIEWTYPE_INVALID
enum ViewType {
	VIEWTYPE_ALL,
	VIEWTYPE_FLOATING,
	VIEWTYPE_EVENT,
	VIEWTYPE_TODO,
	VIEWTYPE_TODAY,
	VIEWTYPE_PAST,
	VIEWTYPE_WEEK,
	VIEWTYPE_PERIOD,
	VIEWTYPE_LABELS,
	VIEWTYPE_INVALID
};

class Command {
private:
	CommandType cmd;
	std::string userInput;

protected:
	static const std::string ERROR_INDEX_OUT_OF_BOUNDS;
	static const std::string ERROR_TASK_START_LATER_THAN_TASK_END;

	static std::vector<Task> currentView;
	static std::vector<Task> taskStore;

	TbLogger* logger;

	//===== FOR UNDO =====
	std::vector<Task>::iterator currViewIter;
	std::vector<Task>::iterator taskStoreIter;
	int currViewPos;
	int taskStorePos;

	bool updateCurrView();
	void updateViewIter();
	void defaultView();

	void initialiseIterators(int taskID);
	void getIterator();

	// Returns false if start is later than end, by checking date then time
	bool isDateLogical(Task task);

	void sortFloating(std::vector<Task> &taskVector);
	void sortPriority(std::vector<Task> &taskVector);
	void sortDate(std::vector<Task> &taskVector);
	void removeDoneTasks(std::vector<Task> & taskVector); // Removes done tasks from currentView
	void removeTaskType(std::vector<Task> &taskVector, TaskType type);
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
	bool isOverlap;
	void checkOverlap();
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
	// void moveToPrevPos();

	bool updateFLOATING();
	bool updateTODO();
	bool updateEVENT();
public:
	Modify(int taskID, std::vector<FieldType> fields, Task task);
	Modify(CommandType pick);
	~Modify();
	int getModifyID();
	std::vector<FieldType> getFieldsToModify();
	Task getTempTask();
	void updateTaskTypes();

	void execute();
	void undo();
	std::string getMessage();
};

class Pick: public Modify {
private:
	//== EXECUTE ==
	int modifyID; // ID on GUI, not taskID
	bool pickReserve;
	//==== UNDO ===
	Task originalTask;
	int prevCurrPos;

	void doPick();

public:
	Pick(int taskID, bool isPick);
	~Pick();

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
	std::string doRegexSearch();
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
	std::string taskName;		// Added to solve iterator error for getMessage
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
	std::vector<std::string> periodParams;
	std::string periodString;
	//==== UNDO ===
	std::vector<Task> previousView;

	bool viewAll();
	bool viewTaskType(TaskType type);
	bool viewDone();
	bool viewToday();
	bool viewLabel(std::vector<std::string> label);
	void viewPeriod(int startDate, int EndDate, int StartTime, int EndTime);
public:
	View(ViewType newView,std::string labels);
	View(std::vector<std::string> viewParameters, std::string periodInput, ViewType period=VIEWTYPE_PERIOD);
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