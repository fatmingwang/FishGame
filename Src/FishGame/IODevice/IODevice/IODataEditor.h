#pragma once

#include "AllDeviceStatusUI.h"

//extern cIOSMDriverInterface*				g_pIOSMDriverInterface;
extern cIOSMBaseDevice*						g_pIOSMBaseDevice;
extern cIOSMIOManager*						g_pIOSMIOManager;
extern cProbableIO*							g_pProbableIO;
extern cActiveIOTimesWithSingnalCircle*		g_pActiveIOTimesWithSingnalCircle;
extern cListenMultiSingnalToActiveIO*		g_pListenMultiSingnalToActiveIO;
extern cSendHighSingnal*					g_pSendHighSingnal;
extern cSendLowSingnal*						g_pSendLowSingnal;
namespace IOSM
{
	extern unsigned char g_ucOutCount;
	extern unsigned char g_ucInputCount;
	extern unsigned char g_ucIrqPICount;
}
namespace IODevice 
{

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Form1 的摘要
	///
	/// 警告: 如果您變更這個類別的名稱，就必須變更與這個類別所依據之所有 .resx 檔案關聯的
	///          Managed 資源編譯器工具的 'Resource File Name' 屬性。
	///          否則，這些設計工具
	///          將無法與這個表單關聯的當地語系化資源
	///          正確互動。
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
#if defined WIN32 || defined LINUX
	        char *locale;
            locale = setlocale(LC_ALL, "");
#endif
			m_iBitAddress = 0;
			//if it's on the machine mark this line.
			IOSM::g_bSimulation = true;
			InitializeComponent();
			m_pAllDeviceStatusUI = gcnew AllDeviceStatusUI(TargetIO_listBox);
			AllUsedBehavior_groupBox->Controls->Add(m_pAllDeviceStatusUI);
			//
			//TODO: 在此加入建構函式程式碼
			//
			g_pIOSMIOManager = new cIOSMIOManager();
			g_pIOSMIOManager->m_pIOSMDriverInterface = new cIOSMDriverInterface;
			g_pIOSMDriverInterface = g_pIOSMIOManager->m_pIOSMDriverInterface;

			g_pIOSMDriverInterface->Init(GUID_DEVINTERFACE_JMGB_M01,0,0);
			OutputCount_label->Text = g_ucOutCount.ToString();
			InputCount_label->Text = g_ucInputCount.ToString();
			IrqCount_label->Text = g_ucIrqPICount.ToString();
			std::vector<std::wstring>*l_pAllDeviceNameVector = g_pIOSMDriverInterface->GetAllDeviceNameVector();
			for(size_t i = 0;i<l_pAllDeviceNameVector->size();++i)
			{
				this->Device_listBox->Items->Add(DNCT::WcharToGcstring((WCHAR*)(*l_pAllDeviceNameVector)[i].c_str()));
			}
			g_pIOSMDriverInterface->WriteGameKey();
			g_pIOSMDriverInterface->WriteInputIOSetup();
			m_iHexValue = -1;
			m_pTimeCounter = new sTimeCounter;
			m_pTimeAndFPS = new sTimeAndFPS;
			m_pTimeAndFPSForUetex = new sTimeAndFPS;
			m_pTimeCounterForUetex = new sTimeCounter;

