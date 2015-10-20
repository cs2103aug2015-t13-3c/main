// @@author A0126677U (Soon Hao Ye)

#include "TextBuddyUI.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Threading;

[STAThread]
int main() {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	UserInterface::TextBuddyUI mainWindow;
	Application::Run(%mainWindow);
	return 0;
}
