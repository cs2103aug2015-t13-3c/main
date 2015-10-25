#include "TextBuddyUI.h"

using namespace UserInterface;

void TextBuddyUI::autoComplete() {
	int position = findKeyword(SEARCH);
	if(keywordIsFound(position) && position == 0) {
		getInput();
		if(input->Text != SEARCH) { 
			processAndExecute();
		} 
	}
}

void TextBuddyUI::undoSearch() {
	if(input->Text == SEARCH) {
		*userInput = "display";	
		processAndExecute();	
	}
}