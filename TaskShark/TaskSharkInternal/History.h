// Ng Ren Zhi @@author A0130463R

#ifndef HISTORY_H_
#define HISTORY_H_

// Holds a stack of executed commands
class History {
private:
	static History* theOne; // Singleton
	History();

	static std::vector<Command*> commandHistory;
	static std::vector<Command*> redoHistory;

	bool checkUndoableCommand(Command* cmd);

public:
	static History* getInstance();
	~History();
	int getUndoSize();

	void add(Command *cmd);
	void undo();
	void redo();
	void clearHistory();
};

#endif