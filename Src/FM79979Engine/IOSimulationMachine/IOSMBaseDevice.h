#ifndef _IOSM_BASE_IO_H_
#define _IOSM_BASE_IO_H_

#include "IOSMIOData.h"

namespace IOSM
{
	class	cIOSMIOManager;
	class	cIOSMDeviceBehavior;
	//========================================
	//after set signal is on,all cIOSMDeviceBehavior member will do their things,
	//
	//========================================
	class cIOSMBaseDevice:public cNamedTypedObjectVector<cIOSMDeviceBehavior>
	{
		friend class cIOSMDeviceBehavior;
	protected:
//		cIOSMIOMachine*m_pOwner;
		cIOSMIOData*m_pIOSMIOData;
	public:
		//cIOSMBaseIO(cIOSMIOMachine*e_pOwner,cIOSMIOData*e_pIOSMIOData);
		cIOSMBaseDevice(cIOSMIOData*e_pIOSMIOData);
		//for clone
		cIOSMBaseDevice(cIOSMBaseDevice*e_pIOSMBaseDevice);
		//to get start IO data
		virtual	void	Init();
		//
		virtual void	Update(float e_ElpaseTime);
		bool	AddActiveIO(cIOSMIOData*e_pIOSMIOData);
		bool	RemoveActiveIO(cIOSMIOData*e_pIOSMIOData);
		//
		void	SetSingnal(cIOSMIOData*e_cIOSMIOData,bool e_bOn);
		bool	IsActive(){ return this->m_pIOSMIOData->IsActive(); }
		
		cIOSMIOData*GetIOData(){return m_pIOSMIOData;}
	};




	//class	cxxx:public cDeviceBase
	//{
	//
	//	virtual WCHAR* GetDeviceInfo()
	//	{
	//		WCHAR*l_strBaseInfo = cDeviceBase::GetDeviceInfo();
	//		WCHAR	l_strInfo[MAX_PATH];
	//		//swprintf(l_strInfo,MAX_PATH,L"%ls,",l_strBaseInfo);
	//		return l_strInfo;
	//	}
	//
	//};

//end namespace
}
#endif