// Soon Hao Ye @@author A0126677U

#include "TextBuddyUI.h"

using namespace System;
using namespace UserInterface;
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

TextBuddyUI::TextBuddyUI() {
	logger = TbLogger::getInstance();
	InitializeComponent();
	labels = new std::vector<std::string>;
	taskDescription = new std::vector<std::string>;
	taskDate = new std::vector<std::string>;
	taskTime = new std::vector<std::string>;
	floatingTasks = new std::vector<std::string>;
	color = new std::vector<int>;
	logic = logic->getInstance();
	logic->subscribe(labels,taskDescription,taskDate,taskTime,floatingTasks,color);
	input->Text = "display";
	getInput();
	processAndExecute();
	input->Clear();
	floatingTaskIndex = 0;
	originalRowPosition = 0;
	input->Focus();
	this->ActiveControl = input;
	floatingTaskDisplay->SelectionAlignment = HorizontalAlignment::Center;
	keywords = gcnew List<String^>();
	keywords->Add(ADD);
	keywords->Add(DEL);
	keywords->Add(MODIFY);
	keywords->Add(SEARCH);
	keywords->Add(DISPLAY);
	keywords->Add(DONE);
	keywords->Add(STAR);
	keywords->Add(FROM);
	keywords->Add(BY);
	keywords->Add(TO);
	keywords->Add(ON);
	keywords->Add(AT);
}

TextBuddyUI::~TextBuddyUI() {
	if(components) {
		delete components;
		delete floatingTasks;
		delete labels;
		delete taskDescription;
		delete taskDate;
		delete taskTime;
		delete color;
	}
}
