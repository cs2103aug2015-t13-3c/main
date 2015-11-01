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

	public ref class TextBuddyUI : public MetroForm {

	public:
		TextBuddyUI(void);

	protected:
		~TextBuddyUI();

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
	private: MetroFramework::Controls::MetroTabPage^  deadline;
	private: MetroFramework::Controls::MetroTabPage^  events;
	private: MetroFramework::Controls::MetroTabPage^  floating;
	private: System::Windows::Forms::DataGridView^  todayTimeline;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  timeline;
	private: MetroFramework::Controls::MetroTabPage^  metroTabPage1;










	private: System::ComponentModel::IContainer^  components;

#pragma region Windows Form Designer generated code

			 /// Required method for Designer support - do not modify	
			 void InitializeComponent(void) {
				 this->components = (gcnew System::ComponentModel::Container());
				 this->input = (gcnew System::Windows::Forms::RichTextBox());
				 this->feedback = (gcnew System::Windows::Forms::TextBox());
				 this->currentTime = (gcnew System::Windows::Forms::TextBox());
				 this->updateCurrentTime = (gcnew System::Windows::Forms::Timer(this->components));
				 this->help = (gcnew System::Windows::Forms::PictureBox());
				 this->dropDown = (gcnew System::Windows::Forms::ComboBox());
				 this->tabs = (gcnew MetroFramework::Controls::MetroTabControl());
				 this->all = (gcnew MetroFramework::Controls::MetroTabPage());
				 this->today = (gcnew MetroFramework::Controls::MetroTabPage());
				 this->todayTimeline = (gcnew System::Windows::Forms::DataGridView());
				 this->timeline = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				 this->week = (gcnew MetroFramework::Controls::MetroTabPage());
				 this->events = (gcnew MetroFramework::Controls::MetroTabPage());
				 this->deadline = (gcnew MetroFramework::Controls::MetroTabPage());
				 this->floating = (gcnew MetroFramework::Controls::MetroTabPage());
				 this->metroTabPage1 = (gcnew MetroFramework::Controls::MetroTabPage());
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->help))->BeginInit();
				 this->tabs->SuspendLayout();
				 this->today->SuspendLayout();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->todayTimeline))->BeginInit();
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
				 this->input->Size = System::Drawing::Size(600, 21);
				 this->input->TabIndex = 0;
				 this->input->Text = L"";
				 this->input->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &TextBuddyUI::input_KeyDown);
				 this->input->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &TextBuddyUI::input_KeyUp);
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
				 this->feedback->Size = System::Drawing::Size(600, 18);
				 this->feedback->TabIndex = 1;
				 this->feedback->TabStop = false;
				 // 
				 // currentTime
				 // 
				 this->currentTime->BackColor = System::Drawing::Color::White;
				 this->currentTime->BorderStyle = System::Windows::Forms::BorderStyle::None;
				 this->currentTime->Location = System::Drawing::Point(443, 55);
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
				 this->updateCurrentTime->Tick += gcnew System::EventHandler(this, &TextBuddyUI::updateCurrentTime_Tick);
				 // 
				 // help
				 // 
				 this->help->Location = System::Drawing::Point(0, 51);
				 this->help->Margin = System::Windows::Forms::Padding(2, 3, 2, 3);
				 this->help->Name = L"help";
				 this->help->Size = System::Drawing::Size(600, 550);
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
				 this->dropDown->Size = System::Drawing::Size(600, 21);
				 this->dropDown->TabIndex = 9;
				 this->dropDown->TabStop = false;
				 this->dropDown->DropDown += gcnew System::EventHandler(this, &TextBuddyUI::dropDown_DropDown);
				 // 
				 // tabs
				 // 
				 this->tabs->Controls->Add(this->today);
				 this->tabs->Controls->Add(this->all);
				 this->tabs->Controls->Add(this->floating);
				 this->tabs->Controls->Add(this->metroTabPage1);
				 this->tabs->Controls->Add(this->week);
				 this->tabs->Controls->Add(this->events);
				 this->tabs->Controls->Add(this->deadline);
				 this->tabs->ItemSize = System::Drawing::Size(30, 20);
				 this->tabs->Location = System::Drawing::Point(0, 51);
				 this->tabs->Name = L"tabs";
				 this->tabs->SelectedIndex = 6;
				 this->tabs->Size = System::Drawing::Size(600, 548);
				 this->tabs->TabIndex = 10;
				 this->tabs->TabStop = false;
				 this->tabs->Theme = MetroFramework::MetroThemeStyle::Light;
				 this->tabs->UseSelectable = true;
				 // 
				 // all
				 // 
				 this->all->AutoScroll = true;
				 this->all->HorizontalScrollbarBarColor = false;
				 this->all->HorizontalScrollbarHighlightOnWheel = false;
				 this->all->HorizontalScrollbarSize = 10;
				 this->all->Location = System::Drawing::Point(4, 24);
				 this->all->Margin = System::Windows::Forms::Padding(3, 0, 3, 3);
				 this->all->Name = L"all";
				 this->all->Size = System::Drawing::Size(592, 520);
				 this->all->TabIndex = 0;
				 this->all->Text = L"All";
				 this->all->VerticalScrollbar = true;
				 this->all->VerticalScrollbarBarColor = true;
				 this->all->VerticalScrollbarHighlightOnWheel = true;
				 this->all->VerticalScrollbarSize = 8;
				 // 
				 // today
				 // 
				 this->today->AutoScroll = true;
				 this->today->Controls->Add(this->todayTimeline);
				 this->today->HorizontalScrollbar = true;
				 this->today->HorizontalScrollbarBarColor = true;
				 this->today->HorizontalScrollbarHighlightOnWheel = false;
				 this->today->HorizontalScrollbarSize = 10;
				 this->today->Location = System::Drawing::Point(4, 24);
				 this->today->Name = L"today";
				 this->today->Size = System::Drawing::Size(592, 520);
				 this->today->TabIndex = 1;
				 this->today->Text = L"Today";
				 this->today->VerticalScrollbar = true;
				 this->today->VerticalScrollbarBarColor = true;
				 this->today->VerticalScrollbarHighlightOnWheel = true;
				 this->today->VerticalScrollbarSize = 8;
				 // 
				 // todayTimeline
				 // 
				 this->todayTimeline->AllowUserToAddRows = false;
				 this->todayTimeline->AllowUserToDeleteRows = false;
				 this->todayTimeline->AllowUserToResizeColumns = false;
				 this->todayTimeline->AllowUserToResizeRows = false;
				 this->todayTimeline->BackgroundColor = System::Drawing::Color::White;
				 this->todayTimeline->BorderStyle = System::Windows::Forms::BorderStyle::None;
				 this->todayTimeline->ColumnHeadersBorderStyle = System::Windows::Forms::DataGridViewHeaderBorderStyle::Single;
				 this->todayTimeline->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
				 this->todayTimeline->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(1) {this->timeline});
				 this->todayTimeline->Location = System::Drawing::Point(3, 77);
				 this->todayTimeline->Name = L"todayTimeline";
				 this->todayTimeline->ReadOnly = true;
				 this->todayTimeline->RowHeadersVisible = false;
				 this->todayTimeline->Size = System::Drawing::Size(65, 446);
				 this->todayTimeline->TabIndex = 2;
				 // 
				 // timeline
				 // 
				 this->timeline->HeaderText = L"time";
				 this->timeline->Name = L"timeline";
				 this->timeline->ReadOnly = true;
				 this->timeline->Width = 60;
				 // 
				 // week
				 // 
				 this->week->AutoScroll = true;
				 this->week->HorizontalScrollbar = true;
				 this->week->HorizontalScrollbarBarColor = true;
				 this->week->HorizontalScrollbarHighlightOnWheel = false;
				 this->week->HorizontalScrollbarSize = 10;
				 this->week->Location = System::Drawing::Point(4, 24);
				 this->week->Name = L"week";
				 this->week->Size = System::Drawing::Size(592, 520);
				 this->week->TabIndex = 2;
				 this->week->Text = L"Week";
				 this->week->VerticalScrollbar = true;
				 this->week->VerticalScrollbarBarColor = true;
				 this->week->VerticalScrollbarHighlightOnWheel = true;
				 this->week->VerticalScrollbarSize = 8;
				 // 
				 // events
				 // 
				 this->events->AutoScroll = true;
				 this->events->HorizontalScrollbar = true;
				 this->events->HorizontalScrollbarBarColor = true;
				 this->events->HorizontalScrollbarHighlightOnWheel = false;
				 this->events->HorizontalScrollbarSize = 10;
				 this->events->Location = System::Drawing::Point(4, 24);
				 this->events->Name = L"events";
				 this->events->Size = System::Drawing::Size(592, 520);
				 this->events->TabIndex = 4;
				 this->events->Text = L"Events";
				 this->events->VerticalScrollbar = true;
				 this->events->VerticalScrollbarBarColor = true;
				 this->events->VerticalScrollbarHighlightOnWheel = true;
				 this->events->VerticalScrollbarSize = 8;
				 // 
				 // deadline
				 // 
				 this->deadline->AutoScroll = true;
				 this->deadline->HorizontalScrollbar = true;
				 this->deadline->HorizontalScrollbarBarColor = true;
				 this->deadline->HorizontalScrollbarHighlightOnWheel = false;
				 this->deadline->HorizontalScrollbarSize = 10;
				 this->deadline->Location = System::Drawing::Point(4, 24);
				 this->deadline->Name = L"deadline";
				 this->deadline->Size = System::Drawing::Size(592, 520);
				 this->deadline->TabIndex = 3;
				 this->deadline->Text = L"Deadlines";
				 this->deadline->VerticalScrollbar = true;
				 this->deadline->VerticalScrollbarBarColor = true;
				 this->deadline->VerticalScrollbarHighlightOnWheel = true;
				 this->deadline->VerticalScrollbarSize = 8;
				 // 
				 // floating
				 // 
				 this->floating->AutoScroll = true;
				 this->floating->HorizontalScrollbar = true;
				 this->floating->HorizontalScrollbarBarColor = true;
				 this->floating->HorizontalScrollbarHighlightOnWheel = false;
				 this->floating->HorizontalScrollbarSize = 10;
				 this->floating->Location = System::Drawing::Point(4, 24);
				 this->floating->Name = L"floating";
				 this->floating->Size = System::Drawing::Size(592, 520);
				 this->floating->TabIndex = 5;
				 this->floating->Text = L"Floating";
				 this->floating->VerticalScrollbar = true;
				 this->floating->VerticalScrollbarBarColor = true;
				 this->floating->VerticalScrollbarHighlightOnWheel = true;
				 this->floating->VerticalScrollbarSize = 8;
				 // 
				 // metroTabPage1
				 // 
				 this->metroTabPage1->AutoScroll = true;
				 this->metroTabPage1->HorizontalScrollbar = true;
				 this->metroTabPage1->HorizontalScrollbarBarColor = true;
				 this->metroTabPage1->HorizontalScrollbarHighlightOnWheel = false;
				 this->metroTabPage1->HorizontalScrollbarSize = 10;
				 this->metroTabPage1->Location = System::Drawing::Point(4, 24);
				 this->metroTabPage1->Name = L"metroTabPage1";
				 this->metroTabPage1->Size = System::Drawing::Size(592, 520);
				 this->metroTabPage1->TabIndex = 6;
				 this->metroTabPage1->Text = L"Search";
				 this->metroTabPage1->VerticalScrollbar = true;
				 this->metroTabPage1->VerticalScrollbarBarColor = true;
				 this->metroTabPage1->VerticalScrollbarHighlightOnWheel = true;
				 this->metroTabPage1->VerticalScrollbarSize = 8;
				 // 
				 // TextBuddyUI
				 // 
				 this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
				 this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				 this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
				 this->ClientSize = System::Drawing::Size(600, 644);
				 this->Controls->Add(this->currentTime);
				 this->Controls->Add(this->tabs);
				 this->Controls->Add(this->feedback);
				 this->Controls->Add(this->help);
				 this->Controls->Add(this->input);
				 this->Controls->Add(this->dropDown);
				 this->Margin = System::Windows::Forms::Padding(2, 3, 2, 3);
				 this->MaximizeBox = false;
				 this->Name = L"TextBuddyUI";
				 this->Padding = System::Windows::Forms::Padding(0, 60, 0, 0);
				 this->Resizable = false;
				 this->Style = MetroFramework::MetroColorStyle::Orange;
				 this->Text = L"UI";
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->help))->EndInit();
				 this->tabs->ResumeLayout(false);
				 this->today->ResumeLayout(false);
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->todayTimeline))->EndInit();
				 this->ResumeLayout(false);
				 this->PerformLayout();

			 }

