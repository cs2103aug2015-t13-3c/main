// @@author Soon Hao Ye

#ifndef UI_H_
#define UI_H_

#include "Parser.h"
#include "Logic.h"

class UI{
private:
	bool isActive;
	Parser parser;
	Logic logic;

public:
	UI();
	UI(bool status);
	~UI();

	bool getStatus();
	bool canCallParser();
	bool canCallLogic();
};

#endif