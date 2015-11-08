// Soon Hao Ye @@author A0126677U

#include "UI.h"

using namespace UserInterface;

// Updates the date display every 5 seconds

System::Void UI::updateCurrentTime_Tick(System::Object^ sender, System::EventArgs^ e) {
	DateTime localDateTime = DateTime::Now;
	String^ month = gcnew String(Utilities::monthToString((Month)localDateTime.Month).c_str());
	String^ t = (localDateTime.DayOfWeek).ToString() + " " + 
		(localDateTime.Day).ToString() + "/" + month;

	currentTime->Text = t ;
}