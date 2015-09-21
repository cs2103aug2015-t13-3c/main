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

vector<string> IO::loadFile(string fileName) {
	ifstream newFile(fileName);

	vector<string> textVector;

	return textVector;
}