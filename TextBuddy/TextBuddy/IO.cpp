// @@author Ng Ren Zhi

#include "stdafx.h"
#include "IO.h"

IO::IO() {
	isActive = false;
}

IO::IO(bool status): isActive(status) {}

IO::~IO() {}

bool IO::getStatus() {
	return isActive;
}
