#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

//extern cIOSMDriverInterface*				g_pIOSMDriverInterface;
extern cIOSMBaseDevice*						g_pIOSMBaseDevice;
extern cProbableIO*							g_pProbableIO;
extern cActiveIOTimesWithSingnalCircle*		g_pActiveIOTimesWithSingnalCircle;
extern cListenMultiSingnalToActiveIO*		g_pListenMultiSingnalToActiveIO;
extern cSendHighSingnal*					g_pSendHighSingnal;
extern cSendLowSingnal*						g_pSendLowSingnal;
namespace IODevice {

	/// <summary>
	/// Summary for AllDeviceStatusUI
	/// </summary>
	public ref class AllDeviceStatusUI : public System::Windows::Forms::UserControl
	{
	public:
		AllDeviceStatusUI(GCFORM::ListBox^e_pListBox)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_TargetListBox = e_pListBox;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~AllDeviceStatusUI()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::FlowLayoutPanel^  flowLayoutPanel1;
	private: System::Windows::Forms::NumericUpDown^  ProbableIO_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  StopAfterTimes_numericUpDown;
	private: System::Windows::Forms::ListBox^  ProbableIO_listBox;
	private: System::Windows::Forms::ListBox^  MultiTriigerIO_listBox;
	private: System::Windows::Forms::Panel^  cProbableIO_panel;
	private: System::Windows::Forms::Panel^  cActiveIOTimesWithSingnalCircle_panel;
	private: System::Windows::Forms::Panel^  cListenMultiSingnalToActiveIO_panel;
	private: System::Windows::Forms::Button^  MultiTriigerIODel_button;
	private: System::Windows::Forms::Button^  MultiTriigerIOAdd_button;
	private: System::Windows::Forms::Button^  ProbableIODel_button;
	private: System::Windows::Forms::Button^  ProbableIOAdd_button;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::CheckBox^  cProbableIOSingnal_checkBox;
	private: System::Windows::Forms::CheckBox^  cListenMultiSingnalToActiveIOSingnal_checkBox;
	private: System::Windows::Forms::Panel^  cSendHighSingnal_panel;
	private: System::Windows::Forms::ListBox^  SendHighSingnal_listBox;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Button^  SendHighSingnalDel_button;
	private: System::Windows::Forms::Button^  SendHighSingnalAdd_button;
	private: System::Windows::Forms::Panel^  cSendLowSingnal_panel;
	private: System::Windows::Forms::ListBox^  SendLowSingnal_listBox;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Button^  SendLowSingnalDel_button;
	private: System::Windows::Forms::Button^  SendLowSingnalAdd_button;
			 //my
		GCFORM::ListBox^m_TargetListBox;
		System::Void	ClearProbableIOData();
		System::Void	ClearActiveIOTimesWithSingnalCircleIOData();
		System::Void	ClearcListenMultiSingnalToActiveIOData();
		System::Void	ClearcSendHighSinnalIOData();
		System::Void	ClearcSendLowSinnalIOData();
	public:
		System::Void	ClearBehaviorData(const WCHAR*e_strType);
		void	AssignData(cIOSMBaseDevice*e_pIOSMBaseDevice);
		void	AssignListData(GCFORM::ListBox^e_pListBox,cObjectListByName<cIOSMIOData>*e_pIOSMIODataList,bool e_bAdd);
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
			this->flowLayoutPanel1 = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->cProbableIO_panel = (gcnew System::Windows::Forms::Panel());
			this->cProbableIOSingnal_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->ProbableIO_listBox = (gcnew System::Windows::Forms::ListBox());
			this->ProbableIO_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->ProbableIODel_button = (gcnew System::Windows::Forms::Button());
			this->ProbableIOAdd_button = (gcnew System::Windows::Forms::Button());
			this->cActiveIOTimesWithSingnalCircle_panel = (gcnew System::Windows::Forms::Panel());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->StopAfterTimes_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->cListenMultiSingnalToActiveIO_panel = (gcnew System::Windows::Forms::Panel());
			this->cListenMultiSingnalToActiveIOSingnal_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->MultiTriigerIO_listBox = (gcnew System::Windows::Forms::ListBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->MultiTriigerIODel_button = (gcnew System::Windows::Forms::Button());
			this->MultiTriigerIOAdd_button = (gcnew System::Windows::Forms::Button());
			this->cSendHighSingnal_panel = (gcnew System::Windows::Forms::Panel());
			this->SendHighSingnal_listBox = (gcnew System::Windows::Forms::ListBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->SendHighSingnalDel_button = (gcnew System::Windows::Forms::Button());
			this->SendHighSingnalAdd_button = (gcnew System::Windows::Forms::Button());
			this->cSendLowSingnal_panel = (gcnew System::Windows::Forms::Panel());
			this->SendLowSingnal_listBox = (gcnew System::Windows::Forms::ListBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->SendLowSingnalDel_button = (gcnew System::Windows::Forms::Button());
			this->SendLowSingnalAdd_button = (gcnew System::Windows::Forms::Button());
			this->flowLayoutPanel1->SuspendLayout();
			this->cProbableIO_panel->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ProbableIO_numericUpDown))->BeginInit();
			this->cActiveIOTimesWithSingnalCircle_panel->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->StopAfterTimes_numericUpDown))->BeginInit();
			this->cListenMultiSingnalToActiveIO_panel->SuspendLayout();
			this->cSendHighSingnal_panel->SuspendLayout();
			this->cSendLowSingnal_panel->SuspendLayout();
			this->SuspendLayout();
			// 
			// flowLayoutPanel1
			// 
			this->flowLayoutPanel1->Controls->Add(this->cProbableIO_panel);
			this->flowLayoutPanel1->Controls->Add(this->cActiveIOTimesWithSingnalCircle_panel);
			this->flowLayoutPanel1->Controls->Add(this->cListenMultiSingnalToActiveIO_panel);
			this->flowLayoutPanel1->Controls->Add(this->cSendHighSingnal_panel);
			this->flowLayoutPanel1->Controls->Add(this->cSendLowSingnal_panel);
			this->flowLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->flowLayoutPanel1->FlowDirection = System::Windows::Forms::FlowDirection::TopDown;
			this->flowLayoutPanel1->Location = System::Drawing::Point(0, 0);
			this->flowLayoutPanel1->Margin = System::Windows::Forms::Padding(0);
			this->flowLayoutPanel1->Name = L"flowLayoutPanel1";
			this->flowLayoutPanel1->Size = System::Drawing::Size(612, 437);
			this->flowLayoutPanel1->TabIndex = 0;
			// 
			// cProbableIO_panel
			// 
			this->cProbableIO_panel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->cProbableIO_panel->Controls->Add(this->cProbableIOSingnal_checkBox);
			this->cProbableIO_panel->Controls->Add(this->label12);
			this->cProbableIO_panel->Controls->Add(this->ProbableIO_listBox);
			this->cProbableIO_panel->Controls->Add(this->ProbableIO_numericUpDown);
			this->cProbableIO_panel->Controls->Add(this->label11);
			this->cProbableIO_panel->Controls->Add(this->ProbableIODel_button);
			this->cProbableIO_panel->Controls->Add(this->ProbableIOAdd_button);
			this->cProbableIO_panel->Location = System::Drawing::Point(3, 3);
			this->cProbableIO_panel->Name = L"cProbableIO_panel";
			this->cProbableIO_panel->Size = System::Drawing::Size(299, 181);
			this->cProbableIO_panel->TabIndex = 1;
			this->cProbableIO_panel->Visible = false;
			// 
			// cProbableIOSingnal_checkBox
			// 
			this->cProbableIOSingnal_checkBox->AutoSize = true;
			this->cProbableIOSingnal_checkBox->Location = System::Drawing::Point(61, 127);
			this->cProbableIOSingnal_checkBox->Name = L"cProbableIOSingnal_checkBox";
			this->cProbableIOSingnal_checkBox->Size = System::Drawing::Size(133, 16);
			this->cProbableIOSingnal_checkBox->TabIndex = 7;
			this->cProbableIOSingnal_checkBox->Text = L"SendSingnal/送出訊號";
			this->cProbableIOSingnal_checkBox->UseVisualStyleBackColor = true;
			this->cProbableIOSingnal_checkBox->CheckedChanged += gcnew System::EventHandler(this, &AllDeviceStatusUI::cProbableIOSingnal_checkBox_CheckedChanged);
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(4, 158);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(113, 12);
			this->label12->TabIndex = 22;
			this->label12->Text = L"不一定被觸發IO機率";
			// 
			// ProbableIO_listBox
			// 
			this->ProbableIO_listBox->FormattingEnabled = true;
			this->ProbableIO_listBox->ItemHeight = 12;
			this->ProbableIO_listBox->Location = System::Drawing::Point(61, 15);
			this->ProbableIO_listBox->Name = L"ProbableIO_listBox";
			this->ProbableIO_listBox->Size = System::Drawing::Size(211, 100);
			this->ProbableIO_listBox->TabIndex = 17;
			// 
			// ProbableIO_numericUpDown
			// 
			this->ProbableIO_numericUpDown->Location = System::Drawing::Point(134, 156);
			this->ProbableIO_numericUpDown->Name = L"ProbableIO_numericUpDown";
			this->ProbableIO_numericUpDown->Size = System::Drawing::Size(120, 22);
			this->ProbableIO_numericUpDown->TabIndex = 21;
			this->ProbableIO_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &AllDeviceStatusUI::ProbableIO_numericUpDown_ValueChanged);
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(0, 0);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(92, 12);
			this->label11->TabIndex = 18;
			this->label11->Text = L"不一定被觸發IO/";
			// 
			// ProbableIODel_button
			// 
			this->ProbableIODel_button->Location = System::Drawing::Point(5, 83);
			this->ProbableIODel_button->Name = L"ProbableIODel_button";
			this->ProbableIODel_button->Size = System::Drawing::Size(52, 24);
			this->ProbableIODel_button->TabIndex = 20;
			this->ProbableIODel_button->Text = L"Del";
			this->ProbableIODel_button->UseVisualStyleBackColor = true;
			this->ProbableIODel_button->Click += gcnew System::EventHandler(this, &AllDeviceStatusUI::ProbableIOAdd_button_Click);
			// 
			// ProbableIOAdd_button
			// 
			this->ProbableIOAdd_button->Location = System::Drawing::Point(5, 29);
			this->ProbableIOAdd_button->Name = L"ProbableIOAdd_button";
			this->ProbableIOAdd_button->Size = System::Drawing::Size(52, 24);
			this->ProbableIOAdd_button->TabIndex = 19;
			this->ProbableIOAdd_button->Text = L"Add";
			this->ProbableIOAdd_button->UseVisualStyleBackColor = true;
			this->ProbableIOAdd_button->Click += gcnew System::EventHandler(this, &AllDeviceStatusUI::ProbableIOAdd_button_Click);
			// 
			// cActiveIOTimesWithSingnalCircle_panel
			// 
			this->cActiveIOTimesWithSingnalCircle_panel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->cActiveIOTimesWithSingnalCircle_panel->Controls->Add(this->label1);
			this->cActiveIOTimesWithSingnalCircle_panel->Controls->Add(this->StopAfterTimes_numericUpDown);
			this->cActiveIOTimesWithSingnalCircle_panel->Location = System::Drawing::Point(3, 190);
			this->cActiveIOTimesWithSingnalCircle_panel->Name = L"cActiveIOTimesWithSingnalCircle_panel";
			this->cActiveIOTimesWithSingnalCircle_panel->Size = System::Drawing::Size(299, 77);
			this->cActiveIOTimesWithSingnalCircle_panel->TabIndex = 2;
			this->cActiveIOTimesWithSingnalCircle_panel->Visible = false;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(6, 13);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(206, 12);
			this->label1->TabIndex = 23;
			this->label1->Text = L"連續觸發幾次之後才停止觸發/0為無限";
			// 
			// StopAfterTimes_numericUpDown
			// 
			this->StopAfterTimes_numericUpDown->Location = System::Drawing::Point(8, 38);
			this->StopAfterTimes_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000, 0, 0, 0});
			this->StopAfterTimes_numericUpDown->Name = L"StopAfterTimes_numericUpDown";
			this->StopAfterTimes_numericUpDown->Size = System::Drawing::Size(120, 22);
			this->StopAfterTimes_numericUpDown->TabIndex = 22;
			this->StopAfterTimes_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &AllDeviceStatusUI::StopAfterTimes_numericUpDown_ValueChanged);
			// 
			// cListenMultiSingnalToActiveIO_panel
			// 
			this->cListenMultiSingnalToActiveIO_panel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->cListenMultiSingnalToActiveIO_panel->Controls->Add(this->cListenMultiSingnalToActiveIOSingnal_checkBox);
			this->cListenMultiSingnalToActiveIO_panel->Controls->Add(this->MultiTriigerIO_listBox);
			this->cListenMultiSingnalToActiveIO_panel->Controls->Add(this->label2);
			this->cListenMultiSingnalToActiveIO_panel->Controls->Add(this->MultiTriigerIODel_button);
			this->cListenMultiSingnalToActiveIO_panel->Controls->Add(this->MultiTriigerIOAdd_button);
			this->cListenMultiSingnalToActiveIO_panel->Location = System::Drawing::Point(3, 273);
			this->cListenMultiSingnalToActiveIO_panel->Name = L"cListenMultiSingnalToActiveIO_panel";
			this->cListenMultiSingnalToActiveIO_panel->Size = System::Drawing::Size(299, 160);
			this->cListenMultiSingnalToActiveIO_panel->TabIndex = 2;
			this->cListenMultiSingnalToActiveIO_panel->Visible = false;
			// 
			// cListenMultiSingnalToActiveIOSingnal_checkBox
			// 
			this->cListenMultiSingnalToActiveIOSingnal_checkBox->AutoSize = true;
			this->cListenMultiSingnalToActiveIOSingnal_checkBox->Location = System::Drawing::Point(74, 134);
			this->cListenMultiSingnalToActiveIOSingnal_checkBox->Name = L"cListenMultiSingnalToActiveIOSingnal_checkBox";
			this->cListenMultiSingnalToActiveIOSingnal_checkBox->Size = System::Drawing::Size(147, 16);
			this->cListenMultiSingnalToActiveIOSingnal_checkBox->TabIndex = 23;
			this->cListenMultiSingnalToActiveIOSingnal_checkBox->Text = L"waittingSingnal/等待訊號";
			this->cListenMultiSingnalToActiveIOSingnal_checkBox->UseVisualStyleBackColor = true;
			this->cListenMultiSingnalToActiveIOSingnal_checkBox->VisibleChanged += gcnew System::EventHandler(this, &AllDeviceStatusUI::cProbableIOSingnal_checkBox_CheckedChanged);
			// 
			// MultiTriigerIO_listBox
			// 
			this->MultiTriigerIO_listBox->FormattingEnabled = true;
			this->MultiTriigerIO_listBox->ItemHeight = 12;
			this->MultiTriigerIO_listBox->Location = System::Drawing::Point(72, 28);
			this->MultiTriigerIO_listBox->Name = L"MultiTriigerIO_listBox";
			this->MultiTriigerIO_listBox->Size = System::Drawing::Size(211, 100);
			this->MultiTriigerIO_listBox->TabIndex = 17;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(3, 9);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(85, 12);
			this->label2->TabIndex = 18;
			this->label2->Text = L"被n重觸發才On";
			// 
			// MultiTriigerIODel_button
			// 
			this->MultiTriigerIODel_button->Location = System::Drawing::Point(2, 96);
			this->MultiTriigerIODel_button->Name = L"MultiTriigerIODel_button";
			this->MultiTriigerIODel_button->Size = System::Drawing::Size(64, 23);
			this->MultiTriigerIODel_button->TabIndex = 20;
			this->MultiTriigerIODel_button->Text = L"Del";
			this->MultiTriigerIODel_button->UseVisualStyleBackColor = true;
			this->MultiTriigerIODel_button->Click += gcnew System::EventHandler(this, &AllDeviceStatusUI::MultiTriigerIOAdd_button_Click);
			// 
			// MultiTriigerIOAdd_button
			// 
			this->MultiTriigerIOAdd_button->Location = System::Drawing::Point(2, 51);
			this->MultiTriigerIOAdd_button->Name = L"MultiTriigerIOAdd_button";
			this->MultiTriigerIOAdd_button->Size = System::Drawing::Size(64, 23);
			this->MultiTriigerIOAdd_button->TabIndex = 19;
			this->MultiTriigerIOAdd_button->Text = L"Add";
			this->MultiTriigerIOAdd_button->UseVisualStyleBackColor = true;
			this->MultiTriigerIOAdd_button->Click += gcnew System::EventHandler(this, &AllDeviceStatusUI::MultiTriigerIOAdd_button_Click);
			// 
			// cSendHighSingnal_panel
			// 
			this->cSendHighSingnal_panel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->cSendHighSingnal_panel->Controls->Add(this->SendHighSingnal_listBox);
			this->cSendHighSingnal_panel->Controls->Add(this->label4);
			this->cSendHighSingnal_panel->Controls->Add(this->SendHighSingnalDel_button);
			this->cSendHighSingnal_panel->Controls->Add(this->SendHighSingnalAdd_button);
			this->cSendHighSingnal_panel->Location = System::Drawing::Point(308, 3);
			this->cSendHighSingnal_panel->Name = L"cSendHighSingnal_panel";
			this->cSendHighSingnal_panel->Size = System::Drawing::Size(299, 124);
			this->cSendHighSingnal_panel->TabIndex = 3;
			this->cSendHighSingnal_panel->Visible = false;
			// 
			// SendHighSingnal_listBox
			// 
			this->SendHighSingnal_listBox->FormattingEnabled = true;
			this->SendHighSingnal_listBox->ItemHeight = 12;
			this->SendHighSingnal_listBox->Location = System::Drawing::Point(61, 15);
			this->SendHighSingnal_listBox->Name = L"SendHighSingnal_listBox";
			this->SendHighSingnal_listBox->Size = System::Drawing::Size(211, 100);
			this->SendHighSingnal_listBox->TabIndex = 17;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(0, 0);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(76, 12);
			this->label4->TabIndex = 18;
			this->label4->Text = L"送出High訊號";
			// 
			// SendHighSingnalDel_button
			// 
			this->SendHighSingnalDel_button->Location = System::Drawing::Point(5, 83);
			this->SendHighSingnalDel_button->Name = L"SendHighSingnalDel_button";
			this->SendHighSingnalDel_button->Size = System::Drawing::Size(52, 24);
			this->SendHighSingnalDel_button->TabIndex = 20;
			this->SendHighSingnalDel_button->Text = L"Del";
			this->SendHighSingnalDel_button->UseVisualStyleBackColor = true;
			this->SendHighSingnalDel_button->Click += gcnew System::EventHandler(this, &AllDeviceStatusUI::SendHighSingnalAdd_button_Click);
			// 
			// SendHighSingnalAdd_button
			// 
			this->SendHighSingnalAdd_button->Location = System::Drawing::Point(5, 29);
			this->SendHighSingnalAdd_button->Name = L"SendHighSingnalAdd_button";
			this->SendHighSingnalAdd_button->Size = System::Drawing::Size(52, 24);
			this->SendHighSingnalAdd_button->TabIndex = 19;
			this->SendHighSingnalAdd_button->Text = L"Add";
			this->SendHighSingnalAdd_button->UseVisualStyleBackColor = true;
			this->SendHighSingnalAdd_button->Click += gcnew System::EventHandler(this, &AllDeviceStatusUI::SendHighSingnalAdd_button_Click);
			// 
			// cSendLowSingnal_panel
			// 
			this->cSendLowSingnal_panel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->cSendLowSingnal_panel->Controls->Add(this->SendLowSingnal_listBox);
			this->cSendLowSingnal_panel->Controls->Add(this->label3);
			this->cSendLowSingnal_panel->Controls->Add(this->SendLowSingnalDel_button);
			this->cSendLowSingnal_panel->Controls->Add(this->SendLowSingnalAdd_button);
			this->cSendLowSingnal_panel->Location = System::Drawing::Point(308, 133);
			this->cSendLowSingnal_panel->Name = L"cSendLowSingnal_panel";
			this->cSendLowSingnal_panel->Size = System::Drawing::Size(299, 124);
			this->cSendLowSingnal_panel->TabIndex = 4;
			this->cSendLowSingnal_panel->Visible = false;
			// 
			// SendLowSingnal_listBox
			// 
			this->SendLowSingnal_listBox->FormattingEnabled = true;
			this->SendLowSingnal_listBox->ItemHeight = 12;
			this->SendLowSingnal_listBox->Location = System::Drawing::Point(61, 15);
			this->SendLowSingnal_listBox->Name = L"SendLowSingnal_listBox";
			this->SendLowSingnal_listBox->Size = System::Drawing::Size(211, 100);
			this->SendLowSingnal_listBox->TabIndex = 17;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(0, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(74, 12);
			this->label3->TabIndex = 18;
			this->label3->Text = L"送出Low訊號";
			// 
			// SendLowSingnalDel_button
			// 
			this->SendLowSingnalDel_button->Location = System::Drawing::Point(5, 83);
			this->SendLowSingnalDel_button->Name = L"SendLowSingnalDel_button";
			this->SendLowSingnalDel_button->Size = System::Drawing::Size(52, 24);
			this->SendLowSingnalDel_button->TabIndex = 20;
			this->SendLowSingnalDel_button->Text = L"Del";
			this->SendLowSingnalDel_button->UseVisualStyleBackColor = true;
			this->SendLowSingnalDel_button->Click += gcnew System::EventHandler(this, &AllDeviceStatusUI::SendLowSingnalAdd_button_Click);
			// 
			// SendLowSingnalAdd_button
			// 
			this->SendLowSingnalAdd_button->Location = System::Drawing::Point(5, 29);
			this->SendLowSingnalAdd_button->Name = L"SendLowSingnalAdd_button";
			this->SendLowSingnalAdd_button->Size = System::Drawing::Size(52, 24);
			this->SendLowSingnalAdd_button->TabIndex = 19;
			this->SendLowSingnalAdd_button->Text = L"Add";
			this->SendLowSingnalAdd_button->UseVisualStyleBackColor = true;
			this->SendLowSingnalAdd_button->Click += gcnew System::EventHandler(this, &AllDeviceStatusUI::SendLowSingnalAdd_button_Click);
			// 
			// AllDeviceStatusUI
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->flowLayoutPanel1);
			this->Name = L"AllDeviceStatusUI";
			this->Size = System::Drawing::Size(612, 437);
			this->flowLayoutPanel1->ResumeLayout(false);
			this->cProbableIO_panel->ResumeLayout(false);
			this->cProbableIO_panel->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ProbableIO_numericUpDown))->EndInit();
			this->cActiveIOTimesWithSingnalCircle_panel->ResumeLayout(false);
			this->cActiveIOTimesWithSingnalCircle_panel->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->StopAfterTimes_numericUpDown))->EndInit();
			this->cListenMultiSingnalToActiveIO_panel->ResumeLayout(false);
			this->cListenMultiSingnalToActiveIO_panel->PerformLayout();
			this->cSendHighSingnal_panel->ResumeLayout(false);
			this->cSendHighSingnal_panel->PerformLayout();
			this->cSendLowSingnal_panel->ResumeLayout(false);
			this->cSendLowSingnal_panel->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void ProbableIOAdd_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( sender == ProbableIODel_button )
				 {
					 int	l_iSelectedIndex = ProbableIO_listBox->SelectedIndex;
					 if( l_iSelectedIndex != -1 )
					 {
						 WCHAR*l_str = DNCT::GcStringToWchar(ProbableIO_listBox->SelectedItem->ToString());
						 g_pProbableIO->m_ProbableIOList.RemoveObjectWithoutDelete(l_str);
						 ProbableIO_listBox->Items->RemoveAt(l_iSelectedIndex);
					 }
				 }
				 else
				 {
					 int	l_iSelectedIndex = m_TargetListBox->SelectedIndex;
					 if( l_iSelectedIndex != -1 )
					 {
						 if(!DNCT::CheckListContainStringAndAdd(ProbableIO_listBox,m_TargetListBox->SelectedItem->ToString()))
						 {
							 WCHAR*l_str = DNCT::GcStringToWchar(m_TargetListBox->SelectedItem->ToString());
							 cIOSMIOData*l_pIOSMIOData = g_pIOSMDriverInterface->GetObject(l_str);
							 bool	l_b = g_pProbableIO->m_ProbableIOList.AddObject(l_pIOSMIOData);
							 assert(l_b);
						 }
					 }
				 }
			 }

