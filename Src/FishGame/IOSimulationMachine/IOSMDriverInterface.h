#ifndef _IOCD_DRIVER_INTERFACE_H
#define _IOCD_DRIVER_INTERFACE_H

#include "IOSMIOData.h"
namespace	IOSM
{
	class	cIOSMIOManager;
	//%x for hex value to decimal
	class	cIOSMDriverInterface:public cIOSMIODataManager
	{
		bool	m_bIOError;
		friend class cIOSMIOManager;
		void	Destroy();
	public:
		cIOSMDriverInterface();
		virtual ~cIOSMDriverInterface();
		//
		void	Init();
		//JMGB
		//void	Init(GUID e_GUID,int e_iStartAddress,int e_iDataLength);
		//for driver name.
		std::vector<std::wstring>	*GetAllDeviceNameVector();
		//string convert to hex
		//sscanf(szBuff, "%x", &m_MemOffset);
		//read a bit
		bool	ReadIOBitAddress(int e_iAddress,int e_iBitAddress,bool*e_pbValue);
		//read a character
		bool	ReadIOChar(int e_iAddress,char*e_pcValue);
		//read specific range
		bool	ReadIORange(int e_iAddress,int e_iSize,char*e_pcOutData);
		//write a bit
		bool	WriteIOBitAddress(int e_iAddress,int e_iBitAddress,bool e_bOn);
		//write a character
		bool	WriteIOChar(int e_iAddress,char e_cValue);
		//write a specific range
		bool	WriteIORange(int e_iAddress,char* e_cValue,int e_iSize);
		//
		bool	WriteIO4ByteOnce(int e_iAddress,int e_iValue);
		//
		bool	ReadIO4ByteOnce(int e_iAddress,int*e_piValue);

		//force to get set input data and output data
		bool	ReadInputData(int e_iAddress,bool*e_pbData);
		bool	ReadOutputData(int e_iAddress,bool*e_pbData);
		bool	WriteInputData(int e_iAddress,bool e_bOn);
		bool	WriteOutputData(int e_iAddress,bool e_bOn);
		//
		//=================================PIO==========================================

		//bool	ReadIOBitAddressByPIO(int e_iAddress,int e_iBitAddress,bool*e_pbValue);
		////read a character
		//bool	ReadIOCharByPIO(int e_iAddress,char*e_pcValue);
		////read specific range
		//bool	ReadIORangeByPIO(int e_iAddress,int e_iSize,char*e_pcOutData);
		////write a bit
		//bool	WriteIOBitAddressByPIO(int e_iAddress,int e_iBitAddress,bool e_bOn);
		////write a character
		//bool	WriteIOCharByPIO(int e_iAddress,char e_cValue);
		////write a specific range
		//bool	WriteIORangeByPIO(int e_iAddress,char* e_cValue,int e_iSize);
		////
		//bool	WriteIO4ByteOnceByPIO(int e_iAddress,int e_iValue);
		////
		//bool	ReadIO4ByteOnceByPIO(int e_iAddress,int*e_piValue);

		//update current IOData status
		virtual	void	Update(float e_fElpaseTime);
		virtual	void	Render();
		void	WriteGameKey();
		void	WriteInputIOSetup();
		void	Clear(){ cIOSMIODataManager::Destroy(); }
		bool	IsIOError(){return m_bIOError;}
		void	SetIOError(bool e_bIOError){m_bIOError = e_bIOError;}
	};
	extern cIOSMDriverInterface*g_pIOSMDriverInterface;
	//if it's simulation not on ral machine,we have to setup some random parameter let simulation more relaible
	extern bool	g_bSimulation;
//end namespace
}

#endif