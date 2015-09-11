#include "stdafx.h"
#include "UI.h"

int main(void) {
	UI ui(ON);
	std::cout << ui.getStatus() << std::endl;
	
	return 0;
}