#pragma endregion

			 //****************** AUTHOR : Soon Hao Ye *******************
			 // Soon Hao Ye @@author A0126677U

			 //==================== PRIVATE VARIABLES ====================
	private:
		DisplayMode mode;
		bool selectingFields;
		bool helpMode;
		std::string* userInput;
		std::string* userFeedback_cppString;
		Logic* logic;
		TbLogger* logger;
		int cursorPosition;
		int originalRowPosition;
		int inputHistoryCount;
		String^ searchPhrase;
		System::Collections::Hashtable^ suggestions;
		System::Collections::Generic::List<String^>^ inputHistory;
		System::Collections::Generic::List<String^>^ keywords;
		System::Collections::Generic::List<String^>^ addCommands;
		System::Collections::Generic::List<String^>^ modifyCommands;
		System::Collections::Generic::List<String^>^ viewCommands;
		System::Collections::Generic::List<String^>^ searchCommands;

		// To be subscribed
		std::vector<DisplayedTask>* tasks;

		//===================== UI FUNCTIONS=======================================
	private:	
		System::Void input_KeyDown(System::Object^  sender,
			System::Windows::Forms::KeyEventArgs^  e);
		System::Void input_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
		System::Void updateCurrentTime_Tick(System::Object^  sender, System::EventArgs^  e);

		/*=====================================================================
		reads the characters from textbox
		converts the entire input from CLI String to std::string
		stores the input into class variable
		=====================================================================*/
		void getInput();

		/*=====================================================================
		passes UserInput to Logic
		prints feedback message if needed
		updates current display of task if needed
		updates floating tasks entries
		=====================================================================*/
		void processAndExecute();

		/*=====================================================================
		loops through the subscribed fields
		writes data to the cells :
		-ID
		-Description
		-Label
		-Date/Time
		=====================================================================*/
		void updateDisplay();
		void printFeedBackMessage(std::string message);

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
		void highlightSyntax();
		void undoHighlight();

		/*====================================================================
		Finds the keyword in the user Input richTextBox
		Returns the position of the first occurrence of keyword
		-1 is returned if keyword is not found
		=====================================================================*/
		int findKeyword(String^ keyword);
		bool keywordIsFound(int position);

		/*=====================================================================
		Performs the change of text in the input richTextBox
		keywords are changed to BLUE
		=====================================================================*/
		void highlightKeywords(int position, String^ keyword);

		/*=====================================================================
		When the user types in "search" as the first word in input box,
		automatically performs search and updates display as user types
		=====================================================================*/
		void autoComplete();
		void searchAutoComplete();
		void commandAutoComplete();
		void viewAutoComplete();
		void putSuggestedText(String^ keyword);
		void undoSearch();
		bool matchKeyword(String^ keyword);
		void showSuggestedCommands(String^ keyword);
		void selectFields();

		void scrollDown();
		void scrollUp();

		System::Void dropDown_DropDown(System::Object^  sender, System::EventArgs^  e); 

};
}