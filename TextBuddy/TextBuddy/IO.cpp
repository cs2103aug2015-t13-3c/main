// @@author Ng Ren Zhi

#include "stdafx.h"
#include "IO.h"

IO::IO() {
	// isActive = false;
}

IO::~IO() {}

// ==================================================
//                      METHODS
// ==================================================

std::vector<Task> loadFile(std::string fileName) {
	std::ifstream newFile(fileName);

	std::vector<Task> taskVector;

	return taskVector;
}

static bool saveFile(std::string fileName, std::vector<Task>) {

}