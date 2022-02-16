#include "stdafx.h"
#ifdef EMPTY_DEVICE
#include "IOSMDriverInterface.h"

namespace	IOSM
{
	cIOSMDriverInterface*g_pIOSMDriverInterface = 0;
	//if it's simulation not on ral machine,we have to setup some random parameter let simulation more relaible
	bool	g_bSimulation = true;
	cIOSMDriverInterface::cIOSMDriverInterface(){m_bIOError = false;}
	cIOSMDriverInterface::~cIOSMDriverInterface(){}
	void	cIOSMDriverInterface::Init(){}
	void	cIOSMDriverInterface::Destroy(){}

	//for driver name.
	std::vector<std::wstring>	*cIOSMDriverInterface::GetAllDeviceNameVector(){ return 0; }
	bool	cIOSMDriverInterface::ReadIOBitAddress(int e_iAddress,int e_iBitAddress,bool*e_pbValue){ return true; }
	//read a character
	bool	cIOSMDriverInterface::ReadIOChar(int e_iAddress,char*e_pcValue){return true;}
	//read specific range
	bool	cIOSMDriverInterface::ReadIORange(int e_iAddress,int e_iSize,char*e_pcOutData){return true;}
	//write a bit
	bool	cIOSMDriverInterface::WriteIOBitAddress(int e_iAddress,int e_iBitAddress,bool e_bOn){return true;}
	//write a character
	bool	cIOSMDriverInterface::WriteIOChar(int e_iAddress,char e_cValue){return true;}
	//write a specific range
	bool	cIOSMDriverInterface::WriteIORange(int e_iAddress,char* e_cValue,int e_iSize){return true;}
	//
	bool	cIOSMDriverInterface::WriteIO4ByteOnce(int e_iAddress,int e_iValue){return true;}
	//
	bool	cIOSMDriverInterface::ReadIO4ByteOnce(int e_iAddress,int*e_piValue){return true;}

	//force to get set input data and output data
	bool	cIOSMDriverInterface::ReadInputData(int e_iAddress,bool*e_pbData){return true;}
	bool	cIOSMDriverInterface::ReadOutputData(int e_iAddress,bool*e_pbData){return true;}
	bool	cIOSMDriverInterface::WriteInputData(int e_iAddress,bool e_bOn){return true;}
	bool	cIOSMDriverInterface::WriteOutputData(int e_iAddress,bool e_bOn){return true;}
	void	cIOSMDriverInterface::Update(float e_fElpaseTime){}
	void	cIOSMDriverInterface::Render(){}
	void	cIOSMDriverInterface::WriteGameKey(){}
	void	cIOSMDriverInterface::WriteInputIOSetup(){}
	void	Clear(){  }

}
//end win32
#endif