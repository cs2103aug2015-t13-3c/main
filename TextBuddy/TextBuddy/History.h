// @@author A0130463R (Ng Ren Zhi)

#ifndef HISTORY_H_
#define HISTORY_H_

#include "Command.h"

// Holds a stack of executed commands
class History {
private:
	static History* theOne; // Singleton
	History();

	static std::vector<Command*> commandHistory;
	static std::vector<Command*> redoHistory;

public:
	static History* getInstance();
	~History();
	int getUndoSize();

	void add(Command *cmd);
	void undo();
	void redo();
};

#endif