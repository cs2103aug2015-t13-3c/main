// Soon Hao Ye @@author A0126677U

#include "TextBuddyUI.h"

using namespace UserInterface;

System::Void TextBuddyUI::updateFloatingTimer_Tick(System::Object^  sender, System::EventArgs^  e) {
	if(floatingTasks != nullptr) {
		if(floatingTasks->empty()) {
			floatingTaskDisplay->Text = "";
			return;
		}
		floatingTaskIndex = floatingTaskIndex % floatingTasks->size();
		String^ title = gcnew String(
			(*floatingTasks)[floatingTaskIndex].c_str());
		floatingTaskDisplay->Text = title;
		++floatingTaskIndex;
	}
}

System::Void TextBuddyUI::updateCurrentTime_Tick(System::Object^  sender, System::EventArgs^  e) {
	DateTime localDateTime = DateTime::Now;
	String^ t = (localDateTime.DayOfWeek).ToString() + " " + 
		(localDateTime.Day).ToString() + "/" +
		(localDateTime.Month).ToString() ;
	currentTime->Text = t ;
}