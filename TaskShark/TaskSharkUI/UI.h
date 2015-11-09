// Soon Hao Ye @@author A0126677U

#pragma once

#include "stdafx.h"
#include "Logic.h"
#include <msclr\marshal_cppstd.h>

// DEFINE KEYWORD STRINGS
#define ADD "add "
#define DEL "delete "
#define MODIFY "modify "
#define SEARCH "search "
#define UNDO "undo"
#define HELP "help"
#define VIEW "view "
#define DONE "done "
#define SAVE "save "
#define CLEAR "clear all"
#define QUIT "exit"
#define FROM " from "
#define BY " by "
#define TO " to "
#define ON " on "
#define AT " at "
#define DISPLAY "display"
#define EDIT "edit "
#define LOAD "load "
#define NOTDONE "notdone "
#define PICK "pick "
#define REDO "redo"

namespace UserInterface {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;
	using namespace MetroFramework::Forms;
	using namespace MetroFramework::Controls;

	public ref class UI : public MetroForm {

	public:
		UI(void);

	protected:
		~UI();

		// @@author generated
		// @@windows form designer generated
	private: System::Windows::Forms::RichTextBox^  input;
	private: System::Windows::Forms::TextBox^  feedback;
	private: System::Windows::Forms::TextBox^  currentTime;
	private: System::Windows::Forms::Timer^  updateCurrentTime;
	private: System::Windows::Forms::PictureBox^  help;
	private: System::Windows::Forms::ComboBox^  dropDown;
	private: MetroFramework::Controls::MetroTabControl^  tabs;
	private: MetroFramework::Controls::MetroTabPage^  all;
	private: MetroFramework::Controls::MetroTabPage^  today;
	private: MetroFramework::Controls::MetroTabPage^  week;
	private: MetroFramework::Controls::MetroTabPage^  events;
	private: MetroFramework::Controls::MetroTabPage^  todo;
	private: MetroFramework::Controls::MetroTabPage^  floating;
	private: MetroFramework::Controls::MetroTabPage^  search;
	private: MetroFramework::Controls::MetroTabPage^  past;
	private: System::Windows::Forms::DataGridView^  display;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  id;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  label;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  description;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Date;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  time;
	private: System::ComponentModel::IContainer^  components;

#pragma region Windows Form Designer generated code

			 /// Required method for Designer support - do not modify	
			 void InitializeComponent(void) {
				 this->components = (gcnew System::ComponentModel::Container());
				 System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle1 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
				 System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle2 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
				 this->input = (gcnew System::Windows::Forms::RichTextBox());
				 this->feedback = (gcnew System::Windows::Forms::TextBox());
				 this->currentTime = (gcnew System::Windows::Forms::TextBox());
				 this->updateCurrentTime = (gcnew System::Windows::Forms::Timer(this->components));
				 this->help = (gcnew System::Windows::Forms::PictureBox());
				 this->dropDown = (gcnew System::Windows::Forms::ComboBox());
				 this->tabs = (gcnew MetroFramework::Controls::MetroTabControl());
				 this->all = (gcnew MetroFramework::Controls::MetroTabPage());
				 this->display = (gcnew System::Windows::Forms::DataGridView());
				 this->id = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				 this->label = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				 this->description = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				 this->Date = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				 this->time = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				 this->today = (gcnew MetroFramework::Controls::MetroTabPage());
				 this->week = (gcnew MetroFramework::Controls::MetroTabPage());
				 this->events = (gcnew MetroFramework::Controls::MetroTabPage());
				 this->todo = (gcnew MetroFramework::Controls::MetroTabPage());
				 this->floating = (gcnew MetroFramework::Controls::MetroTabPage());
				 this->search = (gcnew MetroFramework::Controls::MetroTabPage());
				 this->past = (gcnew MetroFramework::Controls::MetroTabPage());
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->help))->BeginInit();
				 this->tabs->SuspendLayout();
				 this->all->SuspendLayout();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->display))->BeginInit();
				 this->SuspendLayout();
				 // 
				 // input
				 // 
				 this->input->Font = (gcnew System::Drawing::Font(L"Consolas", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
					 static_cast<System::Byte>(0)));
				 this->input->Location = System::Drawing::Point(0, 622);
				 this->input->Margin = System::Windows::Forms::Padding(2, 3, 2, 3);
				 this->input->Multiline = false;
				 this->input->Name = L"input";
				 this->input->ScrollBars = System::Windows::Forms::RichTextBoxScrollBars::None;
				 this->input->Size = System::Drawing::Size(747, 21);
				 this->input->TabIndex = 0;
				 this->input->Text = L"";
				 this->input->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &UI::input_KeyDown);
				 this->input->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &UI::input_KeyUp);
				 // 
				 // feedback
				 // 
				 this->feedback->BackColor = System::Drawing::Color::White;
				 this->feedback->BorderStyle = System::Windows::Forms::BorderStyle::None;
				 this->feedback->Dock = System::Windows::Forms::DockStyle::Bottom;
				 this->feedback->Font = (gcnew System::Drawing::Font(L"Segoe UI Semibold", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
					 static_cast<System::Byte>(0)));
				 this->feedback->ForeColor = System::Drawing::Color::Green;
				 this->feedback->Location = System::Drawing::Point(0, 605);
				 this->feedback->Margin = System::Windows::Forms::Padding(2, 3, 2, 3);
				 this->feedback->Name = L"feedback";
				 this->feedback->ReadOnly = true;
				 this->feedback->Size = System::Drawing::Size(749, 18);
				 this->feedback->TabIndex = 1;
				 this->feedback->TabStop = false;
				 // 
				 // currentTime
				 // 
				 this->currentTime->BackColor = System::Drawing::Color::White;
				 this->currentTime->BorderStyle = System::Windows::Forms::BorderStyle::None;
				 this->currentTime->Location = System::Drawing::Point(600, 55);
				 this->currentTime->Margin = System::Windows::Forms::Padding(2, 3, 2, 3);
				 this->currentTime->Name = L"currentTime";
				 this->currentTime->ReadOnly = true;
				 this->currentTime->Size = System::Drawing::Size(140, 13);
				 this->currentTime->TabIndex = 6;
				 this->currentTime->TabStop = false;
				 this->currentTime->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
				 // 
				 // updateCurrentTime
				 // 
				 this->updateCurrentTime->Enabled = true;
				 this->updateCurrentTime->Interval = 5000;
				 this->updateCurrentTime->Tick += gcnew System::EventHandler(this, &UI::updateCurrentTime_Tick);
				 // 
				 // help
				 // 
				 this->help->Location = System::Drawing::Point(0, 51);
				 this->help->Margin = System::Windows::Forms::Padding(2, 3, 2, 3);
				 this->help->Name = L"help";
				 this->help->Size = System::Drawing::Size(747, 550);
				 this->help->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
				 this->help->TabIndex = 8;
				 this->help->TabStop = false;
				 this->help->Visible = false;
				 // 
				 // dropDown
				 // 
				 this->dropDown->Dock = System::Windows::Forms::DockStyle::Bottom;
				 this->dropDown->FormattingEnabled = true;
				 this->dropDown->Location = System::Drawing::Point(0, 623);
				 this->dropDown->Margin = System::Windows::Forms::Padding(2, 3, 2, 3);
				 this->dropDown->Name = L"dropDown";
				 this->dropDown->Size = System::Drawing::Size(749, 21);
				 this->dropDown->TabIndex = 9;
				 this->dropDown->TabStop = false;
				 // 
				 // tabs
				 // 
				 this->tabs->Controls->Add(this->all);
				 this->tabs->Controls->Add(this->today);
				 this->tabs->Controls->Add(this->week);
				 this->tabs->Controls->Add(this->events);
				 this->tabs->Controls->Add(this->todo);
				 this->tabs->Controls->Add(this->floating);
				 this->tabs->Controls->Add(this->search);
				 this->tabs->Controls->Add(this->past);
				 this->tabs->ItemSize = System::Drawing::Size(30, 20);
				 this->tabs->Location = System::Drawing::Point(0, 51);
				 this->tabs->Name = L"tabs";
				 this->tabs->SelectedIndex = 0;
				 this->tabs->Size = System::Drawing::Size(745, 548);
				 this->tabs->TabIndex = 10;
				 this->tabs->TabStop = false;
				 this->tabs->Theme = MetroFramework::MetroThemeStyle::Light;
				 this->tabs->UseSelectable = true;
				 this->tabs->SelectedIndexChanged += gcnew System::EventHandler(this, &UI::tabs_SelectedIndexChanged);
				 // 
				 // all
				 // 
				 this->all->Controls->Add(this->display);
				 this->all->HorizontalScrollbarBarColor = true;
				 this->all->HorizontalScrollbarHighlightOnWheel = false;
				 this->all->HorizontalScrollbarSize = 10;
				 this->all->Location = System::Drawing::Point(4, 24);
				 this->all->Name = L"all";
				 this->all->Size = System::Drawing::Size(737, 520);
				 this->all->TabIndex = 0;
				 this->all->Text = L"All";
				 this->all->VerticalScrollbarBarColor = true;
				 this->all->VerticalScrollbarHighlightOnWheel = false;
				 this->all->VerticalScrollbarSize = 10;
				 // 
				 // display
				 // 
				 this->display->AllowUserToAddRows = false;
				 this->display->AllowUserToDeleteRows = false;
				 this->display->AllowUserToResizeColumns = false;
				 this->display->AllowUserToResizeRows = false;
				 this->display->AutoSizeRowsMode = System::Windows::Forms::DataGridViewAutoSizeRowsMode::AllCells;
				 this->display->BackgroundColor = System::Drawing::Color::White;
				 this->display->BorderStyle = System::Windows::Forms::BorderStyle::None;
				 this->display->ColumnHeadersBorderStyle = System::Windows::Forms::DataGridViewHeaderBorderStyle::Single;
				 this->display->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
				 this->display->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(5) {this->id, this->label, 
					 this->description, this->Date, this->time});
				 dataGridViewCellStyle1->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
				 dataGridViewCellStyle1->BackColor = System::Drawing::SystemColors::Window;
				 dataGridViewCellStyle1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, 
					 System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
				 dataGridViewCellStyle1->ForeColor = System::Drawing::SystemColors::ControlText;
				 dataGridViewCellStyle1->SelectionBackColor = System::Drawing::SystemColors::Highlight;
				 dataGridViewCellStyle1->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
				 dataGridViewCellStyle1->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
				 this->display->DefaultCellStyle = dataGridViewCellStyle1;
				 this->display->EditMode = System::Windows::Forms::DataGridViewEditMode::EditProgrammatically;
				 this->display->ImeMode = System::Windows::Forms::ImeMode::Disable;
				 this->display->Location = System::Drawing::Point(-1, 3);
				 this->display->MultiSelect = false;
				 this->display->Name = L"display";
				 this->display->ReadOnly = true;
				 this->display->RowHeadersVisible = false;
				 this->display->RowHeadersWidthSizeMode = System::Windows::Forms::DataGridViewRowHeadersWidthSizeMode::DisableResizing;
				 dataGridViewCellStyle2->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
				 dataGridViewCellStyle2->BackColor = System::Drawing::SystemColors::Window;
				 dataGridViewCellStyle2->Font = (gcnew System::Drawing::Font(L"Segoe UI Semibold", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
					 static_cast<System::Byte>(0)));
				 dataGridViewCellStyle2->ForeColor = System::Drawing::SystemColors::ControlText;
				 dataGridViewCellStyle2->SelectionBackColor = System::Drawing::SystemColors::Highlight;
				 dataGridViewCellStyle2->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
				 dataGridViewCellStyle2->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
				 this->display->RowsDefaultCellStyle = dataGridViewCellStyle2;
				 this->display->ScrollBars = System::Windows::Forms::ScrollBars::None;
				 this->display->ShowCellErrors = false;
				 this->display->ShowCellToolTips = false;
				 this->display->ShowEditingIcon = false;
				 this->display->ShowRowErrors = false;
				 this->display->Size = System::Drawing::Size(746, 521);
				 this->display->TabIndex = 11;
				 this->display->TabStop = false;
				 this->display->Click += gcnew System::EventHandler(this, &UI::display_Click);
				 // 
				 // id
				 // 
				 this->id->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::None;
				 this->id->HeaderText = L"ID";
				 this->id->Name = L"id";
				 this->id->ReadOnly = true;
				 this->id->Resizable = System::Windows::Forms::DataGridViewTriState::False;
				 this->id->Width = 35;
				 // 
				 // label
				 // 
				 this->label->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::None;
				 this->label->HeaderText = L"Label";
				 this->label->Name = L"label";
				 this->label->ReadOnly = true;
				 this->label->Resizable = System::Windows::Forms::DataGridViewTriState::True;
				 // 
				 // description
				 // 
				 this->description->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::None;
				 this->description->HeaderText = L"Description";
				 this->description->Name = L"description";
				 this->description->ReadOnly = true;
				 this->description->Resizable = System::Windows::Forms::DataGridViewTriState::True;
				 this->description->Width = 350;
				 // 
				 // Date
				 // 
				 this->Date->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::None;
				 this->Date->HeaderText = L"Date";
				 this->Date->Name = L"Date";
				 this->Date->ReadOnly = true;
				 this->Date->Resizable = System::Windows::Forms::DataGridViewTriState::True;
				 this->Date->Width = 125;
				 // 
				 // time
				 // 
				 this->time->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::None;
				 this->time->HeaderText = L"Time";
				 this->time->Name = L"time";
				 this->time->ReadOnly = true;
				 this->time->Resizable = System::Windows::Forms::DataGridViewTriState::True;
				 this->time->Width = 130;
				 // 
				 // today
				 // 
				 this->today->HorizontalScrollbarBarColor = true;
				 this->today->HorizontalScrollbarHighlightOnWheel = false;
				 this->today->HorizontalScrollbarSize = 10;
				 this->today->Location = System::Drawing::Point(4, 24);
				 this->today->Name = L"today";
				 this->today->Size = System::Drawing::Size(737, 520);
				 this->today->TabIndex = 1;
				 this->today->Text = L"Today";
				 this->today->VerticalScrollbarBarColor = true;
				 this->today->VerticalScrollbarHighlightOnWheel = false;
				 this->today->VerticalScrollbarSize = 10;
				 // 
				 // week
				 // 
				 this->week->HorizontalScrollbarBarColor = true;
				 this->week->HorizontalScrollbarHighlightOnWheel = false;
				 this->week->HorizontalScrollbarSize = 10;
				 this->week->Location = System::Drawing::Point(4, 24);
				 this->week->Name = L"week";
				 this->week->Size = System::Drawing::Size(737, 520);
				 this->week->TabIndex = 2;
				 this->week->Text = L"Week";
				 this->week->VerticalScrollbarBarColor = true;
				 this->week->VerticalScrollbarHighlightOnWheel = false;
				 this->week->VerticalScrollbarSize = 10;
				 // 
				 // events
				 // 
				 this->events->HorizontalScrollbarBarColor = true;
				 this->events->HorizontalScrollbarHighlightOnWheel = false;
				 this->events->HorizontalScrollbarSize = 10;
				 this->events->Location = System::Drawing::Point(4, 24);
				 this->events->Name = L"events";
				 this->events->Size = System::Drawing::Size(737, 520);
				 this->events->TabIndex = 3;
				 this->events->Text = L"Events";
				 this->events->VerticalScrollbarBarColor = true;
				 this->events->VerticalScrollbarHighlightOnWheel = false;
				 this->events->VerticalScrollbarSize = 10;
				 // 
				 // todo
				 // 
				 this->todo->HorizontalScrollbarBarColor = true;
				 this->todo->HorizontalScrollbarHighlightOnWheel = false;
				 this->todo->HorizontalScrollbarSize = 10;
				 this->todo->Location = System::Drawing::Point(4, 24);
				 this->todo->Name = L"todo";
				 this->todo->Size = System::Drawing::Size(737, 520);
				 this->todo->TabIndex = 4;
				 this->todo->Text = L"Todo";
				 this->todo->VerticalScrollbarBarColor = true;
				 this->todo->VerticalScrollbarHighlightOnWheel = false;
				 this->todo->VerticalScrollbarSize = 10;
				 // 
				 // floating
				 // 
				 this->floating->HorizontalScrollbarBarColor = true;
				 this->floating->HorizontalScrollbarHighlightOnWheel = false;
				 this->floating->HorizontalScrollbarSize = 10;
				 this->floating->Location = System::Drawing::Point(4, 24);
				 this->floating->Name = L"floating";
				 this->floating->Size = System::Drawing::Size(737, 520);
				 this->floating->TabIndex = 5;
				 this->floating->Text = L"Floating";
				 this->floating->VerticalScrollbarBarColor = true;
				 this->floating->VerticalScrollbarHighlightOnWheel = false;
				 this->floating->VerticalScrollbarSize = 10;
				 // 
				 // search
				 // 
				 this->search->HorizontalScrollbarBarColor = true;
				 this->search->HorizontalScrollbarHighlightOnWheel = false;
				 this->search->HorizontalScrollbarSize = 10;
				 this->search->Location = System::Drawing::Point(4, 24);
				 this->search->Name = L"search";
				 this->search->Size = System::Drawing::Size(737, 520);
				 this->search->TabIndex = 6;
				 this->search->Text = L"Search";
				 this->search->VerticalScrollbarBarColor = true;
				 this->search->VerticalScrollbarHighlightOnWheel = false;
				 this->search->VerticalScrollbarSize = 10;
				 // 
				 // past
				 // 
				 this->past->HorizontalScrollbarBarColor = true;
				 this->past->HorizontalScrollbarHighlightOnWheel = false;
				 this->past->HorizontalScrollbarSize = 10;
				 this->past->Location = System::Drawing::Point(4, 24);
				 this->past->Name = L"past";
				 this->past->Size = System::Drawing::Size(737, 520);
				 this->past->TabIndex = 7;
				 this->past->Text = L"Past";
				 this->past->VerticalScrollbarBarColor = true;
				 this->past->VerticalScrollbarHighlightOnWheel = false;
				 this->past->VerticalScrollbarSize = 10;
				 // 
				 // UI
				 // 
				 this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
				 this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				 this->AutoValidate = System::Windows::Forms::AutoValidate::EnableAllowFocusChange;
				 this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
				 this->BackImagePadding = System::Windows::Forms::Padding(14, 15, 0, 0);
				 this->BackMaxSize = 35;
				 this->ClientSize = System::Drawing::Size(749, 644);
				 this->Controls->Add(this->currentTime);
				 this->Controls->Add(this->tabs);
				 this->Controls->Add(this->feedback);
				 this->Controls->Add(this->help);
				 this->Controls->Add(this->input);
				 this->Controls->Add(this->dropDown);
				 this->KeyPreview = true;
				 this->Margin = System::Windows::Forms::Padding(2, 3, 2, 3);
				 this->MaximizeBox = false;
				 this->Name = L"UI";
				 this->Padding = System::Windows::Forms::Padding(0, 60, 0, 0);
				 this->Resizable = false;
				 this->Style = MetroFramework::MetroColorStyle::Blue;
				 this->Text = L"      TaskShark";
				 this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &UI::UI_KeyDown);
				 this->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &UI::UI_KeyUp);
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->help))->EndInit();
				 this->tabs->ResumeLayout(false);
				 this->all->ResumeLayout(false);
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->display))->EndInit();
				 this->ResumeLayout(false);
				 this->PerformLayout();

			 }

