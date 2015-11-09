// Soon Hao Ye @@author A0126677U

#include "UI.h"

using namespace UserInterface;

// This function is activated whenever a key is released
// Response to different keys :
// 1. [ALL]  - if helpMode is activated, helpMode will be disabled
// 2. [HOME] - ignore the key
// 3. [ESC]  - minimize window
// 4. [TAB]  - if dropdown box is active, increments the selection index, else
//				check if the input textbox has parameters from autosuggest, and 
//				highlights the nearest field.
// 5. All keys except [UP], [DOWN], [RETURN] :
//			 - performs autosuggest

System::Void UI::input_KeyUp (
	System::Object^ sender,
	System::Windows::Forms::KeyEventArgs^ e) {

		Keys key = e->KeyCode;
		if (e->KeyCode == Keys::Home) {
			// Do nothing 
			return;
		}
		if (key == Keys::Tab) {
			if (dropDown->DroppedDown) {
				incrementDropCount();
			} else {
				selectFields();
			}
			return;
		}
		if (key != Keys::Up && key != Keys::Down && key != Keys::Return) {
			commandAutoComplete();
			// Unstable:
			// highlightSyntax();
			return;
		}
}

// This function is activated whenever a key is pressed
// Response to different keys :
// 1. [RETURN] - if the dropdown menu is active, sets the input textbox to be 
//				 the item selected in the dropdown menu
//				 else, accepts the input textbox text as command and process it
// 2. [DOWN]   - if the dropdown menu is active, selected the next item in the
//				 dropdown menu, else if the user is currently going through 
//				 the previous entered commands, move one step forward
// 3. [UP]	   - if the dropdown menu is active, selected the previous item in 
//				 the dropdown menu, else if the user is currently going through 
//				 the previous entered commands, move one step backward

System::Void UI::input_KeyDown(
	System::Object^ sender,
	System::Windows::Forms::KeyEventArgs^ e) {

		Keys key = e->KeyCode;	
		if (helpMode) {
			closeHelpMode();
			return;
		}
		if (key == Keys::Return) { 
			if (input->Text == "s") {
				if (tileView) {
					tileView = false;
				} else {
					tileView = true;
				}
				updateDisplay();
				input->Clear();
				return;
			}
			if (dropDown->DroppedDown) {
				if (acceptAutosuggest()) {
					return;
				}
			}
			getInput();
			processAndExecute();
			addCommandHistory();
			input->Clear();
			return; 
		}
		if (key == Keys::Down) {
			if (dropDown->DroppedDown) {
				incrementDropCount();
			} else {
				toNextCommand();
			}
			return;
		}
		if (key == Keys::Up) {
			if (dropDown->DroppedDown) {
				decrementDropCount();
			} else {
				toPreviousCommand();
			}
			return;
		}
}

System::Void UI::tabs_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	DisplayMode index = (DisplayMode)tabs->SelectedIndex;
	switch (index) {
	case ALL:
		input->Text = "view all";
		break;
	case TODAY:
		input->Text = "view today";
		break;
	case WEEK:
		input->Text = "view week";
		break;
	case EVENTS:
		input->Text = "view events";
		break;
	case DEADLINES:
		input->Text = "view todo";
		break;
	case FLOATINGS:
		input->Text = "view floating";
		break;
	case SEARCHES:
		// not implemented
		break;
	case PAST_:
		input->Text = "view past";
		break;
	case FREESLOTS:
		// not implemented yet
		break;
	}
	getInput();
	processAndExecute();
	input->Clear();
}
System::Void UI::UI_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
	if(e->KeyCode == Keys::Escape) {	
		this->WindowState = FormWindowState::Minimized;
		return;
	}
}
System::Void UI::display_Click(System::Object^  sender, System::EventArgs^  e) {
	input->Focus();
}
System::Void UI::UI_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
	if(e->Shift) {	
		if(e->KeyCode == Keys::Up) {
			scrollUp();
		} else if(e->KeyCode == Keys::Down) {
			scrollDown();
		}
	}
}
