#include "stdafx.h"
#include "UI.h"

int main(void) {
	UI ui(ON);

	if(ui.getStatus() == ON) {
		std::cout << "All systems go!" << std::endl;
	} else {
		std::cout << "APIs not ready!" << std::endl;
	}

	return 0;
}