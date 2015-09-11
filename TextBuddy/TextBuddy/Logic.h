#ifndef LOGIC_H_
#define LOGIC_H_

#include "IO.h"

class Logic{
private:
	bool isActive;
	IO io;
public:
	Logic();
	Logic(bool status);
	bool getStatus();
	bool canCallIO();
};

#endif