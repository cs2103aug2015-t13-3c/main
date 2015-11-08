// Soon Hao Ye @@author A0126677U

#include "UI.h"

using namespace System;
using namespace UserInterface;
using namespace System::Windows::Forms;

[STAThread]
int main() {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	UserInterface::UI mainWindow;
	Application::Run(%mainWindow);
	return 0;
}

UI::UI() {
	selectingFields = false;
	helpMode = false;
	tileView = false;
	logger = TsLogger::getInstance();
	InitializeComponent();
	logic = logic->getInstance();
	tasks = new std::vector<DisplayedTask>();
	logic->subscribe(tasks);
	input->Text = "view today";
	getInput();
	processAndExecute();
	input->Clear();
	originalRowPosition = 0;
	tabs->Style = this->Style;
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
	keywords->Add(DISPLAY);
	keywords->Add(EDIT);
	keywords->Add(LOAD);
	keywords->Add(NOTDONE);
	keywords->Add(PICK);
	keywords->Add(REDO);
	keywords->Add(FROM);
	keywords->Add(BY);
	keywords->Add(TO);
	keywords->Add(ON);
	keywords->Add(AT);
	this->help->Image = Image::FromFile(System::AppDomain::CurrentDomain->BaseDirectory + "help.png");
	this->Icon = gcnew System::Drawing::Icon(System::AppDomain::CurrentDomain->BaseDirectory + "shark.ico");
	this->BackImage = Image::FromFile(System::AppDomain::CurrentDomain->BaseDirectory + "shark.bmp");

	//*********** ADD command formats ******************
	addCommands = gcnew List<String^>();
	addCommands->Add("add <TASK>");	
	addCommands->Add("add <TODO> by <DATE/TIME>");
	addCommands->Add("add <TODO> by <DATE> at <TIME>");
	addCommands->Add("add <EVENT> on <DATE>");
	addCommands->Add("add <EVENT> on <DATE> at <TIME>");
	addCommands->Add("add <EVENT> on <DATE> from <TIME> to <TIME>");
	addCommands->Add("add <EVENT> from <DATE> to <DATE>");
	addCommands->Add("add <EVENT> from <DATE> at <TIME> to <DATE> at <TIME>");
	addCommands->Add("add <EVENT> from <TIME> to <TIME>");
	// add here...

	//********** VIEW command formats ******************
	viewCommands = gcnew List<String^>();
	viewCommands->Add("view all");
	viewCommands->Add("view today");
	viewCommands->Add("view week");
	viewCommands->Add("view floating");
	viewCommands->Add("view todo");
	viewCommands->Add("view events");
	viewCommands->Add("view past");
	viewCommands->Add("view <LABEL>");
	viewCommands->Add("view after <DATE>");
	viewCommands->Add("view before <DATE>");
	viewCommands->Add("view from <DATE> at <TIME> to <DATE> at <TIME>");
	// add here...

	//********** MODIFY command formats *****************
	modifyCommands = gcnew List<String^>();
	modifyCommands->Add("modify <ID> <DESCRIPTION>");
	modifyCommands->Add("modify <ID> : <LABEL>");
	modifyCommands->Add("modify <ID> from <DATE> at <TIME> to <DATE> at <TIME>");
	modifyCommands->Add("modify <ID> by <NEW DEADLINE>");
	modifyCommands->Add("modify <ID> float");
	modifyCommands->Add("modify <ID> star");
	modifyCommands->Add("modify <ID> unstar");
	// add here...

	//********** SEARCH command formats *****************
	searchCommands = gcnew List<String^>();
	searchCommands->Add("search <TASK DESCRIPTION>");
	searchCommands->Add("search <TASK DESCRIPTION> after <DATE>");
	searchCommands->Add("search <TASK DESCRIPTION> before <DATE>");
	searchCommands->Add("search <TASK DESCRIPTION> from <DATE> at <TIME> to <DATE> at <TIME>");
	searchCommands->Add("search after <DATE> for <NUM> d <NUM> h <NUM> m");
	searchCommands->Add("search before <DATE> for <NUM> d <NUM> h <NUM> m");
	searchCommands->Add("search from <DATE> to <DATE> for <NUM> d <NUM> h <NUM> m");
	// add here...

	//********** PICK command formats *****************
	pickCommands = gcnew List<String^>();
	pickCommands->Add("pick <ID>");
	pickCommands->Add("pick <ID> reserve");
	// add here...

	//********** LOAD command formats *****************
	loadCommands = gcnew List<String^>();
	loadCommands->Add("load <FILEPATH>");
	loadCommands->Add("load from <FILEPATH>");
	// add here...

	//********** SAVE command formats *****************
	saveCommands = gcnew List<String^>();
	saveCommands->Add("save <FILEPATH>");
	saveCommands->Add("save to <FILEPATH>");
	// add here...

	suggestions = gcnew Hashtable();
	suggestions->Add(HELP,HELP);
	suggestions->Add(QUIT,QUIT);
	suggestions->Add(CLEAR,CLEAR);
	suggestions->Add(DISPLAY,DISPLAY);
	suggestions->Add(UNDO,UNDO);
	suggestions->Add(REDO,REDO);
	suggestions->Add(DEL,"delete <ID>");
	suggestions->Add(DONE,"done <ID>");
	suggestions->Add(NOTDONE,"notdone <ID>");

	suggestions->Add(ADD,addCommands);
	suggestions->Add(MODIFY,modifyCommands);
	suggestions->Add(EDIT,modifyCommands);
	suggestions->Add(PICK,pickCommands);
	suggestions->Add(SEARCH,searchCommands);
	suggestions->Add(VIEW,viewCommands);
	suggestions->Add(LOAD,loadCommands);
	suggestions->Add(SAVE,saveCommands);
}

UI::~UI() {
	if(components) {
		delete components;
	}
}

