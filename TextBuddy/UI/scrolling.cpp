// Soon Hao Ye @@author A0126677U

#include "TextBuddyUI.h"

using namespace UserInterface;

void TextBuddyUI::scrollDown() {
	DataGridView^ display = description->DataGridView ;
	if(display->FirstDisplayedScrollingRowIndex < display->RowCount )
		display->FirstDisplayedScrollingRowIndex = 
		display->FirstDisplayedScrollingRowIndex + 1;
	originalRowPosition = display->FirstDisplayedScrollingRowIndex;
}

void TextBuddyUI::scrollUp() {
	DataGridView^ display = description->DataGridView ;
	if(display->FirstDisplayedScrollingRowIndex > 0 )
		display->FirstDisplayedScrollingRowIndex = 
		display->FirstDisplayedScrollingRowIndex - 1;
	originalRowPosition = display->FirstDisplayedScrollingRowIndex;
}
