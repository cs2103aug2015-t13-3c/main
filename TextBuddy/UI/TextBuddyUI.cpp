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
	logic = logic->getInstance();
	tasks = new std::vector<DisplayedTask>();
	logic->subscribe(tasks);
	input->Text = "view all";
	getInput();
	processAndExecute();
	input->Clear();
	originalRowPosition = 0;
	tabs->Style = this->Style;
	input->Focus();
	this->ActiveControl = input;
//	floatingTaskDisplay->SelectionAlignment = HorizontalAlignment::Center;
	inputHistoryCount = 0;
	inputHistory = gcnew System::Collections::Generic::List<String^>();
	keywords = gcnew List<String^>();
	keywords->Add(ADD);
	keywords->Add(DEL);
	keywords->Add(MODIFY);
	keywords->Add(SEARCH);
	keywords->Add(UNDO);
	keywords->Add(VIEW);
	keywords->Add(CLEAR);
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
	addCommands->Add("add <TASK>");	
	addCommands->Add("add <TODO> by <DATE/TIME>");
	addCommands->Add("add <TODO> by <DATE> at <TIME>");
	addCommands->Add("add <EVENT> on <DATE>");
	addCommands->Add("add <EVENT> on <DATE> at <TIME>");
	addCommands->Add("add <EVENT> from <TIME> to <TIME>");
	addCommands->Add("add <EVENT> on <DATE> from <TIME> to <TIME>");
	addCommands->Add("add <EVENT> from <DATE> at <TIME> to <DATE> at <TIME>");
	// add here...

	//********** VIEW command formats ******************
	viewCommands = gcnew List<String^>();
	viewCommands->Add("view all");
	viewCommands->Add("view today");
	viewCommands->Add("view week");
	viewCommands->Add("view todo");
	viewCommands->Add("view events");
	viewCommands->Add("view floating");
	viewCommands->Add("view past");
	viewCommands->Add("view :<label>");
	// add here...

	//********** MODIFY command formats *****************
	modifyCommands = gcnew List<String^>();
	modifyCommands->Add("modify <ID> <DESCRIPTION>");
	modifyCommands->Add("modify <ID> star");
	modifyCommands->Add("modify <ID> unstar");
	modifyCommands->Add("modify <ID> :<LABEL>");
	modifyCommands->Add("modify <ID> by <NEW DEADLINE>");
	// add here...

	//********** SEARCH command formats *****************
	searchCommands = gcnew List<String^>();
	searchCommands->Add("search <TASK DESCRIPTION>");
	// add here...
	
	suggestions = gcnew Hashtable();
	suggestions->Add(QUIT,QUIT);
	suggestions->Add(SAVE,SAVE);
	suggestions->Add(CLEAR,CLEAR);
	suggestions->Add(DONE,"done <ID>");
	suggestions->Add(UNDO,UNDO);
	suggestions->Add(HELP,HELP);
	suggestions->Add(DEL,"delete <ID>");
	suggestions->Add(ADD,addCommands);
	suggestions->Add(SEARCH,searchCommands);
	suggestions->Add(VIEW,viewCommands);
	suggestions->Add(MODIFY,modifyCommands);

}

TextBuddyUI::~TextBuddyUI() {
	if(components) {
		delete components;
	}
}
