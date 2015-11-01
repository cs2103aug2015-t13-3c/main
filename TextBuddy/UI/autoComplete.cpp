// Soon Hao Ye @@author A0126677U

#include "TextBuddyUI.h"

using namespace UserInterface;

void TextBuddyUI::autoComplete() {
	searchAutoComplete();
	commandAutoComplete();
	viewAutoComplete();
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
/*
void TextBuddyUI::commandAutoComplete() {
for each(String^ command in keywords) {
if(command == STAR || String::IsNullOrEmpty(input->Text) ) {
return;
}
if(command == DEL || command == DONE || command == DISPLAY || command == SAVE || command == SEARCH) {
if(input->Text->Length > 1) {
if(command->IndexOf(input->Text[0]) == 0 && 
command->IndexOf(input->Text[1]) == 1) {
if(cursorPosition < command->Length) {
input->Text = command;
int length = (command->Length) - 2;
input->Select(cursorPosition,length);
}
}
}
} else if(command->IndexOf(input->Text[0]) == 0) {
if(cursorPosition < command->Length) {
input->Text = command;
int length = (command->Length) - 2;
input->Select(cursorPosition,length);
}
}

}
}
*/
void TextBuddyUI::viewAutoComplete() {
	int position = findKeyword(VIEW);
	if(keywordIsFound(position) && position == 0) {
		if(input->Text->Length > 5) {
			char c = input->Text[5];
			if(c == 'a') {
				String^ keyword = "all";
				putSuggestedText(keyword);
			} else if(c == 'e') {
				String^ keyword = "event";
				putSuggestedText(keyword);
			} else if(c == 'f') {
				String^ keyword = "floating";
				putSuggestedText(keyword);
			} else if(c == 'p') {
				String^ keyword = "past";
				putSuggestedText(keyword);
			} else if(c == 's') {
				String^ keyword = "star";
				putSuggestedText(keyword);
			} else if(c == 't') {
				String^ keyword = "todo";
				putSuggestedText(keyword);
			} else if(c == 'w') {
				String^ keyword = "week";
				putSuggestedText(keyword);
			} else {
				// Testing
				// putSuggestedText("bla");
			}
		}
	}
}

void TextBuddyUI::putSuggestedText(String^ keyword) {
	if(cursorPosition > 4 + keyword->Length) {
		return;
	}
	if(input->Text[6] != keyword[1]) {
		return;
	}
	input->Select(5,input->Text->Length - 6);
	input->SelectedText = keyword;
	input->Select(cursorPosition,keyword->Length);
}

void TextBuddyUI::undoSearch() {
	if(input->Text == SEARCH) {
		*userInput = "display";	
		processAndExecute();	
	}
}

System::Void TextBuddyUI::dropDown_DropDown(System::Object^ sender, System::EventArgs^ e) {

}