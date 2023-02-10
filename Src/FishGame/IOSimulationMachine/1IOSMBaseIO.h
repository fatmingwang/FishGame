#ifndef _IOSM_BASE_IO_H_
#define _IOSM_BASE_IO_H_

namespace IOSM
{
	class	cIOSMIOManager;
	class	cIOSMIOMachine;

	class cIOSMBaseIO:public NamedTypedObject
	{
	protected:
		GET_SET_DEC( bool, m_bOn, IsOn, SetOn );
		GET_SET_DEC( DWORD, m_dwMemoryAddress, GetMemoryAddress, SetMemoryAddress );
		GET_SET_DEC( BYTE, m_byBitAddress, GetBitAddress, SetBitAddress );
		GET_SET_DEC( float, m_fBounce, GetBounce, SetBounce );
		cIOSMIOMachine*m_pOwner;
	public:
		cIOSMBaseIO(cIOSMIOMachine*e_pOwner);
		//for clone
		cIOSMBaseIO(cIOSMBaseIO*e_pIOSMBaseIO);

		virtual void Update(float e_ElpaseTime) = 0;
		virtual WCHAR* GetDeviceInfo();
		virtual WCHAR* GetDeviceData();
		virtual	void	WriteData(cIOSMBaseIO*e_pIOSMBaseIO);
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