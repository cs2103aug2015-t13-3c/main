// Soon Hao Ye @@author A0126677U

#include "TextBuddyUI.h"

using namespace UserInterface;

String^ TextBuddyUI::wrapWord(std::string content) {
	unsigned int width;
	if(tileView) {

	} else {
		width = 45;
	}
	if(content.size() > width) {
		unsigned int i=width-1;
		while(content[i] != ' ' && i != 0) {
			--i;
		}
		if(i == 0) {
			i = width-1;
		}
		content.insert(i,"\r\n");
	}
	return gcnew String(content.c_str());
}

// Renders the help image invisible and bring the component to the back of form 

void TextBuddyUI::closeHelpMode() {
	helpMode = false;
	help->Visible = false;
	help->SendToBack();
	input->Clear();
	feedback->Clear();
}

// Gets the tabIndex from logic that UI should display to the user

int TextBuddyUI::getTabIndex() {
	mode = logic->getMode();
	assert(mode >= 0 && mode < 8);
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
	case FREESLOTS:
		tabIndex = SEARCHES;
		break;
	}
	return tabIndex;
}

void TextBuddyUI::updateTable(TabPage^ currentTab) {
	display->Rows->Clear();
	unsigned int size = tasks->size();
	for(unsigned int i=0 ; i< size ; ++i) {
		DisplayedTask task = (*tasks)[i];
		String^ index = (i+1).ToString();
		String^ label = gcnew String(task.label.c_str());
		String^ title = wrapWord(task.description);
		String^ d = gcnew String(task.date.c_str());
		String^ t = gcnew String(task.time.c_str());	
		display->Rows->Add(index,label,title,d,t);
		if(task.status == PRIORITY) {
			display->Rows[i]->DefaultCellStyle->ForeColor = Color::Blue;
		} else if(task.status == URGENT) {
			display->Rows[i]->DefaultCellStyle->ForeColor = Color::Red;
		} else if(task.status == PAST) {
			display->Rows[i]->DefaultCellStyle->ForeColor = Color::Gray;
		}
	}
	currentTab->Controls->Add(display);
}

void TextBuddyUI::updateDisplay() {
	int tabIndex = getTabIndex();
	tabs->SelectTab(tabIndex);
	TabPage^ currentTab =  tabs->SelectedTab;
	currentTab->Controls->Clear();
	if(mode == FREESLOTS) {
		//displayFreeSlots(currentTab);
	} else if(tileView) {
		addTilesToTab(currentTab);
	} else {
		updateTable(currentTab);
	}
	input->Focus();
}

void TextBuddyUI::addTilesToTab(TabPage^ currentTab) {
	int x = 0;
	int y = 0;
	int width = currentTab->Width;
	int height = currentTab->Height;
	int squareTileWidth = (width/5)-10;
	int longTileWidth = (width*2/5)-15;
	int tileHeight = (height/5)-5;
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
}

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