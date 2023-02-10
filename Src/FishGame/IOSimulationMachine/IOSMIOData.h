#ifndef _IOSM_IO_DATA_H_
#define _IOSM_IO_DATA_H_


namespace IOSM
{
	class cIOSMIODataManager;
	class cIOSMBaseDevice;
	//====================
	//<IOData MemoryAdress="" BitAdress="" Bounce="" />
	//
	//cIOSMBaseDevice will update it's child behavior,then update IOData,so even change IOData's current singnal,it doesn't mean to write IO,
	//it will check by behavior's update,if condition is full,singnal will be changed.
	//====================
	enum	eIODataType
	{
		eIODT_INPUT = 0,
		eIODT_OUTPUT,
	};

	class cIOSMIOData:public NamedTypedObject
	{
		friend	class	cIOSMIODataManager;
		friend	class	cIOSMBaseDevice;
		//to set start singnal
		GET_SET_DEC( bool, m_bSetStartSingnal, IsSetStartSingnal, SetSetStartSingnal );
		GET_SET_DEC( bool, m_bStartSingnal, IsStartSingnal, SetStartSingnal );
		//
		GET_SET_DEC( DWORD, m_dwMemoryAddress, GetMemoryAddress, SetMemoryAddress );
		GET_SET_DEC( BYTE, m_byBitAddress, GetBitAddress, SetBitAddress );
		//if true high circle active,or low active.
		GET_SET_DEC(bool,m_bWorkingSingnal,IsWorkingSingnal,SetWorkingSingnal);
		bool	m_bProviousCurrentSingnale;
		//the data get from driver,but if m_bCurrentSingnale is not match m_bProviousCurrentSingnale,it will write IO and do not read.
		bool	m_bCurrentSingnale;
		//let device call their function
		bool	m_bFirstActive;
		bool	m_bFirstInactive;
		//if target time is 0.it won't switch status,if time is smaller than 0 it's infinety.
		sTimeCounter	m_TimeCounterForHighCircle;
		sTimeCounter	m_TimeCounterForLowCircle;
		//it's not necessary,and it will to determinate by m_bWorkingSingnal and m_bCurrentSingnale.
		//IsActive
		sTimeCounter	m_TimeCounterForActiveDelay;
		sTimeCounter	m_TimeCounterForInactiveDelay;
		//the device use this IO
		cIOSMBaseDevice*m_pIOSMBaseDevice;
		//check IO status
		bool	m_bEnable;
		//
		void	DoHignLowCircle(float e_fElpaseTime);
		//key data
		GET_SET_DEC(unsigned char*,m_pucKeyData,GetKeyData,SetKeyData);
		//
		GET_SET_DEC(eIODataType,m_eIODataType,GeteIODataType,SeteIODataType);
		void		FetchSingnal(bool*e_pSingnal);
	public:
		cIOSMIOData();
		virtual ~cIOSMIOData();
		DEFINE_TYPE_INFO();
		//
		WCHAR* GetDeviceInfo();
		//u have to get,set real memory bit.
		//if true write it into memory
		bool	IsActive(){ return m_bCurrentSingnale==m_bWorkingSingnal?true:false; }
		//although it has been set,most time device will go to check it,to set it true or false.
		//so behavior's piority of deivce is important,delay to on and selay to off must set well.
		void	SetSingnal(bool e_bOn,cIOSMIOData*e_cIOSMIOData = 0);
		//set start status
		void	Init();
		//this update will setup to write data into memory,it'd called in the last of Device's update to ensure it's time to write data
		virtual	void	Update(float e_fElpaseTime);
		//
		bool	GetCurrentSingnal(){ return m_bCurrentSingnale; }
		void	SetCurrentSingnal(bool e_b);
		//
		float	GetLowCircle(){ return m_TimeCounterForLowCircle.fTargetTime; }
		void	SetLowCircle(float e_fValue){ m_TimeCounterForLowCircle.SetTargetTime(e_fValue); }
		float	GetHighCircle(){ return m_TimeCounterForHighCircle.fTargetTime; }
		void	SetHighCircle(float e_fValue){ m_TimeCounterForHighCircle.SetTargetTime(e_fValue); }

		float	GetDelayTimeForActive(){ return m_TimeCounterForActiveDelay.fTargetTime; }
		void	SetDelayTimeForActive(float e_fValue){ m_TimeCounterForActiveDelay.SetTargetTime(e_fValue); }
		float	GetDelayTimeForInactive(){ return m_TimeCounterForInactiveDelay.fTargetTime; }
		void	SetDelayTimeForInactive(float e_fValue){ m_TimeCounterForInactiveDelay.SetTargetTime(e_fValue); }
		//
		bool	WriteIO(bool e_b);
		bool	ReadIO();
		bool	ForceReadIO(bool*e_pbOutData);

		bool	IsEnable(){ return m_bEnable; }
		void	SetEnable(bool e_b);
	};

	//io noly has 1bit(1/0),but some io is a device has more than 1 bit,so we has this now
	//this one only for get data
	//size as byte
	//    <IOPackData Name="Spin" Read="false"	KeyData="S" MemoryAddress="5" Size="1" />
	class cIOSMIOPackData:public cIOSMIOData
	{
		friend	class	cIOSMIODataManager;
		//
		int		m_iDataSize;
		void*	m_pData;
		bool	m_bRead;
	public:
		cIOSMIOPackData();
		virtual ~cIOSMIOPackData();
		DEFINE_TYPE_INFO();
		void			SetDataSize(int	e_iSize);
		virtual	void	Update(float e_fElpaseTime);
		int				GetPackDataByInt();
		unsigned char	GetPackDataByByte();
		bool			SetPackDataByByte(unsigned char	e_iRestCoin);
		void*			GetData(){ return m_pData; }
	};

//<IODataInfoList Count="2">
//    <IOData Name="BetMax" 	KeyData="G" IOType="Input" SetStartSingnal="0" StartSingnal="0" MemoryAddress="6" BitAdress="0" HighCircle="0.000000" LowCircle="0.000000" ActiveDelay="0.000000" InactiveDelay="0.000000" WorkingSingnal="0" Enable="1" />
//    <IOData Name="Spin" 	KeyData="S" IOType="Input" SetStartSingnal="0" StartSingnal="0" MemoryAddress="5" BitAdress="0" HighCircle="0.000000" LowCircle="0.000000" ActiveDelay="0.000000" InactiveDelay="0.000000" WorkingSingnal="0" Enable="1" />
//</IODataInfoList>
	class	cIOSMIODataManager:public cNamedTypedObjectVector<cIOSMIOData>,public ISAXCallback
	{
		int		m_iSafeRefundCoinCount;
		void	ProcessIOData();
		void	ProcessIOPackData();
		virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement);
	public:
		cIOSMIODataManager();
		virtual ~cIOSMIODataManager();
		cIOSMIOData*			GetObjectByKeyData(unsigned char e_ucKey);
		int						GetSafeRefundCoinCount(){return m_iSafeRefundCoinCount;}
#ifdef WIN32
		bool	Export(char*e_strFileName);
#endif
	};

//end namespace IOSM
}


#endif