			 for( int i=0;i<g_ucOutCount;++i )
			 {
				 bool l_bValue = false;
				 g_pIOSMDriverInterface->ReadOutputData(i,&l_bValue);
				 //PinStatus_listBox->Items->Add("Index:"+i.ToString()+":  "+l_bValue?"1":"0");
				 OutputStatus_listBox->Items->Add("fuck");
			 }
			 for( int i=0;i<g_ucInputCount;++i )
			 {
				 bool l_bValue = false;
				 g_pIOSMDriverInterface->ReadInputData(i,&l_bValue);
				 //PinStatus_listBox->Items->Add("Index:"+i.ToString()+":  "+l_bValue?"1":"0");
				 Input_listBox->Items->Add("fuck");
			 }
			 IOType_comboBox->SelectedIndex = 0;
			 PinStatus_timer->Enabled = true;
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~Form1()
		{
			SAFE_DELETE(g_pIOSMIOManager);
			SAFE_DELETE(m_pTimeCounter);
			SAFE_DELETE(m_pTimeAndFPS);
			SAFE_DELETE(m_pTimeAndFPSForUetex);
			SAFE_DELETE(m_pTimeCounterForUetex);
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveToolStripMenuItem;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::TabControl^  IOSimulation_tabControl;
	private: System::Windows::Forms::TabPage^  IOData_tabPage;
	private: System::Windows::Forms::Button^  CurrentStatus_button;
	private: System::Windows::Forms::ListBox^  Device_listBox;
	private: System::Windows::Forms::CheckBox^  StartStatus_checkBox;
	private: System::Windows::Forms::CheckBox^  SetStartSingnal_checkBox;
	private: System::Windows::Forms::Button^  WriteIO_button;
	private: System::Windows::Forms::TextBox^  BounceStatus_textBox;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  ReEdit_button;
	private: System::Windows::Forms::Button^  Delete_button;
	private: System::Windows::Forms::Button^  Add_button;
	private: System::Windows::Forms::ListBox^  AllIO_listBox;
	private: System::Windows::Forms::NumericUpDown^  BitAddress_numericUpDown;
	private: System::Windows::Forms::Button^  ReadIO_button;
	private: System::Windows::Forms::TextBox^  Name_textBox;
	private: System::Windows::Forms::TextBox^  Adress_textBox;
	private: System::Windows::Forms::TabPage^  DeviceData_tabPage;
	private: System::Windows::Forms::ListBox^  AllIODevice_listBox;
	private: System::Windows::Forms::ToolStripMenuItem^  saveDeviceToolStripMenuItem;
	private: System::Windows::Forms::CheckBox^  checkBox3;
	private: System::Windows::Forms::GroupBox^  AllUsedBehavior_groupBox;
	private: System::Windows::Forms::ListBox^  UsedBehavior_listBox;
	private: System::Windows::Forms::ListBox^  AllBehavior_listBox;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Button^  DelBehavior_button;
	private: System::Windows::Forms::Button^  AddBehavior_button;
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::ListBox^  TargetIO_listBox;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::ToolStripMenuItem^  openDeviceDataToolStripMenuItem;
	private: System::Windows::Forms::CheckBox^  WorkingSingnal_checkBox;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::Button^  GoOff_button;
	private: System::Windows::Forms::Button^  GoUpdate_button;
	private: System::Windows::Forms::Timer^  timer2;
	private: System::Windows::Forms::NumericUpDown^  SizeForIO_numericUpDown;
	private: System::Windows::Forms::Label^  label14;
	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::TextBox^  ValueToWrite_textBox;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::TextBox^  IOStatus_textBox;
	private: System::Windows::Forms::Button^  Write_button;
	private: System::Windows::Forms::Button^  Read_button;
	private: System::Windows::Forms::NumericUpDown^  Low_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  High_numericUpDown;
	private: System::Windows::Forms::Label^  label16;
	private: System::Windows::Forms::Label^  label15;
	private: System::Windows::Forms::NumericUpDown^  InactiveDelay_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  ActiveDelay_numericUpDown;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::TabPage^  Simulation_tabPage;

	private: System::Windows::Forms::ListBox^  SimulationIOStatus_listBox;
	private: System::Windows::Forms::ListBox^  SimulationIOList_listBox;
	private: System::Windows::Forms::CheckBox^  Simulation_checkBox;
	private: System::Windows::Forms::Timer^  timer3;
	private: System::Windows::Forms::TabPage^  Mutex_tabPage;
	private: System::Windows::Forms::Timer^  timer4;
private: System::Windows::Forms::TextBox^  MutexAddress_textBox;

	private: System::Windows::Forms::ListBox^  Mutex_listBox;
	private: System::Windows::Forms::Button^  MutexTest_button;
	private: System::Windows::Forms::Label^  label18;
	private: System::Windows::Forms::Label^  label17;
	private: System::Windows::Forms::Label^  StartAddress_label;
private: System::Windows::Forms::TextBox^  MutexWriteSize_textBox;
private: System::Windows::Forms::TextBox^  MutexValueToWrite__textBox;


private: System::Windows::Forms::Label^  label19;
private: System::Windows::Forms::TextBox^  MutexTime_textBox;
private: System::Windows::Forms::CheckBox^  StartEnable_checkBox;
private: System::Windows::Forms::Label^  OutputCount_label;


private: System::Windows::Forms::Label^  label20;
private: System::Windows::Forms::ListBox^  OutputStatus_listBox;

private: System::Windows::Forms::Timer^  PinStatus_timer;
private: System::Windows::Forms::Button^  AllOutputPinTo0_button;

private: System::Windows::Forms::Button^  AllOutputPinTo1_button;

private: System::Windows::Forms::Button^  AllInputTo0button;

private: System::Windows::Forms::Button^  AllInputTo1button;
private: System::Windows::Forms::Label^  InputCount_label;
private: System::Windows::Forms::Label^  label21;
private: System::Windows::Forms::ListBox^  Input_listBox;
private: System::Windows::Forms::Label^  IrqCount_label;
private: System::Windows::Forms::Label^  label22;
private: System::Windows::Forms::TextBox^  KeyData_textBox;
private: System::Windows::Forms::ComboBox^  IOType_comboBox;

private: System::Windows::Forms::Label^  label23;



		 //my
	AllDeviceStatusUI^m_pAllDeviceStatusUI;
	System::Void	ReEditIOData();
	System::Void	ClearIOData();
	System::Void	ClearUIData();
	//match hex
	bool	IsHexString(String^e_str);
	int	m_iHexValue;
	int	m_iBitAddress;
	sTimeCounter*m_pTimeCounter;
	sTimeAndFPS*m_pTimeAndFPS;
	//for mutex
	sTimeAndFPS*m_pTimeAndFPSForUetex;
	sTimeCounter*m_pTimeCounterForUetex;
	int			m_iWriteValueForMutex;
	int			m_iAddressForMutex;
	int			m_iWriteSizeForMutex;
			 //System::Void	AssignIODataToForm(cIOSMIOData*e_pIOSMIOData);
	private:
		/// <summary>
		/// 設計工具所需的變數。
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改這個方法的內容。
		///
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->toolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openDeviceDataToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveDeviceToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->IOSimulation_tabControl = (gcnew System::Windows::Forms::TabControl());
			this->IOData_tabPage = (gcnew System::Windows::Forms::TabPage());
			this->label23 = (gcnew System::Windows::Forms::Label());
			this->IOType_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->label22 = (gcnew System::Windows::Forms::Label());
			this->KeyData_textBox = (gcnew System::Windows::Forms::TextBox());
			this->label21 = (gcnew System::Windows::Forms::Label());
			this->Input_listBox = (gcnew System::Windows::Forms::ListBox());
			this->IrqCount_label = (gcnew System::Windows::Forms::Label());
			this->InputCount_label = (gcnew System::Windows::Forms::Label());
			this->AllInputTo0button = (gcnew System::Windows::Forms::Button());
			this->AllInputTo1button = (gcnew System::Windows::Forms::Button());
			this->AllOutputPinTo0_button = (gcnew System::Windows::Forms::Button());
			this->AllOutputPinTo1_button = (gcnew System::Windows::Forms::Button());
			this->label20 = (gcnew System::Windows::Forms::Label());
			this->OutputStatus_listBox = (gcnew System::Windows::Forms::ListBox());
			this->OutputCount_label = (gcnew System::Windows::Forms::Label());
			this->StartEnable_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->InactiveDelay_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->ActiveDelay_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->Low_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->High_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->Read_button = (gcnew System::Windows::Forms::Button());
			this->Write_button = (gcnew System::Windows::Forms::Button());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->IOStatus_textBox = (gcnew System::Windows::Forms::TextBox());
			this->SizeForIO_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->ValueToWrite_textBox = (gcnew System::Windows::Forms::TextBox());
			this->WorkingSingnal_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox3 = (gcnew System::Windows::Forms::CheckBox());
			this->CurrentStatus_button = (gcnew System::Windows::Forms::Button());
			this->Device_listBox = (gcnew System::Windows::Forms::ListBox());
			this->StartStatus_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->SetStartSingnal_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->WriteIO_button = (gcnew System::Windows::Forms::Button());
			this->BounceStatus_textBox = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->ReEdit_button = (gcnew System::Windows::Forms::Button());
			this->Delete_button = (gcnew System::Windows::Forms::Button());
			this->Add_button = (gcnew System::Windows::Forms::Button());
			this->AllIO_listBox = (gcnew System::Windows::Forms::ListBox());
			this->BitAddress_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->ReadIO_button = (gcnew System::Windows::Forms::Button());
			this->Name_textBox = (gcnew System::Windows::Forms::TextBox());
			this->Adress_textBox = (gcnew System::Windows::Forms::TextBox());
			this->DeviceData_tabPage = (gcnew System::Windows::Forms::TabPage());
			this->GoOff_button = (gcnew System::Windows::Forms::Button());
			this->GoUpdate_button = (gcnew System::Windows::Forms::Button());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->TargetIO_listBox = (gcnew System::Windows::Forms::ListBox());
			this->DelBehavior_button = (gcnew System::Windows::Forms::Button());
			this->AddBehavior_button = (gcnew System::Windows::Forms::Button());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->AllUsedBehavior_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->UsedBehavior_listBox = (gcnew System::Windows::Forms::ListBox());
			this->AllBehavior_listBox = (gcnew System::Windows::Forms::ListBox());
			this->AllIODevice_listBox = (gcnew System::Windows::Forms::ListBox());
			this->Simulation_tabPage = (gcnew System::Windows::Forms::TabPage());
			this->SimulationIOStatus_listBox = (gcnew System::Windows::Forms::ListBox());
			this->SimulationIOList_listBox = (gcnew System::Windows::Forms::ListBox());
			this->Simulation_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->Mutex_tabPage = (gcnew System::Windows::Forms::TabPage());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->MutexTime_textBox = (gcnew System::Windows::Forms::TextBox());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->StartAddress_label = (gcnew System::Windows::Forms::Label());
			this->MutexWriteSize_textBox = (gcnew System::Windows::Forms::TextBox());
			this->MutexValueToWrite__textBox = (gcnew System::Windows::Forms::TextBox());
			this->MutexAddress_textBox = (gcnew System::Windows::Forms::TextBox());
			this->Mutex_listBox = (gcnew System::Windows::Forms::ListBox());
			this->MutexTest_button = (gcnew System::Windows::Forms::Button());
			this->timer2 = (gcnew System::Windows::Forms::Timer(this->components));
			this->timer3 = (gcnew System::Windows::Forms::Timer(this->components));
			this->timer4 = (gcnew System::Windows::Forms::Timer(this->components));
			this->PinStatus_timer = (gcnew System::Windows::Forms::Timer(this->components));
			this->menuStrip1->SuspendLayout();
			this->IOSimulation_tabControl->SuspendLayout();
			this->IOData_tabPage->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->InactiveDelay_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ActiveDelay_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Low_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->High_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->SizeForIO_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->BitAddress_numericUpDown))->BeginInit();
			this->DeviceData_tabPage->SuspendLayout();
			this->Simulation_tabPage->SuspendLayout();
			this->Mutex_tabPage->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->toolStripMenuItem1});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(975, 24);
			this->menuStrip1->TabIndex = 19;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// toolStripMenuItem1
			// 
			this->toolStripMenuItem1->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->openToolStripMenuItem, 
				this->openDeviceDataToolStripMenuItem, this->saveToolStripMenuItem, this->saveDeviceToolStripMenuItem});
			this->toolStripMenuItem1->Name = L"toolStripMenuItem1";
			this->toolStripMenuItem1->Size = System::Drawing::Size(34, 20);
			this->toolStripMenuItem1->Text = L"File";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(148, 22);
			this->openToolStripMenuItem->Text = L"OpenIOData";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::openToolStripMenuItem_Click);
			// 
			// openDeviceDataToolStripMenuItem
			// 
			this->openDeviceDataToolStripMenuItem->Name = L"openDeviceDataToolStripMenuItem";
			this->openDeviceDataToolStripMenuItem->Size = System::Drawing::Size(148, 22);
			this->openDeviceDataToolStripMenuItem->Text = L"OpenDeviceData";
			this->openDeviceDataToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::saveDeviceToolStripMenuItem_Click);
			// 
			// saveToolStripMenuItem
			// 
			this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
			this->saveToolStripMenuItem->Size = System::Drawing::Size(148, 22);
			this->saveToolStripMenuItem->Text = L"SaveIOData";
			this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::openToolStripMenuItem_Click);
			// 
			// saveDeviceToolStripMenuItem
			// 
			this->saveDeviceToolStripMenuItem->Name = L"saveDeviceToolStripMenuItem";
			this->saveDeviceToolStripMenuItem->Size = System::Drawing::Size(148, 22);
			this->saveDeviceToolStripMenuItem->Text = L"SaveDeviceData";
			this->saveDeviceToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::saveDeviceToolStripMenuItem_Click);
			// 
			// timer1
			// 
			this->timer1->Interval = 16;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// IOSimulation_tabControl
			// 
			this->IOSimulation_tabControl->Controls->Add(this->IOData_tabPage);
			this->IOSimulation_tabControl->Controls->Add(this->DeviceData_tabPage);
			this->IOSimulation_tabControl->Controls->Add(this->Simulation_tabPage);
			this->IOSimulation_tabControl->Controls->Add(this->Mutex_tabPage);
			this->IOSimulation_tabControl->Dock = System::Windows::Forms::DockStyle::Fill;
			this->IOSimulation_tabControl->Location = System::Drawing::Point(0, 24);
			this->IOSimulation_tabControl->Name = L"IOSimulation_tabControl";
			this->IOSimulation_tabControl->SelectedIndex = 0;
			this->IOSimulation_tabControl->Size = System::Drawing::Size(975, 852);
			this->IOSimulation_tabControl->TabIndex = 22;
			this->IOSimulation_tabControl->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::IOSimulation_tabControl_SelectedIndexChanged);
			// 
			// IOData_tabPage
			// 
			this->IOData_tabPage->Controls->Add(this->label23);
			this->IOData_tabPage->Controls->Add(this->IOType_comboBox);
			this->IOData_tabPage->Controls->Add(this->label22);
			this->IOData_tabPage->Controls->Add(this->KeyData_textBox);
			this->IOData_tabPage->Controls->Add(this->label21);
			this->IOData_tabPage->Controls->Add(this->Input_listBox);
			this->IOData_tabPage->Controls->Add(this->IrqCount_label);
			this->IOData_tabPage->Controls->Add(this->InputCount_label);
			this->IOData_tabPage->Controls->Add(this->AllInputTo0button);
			this->IOData_tabPage->Controls->Add(this->AllInputTo1button);
			this->IOData_tabPage->Controls->Add(this->AllOutputPinTo0_button);
			this->IOData_tabPage->Controls->Add(this->AllOutputPinTo1_button);
			this->IOData_tabPage->Controls->Add(this->label20);
			this->IOData_tabPage->Controls->Add(this->OutputStatus_listBox);
			this->IOData_tabPage->Controls->Add(this->OutputCount_label);
			this->IOData_tabPage->Controls->Add(this->StartEnable_checkBox);
			this->IOData_tabPage->Controls->Add(this->InactiveDelay_numericUpDown);
			this->IOData_tabPage->Controls->Add(this->ActiveDelay_numericUpDown);
			this->IOData_tabPage->Controls->Add(this->label5);
			this->IOData_tabPage->Controls->Add(this->label6);
			this->IOData_tabPage->Controls->Add(this->Low_numericUpDown);
			this->IOData_tabPage->Controls->Add(this->High_numericUpDown);
			this->IOData_tabPage->Controls->Add(this->label16);
			this->IOData_tabPage->Controls->Add(this->label15);
			this->IOData_tabPage->Controls->Add(this->Read_button);
			this->IOData_tabPage->Controls->Add(this->Write_button);
			this->IOData_tabPage->Controls->Add(this->label12);
			this->IOData_tabPage->Controls->Add(this->IOStatus_textBox);
			this->IOData_tabPage->Controls->Add(this->SizeForIO_numericUpDown);
			this->IOData_tabPage->Controls->Add(this->label14);
			this->IOData_tabPage->Controls->Add(this->label13);
			this->IOData_tabPage->Controls->Add(this->ValueToWrite_textBox);
			this->IOData_tabPage->Controls->Add(this->WorkingSingnal_checkBox);
			this->IOData_tabPage->Controls->Add(this->checkBox3);
			this->IOData_tabPage->Controls->Add(this->CurrentStatus_button);
			this->IOData_tabPage->Controls->Add(this->Device_listBox);
			this->IOData_tabPage->Controls->Add(this->StartStatus_checkBox);
			this->IOData_tabPage->Controls->Add(this->SetStartSingnal_checkBox);
			this->IOData_tabPage->Controls->Add(this->WriteIO_button);
			this->IOData_tabPage->Controls->Add(this->BounceStatus_textBox);
			this->IOData_tabPage->Controls->Add(this->label4);
			this->IOData_tabPage->Controls->Add(this->label3);
			this->IOData_tabPage->Controls->Add(this->label2);
			this->IOData_tabPage->Controls->Add(this->label1);
			this->IOData_tabPage->Controls->Add(this->ReEdit_button);
			this->IOData_tabPage->Controls->Add(this->Delete_button);
			this->IOData_tabPage->Controls->Add(this->Add_button);
			this->IOData_tabPage->Controls->Add(this->AllIO_listBox);
			this->IOData_tabPage->Controls->Add(this->BitAddress_numericUpDown);
			this->IOData_tabPage->Controls->Add(this->ReadIO_button);
			this->IOData_tabPage->Controls->Add(this->Name_textBox);
			this->IOData_tabPage->Controls->Add(this->Adress_textBox);
			this->IOData_tabPage->Location = System::Drawing::Point(4, 22);
			this->IOData_tabPage->Name = L"IOData_tabPage";
			this->IOData_tabPage->Padding = System::Windows::Forms::Padding(3);
			this->IOData_tabPage->Size = System::Drawing::Size(967, 826);
			this->IOData_tabPage->TabIndex = 0;
			this->IOData_tabPage->Text = L"IOData";
			this->IOData_tabPage->UseVisualStyleBackColor = true;
			// 
			// label23
			// 
			this->label23->AutoSize = true;
			this->label23->Location = System::Drawing::Point(15, 229);
			this->label23->Name = L"label23";
			this->label23->Size = System::Drawing::Size(41, 12);
			this->label23->TabIndex = 80;
			this->label23->Text = L"IOType";
			// 
			// IOType_comboBox
			// 
			this->IOType_comboBox->FormattingEnabled = true;
			this->IOType_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"Input", L"Output"});
			this->IOType_comboBox->Location = System::Drawing::Point(100, 226);
			this->IOType_comboBox->Name = L"IOType_comboBox";
			this->IOType_comboBox->Size = System::Drawing::Size(185, 20);
			this->IOType_comboBox->TabIndex = 79;
			this->IOType_comboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboBox1_SelectedIndexChanged);
			// 
			// label22
			// 
			this->label22->AutoSize = true;
			this->label22->Location = System::Drawing::Point(3, 262);
			this->label22->Name = L"label22";
			this->label22->Size = System::Drawing::Size(96, 12);
			this->label22->TabIndex = 78;
			this->label22->Text = L"KeyData/按鍵訊號";
			// 
			// KeyData_textBox
			// 
			this->KeyData_textBox->Location = System::Drawing::Point(100, 256);
			this->KeyData_textBox->Name = L"KeyData_textBox";
			this->KeyData_textBox->Size = System::Drawing::Size(184, 22);
			this->KeyData_textBox->TabIndex = 77;
			this->KeyData_textBox->TextChanged += gcnew System::EventHandler(this, &Form1::KeyData_textBox_TextChanged);
			// 
			// label21
			// 
			this->label21->AutoSize = true;
			this->label21->Location = System::Drawing::Point(386, 462);
			this->label21->Name = L"label21";
			this->label21->Size = System::Drawing::Size(57, 12);
			this->label21->TabIndex = 76;
			this->label21->Text = L"InputStatus";
			// 
			// Input_listBox
			// 
			this->Input_listBox->FormattingEnabled = true;
			this->Input_listBox->ItemHeight = 12;
			this->Input_listBox->Location = System::Drawing::Point(381, 480);
			this->Input_listBox->Name = L"Input_listBox";
			this->Input_listBox->Size = System::Drawing::Size(243, 340);
			this->Input_listBox->TabIndex = 75;
			// 
			// IrqCount_label
			// 
			this->IrqCount_label->AutoSize = true;
			this->IrqCount_label->Location = System::Drawing::Point(771, 277);
			this->IrqCount_label->Name = L"IrqCount_label";
			this->IrqCount_label->Size = System::Drawing::Size(48, 12);
			this->IrqCount_label->TabIndex = 74;
			this->IrqCount_label->Text = L"IrqCount";
			// 
			// InputCount_label
			// 
			this->InputCount_label->AutoSize = true;
			this->InputCount_label->Location = System::Drawing::Point(716, 279);
			this->InputCount_label->Name = L"InputCount_label";
			this->InputCount_label->Size = System::Drawing::Size(59, 12);
			this->InputCount_label->TabIndex = 73;
			this->InputCount_label->Text = L"InputCount";
			// 
			// AllInputTo0button
			// 
			this->AllInputTo0button->Location = System::Drawing::Point(161, 633);
			this->AllInputTo0button->Name = L"AllInputTo0button";
			this->AllInputTo0button->Size = System::Drawing::Size(111, 32);
			this->AllInputTo0button->TabIndex = 72;
			this->AllInputTo0button->Text = L"AllInputPinTo0";
			this->AllInputTo0button->UseVisualStyleBackColor = true;
			this->AllInputTo0button->Click += gcnew System::EventHandler(this, &Form1::AllInputTo0button_Click);
			// 
			// AllInputTo1button
			// 
			this->AllInputTo1button->Location = System::Drawing::Point(161, 595);
			this->AllInputTo1button->Name = L"AllInputTo1button";
			this->AllInputTo1button->Size = System::Drawing::Size(111, 32);
			this->AllInputTo1button->TabIndex = 71;
			this->AllInputTo1button->Text = L"AllInputPinTo1";
			this->AllInputTo1button->UseVisualStyleBackColor = true;
			this->AllInputTo1button->Click += gcnew System::EventHandler(this, &Form1::AllInputTo1button_Click);
			// 
			// AllOutputPinTo0_button
			// 
			this->AllOutputPinTo0_button->Location = System::Drawing::Point(26, 633);
			this->AllOutputPinTo0_button->Name = L"AllOutputPinTo0_button";
			this->AllOutputPinTo0_button->Size = System::Drawing::Size(111, 32);
			this->AllOutputPinTo0_button->TabIndex = 70;
			this->AllOutputPinTo0_button->Text = L"AllOutputPinTo0";
			this->AllOutputPinTo0_button->UseVisualStyleBackColor = true;
			this->AllOutputPinTo0_button->Click += gcnew System::EventHandler(this, &Form1::AllPinTo0_button_Click);
			// 
			// AllOutputPinTo1_button
			// 
			this->AllOutputPinTo1_button->Location = System::Drawing::Point(26, 595);
			this->AllOutputPinTo1_button->Name = L"AllOutputPinTo1_button";
			this->AllOutputPinTo1_button->Size = System::Drawing::Size(111, 32);
			this->AllOutputPinTo1_button->TabIndex = 69;
			this->AllOutputPinTo1_button->Text = L"AllOutputPinTo1";
			this->AllOutputPinTo1_button->UseVisualStyleBackColor = true;
			this->AllOutputPinTo1_button->Click += gcnew System::EventHandler(this, &Form1::AllPinTo1_button_Click);
			// 
			// label20
			// 
			this->label20->AutoSize = true;
			this->label20->Location = System::Drawing::Point(651, 307);
			this->label20->Name = L"label20";
			this->label20->Size = System::Drawing::Size(64, 12);
			this->label20->TabIndex = 68;
			this->label20->Text = L"OutputStatus";
			// 
			// OutputStatus_listBox
			// 
			this->OutputStatus_listBox->FormattingEnabled = true;
			this->OutputStatus_listBox->ItemHeight = 12;
			this->OutputStatus_listBox->Location = System::Drawing::Point(646, 325);
			this->OutputStatus_listBox->Name = L"OutputStatus_listBox";
			this->OutputStatus_listBox->Size = System::Drawing::Size(243, 496);
			this->OutputStatus_listBox->TabIndex = 67;
			// 
			// OutputCount_label
			// 
			this->OutputCount_label->AutoSize = true;
			this->OutputCount_label->Location = System::Drawing::Point(644, 279);
			this->OutputCount_label->Name = L"OutputCount_label";
			this->OutputCount_label->Size = System::Drawing::Size(66, 12);
			this->OutputCount_label->TabIndex = 66;
			this->OutputCount_label->Text = L"OutputCount";
			// 
			// StartEnable_checkBox
			// 
			this->StartEnable_checkBox->AutoSize = true;
			this->StartEnable_checkBox->Checked = true;
			this->StartEnable_checkBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->StartEnable_checkBox->Location = System::Drawing::Point(32, 332);
			this->StartEnable_checkBox->Name = L"StartEnable_checkBox";
			this->StartEnable_checkBox->Size = System::Drawing::Size(364, 16);
			this->StartEnable_checkBox->TabIndex = 65;
			this->StartEnable_checkBox->Text = L"StartEnable(if it\'s not clicked,it won\'t do anything until someone active it)";
			this->StartEnable_checkBox->UseVisualStyleBackColor = true;
			// 
			// InactiveDelay_numericUpDown
			// 
			this->InactiveDelay_numericUpDown->Location = System::Drawing::Point(100, 130);
			this->InactiveDelay_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			this->InactiveDelay_numericUpDown->Name = L"InactiveDelay_numericUpDown";
			this->InactiveDelay_numericUpDown->Size = System::Drawing::Size(187, 22);
			this->InactiveDelay_numericUpDown->TabIndex = 26;
			// 
			// ActiveDelay_numericUpDown
			// 
			this->ActiveDelay_numericUpDown->Location = System::Drawing::Point(100, 99);
			this->ActiveDelay_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			this->ActiveDelay_numericUpDown->Name = L"ActiveDelay_numericUpDown";
			this->ActiveDelay_numericUpDown->Size = System::Drawing::Size(187, 22);
			this->ActiveDelay_numericUpDown->TabIndex = 25;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(20, 102);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(62, 12);
			this->label5->TabIndex = 64;
			this->label5->Text = L"ActiveDelay";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(20, 133);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(72, 12);
			this->label6->TabIndex = 63;
			this->label6->Text = L"InActiveDelay";
			// 
			// Low_numericUpDown
			// 
			this->Low_numericUpDown->Location = System::Drawing::Point(100, 194);
			this->Low_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			this->Low_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, System::Int32::MinValue});
			this->Low_numericUpDown->Name = L"Low_numericUpDown";
			this->Low_numericUpDown->Size = System::Drawing::Size(187, 22);
			this->Low_numericUpDown->TabIndex = 28;
			// 
			// High_numericUpDown
			// 
			this->High_numericUpDown->Location = System::Drawing::Point(100, 162);
			this->High_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			this->High_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, System::Int32::MinValue});
			this->High_numericUpDown->Name = L"High_numericUpDown";
			this->High_numericUpDown->Size = System::Drawing::Size(187, 22);
			this->High_numericUpDown->TabIndex = 27;
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Location = System::Drawing::Point(3, 163);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(95, 12);
			this->label16->TabIndex = 60;
			this->label16->Text = L"High(-1 as infinity)";
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(1, 195);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(93, 12);
			this->label15->TabIndex = 58;
			this->label15->Text = L"Low(-1 as infinity)";
			// 
			// Read_button
			// 
			this->Read_button->Location = System::Drawing::Point(809, 240);
			this->Read_button->Name = L"Read_button";
			this->Read_button->Size = System::Drawing::Size(66, 23);
			this->Read_button->TabIndex = 56;
			this->Read_button->Text = L"Read";
			this->Read_button->UseVisualStyleBackColor = true;
			this->Read_button->Click += gcnew System::EventHandler(this, &Form1::Write_button_Click);
			// 
			// Write_button
			// 
			this->Write_button->Location = System::Drawing::Point(690, 240);
			this->Write_button->Name = L"Write_button";
			this->Write_button->Size = System::Drawing::Size(66, 23);
			this->Write_button->TabIndex = 55;
			this->Write_button->Text = L"Write";
			this->Write_button->UseVisualStyleBackColor = true;
			this->Write_button->Click += gcnew System::EventHandler(this, &Form1::Write_button_Click);
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(644, 127);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(34, 12);
			this->label12->TabIndex = 54;
			this->label12->Text = L"Result";
			// 
			// IOStatus_textBox
			// 
			this->IOStatus_textBox->Location = System::Drawing::Point(690, 129);
			this->IOStatus_textBox->Multiline = true;
			this->IOStatus_textBox->Name = L"IOStatus_textBox";
			this->IOStatus_textBox->Size = System::Drawing::Size(185, 96);
			this->IOStatus_textBox->TabIndex = 53;
			// 
			// SizeForIO_numericUpDown
			// 
			this->SizeForIO_numericUpDown->Location = System::Drawing::Point(690, 43);
			this->SizeForIO_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {65536, 0, 0, 0});
			this->SizeForIO_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->SizeForIO_numericUpDown->Name = L"SizeForIO_numericUpDown";
			this->SizeForIO_numericUpDown->Size = System::Drawing::Size(185, 22);
			this->SizeForIO_numericUpDown->TabIndex = 52;
			this->SizeForIO_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(644, 43);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(24, 12);
			this->label14->TabIndex = 51;
			this->label14->Text = L"Size";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(644, 73);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(32, 12);
			this->label13->TabIndex = 49;
			this->label13->Text = L"Value";
			// 
			// ValueToWrite_textBox
			// 
			this->ValueToWrite_textBox->Location = System::Drawing::Point(690, 77);
			this->ValueToWrite_textBox->Name = L"ValueToWrite_textBox";
			this->ValueToWrite_textBox->Size = System::Drawing::Size(185, 22);
			this->ValueToWrite_textBox->TabIndex = 46;
			// 
			// WorkingSingnal_checkBox
			// 
			this->WorkingSingnal_checkBox->AutoSize = true;
			this->WorkingSingnal_checkBox->Location = System::Drawing::Point(121, 310);
			this->WorkingSingnal_checkBox->Name = L"WorkingSingnal_checkBox";
			this->WorkingSingnal_checkBox->Size = System::Drawing::Size(272, 16);
			this->WorkingSingnal_checkBox->TabIndex = 32;
			this->WorkingSingnal_checkBox->Text = L"ActiveSingnal(checked for high)/打勾就是high動作";
			this->WorkingSingnal_checkBox->UseVisualStyleBackColor = true;
			// 
			// checkBox3
			// 
			this->checkBox3->AutoSize = true;
			this->checkBox3->Location = System::Drawing::Point(32, 310);
			this->checkBox3->Name = L"checkBox3";
			this->checkBox3->Size = System::Drawing::Size(84, 16);
			this->checkBox3->TabIndex = 31;
			this->checkBox3->Text = L"亂數初始值";
			this->checkBox3->UseVisualStyleBackColor = true;
			// 
			// CurrentStatus_button
			// 
			this->CurrentStatus_button->Location = System::Drawing::Point(198, 353);
			this->CurrentStatus_button->Name = L"CurrentStatus_button";
			this->CurrentStatus_button->Size = System::Drawing::Size(95, 21);
			this->CurrentStatus_button->TabIndex = 35;
			this->CurrentStatus_button->Text = L"CurrentStatus";
			this->CurrentStatus_button->UseVisualStyleBackColor = true;
			this->CurrentStatus_button->Click += gcnew System::EventHandler(this, &Form1::CurrentStatus_button_Click);
			// 
			// Device_listBox
			// 
			this->Device_listBox->FormattingEnabled = true;
			this->Device_listBox->ItemHeight = 12;
			this->Device_listBox->Location = System::Drawing::Point(389, 13);
			this->Device_listBox->Name = L"Device_listBox";
			this->Device_listBox->Size = System::Drawing::Size(243, 136);
			this->Device_listBox->TabIndex = 42;
			// 
			// StartStatus_checkBox
			// 
			this->StartStatus_checkBox->AutoSize = true;
			this->StartStatus_checkBox->Location = System::Drawing::Point(217, 290);
			this->StartStatus_checkBox->Name = L"StartStatus_checkBox";
			this->StartStatus_checkBox->Size = System::Drawing::Size(124, 16);
			this->StartStatus_checkBox->TabIndex = 30;
			this->StartStatus_checkBox->Text = L"StateStatus/初始訊號";
			this->StartStatus_checkBox->UseVisualStyleBackColor = true;
			// 
			// SetStartSingnal_checkBox
			// 
			this->SetStartSingnal_checkBox->AutoSize = true;
			this->SetStartSingnal_checkBox->Location = System::Drawing::Point(32, 288);
			this->SetStartSingnal_checkBox->Name = L"SetStartSingnal_checkBox";
			this->SetStartSingnal_checkBox->Size = System::Drawing::Size(179, 16);
			this->SetStartSingnal_checkBox->TabIndex = 29;
			this->SetStartSingnal_checkBox->Text = L"ForceToSetStatus/強迫設定狀態";
			this->SetStartSingnal_checkBox->UseVisualStyleBackColor = true;
			// 
			// WriteIO_button
			// 
			this->WriteIO_button->Location = System::Drawing::Point(27, 351);
			this->WriteIO_button->Name = L"WriteIO_button";
			this->WriteIO_button->Size = System::Drawing::Size(66, 23);
			this->WriteIO_button->TabIndex = 33;
			this->WriteIO_button->Text = L"WriteIO";
			this->WriteIO_button->UseVisualStyleBackColor = true;
			this->WriteIO_button->Click += gcnew System::EventHandler(this, &Form1::On_button_Click);
			// 
			// BounceStatus_textBox
			// 
			this->BounceStatus_textBox->Location = System::Drawing::Point(26, 397);
			this->BounceStatus_textBox->Multiline = true;
			this->BounceStatus_textBox->Name = L"BounceStatus_textBox";
			this->BounceStatus_textBox->ScrollBars = System::Windows::Forms::ScrollBars::Both;
			this->BounceStatus_textBox->Size = System::Drawing::Size(281, 174);
			this->BounceStatus_textBox->TabIndex = 38;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(24, 382);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(143, 12);
			this->label4->TabIndex = 37;
			this->label4->Text = L"BounceStatus/訊號彈跳狀態";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(20, 20);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(59, 12);
			this->label3->TabIndex = 35;
			this->label3->Text = L"Name/名字";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(20, 74);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(56, 12);
			this->label2->TabIndex = 32;
			this->label2->Text = L"BitAddress";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(20, 45);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(69, 12);
			this->label1->TabIndex = 30;
			this->label1->Text = L"Address/位址";
			// 
			// ReEdit_button
			// 
			this->ReEdit_button->Location = System::Drawing::Point(558, 412);
			this->ReEdit_button->Name = L"ReEdit_button";
			this->ReEdit_button->Size = System::Drawing::Size(72, 21);
			this->ReEdit_button->TabIndex = 38;
			this->ReEdit_button->Text = L"ReEdit/重新編輯";
			this->ReEdit_button->UseVisualStyleBackColor = true;
			this->ReEdit_button->Click += gcnew System::EventHandler(this, &Form1::ReEdit_button_Click);
			// 
			// Delete_button
			// 
			this->Delete_button->Location = System::Drawing::Point(480, 413);
			this->Delete_button->Name = L"Delete_button";
			this->Delete_button->Size = System::Drawing::Size(72, 20);
			this->Delete_button->TabIndex = 37;
			this->Delete_button->Text = L"Delete/刪除";
			this->Delete_button->UseVisualStyleBackColor = true;
			this->Delete_button->Click += gcnew System::EventHandler(this, &Form1::Delete_button_Click);
			// 
			// Add_button
			// 
			this->Add_button->Location = System::Drawing::Point(389, 413);
			this->Add_button->Name = L"Add_button";
			this->Add_button->Size = System::Drawing::Size(72, 20);
			this->Add_button->TabIndex = 36;
			this->Add_button->Text = L"Add/增加";
			this->Add_button->UseVisualStyleBackColor = true;
			this->Add_button->Click += gcnew System::EventHandler(this, &Form1::Add_button_Click);
			// 
			// AllIO_listBox
			// 
			this->AllIO_listBox->FormattingEnabled = true;
			this->AllIO_listBox->ItemHeight = 12;
			this->AllIO_listBox->Location = System::Drawing::Point(389, 173);
			this->AllIO_listBox->Name = L"AllIO_listBox";
			this->AllIO_listBox->ScrollAlwaysVisible = true;
			this->AllIO_listBox->Size = System::Drawing::Size(243, 232);
			this->AllIO_listBox->TabIndex = 36;
			this->AllIO_listBox->DoubleClick += gcnew System::EventHandler(this, &Form1::ReEdit_button_Click);
			this->AllIO_listBox->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::AllIO_listBox_KeyDown);
			// 
			// BitAddress_numericUpDown
			// 
			this->BitAddress_numericUpDown->Location = System::Drawing::Point(100, 73);
			this->BitAddress_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {7, 0, 0, 0});
			this->BitAddress_numericUpDown->Name = L"BitAddress_numericUpDown";
			this->BitAddress_numericUpDown->Size = System::Drawing::Size(187, 22);
			this->BitAddress_numericUpDown->TabIndex = 24;
			this->BitAddress_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::BitAddress_numericUpDown_ValueChanged);
			// 
			// ReadIO_button
			// 
			this->ReadIO_button->Location = System::Drawing::Point(116, 351);
			this->ReadIO_button->Name = L"ReadIO_button";
			this->ReadIO_button->Size = System::Drawing::Size(66, 23);
			this->ReadIO_button->TabIndex = 34;
			this->ReadIO_button->Text = L"ReadIO";
			this->ReadIO_button->UseVisualStyleBackColor = true;
			this->ReadIO_button->Click += gcnew System::EventHandler(this, &Form1::On_button_Click);
			// 
			// Name_textBox
			// 
			this->Name_textBox->Location = System::Drawing::Point(100, 17);
			this->Name_textBox->Name = L"Name_textBox";
			this->Name_textBox->Size = System::Drawing::Size(191, 22);
			this->Name_textBox->TabIndex = 22;
			// 
			// Adress_textBox
			// 
			this->Adress_textBox->Location = System::Drawing::Point(100, 45);
			this->Adress_textBox->Name = L"Adress_textBox";
			this->Adress_textBox->Size = System::Drawing::Size(187, 22);
			this->Adress_textBox->TabIndex = 23;
			// 
			// DeviceData_tabPage
			// 
			this->DeviceData_tabPage->Controls->Add(this->GoOff_button);
			this->DeviceData_tabPage->Controls->Add(this->GoUpdate_button);
			this->DeviceData_tabPage->Controls->Add(this->label11);
			this->DeviceData_tabPage->Controls->Add(this->label10);
			this->DeviceData_tabPage->Controls->Add(this->label7);
			this->DeviceData_tabPage->Controls->Add(this->TargetIO_listBox);
			this->DeviceData_tabPage->Controls->Add(this->DelBehavior_button);
			this->DeviceData_tabPage->Controls->Add(this->AddBehavior_button);
			this->DeviceData_tabPage->Controls->Add(this->label9);
			this->DeviceData_tabPage->Controls->Add(this->label8);
			this->DeviceData_tabPage->Controls->Add(this->AllUsedBehavior_groupBox);
			this->DeviceData_tabPage->Controls->Add(this->UsedBehavior_listBox);
			this->DeviceData_tabPage->Controls->Add(this->AllBehavior_listBox);
			this->DeviceData_tabPage->Controls->Add(this->AllIODevice_listBox);
			this->DeviceData_tabPage->Location = System::Drawing::Point(4, 22);
			this->DeviceData_tabPage->Name = L"DeviceData_tabPage";
			this->DeviceData_tabPage->Padding = System::Windows::Forms::Padding(3);
			this->DeviceData_tabPage->Size = System::Drawing::Size(967, 826);
			this->DeviceData_tabPage->TabIndex = 1;
			this->DeviceData_tabPage->Text = L"DeviceData";
			this->DeviceData_tabPage->UseVisualStyleBackColor = true;
			// 
			// GoOff_button
			// 
			this->GoOff_button->Location = System::Drawing::Point(565, 153);
			this->GoOff_button->Name = L"GoOff_button";
			this->GoOff_button->Size = System::Drawing::Size(81, 22);
			this->GoOff_button->TabIndex = 16;
			this->GoOff_button->Text = L"GoOff";
			this->GoOff_button->UseVisualStyleBackColor = true;
			this->GoOff_button->Click += gcnew System::EventHandler(this, &Form1::GoUpdate_button_Click);
			// 
			// GoUpdate_button
			// 
			this->GoUpdate_button->Location = System::Drawing::Point(478, 153);
			this->GoUpdate_button->Name = L"GoUpdate_button";
			this->GoUpdate_button->Size = System::Drawing::Size(81, 22);
			this->GoUpdate_button->TabIndex = 15;
			this->GoUpdate_button->Text = L"GoUpdate";
			this->GoUpdate_button->UseVisualStyleBackColor = true;
			this->GoUpdate_button->Click += gcnew System::EventHandler(this, &Form1::GoUpdate_button_Click);
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(700, 151);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(204, 12);
			this->label11->TabIndex = 0;
			this->label11->Text = L"Piority is important,it will change the result";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(179, 12);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(47, 12);
			this->label10->TabIndex = 14;
			this->label10->Text = L"TargetIO";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(17, 12);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(49, 12);
			this->label7->TabIndex = 13;
			this->label7->Text = L"SourceIO";
			// 
			// TargetIO_listBox
			// 
			this->TargetIO_listBox->FormattingEnabled = true;
			this->TargetIO_listBox->ItemHeight = 12;
			this->TargetIO_listBox->Location = System::Drawing::Point(181, 27);
			this->TargetIO_listBox->Name = L"TargetIO_listBox";
			this->TargetIO_listBox->Size = System::Drawing::Size(167, 688);
			this->TargetIO_listBox->TabIndex = 12;
			// 
			// DelBehavior_button
			// 
			this->DelBehavior_button->Location = System::Drawing::Point(625, 110);
			this->DelBehavior_button->Name = L"DelBehavior_button";
			this->DelBehavior_button->Size = System::Drawing::Size(38, 23);
			this->DelBehavior_button->TabIndex = 11;
			this->DelBehavior_button->Text = L"Del";
			this->DelBehavior_button->UseVisualStyleBackColor = true;
			this->DelBehavior_button->Click += gcnew System::EventHandler(this, &Form1::AddBehavior_button_Click);
			// 
			// AddBehavior_button
			// 
			this->AddBehavior_button->Location = System::Drawing::Point(625, 38);
			this->AddBehavior_button->Name = L"AddBehavior_button";
			this->AddBehavior_button->Size = System::Drawing::Size(38, 23);
			this->AddBehavior_button->TabIndex = 10;
			this->AddBehavior_button->Text = L">>";
			this->AddBehavior_button->UseVisualStyleBackColor = true;
			this->AddBehavior_button->Click += gcnew System::EventHandler(this, &Form1::AddBehavior_button_Click);
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(700, 11);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(146, 12);
			this->label9->TabIndex = 9;
			this->label9->Text = L"UsedBehavior/被使用的行為";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(375, 12);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(113, 12);
			this->label8->TabIndex = 8;
			this->label8->Text = L"AllBehavior/所有行為";
			// 
			// AllUsedBehavior_groupBox
			// 
			this->AllUsedBehavior_groupBox->Location = System::Drawing::Point(360, 181);
			this->AllUsedBehavior_groupBox->Name = L"AllUsedBehavior_groupBox";
			this->AllUsedBehavior_groupBox->Size = System::Drawing::Size(642, 442);
			this->AllUsedBehavior_groupBox->TabIndex = 7;
			this->AllUsedBehavior_groupBox->TabStop = false;
			// 
			// UsedBehavior_listBox
			// 
			this->UsedBehavior_listBox->FormattingEnabled = true;
			this->UsedBehavior_listBox->ItemHeight = 12;
			this->UsedBehavior_listBox->Location = System::Drawing::Point(702, 24);
			this->UsedBehavior_listBox->Name = L"UsedBehavior_listBox";
			this->UsedBehavior_listBox->Size = System::Drawing::Size(253, 124);
			this->UsedBehavior_listBox->TabIndex = 6;
			// 
			// AllBehavior_listBox
			// 
			this->AllBehavior_listBox->FormattingEnabled = true;
			this->AllBehavior_listBox->ItemHeight = 12;
			this->AllBehavior_listBox->Items->AddRange(gcnew cli::array< System::Object^  >(6) {L"我是最基本的型態加我也沒用...", L"隨機觸發(100%代表一定會觸發某一個)", 
				L"依照訊號週期觸發n次", L"被多重觸發才會執行", L"送出高訊號", L"送出低訊號"});
			this->AllBehavior_listBox->Location = System::Drawing::Point(377, 32);
			this->AllBehavior_listBox->Name = L"AllBehavior_listBox";
			this->AllBehavior_listBox->Size = System::Drawing::Size(227, 112);
			this->AllBehavior_listBox->TabIndex = 5;
			// 
			// AllIODevice_listBox
			// 
			this->AllIODevice_listBox->FormattingEnabled = true;
			this->AllIODevice_listBox->ItemHeight = 12;
			this->AllIODevice_listBox->Location = System::Drawing::Point(8, 27);
			this->AllIODevice_listBox->Name = L"AllIODevice_listBox";
			this->AllIODevice_listBox->Size = System::Drawing::Size(167, 688);
			this->AllIODevice_listBox->TabIndex = 0;
			this->AllIODevice_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::AllIODevice_listBox_SelectedIndexChanged);
			// 
			// Simulation_tabPage
			// 
			this->Simulation_tabPage->Controls->Add(this->SimulationIOStatus_listBox);
			this->Simulation_tabPage->Controls->Add(this->SimulationIOList_listBox);
			this->Simulation_tabPage->Controls->Add(this->Simulation_checkBox);
			this->Simulation_tabPage->Location = System::Drawing::Point(4, 22);
			this->Simulation_tabPage->Name = L"Simulation_tabPage";
			this->Simulation_tabPage->Size = System::Drawing::Size(967, 826);
			this->Simulation_tabPage->TabIndex = 2;
			this->Simulation_tabPage->Text = L"Simulation";
			this->Simulation_tabPage->UseVisualStyleBackColor = true;
			// 
			// SimulationIOStatus_listBox
			// 
			this->SimulationIOStatus_listBox->FormattingEnabled = true;
			this->SimulationIOStatus_listBox->ItemHeight = 12;
			this->SimulationIOStatus_listBox->Location = System::Drawing::Point(299, 36);
			this->SimulationIOStatus_listBox->Name = L"SimulationIOStatus_listBox";
			this->SimulationIOStatus_listBox->Size = System::Drawing::Size(164, 448);
			this->SimulationIOStatus_listBox->TabIndex = 2;
			// 
			// SimulationIOList_listBox
			// 
			this->SimulationIOList_listBox->FormattingEnabled = true;
			this->SimulationIOList_listBox->ItemHeight = 12;
			this->SimulationIOList_listBox->Location = System::Drawing::Point(8, 36);
			this->SimulationIOList_listBox->Name = L"SimulationIOList_listBox";
			this->SimulationIOList_listBox->Size = System::Drawing::Size(276, 448);
			this->SimulationIOList_listBox->TabIndex = 1;
			// 
			// Simulation_checkBox
			// 
			this->Simulation_checkBox->AutoSize = true;
			this->Simulation_checkBox->Location = System::Drawing::Point(23, 14);
			this->Simulation_checkBox->Name = L"Simulation_checkBox";
			this->Simulation_checkBox->Size = System::Drawing::Size(74, 16);
			this->Simulation_checkBox->TabIndex = 0;
			this->Simulation_checkBox->Text = L"Simulation";
			this->Simulation_checkBox->UseVisualStyleBackColor = true;
			this->Simulation_checkBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::Simulation_checkBox_CheckedChanged);
			// 
			// Mutex_tabPage
			// 
			this->Mutex_tabPage->Controls->Add(this->label19);
			this->Mutex_tabPage->Controls->Add(this->MutexTime_textBox);
			this->Mutex_tabPage->Controls->Add(this->label18);
			this->Mutex_tabPage->Controls->Add(this->label17);
			this->Mutex_tabPage->Controls->Add(this->StartAddress_label);
			this->Mutex_tabPage->Controls->Add(this->MutexWriteSize_textBox);
			this->Mutex_tabPage->Controls->Add(this->MutexValueToWrite__textBox);
			this->Mutex_tabPage->Controls->Add(this->MutexAddress_textBox);
			this->Mutex_tabPage->Controls->Add(this->Mutex_listBox);
			this->Mutex_tabPage->Controls->Add(this->MutexTest_button);
			this->Mutex_tabPage->Location = System::Drawing::Point(4, 22);
			this->Mutex_tabPage->Name = L"Mutex_tabPage";
			this->Mutex_tabPage->Size = System::Drawing::Size(967, 826);
			this->Mutex_tabPage->TabIndex = 3;
			this->Mutex_tabPage->Text = L"Mutex";
			this->Mutex_tabPage->UseVisualStyleBackColor = true;
			// 
			// label19
			// 
			this->label19->AutoSize = true;
			this->label19->Location = System::Drawing::Point(65, 155);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(65, 12);
			this->label19->TabIndex = 9;
			this->label19->Text = L"time(second)";
			// 
			// MutexTime_textBox
			// 
			this->MutexTime_textBox->Location = System::Drawing::Point(98, 170);
			this->MutexTime_textBox->Name = L"MutexTime_textBox";
			this->MutexTime_textBox->Size = System::Drawing::Size(100, 22);
			this->MutexTime_textBox->TabIndex = 8;
			this->MutexTime_textBox->Text = L"10";
			// 
			// label18
			// 
			this->label18->AutoSize = true;
			this->label18->Location = System::Drawing::Point(67, 111);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(24, 12);
			this->label18->TabIndex = 7;
			this->label18->Text = L"Size";
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Location = System::Drawing::Point(67, 71);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(32, 12);
			this->label17->TabIndex = 6;
			this->label17->Text = L"Value";
			// 
			// StartAddress_label
			// 
			this->StartAddress_label->AutoSize = true;
			this->StartAddress_label->Location = System::Drawing::Point(67, 16);
			this->StartAddress_label->Name = L"StartAddress_label";
			this->StartAddress_label->Size = System::Drawing::Size(63, 12);
			this->StartAddress_label->TabIndex = 5;
			this->StartAddress_label->Text = L"StartAddress";
			// 
			// MutexWriteSize_textBox
			// 
			this->MutexWriteSize_textBox->Location = System::Drawing::Point(98, 126);
			this->MutexWriteSize_textBox->Name = L"MutexWriteSize_textBox";
			this->MutexWriteSize_textBox->Size = System::Drawing::Size(100, 22);
			this->MutexWriteSize_textBox->TabIndex = 4;
			// 
			// MutexValueToWrite__textBox
			// 
			this->MutexValueToWrite__textBox->Location = System::Drawing::Point(98, 86);
			this->MutexValueToWrite__textBox->Name = L"MutexValueToWrite__textBox";
			this->MutexValueToWrite__textBox->Size = System::Drawing::Size(100, 22);
			this->MutexValueToWrite__textBox->TabIndex = 3;
			// 
			// MutexAddress_textBox
			// 
			this->MutexAddress_textBox->Location = System::Drawing::Point(98, 41);
			this->MutexAddress_textBox->Name = L"MutexAddress_textBox";
			this->MutexAddress_textBox->Size = System::Drawing::Size(100, 22);
			this->MutexAddress_textBox->TabIndex = 2;
			// 
			// Mutex_listBox
			// 
			this->Mutex_listBox->FormattingEnabled = true;
			this->Mutex_listBox->ItemHeight = 12;
			this->Mutex_listBox->Location = System::Drawing::Point(392, 36);
			this->Mutex_listBox->Name = L"Mutex_listBox";
			this->Mutex_listBox->Size = System::Drawing::Size(312, 328);
			this->Mutex_listBox->TabIndex = 1;
			// 
			// MutexTest_button
			// 
			this->MutexTest_button->Location = System::Drawing::Point(96, 221);
			this->MutexTest_button->Name = L"MutexTest_button";
			this->MutexTest_button->Size = System::Drawing::Size(100, 44);
			this->MutexTest_button->TabIndex = 0;
			this->MutexTest_button->Text = L"Go!";
			this->MutexTest_button->UseVisualStyleBackColor = true;
			this->MutexTest_button->Click += gcnew System::EventHandler(this, &Form1::MutexTest_button_Click);
			// 
			// timer2
			// 
			this->timer2->Tick += gcnew System::EventHandler(this, &Form1::timer2_Tick);
			// 
			// timer3
			// 
			this->timer3->Interval = 1;
			this->timer3->Tick += gcnew System::EventHandler(this, &Form1::timer3_Tick);
			// 
			// timer4
			// 
			this->timer4->Tick += gcnew System::EventHandler(this, &Form1::timer4_Tick);
			// 
			// PinStatus_timer
			// 
			this->PinStatus_timer->Tick += gcnew System::EventHandler(this, &Form1::PinStatus_timer_Tick);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(975, 876);
			this->Controls->Add(this->IOSimulation_tabControl);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->IOSimulation_tabControl->ResumeLayout(false);
			this->IOData_tabPage->ResumeLayout(false);
			this->IOData_tabPage->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->InactiveDelay_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ActiveDelay_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Low_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->High_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->SizeForIO_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->BitAddress_numericUpDown))->EndInit();
			this->DeviceData_tabPage->ResumeLayout(false);
			this->DeviceData_tabPage->PerformLayout();
			this->Simulation_tabPage->ResumeLayout(false);
			this->Simulation_tabPage->PerformLayout();
			this->Mutex_tabPage->ResumeLayout(false);
			this->Mutex_tabPage->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void Add_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				if( Name_textBox->Text->Length == 0 )
				{
					WARNING_MSG("please input name");
					return;
				}
				if(AllIO_listBox->Items->Contains(Name_textBox->Text))
				{
					WARING_YES_NO_TO_NO("name exists!!,replace!?")
					{
						return;
					}
					else
					{
						int	l_iIndex = AllIO_listBox->Items->IndexOf(Name_textBox->Text);
						AllIO_listBox->Items->RemoveAt(l_iIndex);
						g_pIOSMDriverInterface->RemoveObject(l_iIndex);
					}
				}
				if( Adress_textBox->Text->Length >= 1 )
				{
					cIOSMIOData*l_pIOSMIOData = new cIOSMIOData();
					if( IOType_comboBox->SelectedIndex == 0)
						l_pIOSMIOData->SeteIODataType(eIODT_INPUT);
					else
						l_pIOSMIOData->SeteIODataType(eIODT_OUTPUT);
					l_pIOSMIOData->SetCurrentSingnal(StartStatus_checkBox->Checked?true:false);
					l_pIOSMIOData->SetBitAddress((BYTE)BitAddress_numericUpDown->Value);
					l_pIOSMIOData->SetStartSingnal(SetStartSingnal_checkBox->Checked?true:false);
					l_pIOSMIOData->SetWorkingSingnal(WorkingSingnal_checkBox->Checked);
					if(KeyData_textBox->Text->Length)
					{
						unsigned char*l_puc = new unsigned char;
						*l_puc = (unsigned char)KeyData_textBox->Text[0];
						l_pIOSMIOData->SetKeyData(l_puc);
					}
					l_pIOSMIOData->SetHighCircle((int)High_numericUpDown->Value/1000.f);
					l_pIOSMIOData->SetLowCircle((int)Low_numericUpDown->Value/1000.f);

					l_pIOSMIOData->SetDelayTimeForActive((int)ActiveDelay_numericUpDown->Value/1000.f);
					l_pIOSMIOData->SetDelayTimeForInactive((int)InactiveDelay_numericUpDown->Value/1000.f);
					l_pIOSMIOData->SetEnable(StartEnable_checkBox->Checked);
					//int	l_iMemoryAddress = UT::HexadecimalStringToDecimal(DNCT::GcStringToChar(Adress_textBox->Text));
					int	l_iMemoryAddress = System::Int32::Parse(Adress_textBox->Text);
					l_pIOSMIOData->SetMemoryAddress((DWORD)l_iMemoryAddress);
					l_pIOSMIOData->SetName(DNCT::GcStringToWchar(Name_textBox->Text));
					bool	l_b = g_pIOSMDriverInterface->AddObject(l_pIOSMIOData);
					assert(l_b);
					if(g_pIOSMIOManager->GetObject(l_pIOSMIOData->GetName()))
					{
						cIOSMBaseDevice*l_pIOSMBaseDevice = new cIOSMBaseDevice(l_pIOSMIOData);
						//get old data
						cIOSMBaseDevice*l_pOldIOSMBaseDevice = g_pIOSMIOManager->GetObject(l_pIOSMIOData->GetName());
						//copy behavior
						l_pOldIOSMBaseDevice->DumpListPointer(l_pIOSMBaseDevice);
						//kill old data
						g_pIOSMIOManager->RemoveObject(l_pIOSMIOData->GetName());
						//add new data
						g_pIOSMIOManager->AddObject(l_pIOSMBaseDevice);
					}
					else
					{
						cIOSMBaseDevice*l_pIOSMBaseDevice = new cIOSMBaseDevice(l_pIOSMIOData);
						g_pIOSMIOManager->AddObject(l_pIOSMBaseDevice);
					}
					AllIO_listBox->Items->Add(Name_textBox->Text);
				}
				else
				{
					WARNING_MSG("memory address error");
				}
			 }

