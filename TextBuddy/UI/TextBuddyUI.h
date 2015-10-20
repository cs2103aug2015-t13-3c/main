#pragma once

#include "Logic.h"
#include "stdafx.h"
#include <msclr\marshal_cppstd.h>

// DEFINE KEYWORD STRINGS
#define ADD "add "
#define DEL "delete "
#define MODIFY "modify "
#define SEARCH "search "
#define DISPLAY "display "
#define DONE "done "
#define TAG "tag "
#define STAR "star "
#define NOSTAR "nostar "
#define FROM " from "
#define BY " by "
#define TO " to "
#define ON " on "
#define AT " at "

namespace UserInterface {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;

	public ref class TextBuddyUI : public System::Windows::Forms::Form {

	public:
		// INITIALIZATION OF COMPONENTS AND REQUIRED VARIABLES
		TextBuddyUI(void) {
			InitializeComponent();
			labels = new std::vector<std::string>;
			taskDescription = new std::vector<std::string>;
			dateTime = new std::vector<std::string>;
			floatingTasks = new std::vector<std::string>;
			priotiryTasks = new std::vector<bool>;
			logic = logic->getInstance();
			logic->subscribe(labels,taskDescription,dateTime,floatingTasks,priotiryTasks);
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
			keywords->Add(TAG);
			keywords->Add(STAR);
			keywords->Add(NOSTAR);
			keywords->Add(FROM);
			keywords->Add(BY);
			keywords->Add(TO);
			keywords->Add(ON);
			keywords->Add(AT);
		}

	protected:
		~TextBuddyUI() {
			if(components) {
				delete components;
				delete logic;
				delete floatingTasks;
				delete labels;
				delete taskDescription;
				delete dateTime;
				delete priotiryTasks;
			}
		}

	private: System::Windows::Forms::RichTextBox^  input;
	private: System::Windows::Forms::TextBox^  feedback;
	private: System::Windows::Forms::RichTextBox^  floatingTaskDisplay;
	private: System::ComponentModel::BackgroundWorker^  hotKey;
	private: System::Windows::Forms::Timer^  updateFloatingTimer;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  id;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Label;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  description;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dateAndTime;
	private: System::ComponentModel::IContainer^  components;

#pragma region Windows Form Designer generated code

		/// Required method for Designer support - do not modify	
		void InitializeComponent(void) {
			this->components = (gcnew System::ComponentModel::Container());
			System::Windows::Forms::DataGridView^  display;
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle1 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			this->input = (gcnew System::Windows::Forms::RichTextBox());
			this->feedback = (gcnew System::Windows::Forms::TextBox());
			this->floatingTaskDisplay = (gcnew System::Windows::Forms::RichTextBox());
			this->hotKey = (gcnew System::ComponentModel::BackgroundWorker());
			this->updateFloatingTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->id = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Label = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->description = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dateAndTime = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			display = (gcnew System::Windows::Forms::DataGridView());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(display))->BeginInit();
			this->SuspendLayout();
			// 
			// display
			// 
			display->AllowUserToAddRows = false;
			display->AllowUserToDeleteRows = false;
			display->AllowUserToResizeColumns = false;
			display->AllowUserToResizeRows = false;
			display->AutoSizeRowsMode = System::Windows::Forms::DataGridViewAutoSizeRowsMode::AllCells;
			display->BackgroundColor = System::Drawing::Color::White;
			display->BorderStyle = System::Windows::Forms::BorderStyle::None;
			display->ColumnHeadersBorderStyle = System::Windows::Forms::DataGridViewHeaderBorderStyle::Single;
			display->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			display->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(4) {this->id, this->Label, this->description, 
				this->dateAndTime});
			dataGridViewCellStyle1->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
			dataGridViewCellStyle1->BackColor = System::Drawing::SystemColors::Window;
			dataGridViewCellStyle1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			dataGridViewCellStyle1->ForeColor = System::Drawing::SystemColors::ControlText;
			dataGridViewCellStyle1->SelectionBackColor = System::Drawing::SystemColors::Highlight;
			dataGridViewCellStyle1->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
			dataGridViewCellStyle1->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
			display->DefaultCellStyle = dataGridViewCellStyle1;
			display->Location = System::Drawing::Point(0, 26);
			display->Name = L"display";
			display->ReadOnly = true;
			display->RowHeadersVisible = false;
			display->RowHeadersWidthSizeMode = System::Windows::Forms::DataGridViewRowHeadersWidthSizeMode::DisableResizing;
			display->ScrollBars = System::Windows::Forms::ScrollBars::None;
			display->Size = System::Drawing::Size(525, 363);
			display->TabIndex = 3;
			// 
			// input
			// 
			this->input->AcceptsTab = true;
			this->input->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->input->Font = (gcnew System::Drawing::Font(L"Consolas", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->input->Location = System::Drawing::Point(0, 418);
			this->input->Multiline = false;
			this->input->Name = L"input";
			this->input->ScrollBars = System::Windows::Forms::RichTextBoxScrollBars::None;
			this->input->Size = System::Drawing::Size(525, 22);
			this->input->TabIndex = 4;
			this->input->Text = L"";
			this->input->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &TextBuddyUI::input_KeyUp_1);
			// 
			// feedback
			// 
			this->feedback->BackColor = System::Drawing::Color::White;
			this->feedback->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->feedback->Font = (gcnew System::Drawing::Font(L"Arial", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->feedback->ForeColor = System::Drawing::Color::Green;
			this->feedback->Location = System::Drawing::Point(12, 395);
			this->feedback->Name = L"feedback";
			this->feedback->ReadOnly = true;
			this->feedback->Size = System::Drawing::Size(501, 16);
			this->feedback->TabIndex = 1;
			// 
			// floatingTaskDisplay
			// 
			this->floatingTaskDisplay->BackColor = System::Drawing::Color::White;
			this->floatingTaskDisplay->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->floatingTaskDisplay->Font = (gcnew System::Drawing::Font(L"Arial", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->floatingTaskDisplay->ForeColor = System::Drawing::Color::Magenta;
			this->floatingTaskDisplay->Location = System::Drawing::Point(12, 0);
			this->floatingTaskDisplay->Multiline = false;
			this->floatingTaskDisplay->Name = L"floatingTaskDisplay";
			this->floatingTaskDisplay->ReadOnly = true;
			this->floatingTaskDisplay->ScrollBars = System::Windows::Forms::RichTextBoxScrollBars::None;
			this->floatingTaskDisplay->Size = System::Drawing::Size(501, 20);
			this->floatingTaskDisplay->TabIndex = 5;
			this->floatingTaskDisplay->Text = L"";
			// 
			// hotKey
			// 
			this->hotKey->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &TextBuddyUI::hotKey_DoWork);
			// 
			// updateFloatingTimer
			// 
			this->updateFloatingTimer->Enabled = true;
			this->updateFloatingTimer->Interval = 5000;
			this->updateFloatingTimer->Tick += gcnew System::EventHandler(this, &TextBuddyUI::updateFloatingTimer_Tick);
			// 
			// id
			// 
			this->id->HeaderText = L"ID";
			this->id->Name = L"id";
			this->id->ReadOnly = true;
			this->id->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			this->id->Width = 20;
			// 
			// Label
			// 
			this->Label->HeaderText = L"Label";
			this->Label->Name = L"Label";
			this->Label->ReadOnly = true;
			this->Label->Width = 80;
			// 
			// description
			// 
			this->description->HeaderText = L"        Description";
			this->description->Name = L"description";
			this->description->ReadOnly = true;
			this->description->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			this->description->Width = 270;
			// 
			// dateAndTime
			// 
			this->dateAndTime->HeaderText = L"Date/Time";
			this->dateAndTime->Name = L"dateAndTime";
			this->dateAndTime->ReadOnly = true;
			this->dateAndTime->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			this->dateAndTime->Width = 160;
			// 
			// TextBuddyUI
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::White;
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->ClientSize = System::Drawing::Size(525, 440);
			this->Controls->Add(this->floatingTaskDisplay);
			this->Controls->Add(this->input);
			this->Controls->Add(this->feedback);
			this->Controls->Add(display);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"TextBuddyUI";
			this->Text = L"TextBuddyUI";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(display))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();
		}

#pragma endregion

// @@author A0126677U (Soon Hao Ye)
//*********************** AUTHOR : Soon Hao Ye ********************************

	//================== PRIVATE VARIABLES=====================================
	private:
		std::string* userInput;
		std::string* userFeedback_cppString;
		Logic *logic;
		int floatingTaskIndex;
		int cursorPosition;
		int originalRowPosition;
		String^ searchPhrase;
		List<String^>^ keywords;

		//to be subscribed
		std::vector<std::string>* labels;
		std::vector<std::string>* taskDescription;
		std::vector<std::string>* dateTime;
		std::vector<std::string>* floatingTasks;
		std::vector<bool>* priotiryTasks;

	//===================== UI FUNCTIONS=======================================
	private:	

		/*=====================================================================
			reads the characters from textbox
			converts the entire input from CLI String to std::string
			stores the input into class variable
		=====================================================================*/
		void getInput() {
			msclr::interop::marshal_context context;
			userInput = new std::string(context.marshal_as<std::string>(input->Text));			
		}

		/*=====================================================================
			passes UserInput to Logic
			prints feedback message if needed
			updates current display of task if needed
			updates floating tasks entries
		=====================================================================*/
		void processAndExecute() {
			feedback->Clear();
			std::string message;
			try {
				feedback->ForeColor = Color::Green;
				message = logic->processCommand(*userInput);
				updateDisplay();			
				printFeedBackMessage(message);
				delete userInput;
			} catch(std::exception e) {
				feedback->ForeColor = Color::Red;
				//suggestHelpPage()
			}
		}

		/*=====================================================================
			loops through the subscribed fields
			writes data to the cells :
				-ID
				-Description
				-Label
				-Date/Time
		=====================================================================*/
		void updateDisplay() {
			DataGridView^ display = description->DataGridView ;
			display->Rows->Clear();
			unsigned int size = taskDescription->size();
			for(unsigned int i=0 ; i< size ; ++i) {
				String^ index = gcnew String((i+1).ToString());
				String^ label = gcnew String((*labels)[i].c_str());
				String^ title = gcnew String((*taskDescription)[i].c_str());
				String^ dt = gcnew String((*dateTime)[i].c_str());
				display->Rows->Add(index,label,title,dt);
				if((*priotiryTasks)[i]) {
					display->Rows[i]->DefaultCellStyle->ForeColor = Color::Red;				
				} else {
					display->Rows[i]->DefaultCellStyle->ForeColor = Color::Black;
				}
				display->FirstDisplayedScrollingRowIndex = originalRowPosition;
			}

/*
			for(unsigned int i=0 ; i<tasks.size() ; ++i) {
				Task currentTask = tasks[i];
				bool isUrgent = currentTask.getPriorityStatus();
			//	TaskType type = currentTask.getType();
				String^ name = gcnew String(currentTask.getName().c_str());
				std::set<std::string> labels = currentTask.getLabels();
				std::set<std::string>::iterator labelsCurr = labels.begin();
				std::string l;
				for(unsigned int j=0; j< labels.size(); ++j) {
					l = l + *labelsCurr + " ";
					labelsCurr++;
				}
				String^ label = gcnew String(l.c_str());
				String^ dateTime = gcnew String(currentTask.getDateAndTime_UI().c_str());
				display->Rows->Add((i+1).ToString(),label,name,dateTime);
//				display->FirstDisplayedScrollingRowIndex = originalRowPosition;
				if(isUrgent) {
					display->Rows[i]->DefaultCellStyle->ForeColor = Color::Red;
				} else {
					display->Rows[i]->DefaultCellStyle->ForeColor = Color::Black;
				}
			}*/
		}

		void printFeedBackMessage(std::string message) {
			feedback->Text = gcnew String(message.c_str());
		}

		/*=====================================================================
			reads the characters as the user types in the richTextBox
			change colour to blue if they match keywords
			KEYWORDS TO HIGHLIGHT
			-ADD			-FROM
			-MODIFY			-BY
			-SEARCH			-TO
			-DISPLAY		-ON
			-DONE			-AT
			-TAG			**MORE TO COME**
			-STAR
			-UNSTAR
		=====================================================================*/
		void highlightSyntax() {		
			input->SelectionColor = Color::Black;
			for each(String^ keyword in keywords) {
				int position = findKeyword(keyword);
				if(keywordIsFound(position)) {
					highlightKeywords(position,keyword);
				} 
			}
		}

		/*====================================================================
			Finds the keyword in the user Input richTextBox
			Returns the position of the first occurrence of keyword
			-1 is returned if keyword is not found
		=====================================================================*/
		int findKeyword(String^ keyword) {
			int position; 
			position = input->Find(keyword,0,input->Text->Length,
				RichTextBoxFinds::NoHighlight);
			return position;
		}

		bool keywordIsFound(int position) {
			return position >= 0;
		}

		/*=====================================================================
			Performs the change of text in the input richTextBox
			keywords are changed to BLUE
		=====================================================================*/
		void highlightKeywords(int position, String^ keyword) {
			input->Select(position,keyword->Length);
			input->SelectionColor = Color::Blue;
			input->Select(cursorPosition,0);
			input->SelectionColor = Color::Black;
		}

		/*=====================================================================
			When the user types in "search" as the first word in input box,
			automatically performs search and updates display as user types
		=====================================================================*/
		void autoComplete() {
			int position = findKeyword(SEARCH);
			if(keywordIsFound(position) && position == 0) {
				getInput();
				if(input->Text != SEARCH) { 
					processAndExecute();
				} 
			}
		}

		/*====================================================================
								REQUIRES REFINEMENT!!
		=====================================================================*/
		void undoSearch() {
			if(input->Text == SEARCH) {
				*userInput = "undo";	
				processAndExecute();	
			}
		}

		void scrollDown() {
			DataGridView^ display = description->DataGridView ;
			if(display->FirstDisplayedScrollingRowIndex < display->RowCount )
			display->FirstDisplayedScrollingRowIndex = 
					display->FirstDisplayedScrollingRowIndex + 1;
			originalRowPosition = display->FirstDisplayedScrollingRowIndex;
		}

		void scrollUp() {
			DataGridView^ display = description->DataGridView ;
			if(display->FirstDisplayedScrollingRowIndex > 0 )
			display->FirstDisplayedScrollingRowIndex = 
					display->FirstDisplayedScrollingRowIndex - 1;
			originalRowPosition = display->FirstDisplayedScrollingRowIndex;
		}

//************************** EVENT HANDLERS ***********************************

		//====================== MAIN FUNCTION ================================
private: 
	System::Void input_KeyUp_1(System::Object^  sender,
		System::Windows::Forms::KeyEventArgs^  e) {
			cursorPosition = input->SelectionStart;
			if(e->KeyCode == Keys::Return) { //if user presses 'Return' key
				getInput();
				processAndExecute();
				input->Clear();
			} else if(e->KeyCode != Keys::Left && e->KeyCode != Keys::Right){
				highlightSyntax();
				autoComplete();
			} else {
				 autoComplete();
			}
			if(e->KeyCode == Keys::Back) {
				undoSearch();
			}
			if(e->KeyCode == Keys::Down) {
				scrollDown();
			}
			if(e->KeyCode == Keys::Up) {
				scrollUp();
			}
		 }

		//======= Updates floating Tasks display textbox every 5 seconds ======
private: 
	System::Void updateFloatingTimer_Tick(System::Object^  sender, System::EventArgs^  e) {
		if(floatingTasks != nullptr && !floatingTasks->empty()) {	
				floatingTaskIndex = floatingTaskIndex % floatingTasks->size();
				String^ title = gcnew String(
					(*floatingTasks)[floatingTaskIndex].c_str());
				floatingTaskDisplay->Text = title;
				++floatingTaskIndex;
		}
	}
private: 
	System::Void hotKey_DoWork(System::Object^  sender, 
			 System::ComponentModel::DoWorkEventArgs^  e) {
		DWORD foreThread = GetWindowThreadProcessId(GetForegroundWindow(), nullptr);
		DWORD appThread = GetCurrentThreadId();
			//	const DWORD SW_SHOW = 5;

		if (foreThread != appThread) {
			AttachThreadInput(foreThread, appThread, true);
		}
		RegisterHotKey(NULL,NULL,MOD_ALT,0x54);
		MSG msg;
		if (GetMessage(&msg, nullptr, 0, 0)) {
			MessageBox::Show("hotkey!");
			SetForegroundWindow(static_cast<HWND>(Handle.ToPointer()));
		}
	}
};
}
