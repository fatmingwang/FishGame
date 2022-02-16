#include "stdafx.h"
#include "IOSMDeviceBehavior.h"
#include "IOSMBaseDevice.h"
#include "IOSMManager.h"
//#include "IOSMIOMachine.h"

namespace IOSM
{
	cIOSMBaseDevice::cIOSMBaseDevice(cIOSMIOData*e_pIOSMIOData)
	{
		//m_pOwner = e_pOwner;
		m_pIOSMIOData = e_pIOSMIOData;
		this->SetName(m_pIOSMIOData->GetName());
	}

	cIOSMBaseDevice::cIOSMBaseDevice(cIOSMBaseDevice*e_pIOSMBaseDevice)
	{

	}

	void	cIOSMBaseDevice::SetSingnal(cIOSMIOData*e_cIOSMIOData,bool e_bOn)
	{
		int	l_iNum = this->Count();
		for( int i=0;i<l_iNum;++i )
		{
			(*this)[i]->SetSingnal(e_cIOSMIOData,e_bOn);
		}
	}

	void	cIOSMBaseDevice::Init()
	{
		this->m_pIOSMIOData->Init();
	}

	void	cIOSMBaseDevice::Update(float e_ElpaseTime)
	{
		int	l_iNum = this->Count();
		if( l_iNum )
		{
			cIOSMDeviceBehavior**l_ppIOSMDeviceBehavior = &this->m_ObjectList[0];
			for( int i=0;i<l_iNum;++i )
			{
				l_ppIOSMDeviceBehavior[i]->Update(e_ElpaseTime);
			}
		}
		//set IO status back if needed
	}
//end namespace
}