#pragma once
#include "Logic.h"
#include "stdafx.h"
#include <msclr\marshal_cppstd.h>

//DEFINES STRINGS
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
		TextBuddyUI(void) {
			InitializeComponent();
			logic = new Logic();
			input->Focus();
			this->ActiveControl = input;
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
		}

	protected:
		~TextBuddyUI() {
			if (components) {
				delete components;
				delete logic;
			}
		}

	private: System::Windows::Forms::RichTextBox^  input;
	private: System::Windows::Forms::TextBox^  feedback;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  id;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  description;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Label;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dateAndTime;
	private: System::Windows::Forms::RichTextBox^  richTextBox1;
	private: System::ComponentModel::IContainer^  components;

#pragma region Windows Form Designer generated code

		/// Required method for Designer support - do not modify	
		void InitializeComponent(void) {
			System::Windows::Forms::DataGridView^  display;
			this->id = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->description = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Label = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dateAndTime = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->input = (gcnew System::Windows::Forms::RichTextBox());
			this->feedback = (gcnew System::Windows::Forms::TextBox());
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
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
			display->BackgroundColor = System::Drawing::Color::White;
			display->BorderStyle = System::Windows::Forms::BorderStyle::None;
			display->CellBorderStyle = System::Windows::Forms::DataGridViewCellBorderStyle::None;
			display->ColumnHeadersBorderStyle = System::Windows::Forms::DataGridViewHeaderBorderStyle::Single;
			display->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			display->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(4) {this->id, this->description, 
				this->Label, this->dateAndTime});
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
			// description
			// 
			this->description->HeaderText = L"        Description";
			this->description->Name = L"description";
			this->description->ReadOnly = true;
			this->description->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			this->description->Width = 270;
			// 
			// Label
			// 
			this->Label->HeaderText = L"Label";
			this->Label->Name = L"Label";
			this->Label->ReadOnly = true;
			this->Label->Width = 80;
			// 
			// dateAndTime
			// 
			this->dateAndTime->HeaderText = L"Date/Time";
			this->dateAndTime->Name = L"dateAndTime";
			this->dateAndTime->ReadOnly = true;
			this->dateAndTime->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			this->dateAndTime->Width = 160;
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
			// richTextBox1
			// 
			this->richTextBox1->BackColor = System::Drawing::Color::White;
			this->richTextBox1->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->richTextBox1->Location = System::Drawing::Point(12, 0);
			this->richTextBox1->Multiline = false;
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->ReadOnly = true;
			this->richTextBox1->Size = System::Drawing::Size(501, 20);
			this->richTextBox1->TabIndex = 5;
			this->richTextBox1->Text = L"";
			// 
			// TextBuddyUI
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::White;
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->ClientSize = System::Drawing::Size(525, 440);
			this->Controls->Add(this->richTextBox1);
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

//*********************** AUTHOR : Soon Hao Ye ********************************

	//================== PRIVATE VARIABLES=====================================
	private:
		std::string* userInput;
		std::string* userFeedback_cppString;
		Logic* logic ;
		int cursorPosition;
		List<String^>^ keywords;

	//===================== UI FUNCTIONS=======================================
	private:	

		/*=====================================================================
			reads the characters from textbox
			converts the entire input from CLI String to std::string
			stores the input into class variable
			clears the textbox
		=====================================================================*/
		void getInput() {
			msclr::interop::marshal_context context;
			userInput = new std::string(context.marshal_as<std::string>(input->Text));
			input->Clear();
		}

		/*=====================================================================
			passes UserInput to Logic
			prints feedback message if needed
			updates current display of task if needed
		=====================================================================*/
		void processAndExecute() {
			feedback->Clear();
			Feedback results = logic->processCommand(*userInput);
			if(results.needToUpdateDisplay()) {
				updateDisplay(results.getTaskToShow());
			}
			configureMessageColor(&results);
			printFeedBackMessage(results.getFeedbackMessage());
			delete userInput;
		}

		/*=====================================================================
			loops through the vector returned by logic
			writes data to the cells :
				-ID
				-Description
				-Label
				-Date/Time
		=====================================================================*/
		void updateDisplay(std::vector<Task> tasks) {
			DataGridView^ display = description->DataGridView ;
			display->Rows->Clear();

			for(unsigned int i=0 ; i<tasks.size() ; ++i) {
				TaskType type = tasks[i].getType();
				String^ name = gcnew String(tasks[i].getName().c_str());
				display->Rows->Add((i+1).ToString(),name);
			}
		}
		/*=====================================================================
			change feedback message text colour 
				- green  : ok
				- red	 : error
				- yellow : warning
		=====================================================================*/
		void configureMessageColor(Feedback* results) {
			if(results->isWarning()) {
				feedback->ForeColor = Color::Yellow;
			} else if(results->isSuccess()) {
				feedback->ForeColor = Color::Green;
			} else {
				feedback->ForeColor = Color::Red;
			}
		}

		void printFeedBackMessage(std::string message) {
			feedback->Text = gcnew String(message.c_str());
		}

		/*=====================================================================
			reads the characters as the user types in the richTextBox
			change colour of words if they match keywords
		=====================================================================*/
		void syntaxHighlight() {			
			int position; 
			for each(String^ keyword in keywords) {
			position = input->Find(keyword,0,input->Text->Length,RichTextBoxFinds::NoHighlight);
				if(position >= 0) {
					input->Select(position,keyword->Length);
					input->SelectionColor = Color::Blue;
					input->Select(cursorPosition,0);
					input->SelectionColor = Color::Black;
				} 
		/*	position = input->Find(" by ",0,input->Text->Length,RichTextBoxFinds::NoHighlight);
				if(position >0) {
					input->Select(position,4);
					input->SelectionColor = Color::Blue;
					input->Select(cursorPosition,0);
					input->SelectionColor = Color::Black;
				}*/
			}
		}

		void autoComplete() {

		}

	//********************** EVENT HANDLERS ***********************************

		//====================== MAIN FUNCTION ================================
private: 
	System::Void input_KeyUp_1(System::Object^  sender,
		System::Windows::Forms::KeyEventArgs^  e) {
			cursorPosition = input->SelectionStart;
			if (e->KeyCode == Keys::Return) { //if user presses 'Return' key
				getInput();
				processAndExecute();
			} else if(e->KeyCode != Keys::Left && e->KeyCode != Keys::Right){
				syntaxHighlight();
				autoComplete();
			 }
		 }
};
}
