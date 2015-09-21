// @@author Chin Boon Kiat

#include "stdafx.h"
#include "Logic.h"

Logic::Logic() {
	isActive = false;
}

Logic::Logic(bool status): isActive(status), io(ON) {}

Logic::~Logic() {}

bool Logic::getStatus() {
	return isActive && canCallIO();
}

bool Logic::canCallIO()
{
	return io.getStatus();
}
