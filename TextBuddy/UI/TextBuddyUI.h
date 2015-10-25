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
#define DISPLAY "display "
#define VIEW "view"
#define DONE "done "
#define STAR "star "
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
		TextBuddyUI(void);

	protected:
		~TextBuddyUI();

		// @@author generated
		// @@windows form designer generated
	private: System::Windows::Forms::RichTextBox^  input;
	private: System::Windows::Forms::TextBox^  feedback;
	private: System::Windows::Forms::RichTextBox^  floatingTaskDisplay;
	private: System::Windows::Forms::Timer^  updateFloatingTimer;

	private: System::Windows::Forms::TextBox^  currentTime;
	private: System::Windows::Forms::Timer^  updateCurrentTime;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  id;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Label;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  description;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  date;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  time;

	private: System::ComponentModel::IContainer^  components;

#pragma region Windows Form Designer generated code

			 /// Required method for Designer support - do not modify	
			 void InitializeComponent(void) {
				 this->components = (gcnew System::ComponentModel::Container());
				 System::Windows::Forms::DataGridView^  display;
				 System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle1 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
				 this->id = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				 this->Label = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				 this->description = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				 this->date = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				 this->time = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				 this->input = (gcnew System::Windows::Forms::RichTextBox());
				 this->feedback = (gcnew System::Windows::Forms::TextBox());
				 this->floatingTaskDisplay = (gcnew System::Windows::Forms::RichTextBox());
				 this->updateFloatingTimer = (gcnew System::Windows::Forms::Timer(this->components));
				 this->currentTime = (gcnew System::Windows::Forms::TextBox());
				 this->updateCurrentTime = (gcnew System::Windows::Forms::Timer(this->components));
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
				 display->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(5) {this->id, this->Label, this->description, 
					 this->date, this->time});
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
				 // date
				 // 
				 this->date->HeaderText = L"Date";
				 this->date->Name = L"date";
				 this->date->ReadOnly = true;
				 this->date->Resizable = System::Windows::Forms::DataGridViewTriState::False;
				 this->date->Width = 80;
				 // 
				 // time
				 // 
				 this->time->HeaderText = L"Time";
				 this->time->Name = L"time";
				 this->time->ReadOnly = true;
				 this->time->Resizable = System::Windows::Forms::DataGridViewTriState::False;
				 this->time->Width = 80;
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
				 this->floatingTaskDisplay->Size = System::Drawing::Size(379, 20);
				 this->floatingTaskDisplay->TabIndex = 5;
				 this->floatingTaskDisplay->Text = L"";
				 // 
				 // updateFloatingTimer
				 // 
				 this->updateFloatingTimer->Enabled = true;
				 this->updateFloatingTimer->Interval = 5000;
				 this->updateFloatingTimer->Tick += gcnew System::EventHandler(this, &TextBuddyUI::updateFloatingTimer_Tick);
				 // 
				 // currentTime
				 // 
				 this->currentTime->BackColor = System::Drawing::Color::White;
				 this->currentTime->BorderStyle = System::Windows::Forms::BorderStyle::None;
				 this->currentTime->Location = System::Drawing::Point(397, 7);
				 this->currentTime->Name = L"currentTime";
				 this->currentTime->ReadOnly = true;
				 this->currentTime->Size = System::Drawing::Size(128, 13);
				 this->currentTime->TabIndex = 6;
				 // 
				 // updateCurrentTime
				 // 
				 this->updateCurrentTime->Enabled = true;
				 this->updateCurrentTime->Interval = 5000;
				 this->updateCurrentTime->Tick += gcnew System::EventHandler(this, &TextBuddyUI::updateCurrentTime_Tick);
				 // 
				 // TextBuddyUI
				 // 
				 this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
				 this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				 this->BackColor = System::Drawing::Color::White;
				 this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
				 this->ClientSize = System::Drawing::Size(525, 440);
				 this->Controls->Add(this->currentTime);
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

			 //****************** AUTHOR : Soon Hao Ye *******************
			 // Soon Hao Ye @@author A0126677U

			 //==================== PRIVATE VARIABLES ====================
	private:
		std::string* userInput;
		std::string* userFeedback_cppString;
		Logic* logic;
		TbLogger* logger;
		int floatingTaskIndex;
		int cursorPosition;
		int originalRowPosition;
		String^ searchPhrase;
		List<String^>^ keywords;

		// To be subscribed
		std::vector<std::string>* labels;
		std::vector<std::string>* taskDescription;
		std::vector<std::string>* taskDate;
		std::vector<std::string>* taskTime;
		std::vector<std::string>* floatingTasks;
		std::vector<int>* color;

		//===================== UI FUNCTIONS=======================================
	private:	
		System::Void input_KeyUp_1(System::Object^  sender,
			System::Windows::Forms::KeyEventArgs^  e);
		System::Void updateFloatingTimer_Tick(System::Object^  sender, System::EventArgs^  e);
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
		void undoSearch();

		void scrollDown();
		void scrollUp();
	};
}