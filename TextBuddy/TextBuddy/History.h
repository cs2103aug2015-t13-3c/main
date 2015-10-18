// @@author Ng Ren Zhi

#include "Command.h"

#ifndef HISTORY_H_
#define HISTORY_H_

// Holds a stack of executed commands
class History {
private:
	std::vector<Command> commandHistory;
	
	static History *theOne; // Singleton
	History(); 

public:
	static History* getInstance();
	~History();

	void add(Command cmd);
	void undo();
};

#endif