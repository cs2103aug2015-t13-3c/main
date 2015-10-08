#pragma once
#include "Logic.h"
#include "stdafx.h"
#include <msclr\marshal_cppstd.h>


namespace UserInterface {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;

	public ref class TextBuddyUI : public System::Windows::Forms::Form {

	public:
		TextBuddyUI(void) {
			InitializeComponent();
			logic = new Logic();
		}

	protected:
		~TextBuddyUI() {
			if (components) {
				delete components;
				delete logic;
			}
		}
	private:
		System::Windows::Forms::TextBox^  input;
		System::Windows::Forms::TextBox^  feedback;
		System::Windows::Forms::DataGridViewTextBoxColumn^  id;
		System::Windows::Forms::DataGridViewTextBoxColumn^  description;
		System::Windows::Forms::DataGridViewTextBoxColumn^  dateAndTime;
		System::ComponentModel::IContainer^  components;

#pragma region Windows Form Designer generated code

		/// Required method for Designer support - do not modify	
		void InitializeComponent(void) {
			System::Windows::Forms::DataGridView^  display;
			this->id = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->description = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dateAndTime = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->input = (gcnew System::Windows::Forms::TextBox());
			this->feedback = (gcnew System::Windows::Forms::TextBox());
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
			display->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			display->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(3) {this->id, this->description, 
				this->dateAndTime});
			display->Location = System::Drawing::Point(0, 0);
			display->Name = L"display";
			display->ReadOnly = true;
			display->RowHeadersVisible = false;
			display->RowHeadersWidthSizeMode = System::Windows::Forms::DataGridViewRowHeadersWidthSizeMode::DisableResizing;
			display->ScrollBars = System::Windows::Forms::ScrollBars::None;
			display->Size = System::Drawing::Size(449, 379);
			display->TabIndex = 3;
			// 
			// id
			// 
			this->id->HeaderText = L"ID";
			this->id->Name = L"id";
			this->id->ReadOnly = true;
			this->id->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			this->id->Width = 40;
			// 
			// description
			// 
			this->description->HeaderText = L"Description";
			this->description->Name = L"description";
			this->description->ReadOnly = true;
			this->description->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			this->description->Width = 300;
			// 
			// dateAndTime
			// 
			this->dateAndTime->HeaderText = L"Date/Time";
			this->dateAndTime->Name = L"dateAndTime";
			this->dateAndTime->ReadOnly = true;
			this->dateAndTime->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			this->dateAndTime->Width = 110;
			// 
			// input
			// 
			this->input->AcceptsReturn = true;
			this->input->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->input->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->input->Location = System::Drawing::Point(0, 418);
			this->input->Name = L"input";
			this->input->Size = System::Drawing::Size(449, 22);
			this->input->TabIndex = 0;
			this->input->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &TextBuddyUI::input_KeyDown);
			// 
			// feedback
			// 
			this->feedback->BackColor = System::Drawing::Color::White;
			this->feedback->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->feedback->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->feedback->ForeColor = System::Drawing::Color::Green;
			this->feedback->Location = System::Drawing::Point(12, 385);
			this->feedback->Multiline = true;
			this->feedback->Name = L"feedback";
			this->feedback->ReadOnly = true;
			this->feedback->Size = System::Drawing::Size(425, 27);
			this->feedback->TabIndex = 1;
			// 
			// TextBuddyUI
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::White;
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->ClientSize = System::Drawing::Size(449, 440);
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

// author : Soon Hao Ye

	private:
		std::string* userInput;
		std::string* userFeedback_cppString;
		Logic* logic ;

	private:
		
		void getInput() {
			//convert CLR/C++ String to C++ std::string
			msclr::interop::marshal_context context;
			userInput = new std::string(context.marshal_as<std::string>(input->Text));
			input->Clear();
		}

		void processAndExecute() {
			feedback->Clear();
			Feedback results = logic->processCommand(*userInput);
			if(results.needToUpdateDisplay()) {
				updateDisplay(results.getTaskToShow());
			}
			if(results.isSuccess()) {
				feedback->ForeColor = Color::Green;
			} else {
				feedback->ForeColor = Color::Red;
			}
			feedback->Text = gcnew String((results.getFeedbackMessage()).c_str());

			logic->saveFile("Text.txt");
		}

		void updateDisplay(std::vector<Task> tasks) {
			DataGridView^ display = description->DataGridView ;
			display->Rows->Clear();

			for(unsigned int i=0 ; i<tasks.size() ; ++i) {
				TaskType type = tasks[i].getType();
				if(type == TODO) {

				} else if (type == EVENT) {

				}
				String^ name = gcnew String(tasks[i].getName().c_str());
				display->Rows->Add((i+1).ToString(),name);
			}
		}


//=======================EVENT HANDLERS=======================================

	// get input and calls logic when return key is pressed
	private: System::Void input_KeyDown(System::Object^  sender, 
				 System::Windows::Forms::KeyEventArgs^  e) {
		if (e->KeyCode == Keys::Return) { //if user presses 'Return' key
			getInput();
			processAndExecute();
		}
	}

};
}
