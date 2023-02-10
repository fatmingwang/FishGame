#include "stdafx.h"
#include "IOSMBaseIO.h"
#include "IOSMManager.h"
#include "IOSMIOMachine.h"

namespace IOSM
{
	cIOSMBaseIO::cIOSMBaseIO(cIOSMIOMachine*e_pOwner)
	{
		m_pOwner = e_pOwner;
		m_bOn = false;
		m_dwMemoryAddress = 0;
		m_byBitAddress = 0;
		m_fBounce = 0.f;
	}

	cIOSMBaseIO::cIOSMBaseIO(cIOSMBaseIO*e_pIOSMBaseIO)
	{
		WriteData(e_pIOSMBaseIO);
	}

	WCHAR* cIOSMBaseIO::GetDeviceInfo()
	{
		static WCHAR	l_strInfo[MAX_PATH];
		swprintf(l_strInfo,MAX_PATH,L"Name:%ls,On:%d,BitAddress:%d,MemoryAdress:%d,Bounce:%f\0",GetName(),m_bOn?1:0,(int)m_byBitAddress,(int)m_dwMemoryAddress,this->m_fBounce);
		return l_strInfo;
	}

	WCHAR* cIOSMBaseIO::GetDeviceData()
	{
		return 0;
	}

	void	cIOSMBaseIO::WriteData(cIOSMBaseIO*e_pIOSMBaseIO)
	{
		this->m_bOn = e_pIOSMBaseIO->m_bOn;
		this->m_byBitAddress = e_pIOSMBaseIO->m_byBitAddress;
		this->m_dwMemoryAddress = e_pIOSMBaseIO->m_dwMemoryAddress;
		this->m_fBounce = e_pIOSMBaseIO->m_fBounce;
	}

//end namespace
}