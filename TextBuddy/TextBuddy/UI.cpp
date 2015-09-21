// @@author Soon Hao Ye

#include "stdafx.h"
#include "UI.h"

UI::UI() {
	isActive = false;
}

UI::UI(bool status): isActive(status), parser(ON), logic(ON) {}

UI::~UI() {}

bool UI::getStatus() {
	return isActive && canCallParser() && canCallLogic();
}

bool UI::canCallParser() {
	return parser.getStatus();
}

bool UI::canCallLogic() {
	return logic.getStatus();
}
