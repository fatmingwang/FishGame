#include "..\stdafx.h"
#ifdef USING_FPGA
#include "..\IOSMDriverInterface.h"
namespace	IOSM
{
	bool	g_bSimulation = false;
	cIOSMDriverInterface*g_pIOSMDriverInterface = 0;
	std::vector<std::wstring>	g_AllDeviceNameVector;
	unsigned char g_ucOutCount = 0;
	unsigned char g_ucInputCount = 0;
	unsigned char g_ucIrqPICount = 0;

	cIOSMDriverInterface::cIOSMDriverInterface()
	{
		cFPGA l_FPGA;
		DWORD dwError;
		if(!l_FPGA.FPGAInit(&dwError,0))
		{
			UT::ErrorMsg("Failed to initialize the FPGA library","FPGA");
			printf("FPGAInit Failed\n");
			return;
		}
		printf("FPGAInit OK\n");
		bool l_bPWEnable = false;
		l_FPGA.GetPWOffIrqEn(&l_bPWEnable);
		if(l_bPWEnable)
		{
			l_FPGA.SetPWOffIrqOpen(true);
			printf("FPGAInit SetPWOffIrqOpen(true)\n");
		}
		if(!l_FPGA.GetFPGAOCount(&g_ucOutCount))
		{
			UT::ErrorMsg("Failed to GetFPGAICount","FPGA");
			printf("Failed to GetFPGAICount\n");
			return;
		}
		if(!l_FPGA.GetFPGAICount(&g_ucInputCount))
		{
			UT::ErrorMsg("Failed to GetFPGAICount","FPGA");
			printf("Failed to GetFPGAICount\n");
			return;
		}
		if(!l_FPGA.GetIrqPICount(&g_ucIrqPICount))
		{
			UT::ErrorMsg("Failed to GetFPGAICount","FPGA");
			printf("Failed to GetFPGAICount\n");
			return;
		}
		for( int i=0;i<g_ucIrqPICount;++i )
		{
			l_FPGA.SetIrqPIEnableIdx(i,true);
		}
		printf("out count:%d\n",g_ucOutCount);
		printf("input count:%d\n",g_ucInputCount);
	}
	std::vector<std::wstring>	*cIOSMDriverInterface::GetAllDeviceNameVector(){ return &g_AllDeviceNameVector; }

	cIOSMDriverInterface::~cIOSMDriverInterface()
	{
		if(cGameApp::m_sbDeviceExist)
		{
			cFPGA l_FPGA;
			l_FPGA.SetPWOffIrqOpen(false);
			l_FPGA.FPGAUnInit();
		}
	}

	void	cIOSMDriverInterface::Destroy()
	{

	}

	void	cIOSMDriverInterface::Init()
	{
	}

	bool cIOSMDriverInterface::WriteIOBitAddress(int e_iAddress,int e_iBitAddress,bool e_bOn)
	{
		return false;
	}


	bool	cIOSMDriverInterface::ReadInputData(int e_iAddress,bool*e_pbData)
	{
		if(!cGameApp::m_sbDeviceExist)
			return false;
		if( e_iAddress>g_ucInputCount )
			return false;
		cFPGA l_FPGA;
		if(l_FPGA.GetFPGAIIdx(e_iAddress, e_pbData))
			return true;
		return false;
	}

	bool	cIOSMDriverInterface::ReadOutputData(int e_iAddress,bool*e_pbData)
	{
		if(!cGameApp::m_sbDeviceExist)
			return false;
		if( e_iAddress>	g_ucOutCount )
			return false;
		cFPGA l_FPGA;
		if(l_FPGA.GetFPGAOIdx(e_iAddress, e_pbData))
			return true;
		return false;
	}

	bool	cIOSMDriverInterface::WriteInputData(int e_iAddress,bool e_bOn)
	{
		if(!cGameApp::m_sbDeviceExist)
			return false;
		if( e_iAddress>g_ucInputCount )
			return false;
		cFPGA l_FPGA;
		if(l_FPGA.SetIrqPIEnableIdx(e_iAddress,e_bOn))
			return true;
		return false;
	}

	bool	cIOSMDriverInterface::WriteOutputData(int e_iAddress,bool e_bOn)
	{
		if(!cGameApp::m_sbDeviceExist)
			return false;
		if( e_iAddress>g_ucOutCount )
			return false;
		cFPGA l_FPGA;
		if(l_FPGA.SetFPGAOIdx(e_iAddress, e_bOn))
			return true;
		return false;	
	}

	bool cIOSMDriverInterface::ReadIOBitAddress(int e_iAddress,int e_iBitAddress,bool*e_pbValue)
	{
		if(!cGameApp::m_sbDeviceExist)
			return false;
		if( e_iAddress>	g_ucOutCount )
			return false;
		cFPGA l_FPGA;
		if(l_FPGA.GetFPGAOIdx(e_iAddress, e_pbValue))
			return true;
		return false;
	}

	bool	cIOSMDriverInterface::ReadIOChar(int e_iAddress,char*e_pcValue)
	{
		
		return true;
	}

	bool	cIOSMDriverInterface::WriteIOChar(int e_iAddress,char e_cValue)
	{

		return true;		
	}

	bool	cIOSMDriverInterface::WriteIORange(int e_iAddress,char* e_cValue,int e_iSize)
	{

		return true;			
	}

	bool	cIOSMDriverInterface::ReadIORange(int e_iAddress,int e_iSize,char*e_pcOutData)
	{
		
		return true;
	}


	bool	cIOSMDriverInterface::ReadIO4ByteOnce(int e_iAddress,int*e_piValue)
	{

		return false;
	}

	bool	cIOSMDriverInterface::WriteIO4ByteOnce(int e_iAddress,int e_iValue)
	{
		
		return false;
	}


	void	cIOSMDriverInterface::Update(float e_fElpaseTime)
	{
		if(!cGameApp::m_sbDeviceExist)
			return;
		int	l_iCount = this->Count();
		for( int i=0;i<l_iCount;++i )
			this->m_ObjectList[i]->Update(e_fElpaseTime);
	}

	void	cIOSMDriverInterface::WriteGameKey()
	{
		WriteIO4ByteOnce(0x3000,0xb3a29180);
		WriteIO4ByteOnce(0x3004,0xf7e6d5c4);
	}

	void	cIOSMDriverInterface::WriteInputIOSetup()
	{
		//1020,1024,1028,102c,1030=>00
		//1034,1038,103c=>ff
		//int	l_iInputKeyWriteFor00[5] = { 0x1020,0x1024,0x1028,0x102c,0x1030 };	//count 5,00
		//int	l_iInputKeyWriteForff[3] = { 0x1034,0x1038,0x103c };				//count 3,ff
		int	l_For00[5] = {0,0,0,0,0};
		int	l_Forff[3] = {0xff,0xff,0xff};
		this->WriteIORange(0x1020,(char*)l_For00,sizeof(int)*5);
		this->WriteIORange(0x1034,(char*)l_Forff,sizeof(int)*3);
	}

	void	cIOSMDriverInterface::Render()
	{
	}
//end namespace
}

#endif