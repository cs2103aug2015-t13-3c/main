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
	selectingFields = false;
	helpMode = false;
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
	keywords->Add(UNDO);
	keywords->Add(VIEW);
	keywords->Add(DONE);
	keywords->Add(SAVE);
	keywords->Add(QUIT);
	keywords->Add(HELP);
	keywords->Add(FROM);
	keywords->Add(BY);
	keywords->Add(TO);
	keywords->Add(ON);
	keywords->Add(AT);
	this->help->Image = Image::FromFile(System::AppDomain::CurrentDomain->BaseDirectory + "helpPage.png");
	this->Icon = gcnew System::Drawing::Icon(System::AppDomain::CurrentDomain->BaseDirectory + "tick.ico");

	//*********** ADD command formats ******************
	addCommands = gcnew List<String^>();
	// floating task
	addCommands->Add("add <TASK>");	
	// deadline
	addCommands->Add("add <TODO> by <DATE/TIME>");
	addCommands->Add("add <TODO> by <DATE> at <TIME>");
	// event
	addCommands->Add("add <EVENT> on <DATE>");
	addCommands->Add("add <EVENT> on <DATE> at <TIME>");
	addCommands->Add("add <EVENT> from <TIME> to <TIME>");
	addCommands->Add("add <EVENT> on <DATE> from <TIME> to <TIME>");
	addCommands->Add("add <EVENT> from <DATE> at <TIME> to <DATE> at <TIME>");
	

	suggestions = gcnew Hashtable();
	suggestions->Add(QUIT,QUIT);
	suggestions->Add(DONE,"done <ID>");
	suggestions->Add(UNDO,UNDO);
	suggestions->Add(HELP,HELP);
	suggestions->Add(DEL,"delete <ID>");
	suggestions->Add(ADD,addCommands);
	


}

TextBuddyUI::~TextBuddyUI() {
	if(components) {
		delete components;
	}
}
