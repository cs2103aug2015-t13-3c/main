#include "TextBuddyUI.h"

using namespace UserInterface;

System::Void TextBuddyUI::updateFloatingTimer_Tick(System::Object^  sender, System::EventArgs^  e) {
	if(floatingTasks != nullptr && !floatingTasks->empty()) {	
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