private: System::Void Adress_textBox_TextChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(Adress_textBox->Text->Length>4)
			 {
				Adress_textBox->Text = Adress_textBox->Text->Substring(0,4);
				WARNING_MSG("Maxiuma length is 4");
			 }
			 else
			 {
				 if( Adress_textBox->Text->Length > 0 )
				 {
					wchar_t	l_c = Adress_textBox->Text[Adress_textBox->Text->Length-1];
					if( (l_c>=L'0'&& l_c<= L'9') ||
						(l_c>=L'a'&& l_c<= L'f') ||
						(l_c>=L'A'&& l_c<= L'F') )
					{
					
					}
					else
					{
						WARNING_MSG("inligal");
						if( Adress_textBox->Text->Length == 1 )
							Adress_textBox->Text = "";
						else
							Adress_textBox->Text = Adress_textBox->Text->Substring(0,Adress_textBox->Text->Length-1);
					}
				 }
			 }
		 }

private: System::Void AllIO_listBox_DoubleClick(System::Object^  sender, System::EventArgs^  e)
		 {
			ReEditIOData();
		 }
private: System::Void AllIO_listBox_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
		 {
			 if( e->KeyData == GCFORM::Keys::Enter )
			 {
				ReEditIOData();
			 }
		 }
private: System::Void ReEdit_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 ReEditIOData();
		 }
