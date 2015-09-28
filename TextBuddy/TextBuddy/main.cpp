/*
* National University of Singapore
* Module Info:  CS2103 Software Engineering
* Version Info: V0.1
*
* @@author Aaron Chong Jun Hao
* @@author Chin Kiat Boon
* @@author Ng Ren Zhi
* @@author Soon Hao Ye
*/

#include "stdafx.h"
#include "UI.h"

int main(int argc, char* argv[]) {
	UI textBuddy(argc,argv);
	bool isNotExit = true;

	while (isNotExit) {
		std::string userCommand = textBuddy.readUserCommand();        // empty method
		std::string feedback = textBuddy.processCommand(userCommand); // empty method
		isNotExit = textBuddy.showToUser(feedback);                   // empty method
		
		isNotExit = false; // to remove
	}	

	return 0;
}


// ==================================================
//                      TO DELETE
// ==================================================

/*
IO::IO(bool status): isActive(status) {}

bool IO::getStatus() {
return isActive;
}
*/

/*
UI ui(ON);
if(ui.getStatus() == ON) {
std::cout << "All systems go!" << std::endl;
} else {
std::cout << "APIs not ready!" << std::endl;
}*/
