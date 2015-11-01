// Soon Hao Ye @@author A0126677U

#include "TextBuddyUI.h"

using namespace UserInterface;

void TextBuddyUI::updateDisplay() {
	mode = logic->getMode();
	int tabIndex = 0;
	switch (mode) {
	case ALL:
		tabIndex = ALL;
		break;
	case TODAY:
		tabIndex = TODAY;
		break;
	case WEEK:
		tabIndex = WEEK;
		break;
	case EVENTS:
		tabIndex = EVENTS;
		break;
	case DEADLINES:
		tabIndex = DEADLINES;
		break;
	case FLOATINGS:
		tabIndex = FLOATINGS;
		break;
	}
	tabs->SelectTab(tabIndex);
	TabPage^ currentTab =  tabs->SelectedTab;
	currentTab->TabStop = false;
	int width = currentTab->Width;
	int height = currentTab->Height;
	currentTab->Controls->Clear();
	unsigned int size = tasks->size();
	for(unsigned int i=0 ; i< size ; ++i) {
		DisplayedTask task = (*tasks)[i];
		String^ index = (i+1).ToString() + "\r\n";
		String^ label = gcnew String(task.label.c_str()) + "\r\n";
		String^ title = gcnew String(task.description.c_str()) + "\r\n";
		String^ d = gcnew String(task.date.c_str()) + "\r\n";
		String^ t = gcnew String(task.time.c_str()) + "\r\n";		
		
		MetroTile^ tile = gcnew MetroTile();
		int tileWidth = (width/4)-10 ;
		int tileHeight = (height/5)-5 ;
		tile->ActiveControl = nullptr;
		tile->TabStop = false;
		tile->Style = this->Style;
		tile->Location = System::Drawing::Point((i%4*(tileWidth+5))+12, (i/4*(height/5))+5);
 		tile->Name = tile + i.ToString();
		tile->Size = System::Drawing::Size(tileWidth,tileHeight);	
		tile->TextAlign = System::Drawing::ContentAlignment::TopLeft;

		if(task.status == PRIORITY || task.status == URGENT) {
			index = "\r\n";
			tile->TileCount = i+1 ;
			tile->TileTextFontWeight = MetroFramework::MetroTileTextWeight::Regular;
			tile->Style = MetroFramework::MetroColorStyle::Purple;
			if(task.status == URGENT) {
				tile->Style = MetroFramework::MetroColorStyle::Red;
			}
		} else {
			tile->TileTextFontWeight = MetroFramework::MetroTileTextWeight::Regular;
		}
		
		String^ tileText = index + title + label + d + t ;
		tile->Text = tileText;
		currentTab->Controls->Add(tile);
		input->Focus();
	}
}
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
	if(e->KeyCode == Keys::Home) {
		//do nothing 
		return;
	}
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
		highlightSyntax();
	}
}

System::Void TextBuddyUI::input_KeyDown(System::Object^  sender, 
									System::Windows::Forms::KeyEventArgs^  e) {
	assert(inputHistoryCount >= 0);
	Keys key = e->KeyCode;
	if(helpMode) {
		helpMode = false;
		help->Visible = false;
		help->SendToBack();
		input->Clear();
		feedback->Clear();
		return;
	}
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
		inputHistory->Insert(0,input->Text);
		inputHistoryCount = 0;
		input->Clear();
		return; 
	}
	if(key == Keys::Down) {
		if(dropDown->DroppedDown) {
			if(dropDown->SelectedIndex + 1 < dropDown->Items->Count) { 
				dropDown->SelectedIndex ++ ;
			}
		} else {
			if(inputHistoryCount <= inputHistory->Count) {
				if(inputHistoryCount != 0) {
					--inputHistoryCount;
					input->Text = inputHistory[inputHistoryCount];
				} else {
					input->Text = "";
				}
				
			}
		}
		return;
	}
	if(key == Keys::Up) {
		if(dropDown->DroppedDown) {
			if(dropDown->SelectedIndex > 0 ) {
				dropDown->SelectedIndex -- ;
			} else {
				dropDown->DroppedDown = false;
			}
		} else {
			if(inputHistoryCount < inputHistory->Count) {
				input->Text = inputHistory[inputHistoryCount];
				if(inputHistoryCount != inputHistory->Count -1) {
					++inputHistoryCount;
				}
			}
		}
		return;
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
		List<String^>^ commands = dynamic_cast<List<String^>^>(suggestions[keyword]);
		for each(String^ command in commands) {
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
		feedback->ForeColor = Color::Green;
		printFeedBackMessage("  press any key to exit help");
		return;
	}
	try {
		message = logic->processCommand(*userInput);
		feedback->ForeColor = Color::Green;
		updateDisplay();			
		delete userInput;
	} catch(std::exception e) {
		feedback->ForeColor = Color::Red;
		message = std::string(e.what()) + "\t(enter \"help\" for command formats)";
	}
	printFeedBackMessage("  " + message);
}
/*
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
}*/

void TextBuddyUI::printFeedBackMessage(std::string message) {
	feedback->Text = gcnew String(message.c_str());
}