private: System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( saveToolStripMenuItem == sender  )
			 {
				String^l_strFileName = DNCT::SaveFileAndGetName("xml files (*.xml)|*.xml|All files (*.*)|*.*");
				g_pIOSMDriverInterface->Export(DNCT::GcStringToChar(l_strFileName));
				//if( g_pIOSMDriverInterface->Count() )
				//	FileToUnicode(l_strFileName);
			 }
			 else
			 if( openToolStripMenuItem == sender )
			 {
				ClearIOData();
				ClearUIData();
				String^l_strFileName = DNCT::OpenFileAndGetName("xml files (*.xml)|*.xml|All files (*.*)|*.*");
				if( l_strFileName )
				{
					bool	l_b = g_pIOSMDriverInterface->Parse(DNCT::GcStringToChar(l_strFileName));
					if( l_b )
					{
						for( int i=0;i<g_pIOSMDriverInterface->Count();++i )
						{
							cIOSMIOData*l_pIOSMIOData = g_pIOSMDriverInterface->GetObject(i);
							AllIO_listBox->Items->Add(DNCT::WcharToGcstring(l_pIOSMIOData->GetName()));
						}
					}
				}
			 }
		 }

private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
		 {
			 m_pTimeAndFPS->Update();
			 m_pTimeCounter->Update(m_pTimeAndFPS->fElpaseTime);
			 if(m_pTimeCounter->bTragetTimrReached)
			 {
				 this->timer1->Enabled = false;
				 bool	l_b;
				 if(g_pIOSMDriverInterface->ReadIOBitAddress(m_iHexValue,m_iBitAddress,&l_b))
				 {
					 String^l_strOn = l_b?"On":"Off";
					 BounceStatus_textBox->Text = BounceStatus_textBox->Text+l_strOn+DNCT::GetChanglineString();
				 }
				 else
				 {
					WARNING_MSG("Read failed");
				 }
			 }
		 }

