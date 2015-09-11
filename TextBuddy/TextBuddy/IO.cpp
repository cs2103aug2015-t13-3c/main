#include "stdafx.h"
#include "IO.h"

IO::IO() {
	isActive = false;
}

IO::IO(bool status): isActive(status) {
}

bool IO::getStatus() {
	return isActive;
}
