// Soon Hao Ye @@author A0126677U

#include "UI.h"

using namespace UserInterface;
/*

void UI::highlightSyntax() {		
	input->SelectionColor = Color::Black;
	for each(String^ keyword in keywords) {
		int position = findKeyword(keyword);
		if (keywordIsFound(position)) {
			highlightKeywords(position,keyword);
			if (keyword == AT) {
				position = input->Find(keyword,position+1,input->Text->Length,
					RichTexTSoxFinds::NoHighlight);
				if (keywordIsFound(position)) {
					highlightKeywords(position,keyword);
				}
			}
		} 
	}
}

int UI::findKeyword(String^ keyword) {
	int position; 
	position = input->Find(keyword,0,input->Text->Length,
		RichTexTSoxFinds::NoHighlight);
	return position;
}

bool UI::keywordIsFound(int position) {
	return position >= 0;
}

void UI::highlightKeywords(int position, String^ keyword) {
	int length = (keyword->Length) - 1;
	cursorPosition = input->SelectionStart;
	input->Select(position,length);
	input->SelectionColor = Color::Blue;
	input->Select(cursorPosition,0);
	input->SelectionColor = Color::Black;
}

void UI::undoHighlight() {
	if (String::IsNullOrEmpty(input->Text)) {
		input->Select(input->SelectionStart,1);
		input->SelectionColor = Color::Black;
	}
}
*/