private: System::Void IOSimulation_tabControl_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( IOSimulation_tabControl->SelectedTab != IOData_tabPage )
			 {
				 AllIODevice_listBox->Items->Clear();
				 TargetIO_listBox->Items->Clear();
				 SimulationIOList_listBox->Items->Clear();
				 for each(System::Object^l_p in AllIO_listBox->Items)
				 {
					 AllIODevice_listBox->Items->Add(l_p);
					 TargetIO_listBox->Items->Add(l_p);
					 SimulationIOList_listBox->Items->Add(l_p);
				 }
			 }
			 if( DeviceData_tabPage != IOSimulation_tabControl->SelectedTab )
			 {
				this->timer2->Enabled = false;
			 }
			 if( Simulation_tabPage != IOSimulation_tabControl->SelectedTab )
			 {
				this->timer3->Enabled = false;
			 }
			 else
			 {
				if(g_pIOSMIOManager)
				{
					SimulationIOStatus_listBox->Items->Clear();
					int	l_iNum = g_pIOSMIOManager->Count();
					for( int i=0;i<l_iNum;++i )
					{
						cIOSMBaseDevice*l_pIOSMBaseDevice = g_pIOSMIOManager->GetObject(i);
						bool	l_bResult = false;
						if(l_pIOSMBaseDevice->GetIOData()->ForceReadIO(&l_bResult))
						{
							SimulationIOStatus_listBox->Items->Add(l_bResult?"1":"0");
						}
						else
							SimulationIOStatus_listBox->Items->Add("ReadFailed");
					}
					this->m_pTimeAndFPS->Update();
				}
			 }
		 }

