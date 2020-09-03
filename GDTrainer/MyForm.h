#pragma once


#include <Windows.h>
#include <string>
#include <chrono>
#include <thread>
#include <msclr\marshal_cppstd.h>

typedef void(__stdcall* fPasteFunction)(std::string testString);

namespace GDTrainer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
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
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::TextBox^ textBox1;
	protected:

	private:
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
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(25, 20);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(131, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Level loader by Adafcaefc";
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(28, 62);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(530, 41);
			this->button1->TabIndex = 1;
			this->button1->Text = L"Load";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(28, 36);
			this->textBox1->MaxLength = 0;
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(530, 20);
			this->textBox1->TabIndex = 2;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(582, 128);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->label1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->Name = L"MyForm";
			this->Text = L"Geometry Dash Live Editor";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {

		DWORD oldProtect, newProtect;

		DWORD base = (DWORD)GetModuleHandleA(0);
		DWORD libcocosbase = (DWORD)GetModuleHandleA("libcocos2d.dll");

		fPasteFunction pasteFunction = (fPasteFunction)(base + 0x88240);

		VirtualProtect((LPVOID)(libcocosbase + 0xC16A3), 8, PAGE_EXECUTE_READWRITE, &oldProtect);
		*((__int64*)(libcocosbase + 0xC16A3)) = 0x0E74000000026DE9;

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		msclr::interop::marshal_context context;
		std::string objectString = context.marshal_as<std::string>(textBox1->Text);
		pasteFunction(objectString);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		*((__int64*)(libcocosbase + 0xC16A3)) = 0x0E74000000958638;

		VirtualProtect((LPVOID)(libcocosbase + 0xC16A3), 8, oldProtect, &newProtect);

	}
	private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
	}
	};
}
