// Soon Hao Ye @@author A0126677U

#include "TextBuddyUI.h"

using namespace UserInterface;
/*
System::Void TextBuddyUI::input_KeyUp_1(System::Object^  sender, 
									System::Windows::Forms::KeyEventArgs^  e) {
	this->dropDown->DroppedDown = true;
	cursorPosition = input->SelectionStart;
	if(e->KeyCode == Keys::Home) {
		//do nothing 
		return;
	}
	if(helpMode) {
		helpMode = false;
		help->Visible = false;
		help->SendToBack();
		input->Clear();
		return;
	}
	if(e->KeyCode == Keys::Return) { // If user presses 'Return' key
		if(dropDown->DroppedDown) {
			String^ command = dropDown->SelectedItem->ToString();
			input->Text = command;
			dropDown->DroppedDown = false;
			return;
		}
		getInput();
		processAndExecute();
		input->Clear();
	} else if(e->KeyCode != Keys::Left && e->KeyCode != Keys::Right && e->KeyCode != Keys::Back){
		highlightSyntax();
		autoComplete();
	}
	if(e->KeyCode == Keys::Back) {
		undoSearch();
		undoHighlight();
	}
	if(e->KeyCode == Keys::Down) {
		//scrollDown();
		this->dropDown->SelectedIndex ++ ;
	}
	if(e->KeyCode == Keys::Up) {
		//scrollUp();
		if(this->dropDown->SelectedIndex > 0 ) {
			this->dropDown->SelectedIndex -- ;
		}
	}
}
*/
//====================== DEVELOPING ===========================================
System::Void TextBuddyUI::input_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
	Keys key = e->KeyCode;
	if(key == Keys::Tab) {
		if(dropDown->DroppedDown) {
			if(dropDown->SelectedIndex + 1 < dropDown->Items->Count) { 
				dropDown->SelectedIndex ++ ;
			}
		} else {
			selectFields();
		}
		return;
	}
	if(key != Keys::Up && key != Keys::Down && key != Keys::Return) {
		commandAutoComplete();
	}
}

System::Void TextBuddyUI::input_KeyDown(System::Object^  sender, 
									System::Windows::Forms::KeyEventArgs^  e) {
	Keys key = e->KeyCode;
	if(key == Keys::Return) { // If user presses 'Return' key
		if(dropDown->DroppedDown) {
			if(dropDown->SelectedIndex > -1) {
				String^ command = dropDown->SelectedItem->ToString();
				input->Clear();
				input->Text = command;
				dropDown->DroppedDown = false;
				selectFields();
				return;
			} else {
				dropDown->DroppedDown = false;
			}
		}
		getInput();
		processAndExecute();
		input->Clear();
		return;
	}
	if(key == Keys::Down) {
		if(dropDown->SelectedIndex + 1 < dropDown->Items->Count) { 
			dropDown->SelectedIndex ++ ;
		}
		return;
	}
	if(key == Keys::Up) {
		if(dropDown->SelectedIndex > 0 ) {
			dropDown->SelectedIndex -- ;
		} else {
			dropDown->DroppedDown = false;
		}
		return;
	}
	if(key == Keys::Back) {
		if(String::IsNullOrEmpty(input->Text)) {

		}
	}
}

void TextBuddyUI::selectFields() {
	int i=0;
	int length = input->Text->Length;
	int start;
	int end;
	bool found = false;
	String^ text = input->Text;
	while(i < length) {
		if(text[i] == '<') {
			start = i;
		}
		if(text[i] == '>') {
			end = i+1;
			found = true;
			break;
		}
		++i;
	}
	if(found) {
		input->Select(start,end-start);
	} else {
		input->Select(length,1);
	}
}

void TextBuddyUI::commandAutoComplete() {
	bool found = false;
	for each(String^ command in keywords) {
		if(command == FROM || String::IsNullOrEmpty(input->Text) ) {
			break;
		}
		if(matchKeyword(command)) {
			showSuggestedCommands(command);
			found = true;
			dropDown->DroppedDown = true;
			break;
		}
	}
	if(!found) {
		dropDown->DroppedDown = false;
	}
}

void TextBuddyUI::showSuggestedCommands(String^ keyword) {
	dropDown->Items->Clear();
	if(suggestions[keyword] == nullptr) {
		return;
	} else if(suggestions[keyword]->GetType() == keyword->GetType()) {
		dropDown->Items->Add(suggestions[keyword]);
	} else {
		for each(String^ command in addCommands) {
			dropDown->Items->Add(command);
		}
	}
}

bool TextBuddyUI::matchKeyword(String^ keyword) {
	String^ inputText = input->Text;
	int inputLength = input->Text->Length;
	int keywordLength = keyword->Length;
	int i=0;
	while(i < inputLength && i<keywordLength) {
		if(inputText[i] == ' ' && i != 0) {
			return false;
		}
		if(inputText[i] != keyword[i]) {
			return false;
		}
		++i;
	}
	return true;
}

//============================================================================
void TextBuddyUI::getInput() {
	msclr::interop::marshal_context context;
	userInput = new std::string(context.marshal_as<std::string>(input->Text));			
}

void TextBuddyUI::processAndExecute() {
	feedback->Clear();
	std::string message;
	if(input->Text == "help") {
		helpMode = true;
		help->Visible = true;
		help->BringToFront();
		return;
	}
	try {
		feedback->ForeColor = Color::Green;
		message = logic->processCommand(*userInput);
		updateDisplay();			
		printFeedBackMessage(message);
		delete userInput;
	} catch(std::exception e) {
		feedback->ForeColor = Color::Red;
		printFeedBackMessage(e.what());
		// suggestHelpPage()
	}
}

void TextBuddyUI::updateDisplay() {
	DataGridView^ display = description->DataGridView ;
	display->Rows->Clear();
	unsigned int size = taskDescription->size();
	for(unsigned int i=0 ; i< size ; ++i) {
		String^ index = gcnew String((i+1).ToString());
		String^ label = gcnew String((*labels)[i].c_str());
		String^ title = gcnew String((*taskDescription)[i].c_str());
		String^ d = gcnew String((*taskDate)[i].c_str());
		String^ t = gcnew String((*taskTime)[i].c_str());
		display->Rows->Add(index,label,title,d,t);
		switch ((*color)[i]) {
		case 0:
			display->Rows[i]->DefaultCellStyle->ForeColor = Color::Gray;
			break;
		case 1:
			display->Rows[i]->DefaultCellStyle->ForeColor = Color::Blue;
			break;
		case 2:
			display->Rows[i]->DefaultCellStyle->ForeColor = Color::Red;
			break;
		case 3:
			display->Rows[i]->DefaultCellStyle->ForeColor = Color::Black;
		default:
			break;
		}
		display->FirstDisplayedScrollingRowIndex = 0;
	}
}

void TextBuddyUI::printFeedBackMessage(std::string message) {
	feedback->Text = gcnew String(message.c_str());
}