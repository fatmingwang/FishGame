#include "StdAfx.h"
#include "AllDeviceStatusUI.h"


namespace IODevice
{

	System::Void	AllDeviceStatusUI::ClearProbableIOData()
	{
		ProbableIO_listBox->Items->Clear();
		ProbableIO_numericUpDown->Value = 0;
		cProbableIOSingnal_checkBox->Checked = false;
	}

	System::Void	AllDeviceStatusUI::ClearActiveIOTimesWithSingnalCircleIOData()
	{
		StopAfterTimes_numericUpDown->Value = 0;
	}

	System::Void	AllDeviceStatusUI::ClearcListenMultiSingnalToActiveIOData()
	{
		MultiTriigerIO_listBox->Items->Clear();
		cListenMultiSingnalToActiveIOSingnal_checkBox->Checked = false;
	}

	System::Void	AllDeviceStatusUI::ClearcSendHighSinnalIOData()
	{
		SendHighSingnal_listBox->Items->Clear();
	}

	System::Void	AllDeviceStatusUI::ClearcSendLowSinnalIOData()
	{
		SendLowSingnal_listBox->Items->Clear();
	}

	System::Void	AllDeviceStatusUI::ClearBehaviorData(const WCHAR*e_strType)
	{
		int	l_iIndex = -1;
		for( int i=0;i<g_iAllBehaviorType;++i )
		{
			if( !wcscmp(e_strType,g_strAllBehaviorName[i]) )
			{
				l_iIndex = i;
				break;
			}
		}
		assert(l_iIndex != -1);
		switch(l_iIndex)
		{
			case 0:
				break;
			case 1:
				ClearProbableIOData();
				break;
			case 2:
				ClearActiveIOTimesWithSingnalCircleIOData();
				break;
			case 3:
				ClearcListenMultiSingnalToActiveIOData();
				break;
			case 4:
				ClearcSendHighSinnalIOData();
				break;
			case 5:
				ClearcSendLowSinnalIOData();
				break;
			default:
				WARNING_MSG("No Clear BehaviorData");
				break;
		}
	}

	void	AssignObjectListToListBox(GCFORM::ListBox^e_pListBox,cObjectListByName<cIOSMIOData>*e_pObjectList)
	{
		e_pListBox->Items->Clear();
		int	l_iActiveIO = e_pObjectList->Count();
		for( int j=0;j<l_iActiveIO;++j )
		{
			e_pListBox->Items->Add(DNCT::WcharToGcstring((*e_pObjectList)[j]->GetName()));
		}	
	}

	void	AllDeviceStatusUI::AssignData(cIOSMBaseDevice*e_pIOSMBaseDevice)
	{
		int	l_iNum  = e_pIOSMBaseDevice->Count();
		for( int i=0;i<l_iNum;++i  )
		{
			cIOSMDeviceBehavior*l_pIOSMDeviceBehavior = (*e_pIOSMBaseDevice)[i];
			const WCHAR*l_strType = l_pIOSMDeviceBehavior->Type();
			if( l_strType == cIOSMDeviceBehavior::TypeID )
			{
				
			}
			else
			if( l_strType == cProbableIO::TypeID )
			{
				ProbableIO_listBox->Items->Clear();
				cProbableIO*l_pProbableIO = (cProbableIO*)l_pIOSMDeviceBehavior;
				g_pProbableIO = l_pProbableIO;
				int	l_iNumProbableIO = l_pProbableIO->m_ProbableIOList.Count();
				for( int j=0;j<l_iNumProbableIO;++j )
				{
					ProbableIO_listBox->Items->Add(DNCT::WcharToGcstring(l_pProbableIO->m_ProbableIOList[j]->GetName()));
				}
				ProbableIO_numericUpDown->Value = l_pProbableIO->m_iProbability;
				cProbableIOSingnal_checkBox->Checked = l_pProbableIO->m_bSendSingnal;
			}
			else
			if( l_strType == cActiveIOTimesWithSingnalCircle::TypeID )
			{
				cActiveIOTimesWithSingnalCircle*l_pActiveIOTimesWithSingnalCircle = (cActiveIOTimesWithSingnalCircle*)l_pIOSMDeviceBehavior;
				g_pActiveIOTimesWithSingnalCircle = l_pActiveIOTimesWithSingnalCircle;
				StopAfterTimes_numericUpDown->Value = l_pActiveIOTimesWithSingnalCircle->m_iTimes;
			}
			else
			if( l_strType == cListenMultiSingnalToActiveIO::TypeID )
			{
				cListenMultiSingnalToActiveIO*l_pListenMultiSingnalToActiveIO = (cListenMultiSingnalToActiveIO*)l_pIOSMDeviceBehavior;
				g_pListenMultiSingnalToActiveIO = l_pListenMultiSingnalToActiveIO;
				AssignObjectListToListBox(MultiTriigerIO_listBox,&g_pListenMultiSingnalToActiveIO->m_ActiveIOList);
				cListenMultiSingnalToActiveIOSingnal_checkBox->Checked = l_pListenMultiSingnalToActiveIO->m_bSendSingnal;
			}
			else
			if( l_strType == cSendHighSingnal::TypeID )
			{
				cSendHighSingnal*l_pcSendHighSingnal = (cSendHighSingnal*)l_pIOSMDeviceBehavior;
				g_pSendHighSingnal = l_pcSendHighSingnal;
				AssignObjectListToListBox(SendHighSingnal_listBox,&g_pSendHighSingnal->m_ActiveIOList);
			}
			else
			if( l_strType == cSendLowSingnal::TypeID )
			{
				cSendLowSingnal*l_pcSendLowSingnal = (cSendLowSingnal*)l_pIOSMDeviceBehavior;
				g_pSendLowSingnal = l_pcSendLowSingnal;
				AssignObjectListToListBox(SendLowSingnal_listBox,&g_pSendLowSingnal->m_ActiveIOList);
			}
			else
			{
				UT::ErrorMsg((WCHAR*)l_strType,L"no such behavior data assign to UI");
			}
		}
	}

	void	AllDeviceStatusUI::AssignListData(GCFORM::ListBox^e_pListBox,cObjectListByName<cIOSMIOData>*e_pIOSMIODataList,bool e_bAdd)
	{
		 if( !e_bAdd )
		 {
			 int	l_iSelectedIndex = e_pListBox->SelectedIndex;
			 if( l_iSelectedIndex != -1 )
			 {
				 WCHAR*l_str = DNCT::GcStringToWchar(e_pListBox->SelectedItem->ToString());
				 e_pIOSMIODataList->RemoveObjectWithoutDelete(l_str);
				 e_pListBox->Items->RemoveAt(l_iSelectedIndex);
			 }
		 }
		 else
		 {
			 int	l_iSelectedIndex = m_TargetListBox->SelectedIndex;
			 if( l_iSelectedIndex != -1 )
			 {
				 if(!DNCT::CheckListContainStringAndAdd(e_pListBox,m_TargetListBox->SelectedItem->ToString()))
				 {
					 WCHAR*l_str = DNCT::GcStringToWchar(m_TargetListBox->SelectedItem->ToString());
					 cIOSMIOData*l_pIOSMIOData = g_pIOSMDriverInterface->GetObject(l_str);
					 bool	l_b = e_pIOSMIODataList->AddObject(l_pIOSMIOData);
					 assert(l_b);
				 }
			 }
		 }	
	}
//end namespace IODevice
}