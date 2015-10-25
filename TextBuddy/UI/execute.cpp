#include "TextBuddyUI.h"

using namespace UserInterface;

System::Void TextBuddyUI::input_KeyUp_1(System::Object^  sender, 
									System::Windows::Forms::KeyEventArgs^  e) {
	cursorPosition = input->SelectionStart;
	if(e->KeyCode == Keys::Return) { // If user presses 'Return' key
		getInput();
		processAndExecute();
		input->Clear();
	} else if(e->KeyCode != Keys::Left && e->KeyCode != Keys::Right){
		highlightSyntax();
		autoComplete();
	} else {
		autoComplete();
	}
	if(e->KeyCode == Keys::Back) {
		undoSearch();
	}
	if(e->KeyCode == Keys::Down) {
		scrollDown();
	}
	if(e->KeyCode == Keys::Up) {
		scrollUp();
	}
}


void TextBuddyUI::getInput() {
	msclr::interop::marshal_context context;
	userInput = new std::string(context.marshal_as<std::string>(input->Text));			
}

void TextBuddyUI::processAndExecute() {
	feedback->Clear();
	std::string message;
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