private: System::Void ProbableIO_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 g_pProbableIO->m_iProbability = (int)ProbableIO_numericUpDown->Value;
		 }
private: System::Void StopAfterTimes_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 g_pActiveIOTimesWithSingnalCircle->m_iTimes = (int)StopAfterTimes_numericUpDown->Value;
		 }
private: System::Void MultiTriigerIOAdd_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( sender == MultiTriigerIODel_button )
			 {
				 int	l_iSelectedIndex = ProbableIO_listBox->SelectedIndex;
				 if( l_iSelectedIndex != -1 )
				 {
					 WCHAR*l_str = DNCT::GcStringToWchar(MultiTriigerIO_listBox->SelectedItem->ToString());
					 g_pListenMultiSingnalToActiveIO->m_ActiveIOList.RemoveObjectWithoutDelete(l_str);
					 MultiTriigerIO_listBox->Items->RemoveAt(l_iSelectedIndex);
				 }
			 }
			 else
			 {
				 int	l_iSelectedIndex = m_TargetListBox->SelectedIndex;
				 if( l_iSelectedIndex != -1 )
				 {
					 if(!DNCT::CheckListContainStringAndAdd(MultiTriigerIO_listBox,m_TargetListBox->SelectedItem->ToString()))
					 {
						 WCHAR*l_str = DNCT::GcStringToWchar(m_TargetListBox->SelectedItem->ToString());
						 cIOSMIOData*l_pIOSMIOData = g_pIOSMDriverInterface->GetObject(l_str);
						 bool	l_b = g_pListenMultiSingnalToActiveIO->m_ActiveIOList.AddObject(l_pIOSMIOData);
						 assert(l_b);
					 }
				 }
			 }
		 }

private: System::Void cProbableIOSingnal_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( sender == cProbableIOSingnal_checkBox )
			 {
				 if( g_pProbableIO )
					g_pProbableIO->m_bSendSingnal = cProbableIOSingnal_checkBox->Checked;
			 }
			 else
			 if( sender == cListenMultiSingnalToActiveIOSingnal_checkBox )
			 {
				 if( g_pListenMultiSingnalToActiveIO )
					g_pListenMultiSingnalToActiveIO->m_bSendSingnal = cListenMultiSingnalToActiveIOSingnal_checkBox->Checked;
			 }
		 }
private: System::Void SendHighSingnalAdd_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 AssignListData(SendHighSingnal_listBox,&g_pSendHighSingnal->m_ActiveIOList,sender == SendHighSingnalDel_button?false:true);
		 }
private: System::Void SendLowSingnalAdd_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 AssignListData(SendLowSingnal_listBox,&g_pSendLowSingnal->m_ActiveIOList,sender == SendLowSingnalDel_button?false:true);
		 }
};
}