private: System::Void Delete_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( AllIO_listBox->SelectedIndex != -1 )
			 {
				 std::wstring	l_strSelectedName = g_pIOSMDriverInterface->GetObject(AllIO_listBox->SelectedIndex)->GetName();
				 g_pIOSMIOManager->RemoveObject(l_strSelectedName.c_str());
				 g_pIOSMDriverInterface->RemoveObject(l_strSelectedName.c_str());
				 AllIO_listBox->Items->RemoveAt(AllIO_listBox->SelectedIndex);
			 }
		 }

private: System::Void CurrentStatus_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {

		 }
private: System::Void AddBehavior_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( sender == DelBehavior_button )
			 {
				 if( g_pIOSMBaseDevice == 0 )
				 {
					 WARNING_MSG("please select a IO");
					 return;
				 }
				 int l_iSelectedIndex = UsedBehavior_listBox->SelectedIndex;
				 if( l_iSelectedIndex != -1 )
				 {
					WCHAR*l_strTypeName = DNCT::GcStringToWchar(UsedBehavior_listBox->SelectedItem->ToString());
					g_pIOSMBaseDevice->RemoveObject(l_iSelectedIndex);
					int	l_iIndex = AllBehavior_listBox->Items->IndexOf(UsedBehavior_listBox->SelectedItem->ToString());
					String^l_strPanel = DNCT::WcharToGcstring((WCHAR*)g_strAllBehaviorName[l_iIndex])+"_panel";
					m_pAllDeviceStatusUI->flowLayoutPanel1->Controls[l_strPanel]->Visible = false;
					UsedBehavior_listBox->Items->RemoveAt(UsedBehavior_listBox->SelectedIndex);
				 }
			 }
			 else
			 {
				 if( g_pIOSMBaseDevice == 0 )
				 {
					 WARNING_MSG("please select a IO");
					 return;
				 }
				 int l_iSelectedIndex = AllBehavior_listBox->SelectedIndex;
				 if( l_iSelectedIndex > 0 )
				 {
					 if( l_iSelectedIndex>=g_iAllBehaviorType )
					 {
						 WARNING_MSG("over all behavior list");
						 return;
					 }
					 String^l_strPanel = DNCT::WcharToGcstring((WCHAR*)g_strAllBehaviorName[l_iSelectedIndex])+"_panel";
					 if(!DNCT::CheckListContainStringAndAdd(UsedBehavior_listBox,AllBehavior_listBox->SelectedItem->ToString()))
					 {
						WCHAR*l_strSelectedItemName = DNCT::GcStringToWchar(AllBehavior_listBox->SelectedItem->ToString());
						if( !g_pIOSMBaseDevice->GetObject(l_strSelectedItemName) )
						{
							cIOSMDeviceBehavior*l_pIOSMDeviceBehavior = 0;
							switch(l_iSelectedIndex)
							{
								case 1:
									l_pIOSMDeviceBehavior = new cProbableIO(g_pIOSMBaseDevice);
									g_pProbableIO = (cProbableIO*)l_pIOSMDeviceBehavior;
									break;
								case 2:
									if(!cActiveIOTimesWithSingnalCircle::IsIODataLegal(g_pIOSMBaseDevice->GetIOData()))
									{
										WARNING_MSG("this IO low and high must both not zero or infinity");
										UsedBehavior_listBox->Items->Remove(AllBehavior_listBox->SelectedItem->ToString());
										return;
									}
									l_pIOSMDeviceBehavior = new cActiveIOTimesWithSingnalCircle(g_pIOSMBaseDevice);
									g_pActiveIOTimesWithSingnalCircle = (cActiveIOTimesWithSingnalCircle*)l_pIOSMDeviceBehavior;
									break;
								case 3:
									l_pIOSMDeviceBehavior = new cListenMultiSingnalToActiveIO(g_pIOSMBaseDevice);
									g_pListenMultiSingnalToActiveIO = (cListenMultiSingnalToActiveIO*)l_pIOSMDeviceBehavior;
									break;
								case 4:
									l_pIOSMDeviceBehavior = new cSendHighSingnal(g_pIOSMBaseDevice);
									g_pSendHighSingnal = (cSendHighSingnal*)l_pIOSMDeviceBehavior;
									break;
								case 5:
									l_pIOSMDeviceBehavior = new cSendLowSingnal(g_pIOSMBaseDevice);
									g_pSendLowSingnal = (cSendLowSingnal*)l_pIOSMDeviceBehavior;
									break;
							}
							assert(l_pIOSMDeviceBehavior&&"not such behavior");
							l_pIOSMDeviceBehavior->SetName(l_pIOSMDeviceBehavior->Type());
							this->m_pAllDeviceStatusUI->ClearBehaviorData(l_pIOSMDeviceBehavior->Type());
							bool	l_b = g_pIOSMBaseDevice->AddObject(l_pIOSMDeviceBehavior);
							assert(l_b);
						}
						m_pAllDeviceStatusUI->flowLayoutPanel1->Controls[l_strPanel]->Visible = true;
					 }
				 }
			 }
		 }

