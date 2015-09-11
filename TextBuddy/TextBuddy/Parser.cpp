#include "stdafx.h"
#include "Parser.h"

Parser::Parser() {
	isActive = false;
}

Parser::Parser(bool status): isActive(status) {
}

bool Parser::getStatus() {
	return isActive;
}
