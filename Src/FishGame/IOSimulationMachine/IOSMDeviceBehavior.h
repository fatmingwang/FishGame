#ifndef _DEVICE_BEHAVIOR_H
#define _DEVICE_BEHAVIOR_H

#include "IOSMIOData.h"
//
//times and singnal circle should be merge to one behavior.
namespace	IOSM
{
	class	cIOSMBaseDevice;
	//some behavior must immediately to know on or off(SetOn),
	//and set it as the status aswe expected,some only need to get on or off in the update,
	//so ensure the setOn event is correctly overwrite
	//
	class	cIOSMDeviceBehavior:public NamedTypedObject
	{
	protected:
		//if singnal is off it's true or false,so we could know is it time to active target IO
		bool	m_bActived;
		cIOSMIOData*m_pIOSMIOData;
		cIOSMBaseDevice*m_pIOSMBaseDevice;
	public:
		//for the singnal to send
		bool	m_bSendSingnal;
		DEFINE_TYPE_INFO();
		cIOSMDeviceBehavior(cIOSMBaseDevice*e_pIOSMBaseDevice);
		virtual ~cIOSMDeviceBehavior();
		virtual	void	Update(float e_fElpaseTime) = 0;
		void	SetSingnal(cIOSMIOData*e_cIOSMIOData,bool e_bOn){}
	};

	//if m_fProbability is 100,
	//m_ProbableIOList.Count()/100;
	//one of them will be actived,or it is possible to non-active
	class	cProbableIO:public cIOSMDeviceBehavior
	{
	public:
		DEFINE_TYPE_INFO();
		cNamedTypedObjectVector<cIOSMIOData>	m_ProbableIOList;
		int	m_iProbability;
		cProbableIO(cIOSMBaseDevice*e_pIOSMBaseDevice);
		virtual ~cProbableIO();
		virtual	void	Update(float e_fElpaseTime);
	};

	class	cActiveIOTimesWithSingnalCircle:public cIOSMDeviceBehavior
	{
		//if IO's singnal is not same,m_iRestTimeToCount will minus one,if m_iRestTimeToCount is zero,condition is full.
		bool	m_bCurrentIOSingnal;
		//respond to m_iTimes
		int		m_iRestTimeToCount;
		//if target is actived,start do counting,or check is it to start
		bool	m_bStartToCount;
	public:
		DEFINE_TYPE_INFO();
		int		m_iTimes;
		cActiveIOTimesWithSingnalCircle(cIOSMBaseDevice*e_pIOSMBaseDevice);
		virtual ~cActiveIOTimesWithSingnalCircle();
		virtual	void	Update(float e_fElpaseTime);
		static bool	IsIODataLegal(cIOSMIOData*e_pIOSMIOData){ if( e_pIOSMIOData->GetLowCircle()>0.f && e_pIOSMIOData->GetHighCircle()>0.f)return true;return false; }
	};

	//delay and multi active has complex situation,the upate sort is important
	//it will has different result if piority is different.
	//after all active IO list is sent singnal,the target IO will be set enable as true
	//if sent singnal is not working singnal it will set enable as false
	class	cListenMultiSingnalToActiveIO:public cIOSMDeviceBehavior
	{
		cNamedTypedObjectVector<cIOSMIOData>	m_WaitLineIOList;
		bool	m_bListenSingnal;
	public:
		DEFINE_TYPE_INFO();
		cNamedTypedObjectVector<cIOSMIOData>	m_ActiveIOList;
		cListenMultiSingnalToActiveIO(cIOSMBaseDevice*e_pIOSMBaseDevice);
		virtual ~cListenMultiSingnalToActiveIO();
		virtual	void	Update(float e_fElpaseTime);
		void	SetSingnal(cIOSMIOData*e_cIOSMIOData,bool e_bOn);
	};
	//===================
	//very stupid but works
	//===================
	class	cSendHighSingnal:public cIOSMDeviceBehavior
	{
	public:
		DEFINE_TYPE_INFO();
		cNamedTypedObjectVector<cIOSMIOData>	m_ActiveIOList;
		cSendHighSingnal(cIOSMBaseDevice*e_pIOSMBaseDevice);
		virtual ~cSendHighSingnal();
		virtual	void	Update(float e_fElpaseTime);
	};

	class	cSendLowSingnal:public cIOSMDeviceBehavior
	{
	public:
		DEFINE_TYPE_INFO();
		cNamedTypedObjectVector<cIOSMIOData>	m_ActiveIOList;
		cSendLowSingnal(cIOSMBaseDevice*e_pIOSMBaseDevice);
		virtual ~cSendLowSingnal();
		virtual	void	Update(float e_fElpaseTime);
	};
	//in the AllDeviceStatusUI.h
	//the panel name is match this,
	static const WCHAR*g_strAllBehaviorName[] =
	{
			cIOSMDeviceBehavior::TypeID,				//0
			cProbableIO::TypeID,						//1
			cActiveIOTimesWithSingnalCircle::TypeID,	//2
			cListenMultiSingnalToActiveIO::TypeID,		//3
			cSendHighSingnal::TypeID,					//4
			cSendLowSingnal::TypeID,					//5
	};

	static const int g_iAllBehaviorType = 6;
	int	GetDeviceBehaviorTypeIDIndex(const WCHAR*e_strType);
//end namespace IOSM
};

#endif