private: System::Void AllIODevice_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( AllIODevice_listBox->SelectedIndex != -1 )
			 {
				 for each(GCFORM::Control^l_pControl in m_pAllDeviceStatusUI->flowLayoutPanel1->Controls)
				 {
					 l_pControl->Visible = false;
				 }
				 UsedBehavior_listBox->Items->Clear();
				 WCHAR*l_strIODataName = DNCT::GcStringToWchar(AllIODevice_listBox->SelectedItem->ToString());
				 cIOSMBaseDevice*l_pIOSMBaseDevice = g_pIOSMIOManager->GetObject(l_strIODataName);
				 //ClearUIData();
				 for( int i=0;i<l_pIOSMBaseDevice->Count();++i )
				 {
					 String^l_strPanel = DNCT::WcharToGcstring((WCHAR*)l_pIOSMBaseDevice->GetObject(i)->Type())+"_panel";
					 int l_iIndex = GetDeviceBehaviorTypeIDIndex(l_pIOSMBaseDevice->GetObject(i)->Type());
					 if(!DNCT::CheckListContainStringAndAdd(UsedBehavior_listBox,AllBehavior_listBox->Items[l_iIndex]->ToString()))
					 {
						m_pAllDeviceStatusUI->flowLayoutPanel1->Controls[l_strPanel]->Visible = true;
					 }
				 }
				 g_pIOSMBaseDevice = l_pIOSMBaseDevice;
				 m_pAllDeviceStatusUI->AssignData(l_pIOSMBaseDevice);
			 }
			 else
				 g_pIOSMBaseDevice = 0;
		 }
private: System::Void saveDeviceToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			if( sender == saveDeviceToolStripMenuItem )
			{
				String^l_strFileName = DNCT::SaveFileAndGetName();
				g_pIOSMIOManager->Export(DNCT::GcStringToChar(l_strFileName));
			}
			else
			if( sender == openDeviceDataToolStripMenuItem )
			{
				ClearIOData();
				ClearUIData();
				String^l_strFileName = DNCT::OpenFileAndGetName("xml files (*.xml)|*.xml|All files (*.*)|*.*");
				if( l_strFileName )
				{
					bool	l_b = g_pIOSMIOManager->Parse(DNCT::GcStringToChar(l_strFileName));
					if( l_b )
					{
						for( int i=0;i<g_pIOSMDriverInterface->Count();++i )
						{
							cIOSMIOData*l_pIOSMIOData = g_pIOSMDriverInterface->GetObject(i);
							AllIO_listBox->Items->Add(DNCT::WcharToGcstring(l_pIOSMIOData->GetName()));
						}
					}
				}
			}
		 }
