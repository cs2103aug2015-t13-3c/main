// Soon Hao Ye @@author A0126677U

#include "UI.h"

using namespace UserInterface;

// Pushes the userInput into the CommandHistory List

void UI::addCommandHistory() {
	inputHistory->Insert(0,input->Text);
	inputHistoryCount = 0;
}

// Retrieves the previous command the user entered and place it into input textbox

void UI::toPreviousCommand() {
	assert(inputHistoryCount >= 0);
	if (inputHistoryCount < inputHistory->Count) {
		input->Text = inputHistory[inputHistoryCount];
		if (inputHistoryCount != inputHistory->Count -1) {
			++inputHistoryCount;
		}
	}
}

// while going through the previous commands the user entered, this function
// changes the input textbox text to one step forward from the past command the
// user is currently at

void UI::toNextCommand() {
	assert(inputHistoryCount >= 0);
	if (inputHistoryCount <= inputHistory->Count) {
		if (inputHistoryCount != 0) {
			--inputHistoryCount;
			input->Text = inputHistory[inputHistoryCount];
		} else {
			input->Text = "";
		}
	}
}