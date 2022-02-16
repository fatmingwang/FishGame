#include "StdAfx.h"
#include "IODataEditor.h"

namespace IODevice
{

	//System::Void	ClearFormData()
	//{
	//	StartStatus_checkBox->Checked = false;
	//	BitAddress_numericUpDown->Value = 0;
	//	SetStartSingnal_checkBox->Checked = false;
	//	Adress_textBox->Text = "";
	//	Name_textBox->Text = "";
	//}

	System::Void	Form1::ReEditIOData()
	{
		 int	l_iSelectedIndex = AllIO_listBox->SelectedIndex;
		 if ( l_iSelectedIndex != -1 )
		 {
			cIOSMIOData*l_pIOSMIOData = g_pIOSMDriverInterface->GetObject(l_iSelectedIndex);
			BYTE l_cBitAddress = l_pIOSMIOData->GetBitAddress();
			int	l_iMemoryAddress = l_pIOSMIOData->GetMemoryAddress();
			WCHAR*	l_strName = l_pIOSMIOData->GetName();
			bool	l_bSetStartStatus = l_pIOSMIOData->IsSetStartSingnal();
			bool	l_bStartSingnal = l_pIOSMIOData->IsSetStartSingnal();

			WorkingSingnal_checkBox->Checked = l_pIOSMIOData->IsWorkingSingnal();
			StartStatus_checkBox->Checked = l_bStartSingnal;
			BitAddress_numericUpDown->Value = l_cBitAddress;
			SetStartSingnal_checkBox->Checked = l_bSetStartStatus;
			High_numericUpDown->Value = (int)(l_pIOSMIOData->GetHighCircle()*1000);
			Low_numericUpDown->Value = (int)(l_pIOSMIOData->GetLowCircle()*1000);
			ActiveDelay_numericUpDown->Value = (int)(l_pIOSMIOData->GetDelayTimeForActive()*1000);
			InactiveDelay_numericUpDown->Value = (int)(l_pIOSMIOData->GetDelayTimeForInactive()*1000);
			this->KeyData_textBox->Text = "";
			if( l_pIOSMIOData->GetKeyData() )
			{
				char	l_str[2] = {*l_pIOSMIOData->GetKeyData(),'\n'};
				
				//unsigned char	l_Data = *l_pIOSMIOData->GetKeyData();
				this->KeyData_textBox->Text = String(l_str).ToString();
			}
			if(l_pIOSMIOData->GetKeyData())
			{
			
			}
			StartEnable_checkBox->Checked = l_pIOSMIOData->IsEnable();
			char l_cAddress[100];
			sprintf(l_cAddress, "%04X", l_iMemoryAddress);
			Adress_textBox->Text = String(l_cAddress).ToString();
			Name_textBox->Text = DNCT::WcharToGcstring(l_strName);
		 }
	}

	System::Void	Form1::ClearUIData()
	{
		UsedBehavior_listBox->Items->Clear();
		StartStatus_checkBox->Checked = false;
		BitAddress_numericUpDown->Value = 0;
		SetStartSingnal_checkBox->Checked = false;
		Adress_textBox->Text = "";
		Name_textBox->Text = "";
		AllIO_listBox->Items->Clear();
		g_pIOSMBaseDevice = 0;
		g_pProbableIO = 0;
		g_pActiveIOTimesWithSingnalCircle = 0;
		g_pListenMultiSingnalToActiveIO = 0;
		for each(GCFORM::Control^l_pControl in m_pAllDeviceStatusUI->flowLayoutPanel1->Controls)
		{
			l_pControl->Visible = false;
		}	
	}

	System::Void	Form1::ClearIOData()
	{
		//clear io data
		g_pIOSMIOManager->Destory();
		g_pIOSMDriverInterface->Clear();
		AllIODevice_listBox->Items->Clear();
		TargetIO_listBox->Items->Clear();
	}

	bool	Form1::IsHexString(String^e_str)
	{
		for( int i=0;i<e_str->Length;++i )
		{
			WCHAR	l_c = e_str[i];
			if( !((l_c>=L'0'&&l_c<=L'9')||
				(l_c>=L'a'&&l_c<=L'f')||
				(l_c>=L'A'&&l_c<=L'F')))
			{
				return false;
			}
		}
		return true;
	}
//end namespace IODevice
}