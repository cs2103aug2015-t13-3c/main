// Soon Hao Ye @@author A0126677U

#include "UI.h"

using namespace UserInterface;

// if the selected index in the dropdown menu is not the last item, move the 
// selection to the next item

void UI::incrementDropCount() {
	assert(dropDown->DroppedDown);
	if (dropDown->SelectedIndex + 1 < dropDown->Items->Count) {
		dropDown->SelectedIndex++;		
	}
}

void UI::decrementDropCount() {
	if (dropDown->SelectedIndex > 0 ) {
		dropDown->SelectedIndex--;
	} else {
		dropDown->DroppedDown = false;
	}
}

// copies the selected item in the dropdown menu into the userInput textbox, 
// and closes the dropdown menu.

bool UI::acceptAutosuggest() {
	assert(dropDown->DroppedDown);
	if (dropDown->SelectedIndex > -1) {
		String^ command = dropDown->SelectedItem->ToString();
		input->Clear();
		input->Text = command;
		selectFields();	
		dropDown->DroppedDown = false;
		return true;
	}
	dropDown->DroppedDown = false;
	return false;
}

// Loops through the list of command keywords, if the first few characters of 
// a certain keyword is matched, activates the dropdown menu with a list of 
// items corresponding to the command 

void UI::commandAutoComplete() {
	bool found = false;
	for each(String^ command in keywords) {
		if (command == FROM || String::IsNullOrEmpty(input->Text) ) {
			break;
		}
		if (matchKeyword(command)) {
			showSuggestedCommands(command);
			found = true;
			dropDown->DroppedDown = true;
			break;
		}
	}
	if (!found) {
		dropDown->DroppedDown = false;
	}
}

// Populates the dropdown menu with the command patterns corresponding to the
// keyword passed in

void UI::showSuggestedCommands(String^ keyword) {
	dropDown->Items->Clear();
	if (suggestions[keyword] == nullptr) {
		return;
	} else if (suggestions[keyword]->GetType() == keyword->GetType()) {
		dropDown->Items->Add(suggestions[keyword]);
	} else {
		List<String^>^ commands = dynamic_cast<List<String^>^>(suggestions[keyword]);
		for each(String^ command in commands) {
			dropDown->Items->Add(command);
		}
	}
}

// Performs a character by character comparison of the Command keyword
// and the text in input textbox, returns:
// TRUE  - if one or more characters match the sequence 
// FALSE - otherwise

bool UI::matchKeyword(String^ keyword) {
	String^ inputText = input->Text;
	int inputLength = input->Text->Length;
	int keywordLength = keyword->Length;
	int i=0;
	while (i < inputLength && i<keywordLength) {
		if (inputText[i] == ' ' && i != 0) {
			return false;
		}
		if (inputText[i] != keyword[i]) {
			return false;
		}
		++i;
	}
	return true;
}

// Parses the text in input textbox, highlights the nearest parameter with 
// the format : <.....>

void UI::selectFields() {
	int i=0;
	int length = input->Text->Length;
	int start;
	int end;
	bool found = false;
	String^ text = input->Text;
	while (i < length) {
		if (text[i] == '<') {
			start = i;
		}
		if (text[i] == '>') {
			end = i+1;
			found = true;
			break;
		}
		++i;
	}
	if (found) {
		input->Select(start,end-start);
	} else {
		input->Select(length,1);
	}
}

