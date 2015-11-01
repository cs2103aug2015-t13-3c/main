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
	case SEARCHES:
		tabIndex = SEARCHES;
		break;
	case PAST_:
		tabIndex = PAST_;
		break;
	}
	tabs->SelectTab(tabIndex);
	TabPage^ currentTab =  tabs->SelectedTab;
	currentTab->TabStop = false;
	//	std::vector<MetroTile> longTiles;
	//	std::vector<MetroTile> squareTiles;
	int x = 0;
	int y = 0;
	int width = currentTab->Width;
	int height = currentTab->Height;
	int squareTileWidth = (width/5)-10;
	int longTileWidth = (width*2/5)-15;
	int tileHeight = (height/5)-5;
	currentTab->Controls->Clear();
	unsigned int size = tasks->size();
	for(unsigned int i=0 ; i< size ; ++i) {
		DisplayedTask task = (*tasks)[i];
		String^ index = (i+1).ToString() + "\r\n";
		String^ label = "\r\n";
		if(task.label != "") {
			label = "[" + gcnew String(task.label.c_str()) + "]" + label;
		}
		String^ title = gcnew String(task.description.c_str()) + "\r\n";
		String^ d = gcnew String(task.date.c_str()) + "\r\n";
		String^ t = gcnew String(task.time.c_str()) + "\r\n";		

		MetroTile^ tile = gcnew MetroTile();
		tile->ActiveControl = nullptr;
		tile->TabStop = false;
		tile->Style = this->Style;
		tile->Name = tile + i.ToString();
		if(title->Length <= 10) {
			index = "";
			tile->TileCount = i+1 ;
		}
		tile->TextAlign = System::Drawing::ContentAlignment::TopLeft;
		tile->TileTextFontWeight = MetroFramework::MetroTileTextWeight::Regular;
		if(task.status == PRIORITY) {
			tile->Style = MetroFramework::MetroColorStyle::Purple;
		} else if(task.status == URGENT) {
			tile->Style = MetroFramework::MetroColorStyle::Red;
		} else if(task.status == PAST) {
			tile->Style = MetroFramework::MetroColorStyle::Silver;
		}
		else {
			if(task.type == FLOATING) {
				tile->Style = MetroFramework::MetroColorStyle::Lime;
			}
		}	
		String^ tileText = index + title + label + d + t ;
		tile->Text = tileText;
		if(task.type == EVENT || title->Length > 20) {
			tile->Size = System::Drawing::Size(longTileWidth,tileHeight);
		} else {
			tile->Size = System::Drawing::Size(squareTileWidth,tileHeight);	
		}
		if(tile->Width > 140 && x%5 == 4) {
			++x;
			++y;
		}
		tile->Location = System::Drawing::Point((x%5*(squareTileWidth+5))+17, (y/5*(height/5))+5);
		currentTab->Controls->Add(tile);
		++x;
		++y;
		if(tile->Width > 140) {
			++x;
			++y;
		}

	}		 
	input->Focus();
}
/*
System::Void TextBuddyUI::input_KeyUp_1(System::Object^ sender, 
System::Windows::Forms::KeyEventArgs^ e) {
this->dropDown->DroppedDown = true;
cursorPosition = input->SelectionStart;
if(e->KeyCode == Keys::Home) {
// Do nothing 
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
// scrollDown();
this->dropDown->SelectedIndex++;
}
if(e->KeyCode == Keys::Up) {
// scrollUp();
if(this->dropDown->SelectedIndex > 0 ) {
this->dropDown->SelectedIndex--;
}
}
}
*/
//====================== DEVELOPING ===========================================
System::Void TextBuddyUI::input_KeyUp(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
	Keys key = e->KeyCode;
	if(e->KeyCode == Keys::Home) {
		//do nothing 
		return;
	}
	if(key == Keys::Tab) {
		if(dropDown->DroppedDown) {
			if(dropDown->SelectedIndex + 1 < dropDown->Items->Count) {
				dropDown->SelectedIndex++;
			}
		} else {
			selectFields();
		}
		return;
	}
	if(key != Keys::Up && key != Keys::Down && key != Keys::Return) {
		commandAutoComplete();
		//highlightSyntax();
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

void TextBuddyUI::printFeedBackMessage(std::string message) {
	feedback->Text = gcnew String(message.c_str());
}