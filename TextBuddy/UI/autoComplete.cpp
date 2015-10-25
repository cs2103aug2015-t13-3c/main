// Soon Hao Ye @@author A0126677U

#include "TextBuddyUI.h"

using namespace UserInterface;

void TextBuddyUI::autoComplete() {
	searchAutoComplete();
	commandAutoComplete();
}

void TextBuddyUI::searchAutoComplete() {
	int position = findKeyword(SEARCH);
	if(keywordIsFound(position) && position == 0) {
		getInput();
		if(input->Text != SEARCH) { 
			processAndExecute();
		} 
	}
}

void TextBuddyUI::commandAutoComplete() {
	for each(String^ command in keywords) {
		if(command == STAR || String::IsNullOrEmpty(input->Text) ) {
			return;
		}
		if(command->IndexOf(input->Text[0]) == 0) {
			if(cursorPosition < command->Length) {
				input->Text = command;
				input->Select(cursorPosition,(command->Length) - 1);
			}
		}

	}
}

void TextBuddyUI::undoSearch() {
	if(input->Text == SEARCH) {
		*userInput = "display";	
		processAndExecute();	
	}
}