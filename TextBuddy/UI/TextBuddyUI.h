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
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  id;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  description;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dateAndTime;








			 System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code

		/// Required method for Designer support - do not modify	
		void InitializeComponent(void) {
			System::Windows::Forms::DataGridView^  display;
			this->input = (gcnew System::Windows::Forms::TextBox());
			this->feedback = (gcnew System::Windows::Forms::TextBox());
			this->id = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
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
			display->BackgroundColor = System::Drawing::Color::White;
			display->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			display->CellBorderStyle = System::Windows::Forms::DataGridViewCellBorderStyle::None;
			display->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			display->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(3) {this->id, this->description, 
				this->dateAndTime});
			display->Dock = System::Windows::Forms::DockStyle::Fill;
			display->Location = System::Drawing::Point(0, 0);
			display->Name = L"display";
			display->ReadOnly = true;
			display->RowHeadersVisible = false;
			display->RowHeadersWidthSizeMode = System::Windows::Forms::DataGridViewRowHeadersWidthSizeMode::DisableResizing;
			display->ScrollBars = System::Windows::Forms::ScrollBars::None;
			display->Size = System::Drawing::Size(449, 440);
			display->TabIndex = 3;
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
			this->feedback->BackColor = System::Drawing::SystemColors::InfoText;
			this->feedback->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->feedback->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 13, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->feedback->ForeColor = System::Drawing::Color::Green;
			this->feedback->Location = System::Drawing::Point(0, 392);
			this->feedback->Name = L"feedback";
			this->feedback->ReadOnly = true;
			this->feedback->Size = System::Drawing::Size(449, 20);
			this->feedback->TabIndex = 1;
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
			// TextBuddyUI
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::Azure;
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->ClientSize = System::Drawing::Size(449, 440);
			this->Controls->Add(this->input);
			this->Controls->Add(this->feedback);
			this->Controls->Add(display);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"TextBuddyUI";
			this->Opacity = 0.8;
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
			userFeedback_cppString = new std::string;
			*userFeedback_cppString = logic->processCommand(*userInput);
			String^ userFeedback = gcnew String(userFeedback_cppString->c_str());
			//display->AppendText(userFeedback + "\r\n");
			delete userInput;
			delete userFeedback_cppString;
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