private: System::Void timer2_Tick(System::Object^  sender, System::EventArgs^  e)
		 {
			 m_pTimeAndFPS->Update();
			 g_pIOSMIOManager->Update(m_pTimeAndFPS->fElpaseTime);
		 }
private: System::Void GoUpdate_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( sender == GoOff_button )
			 {
				 this->timer2->Enabled = false;
			 }
			 else
			 {
				 m_pTimeAndFPS->Update();
				this->timer2->Enabled = true;
			 }
		 }

private: System::Void Write_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			if( !IsHexString(Adress_textBox->Text) )
			{
				WARNING_MSG("inlegal value!!");
				return;			
			}
			if( !IsHexString(ValueToWrite_textBox->Text) )
			{
				WARNING_MSG("inlegal value!!");
				return;			
			}
			int	l_iAddress = UT::HexadecimalStringToDecimal(DNCT::GcStringToChar(Adress_textBox->Text));
			int	l_iSzie = (int)SizeForIO_numericUpDown->Value;
			int	l_iValueToWrite = UT::HexadecimalStringToDecimal(DNCT::GcStringToChar(ValueToWrite_textBox->Text));
			char	l_str[65536];
			IOStatus_textBox->Text = "";
			if( sender == Read_button )
			{
				if(g_pIOSMDriverInterface->ReadIORange(l_iAddress,l_iSzie,l_str))
				{
					if( l_iSzie > 50 )
						l_iSzie = 50;
					for( int i=0;i<l_iSzie;++i )
					{
						IOStatus_textBox->Text += l_str[i].ToString()+DNCT::GetChanglineString();
					}
				}
			}
			else
			if( sender == Write_button )
			{
				if( l_iSzie == 1 )//one byte
				{
					g_pIOSMDriverInterface->WriteIOChar(l_iAddress,l_iValueToWrite);
					IOStatus_textBox->Text = "Write a char " +l_iValueToWrite.ToString()+ " OK!";
				}
				else
				if( l_iSzie <= 4 )//ont int
				{
					g_pIOSMDriverInterface->WriteIO4ByteOnce(l_iAddress,l_iValueToWrite);
					IOStatus_textBox->Text = "Write a int " +l_iValueToWrite.ToString()+ " OK!";
				}
				else
				{
					unsigned char	l_cValue = (unsigned char)l_iValueToWrite;
					char	l_cData[65536];
					memset(l_cData,l_cValue,sizeof(char)*l_iSzie);
					g_pIOSMDriverInterface->WriteIORange(l_iAddress,l_cData,l_iSzie);
					IOStatus_textBox->Text = "Write " +l_iValueToWrite.ToString()+"to target fregment io OK!";
				}
			}
		 }

private: System::Void On_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			int	l_iValue = System::Int32::Parse(Adress_textBox->Text);
			//if( !IsHexString(Adress_textBox->Text) || m_iBitAddress == -1 )
			//{
			//	WARNING_MSG("inlegal value!!");
			//	return;
			//}

			//int	l_iAddress = UT::HexadecimalStringToDecimal(DNCT::GcStringToChar(Adress_textBox->Text));
			int	l_iAddress  = l_iValue;
			if( sender == WriteIO_button )
			{
				bool	l_b;
				printf("WriteIOBitAddress\n");
				if( IOType_comboBox->SelectedIndex == 1 )
				{
					if(g_pIOSMDriverInterface->WriteOutputData(l_iAddress,WorkingSingnal_checkBox->Checked))
					{
						if(g_pIOSMDriverInterface->ReadOutputData(l_iAddress,&l_b))
						{
							IOStatus_textBox->Text = l_b?"1":"0";
						}					
					}
				}
				else
				{
					if(g_pIOSMDriverInterface->WriteInputData(l_iAddress,WorkingSingnal_checkBox->Checked))
					{
						printf("ReadIOBitAddress\n");
						if(g_pIOSMDriverInterface->ReadInputData(l_iAddress,&l_b))
						{
							IOStatus_textBox->Text = l_b?"1":"0";
						}
					}
				}
			}
			else
			if( sender == ReadIO_button )
			{
				bool	l_b;
				if( IOType_comboBox->SelectedIndex == 1 )
				{
					if(g_pIOSMDriverInterface->ReadInputData(l_iAddress,&l_b))
					{
						IOStatus_textBox->Text = l_b?"1":"0";
					}
					else
						IOStatus_textBox->Text = "read filaed";
				}
				else
				{
					if(g_pIOSMDriverInterface->ReadOutputData(l_iAddress,&l_b))
					{
					
					}
					else
						IOStatus_textBox->Text = "read filaed";
				}
			}
		 }

private: System::Void BitAddress_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			m_iBitAddress = (int)BitAddress_numericUpDown->Value;
		 }

private: System::Void timer3_Tick(System::Object^  sender, System::EventArgs^  e)
		 {
			if( g_pIOSMIOManager )
			{
				this->m_pTimeAndFPS->Update();
				g_pIOSMIOManager->Update(m_pTimeAndFPS->fElpaseTime);
				int	l_iNum = g_pIOSMIOManager->Count();
				for( int i=0;i<l_iNum;++i )
				{
					cIOSMBaseDevice*l_pIOSMBaseDevice = g_pIOSMIOManager->GetObject(i);
					bool	l_bResult = false;
					if(l_pIOSMBaseDevice->GetIOData()->ForceReadIO(&l_bResult))
					{
						SimulationIOStatus_listBox->Items[i] = l_bResult?String("1").ToString():String("0").ToString();
					}
					else
						SimulationIOStatus_listBox->Items[i] = String("ReadFailed").ToString();
				}
			}
		 }
private: System::Void Simulation_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 timer3->Enabled = Simulation_checkBox->Checked;
			 if( timer3->Enabled )
			 {
				if(g_pIOSMDriverInterface)
				{
					int	l_iNum = g_pIOSMDriverInterface->Count();
					for(int i=0;i<l_iNum;++i)
					{
						(*g_pIOSMDriverInterface)[i]->Init();
					}
				}
			 }

		 }
private: System::Void timer4_Tick(System::Object^  sender, System::EventArgs^  e)
		 {
			 m_pTimeAndFPSForUetex->Update();
			 m_pTimeCounterForUetex->Update(m_pTimeAndFPSForUetex->fElpaseTime);
			 if( m_pTimeCounterForUetex->bTragetTimrReached )
			 {
				timer4->Enabled = false;
				Mutex_listBox->Items->Add("End");
			 }
			 else
			 {
				 char	l_cValue = (char)m_iWriteValueForMutex;
				 char	l_cData[65536];
				 memset(l_cData,l_cValue,sizeof(char)*m_iWriteSizeForMutex);
				 g_pIOSMDriverInterface->WriteIORange(m_iAddressForMutex,l_cData,m_iWriteSizeForMutex);
				 g_pIOSMDriverInterface->ReadIORange(m_iAddressForMutex,m_iWriteSizeForMutex,l_cData);
				 for( int i=1;i<m_iWriteSizeForMutex-1;++i )
				 {
					if( l_cData[i] != l_cData[0] )
					{
						Mutex_listBox->Items->Add(l_cData[i-1].ToString());
						Mutex_listBox->Items->Add(l_cData[i].ToString());
						Mutex_listBox->Items->Add(l_cData[i+1].ToString());
						return;
					}
				 }
			 }
		 }

private: System::Void MutexTest_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 Mutex_listBox->Items->Clear();
			 m_iWriteValueForMutex = 10;
			 m_iWriteSizeForMutex = 10;
			 int	l_iTime = 10;
			 try
			 {
				m_iWriteValueForMutex = System::Int32::Parse(MutexValueToWrite__textBox->Text);
				m_iWriteSizeForMutex = System::Int32::Parse(MutexWriteSize_textBox->Text);
				l_iTime = System::Int32::Parse(MutexTime_textBox->Text);
			 }
			 catch(System::Exception^l_pExp)
			 {
				WARNING_MSG(l_pExp->ToString());
				m_iWriteValueForMutex = 10;
				m_iWriteSizeForMutex = 10;
				l_iTime = 10;
				return;
			 }
			 if( MutexAddress_textBox->Text->Length == 4 )
			 {
				 m_iAddressForMutex = UT::HexadecimalStringToDecimal(DNCT::GcStringToChar(MutexAddress_textBox->Text));
				 m_pTimeAndFPSForUetex->Update();
				 m_pTimeCounterForUetex->SetTargetTime((float)l_iTime);
				 m_pTimeCounterForUetex->Start();
				 timer4->Enabled = true;
			 }
			 else
			 {
				WARNING_MSG("Address error");
			 }
		 }
private: System::Void PinStatus_timer_Tick(System::Object^  sender, System::EventArgs^  e)
		 {
			 for( int i=0;i<g_ucOutCount;++i )
			 {
				 bool l_bValue = false;
				 if(g_pIOSMDriverInterface->ReadOutputData(i,&l_bValue))
				 {
					 String^l_str = "Index:"+i.ToString()+":  ";
					 l_str += l_bValue?"1":"0";
					 OutputStatus_listBox->Items[i] = l_str;
				 }
				 else
					 OutputStatus_listBox->Items[i] = String("read failed").ToString();
			 }
			 for( int i=0;i<g_ucInputCount;++i )
			 {
				 bool l_bValue = false;
				 if(g_pIOSMDriverInterface->ReadInputData(i,&l_bValue))
				 {
					 String^l_str = "Index:"+i.ToString()+":  ";
					 l_str += l_bValue?"1":"0";
					 Input_listBox->Items[i] = l_str;
				 }
				 else
					 Input_listBox->Items[i] = String("read failed").ToString();
			 }

		 }
private: System::Void AllPinTo1_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 for( int i=0;i<g_ucOutCount;++i )
				 g_pIOSMDriverInterface->WriteOutputData(i,true);
		 }
private: System::Void AllPinTo0_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 for( int i=0;i<g_ucOutCount;++i )
				g_pIOSMDriverInterface->WriteOutputData(i,false);
		 }
private: System::Void AllInputTo1button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 for( int i=0;i<g_ucInputCount;++i )
				 g_pIOSMDriverInterface->WriteInputData(i,true);
		 }
private: System::Void AllInputTo0button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 for( int i=0;i<g_ucInputCount;++i )
				g_pIOSMDriverInterface->WriteInputData(i,true);
		 }
private: System::Void comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {

		 }
private: System::Void KeyData_textBox_TextChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(KeyData_textBox->Text->Length >1)
				 KeyData_textBox->Text = KeyData_textBox->Text->Substring(0,1);
		 }
};
//end namespace
}