#pragma endregion

//========================== PRIVATE VARIABLES ================================
	private:	
		Logic* logic;
		TsLogger* logger;
		std::string* userInput;
		std::string* userFeedback_cppString;
		int inputHistoryCount;
		
		// To be subscribed
		std::vector<DisplayedTask>* tasks;

		// Flags for different states
		DisplayMode mode;
		bool tileView;
		bool selectingFields;
		bool helpMode;

		// Data structures to store autoComplete suggestions
		System::Collections::Hashtable^ suggestions;
		System::Collections::Generic::List<String^>^ inputHistory;
		System::Collections::Generic::List<String^>^ keywords;
		System::Collections::Generic::List<String^>^ addCommands;
		System::Collections::Generic::List<String^>^ modifyCommands;		
		System::Collections::Generic::List<String^>^ loadCommands;
		System::Collections::Generic::List<String^>^ pickCommands;
		System::Collections::Generic::List<String^>^ saveCommands;
		System::Collections::Generic::List<String^>^ searchCommands;
		System::Collections::Generic::List<String^>^ viewCommands;

//========================= UI FUNCTIONS=======================================
	private:
		// Definition in Keypress.cpp
		System::Void input_KeyUp(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e);
		System::Void input_KeyDown(System::Object^ sender,System::Windows::Forms::KeyEventArgs^ e);
		System::Void UI_KeyUp(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e);
		System::Void UI_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e);	
		System::Void display_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void tabs_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e); 
		
		// Definition in Execute.cpp
		void getInput();
		void processAndExecute();
		void updateDisplay();
		void updateTable(TabPage^ currentTab);
		int getTabIndex();
		void addTilesToTab(TabPage^ currentTab);
		String^ wrapWord(std::string content);
		void printFeedBackMessage(std::string message);
		void closeHelpMode();
		void scrollUp();
		void scrollDown();
		System::Void updateCurrentTime_Tick(System::Object^ sender, System::EventArgs^ e);

		// Definition in Dropdown.cpp
		void commandAutoComplete();
		bool matchKeyword(String^ keyword);
		void showSuggestedCommands(String^ keyword);
		void incrementDropCount();
		void decrementDropCount();
		bool acceptAutosuggest();
		void selectFields();

		// Definition in CommandHistory.cpp
		void addCommandHistory();
		void toPreviousCommand();
		void toNextCommand();
	};
}