// Soon Hao Ye @@author A0126677U

#include "TextBuddyUI.h"

using namespace UserInterface;

void TextBuddyUI::highlightSyntax() {		
	input->SelectionColor = Color::Black;
	for each(String^ keyword in keywords) {
		int position = findKeyword(keyword);
		if(keywordIsFound(position)) {
			highlightKeywords(position,keyword);
			if(keyword == AT) {
				position = input->Find(keyword,position+1,input->Text->Length,
					RichTextBoxFinds::NoHighlight);
				if(keywordIsFound(position)) {
					highlightKeywords(position,keyword);
				}
			}
		} 
	}
}

int TextBuddyUI::findKeyword(String^ keyword) {
	int position; 
	position = input->Find(keyword,0,input->Text->Length,
		RichTextBoxFinds::NoHighlight);
	return position;
}

bool TextBuddyUI::keywordIsFound(int position) {
	return position >= 0;
}



void TextBuddyUI::highlightKeywords(int position, String^ keyword) {
	int length = (keyword->Length) - 1;
	input->Select(position,length);
	input->SelectionColor = Color::Blue;
	input->Select(cursorPosition,0);
	input->SelectionColor = Color::Black;
}

void TextBuddyUI::undoHighlight() {
	if(String::IsNullOrEmpty(input->Text)) {
		input->Select(input->SelectionStart,1);
		input->SelectionColor = Color::Black;
	}
}