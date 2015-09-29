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
	private: System::Windows::Forms::RichTextBox^  display;

		System::Windows::Forms::TextBox^  feedback;
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code

		/// Required method for Designer support - do not modify	
		void InitializeComponent(void) {
			this->input = (gcnew System::Windows::Forms::TextBox());
			this->feedback = (gcnew System::Windows::Forms::TextBox());
			this->display = (gcnew System::Windows::Forms::RichTextBox());
			this->SuspendLayout();
			// 
			// input
			// 
			this->input->AcceptsReturn = true;
			this->input->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->input->Location = System::Drawing::Point(0, 420);
			this->input->Name = L"input";
			this->input->Size = System::Drawing::Size(449, 20);
			this->input->TabIndex = 0;
			this->input->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &TextBuddyUI::input_KeyDown);
			// 
			// feedback
			// 
			this->feedback->BackColor = System::Drawing::SystemColors::InfoText;
			this->feedback->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->feedback->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->feedback->ForeColor = System::Drawing::Color::Green;
			this->feedback->Location = System::Drawing::Point(12, 400);
			this->feedback->Name = L"feedback";
			this->feedback->ReadOnly = true;
			this->feedback->Size = System::Drawing::Size(422, 13);
			this->feedback->TabIndex = 1;
			// 
			// display
			// 
			this->display->Location = System::Drawing::Point(13, 13);
			this->display->Name = L"display";
			this->display->Size = System::Drawing::Size(421, 381);
			this->display->TabIndex = 2;
			this->display->Text = L"";
			// 
			// TextBuddyUI
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::Control;
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->ClientSize = System::Drawing::Size(449, 440);
			this->Controls->Add(this->display);
			this->Controls->Add(this->input);
			this->Controls->Add(this->feedback);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"TextBuddyUI";
			this->Text = L"TextBuddyUI";
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
			display->AppendText(userFeedback);
			delete userInput;
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
