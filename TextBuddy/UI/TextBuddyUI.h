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

	/// <summary>
	/// Summary for TextBuddyUI
	/// </summary>
	public ref class TextBuddyUI : public System::Windows::Forms::Form
	{
	private:
		//TODO : INITIALIZE LOGIC OBJECT

	public:
		TextBuddyUI(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			
		}

	protected:

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~TextBuddyUI()
		{
			if (components)
			{
				delete components;
				
			}
		//	attachThread->Abort();
		//	hotKeyThread->Abort();
		}
	private:
		System::Windows::Forms::TextBox^  input;
		Thread^ attachThread;
		Thread^ hotKeyThread;

	private: System::Windows::Forms::TextBox^  feedback;
	protected:

	private:
		void attachThreadToOtherApps() {
			while (true) {
				DWORD foreThread = GetWindowThreadProcessId(GetForegroundWindow(), nullptr);
				DWORD appThread = GetCurrentThreadId();
				//	const DWORD SW_SHOW = 5;

				if (foreThread != appThread) {
					AttachThreadInput(foreThread, appThread, true);
					//		BringWindowToTop(hWnd);
					//		ShowWindow(hWnd, SW_SHOW);
				}
			}
		}

		void forceWindowPopUp() {
			while (true) {
				MSG msg;
				if (GetMessage(&msg, nullptr, 0, 0)) {
					if (true) {
						MessageBox::Show("hotkey!");
						SetForegroundWindow(static_cast<HWND>(Handle.ToPointer()));
					}
				}
			}
		}
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->input = (gcnew System::Windows::Forms::TextBox());
			this->feedback = (gcnew System::Windows::Forms::TextBox());
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
			// TextBuddyUI
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::Control;
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->ClientSize = System::Drawing::Size(449, 440);
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
	private: System::Void input_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
		if (e->KeyCode == Keys::Return) {
			// call logic

			//get input
			msclr::interop::marshal_context context;
			std::string userInput = context.marshal_as<std::string>(input->Text);
			input->Clear();
		}
	}

};
}
