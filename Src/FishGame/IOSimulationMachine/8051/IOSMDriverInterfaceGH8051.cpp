#include "../stdafx.h"
#ifdef USING_GH8051
#include "../../FishGame/GameApp/FileNameDefine.h"
#include "GH8051.h"
#include "../IOSMDriverInterface.h"
#include	<windows.h>
#include "../../Core/NetWork/Network.h"
//IO Polling serial Data
//BYTE g_dataPollingWrite[COMMON_SEND_FOR_RECEVE_BYTE_COUNT] = {0xAA, 0xF0, 0xFF,0xFF,0xFF,0xFF,0xFF,0x95,0x68,0x16};
BYTE g_dataPollingWrite[COMMON_SEND_FOR_RECEVE_BYTE_COUNT] = 
{0xAA, 0xF0, 0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0xb8,0x68,0x16};

//all player's coin to reset
//BYTE l_cRefundDeviceSingnalData[REFUND_SINGNAL_BYTE_COUNT] = {0xAA, 0xF2,0xff,0xff,0x00,0x68,0x16};
BYTE g_dataNotEnoughCoinResetPollingWrite[COMMON_SEND_FOR_RECEVE_BYTE_COUNT] = 
{0xAA, 0xF2, 0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0xba,0x68,0x16};

//all empty but fill 03 in specific position to reset player's coin data
BYTE g_DataNotEnoughCoinEmptyResetPollingWrite[COMMON_SEND_FOR_RECEVE_BYTE_COUNT] = 
{0xAA, 0xF2, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xba,0x68,0x16};

//ask to refund coin(ticket) count
BYTE g_cRefundDeviceEmptySingnalData[REFUND_SINGNAL_BYTE_COUNT] = 
{0xAA, 0xF1,
0x00,0x00,//1p,coin,ticket
0x00,0x00,
0x00,0x00,
0x00,0x00,//4p
0x00,0x00,
0x00,0x00,
0x00,0x00,
0x00,0x00,//8p
0x00,0x00,
0x00,0x00,
0,0x68,0x16};

bool	g_bCoinRefundSingnal = false;
bool	g_bIOAlive = true;
bool	g_bMultihread = false;



int		g_iRefundSingnalStep[MAX_PLAYER] = {false,false,false,false,false,
											false,false,false,false,false,
											false,false,false,false};


bool	g_bPlayerRefundIndex[MAX_PLAYER] = {false,false,false,false,false,
											false,false,false,false,false,
											false,false,false,false};


extern void		WritePlayerMoney(float e_fElpaseTime);

void	FileWorkThread( size_t _workParameter, size_t _pUri )
{
	UT::sTimeAndFPS	l_sTimeAndFPS;
	l_sTimeAndFPS.Update();
	while( g_bIOAlive )
	{
		Sleep(10);
		l_sTimeAndFPS.Update();
		WritePlayerMoney(l_sTimeAndFPS.fElpaseTime);
	}
}

void	IOWorkThreadDone(size_t _workParameter, size_t _pUri)
{
}

void	IOWorkThread( size_t _workParameter, size_t _pUri )
{
	cIO8051Setup *l_pIO8051Setup = (cIO8051Setup*)_workParameter;
	UT::sTimeAndFPS	l_sTimeAndFPS;
	l_sTimeAndFPS.Update();
	while( g_bIOAlive )
	{
		Sleep(1);
		l_sTimeAndFPS.Update();
		l_pIO8051Setup->Update(l_sTimeAndFPS.fElpaseTime);
	}
}

cIO8051Setup::cIO8051Setup()
{
	m_bIOError = false;
	m_iNumComport = 0;
	for( int i=0;i<MAX_COM_PORT_COUNT;++i )
	{
		this->m_pFileHandler[i] = 0;
		this->m_iIOstate[i] = 0;
	}
	this->m_fSeconds = 0.0f;
	this->m_iBoardSelect = 0;
	memset(m_iIOstate,0,sizeof(m_iIOstate));
	memset(m_bReadBit,0,sizeof(m_bReadBit));
	//Store Total Receive
	memset(m_cDataBytesReceive,0,sizeof(m_cDataBytesReceive));
	memset(m_cRefundDeviceSingnalData,0,sizeof(m_cRefundDeviceSingnalData));
	m_iRefundSingnalSendDataSize = 0;
	m_NoIOSingnalTimeout.SetTargetTime(2.f);
	m_pThread = 0;
	///for file write
	m_pThread = new cFUThreadPool();
	m_pThread->Spawn(1);
	FUStaticFunctor2<size_t, size_t, void>* workFunctor = new FUStaticFunctor2<size_t, size_t, void>(&FileWorkThread);
	FUStaticFunctor2<size_t, size_t, void>* doneFunctor = new FUStaticFunctor2<size_t, size_t, void>(&IOWorkThreadDone);
	m_pThread->ExecuteWork(workFunctor,doneFunctor,(size_t)this,0);
}

cIO8051Setup::~cIO8051Setup()
{
	g_bIOAlive = false;
	if( m_pThread )
	{
		m_pThread->WaitForIdle();
		m_pThread->Clear();
		SAFE_DELETE(m_pThread);
	}
	for( int i=0;i<MAX_COM_PORT_COUNT;++i )
	{
		if( this->m_pFileHandler[i] )
			CloseHandle(m_pFileHandler[i]);
		this->m_pFileHandler[i] = 0;
	}
}

static	int	g_siErrorCount = 0;

void	cIO8051Setup::Init(bool e_bResetCoin)
{
	//g_siErrorCount = 0;
	this->m_iNumComport = 1;
	if(cGameApp::m_sbDeviceExist)
	{
		for( int i=0;i<MAX_COM_PORT_COUNT;++i )
		{
			if( this->m_pFileHandler[i] )
				CloseHandle(m_pFileHandler[i]);
			this->m_pFileHandler[i] = 0;
		}
		//for( int i=0;i<5;++i )
		{
			int i=1;
			int	l_iComportIndex = 0;
			std::wstring	l_str = UT::ComposeMsgByFormat(L"COM%d",i);
			this->m_pFileHandler[l_iComportIndex] = OpenComPort(l_str.c_str(),&m_DCB[l_iComportIndex]);
			if( !m_pFileHandler[l_iComportIndex] )
			{
				m_bIOError = true;
				std::wstring	l_strError = l_str.c_str();
				l_strError += L"open failed , ";
				l_strError += this->m_strError;
				//cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(UT::WcharToChar(l_strError.c_str()));
			}
			else
			if( e_bResetCoin )
			{
				//++m_iNumComport;
				//std::wstring	l_strMsg = l_str.c_str();
				//l_strMsg += L" create ok!";
				//cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(UT::WcharToChar(l_strMsg.c_str()));
				//this->m_iIOstate[l_iComportIndex] = 1;
				m_iRefundSingnalSendDataSize = RESET_REFUND_SINGNAL_BYTE_COUNT;
				//player reset(2 for coin,3 for coin) 4 for security check
				memcpy(m_cRefundDeviceSingnalData,g_dataNotEnoughCoinResetPollingWrite,sizeof(char)*REFUND_SINGNAL_BYTE_COUNT);
				unsigned char	l_ucSecurityCode = 0;
				for(int i=0;i<RESET_CHECK_SUM;i++)	{l_ucSecurityCode += m_cRefundDeviceSingnalData[i];}
				m_cRefundDeviceSingnalData[RESET_CHECK_SUM] = l_ucSecurityCode;
				m_iRefundSingnalSendDataSize = REFUND_SINGNAL_BYTE_COUNT;
				g_bCoinRefundSingnal = true;
			}
		}
	}
	//if (wcslen(m_strError))
		//cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(m_strError);
	if(!cGameApp::m_sbDeviceExist)
		return;
	if( !m_pThread )
	{
		if( g_bMultihread )
		{
			m_pThread = new cFUThreadPool();
			m_pThread->Spawn(1);
			FUStaticFunctor2<size_t, size_t, void>* workFunctor = new FUStaticFunctor2<size_t, size_t, void>(&IOWorkThread);
			FUStaticFunctor2<size_t, size_t, void>* doneFunctor = new FUStaticFunctor2<size_t, size_t, void>(&IOWorkThreadDone);
			m_pThread->ExecuteWork(workFunctor,doneFunctor,(size_t)this,0);
		}
	}
}
void   cIO8051Setup::InitXML(const char *filename)
{
	cNodeISAX	l_NodeISAX;
	bool l_b = l_NodeISAX.ParseDataIntoXMLNode(filename);
	if(l_b)
	{
		TiXmlElement*l_pElement = l_NodeISAX.GetRootElement();
		const WCHAR*l_strName = l_pElement->Value();
		COMPARE_NAME("IOboard")
		{
			TiXmlElement*IndexTable = l_pElement->FirstChildElement();
			while(IndexTable)
			{
				const WCHAR*l_strMax   = IndexTable->Attribute(L"ExchangeIndex");
				//this->m_ExchangeIndexVector.push_back(_wtoi(l_strMax));
				IndexTable = IndexTable->NextSiblingElement();
			}
		}		
	}	
}

bool	cIO8051Setup::IsCommStateOk(DCB*e_pDCB,HANDLE e_Handle)
{
	//  Initialize the DCB structure.
	BOOL fSuccess = GetCommState(e_Handle,e_pDCB);
	if (!fSuccess)
	{
		//  Handle the error.
		swprintf (m_strError,MAX_PATH, L"GetCommState failed with error %d.\n", GetLastError());
		return false;
	}
	return true;
}

HANDLE	cIO8051Setup::OpenComPort(const WCHAR*e_strComPortName,DCB*e_pDCB)
{
	HANDLE	l_pFileHandler = CreateFile(e_strComPortName, GENERIC_READ|GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (l_pFileHandler == INVALID_HANDLE_VALUE) 
	{
		//cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(L"CreateFile Failed");
		//swprintf(m_strError,MAX_PATH,L"CreateFile failed with error %d.", GetLastError());
		//cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine("com port create file OK");
		return 0;
	}
	SecureZeroMemory(e_pDCB, sizeof(DCB));
	e_pDCB->DCBlength = sizeof(DCB);
	if(!IsCommStateOk(e_pDCB,l_pFileHandler))
	{
		//  Handle the error.
		swprintf (m_strError,MAX_PATH, L"GetCommState failed with error %d.\n", GetLastError());
		return 0;
	}
	//  Fill in some DCB values and set the com state: 
	//  38400 bps, 8 data bits, no parity, and 1 stop bit.
	e_pDCB->BaudRate = CBR_115200;     //  baud rate
	e_pDCB->ByteSize = 8;             //  data size, xmit and rcv
	e_pDCB->Parity   = NOPARITY;      //  parity bit
	e_pDCB->StopBits = ONESTOPBIT;    //  stop bit
	BOOL fSuccess = SetCommState(l_pFileHandler, e_pDCB);
	if (!fSuccess) 
	{
	  //  Handle the error.
		swprintf (m_strError,MAX_PATH, L"SetCommState failed with error %d.\n", GetLastError());
		return 0;
	}
	//  Get the comm config again.
	fSuccess = GetCommState(l_pFileHandler, e_pDCB);
	if (!fSuccess) 
	{
		//  Handle the error.
		swprintf (m_strError,MAX_PATH, L"GetCommState failed with error %d.\n", GetLastError());
		return 0;
	}
	//swprintf(m_strReceive, MAX_PATH, L"");
	COMMTIMEOUTS timeouts;
	timeouts.ReadIntervalTimeout = 1;
	timeouts.ReadTotalTimeoutMultiplier = 1;
	timeouts.ReadTotalTimeoutConstant = 1;
	timeouts.WriteTotalTimeoutMultiplier = 1;
	timeouts.WriteTotalTimeoutConstant = 1;
	if (!SetCommTimeouts(l_pFileHandler, &timeouts))
	{
		swprintf(m_strError,MAX_PATH,L"setting port time-outs.(%d)", GetLastError());
		return 0;
	}
	swprintf(m_strError,MAX_PATH,L"Open Com1 Success.(%d)", GetLastError());
	//clear comport data
	PurgeComm(l_pFileHandler,PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
	return l_pFileHandler;
}

void	cIO8051Setup::ErrorProcess(HANDLE e_Handle)
{
	PurgeComm(e_Handle,PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
	++g_siErrorCount;
	if( g_siErrorCount >= 5 )
		m_bIOError = true;
	if( g_siErrorCount % 4 == 0 )
		this->Init(false);
}


void	cIO8051Setup::SingnalProcess(float e_fElpaseTime)
{
	//if( m_iIOstate[m_iBoardSelect] == 0 )
	{
		DWORD dwStartWritten = 0;//start bit value
		DWORD dwBytesReceived = 0;//start bit value
		//IO Polling parameter 
		DWORD dwPollingToWrite = COMMON_SEND_FOR_RECEVE_BYTE_COUNT;//IO polling byte count
		//Hopper Request parameter
		DWORD dwHopperToWrite = REFUND_SINGNAL_BYTE_COUNT;//Hopper request byte count
		//IO receive parameter
		DWORD dwBytesToReceive = IO_8051_BYTE_COUNT;//IO receive byte count
		//IO Hopper	Require	serial Data		
		HANDLE	l_Handle = 0;
		//polling write
		//cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(L"polling state");
		l_Handle = this->m_pFileHandler[0];
		if(!IsCommStateOk(&this->m_DCB[0],l_Handle))
		{
			//m_bIOError = true;
			return;
		}
		static	bool	l_bFlag = true;
		BYTE	l_cDataBytesReceive[IO_8051_BYTE_COUNT];
		if( l_Handle )
		{
			if (l_bFlag )
			{
				l_bFlag = false;
				if( g_bCoinRefundSingnal )
				{
					RefundSingnalSending();
//					OutputDebugString(L"RefundSingnalSending\n");
				}
				else
				{
					BOOL l_bOk = WriteFile(l_Handle, g_dataPollingWrite, dwPollingToWrite, &dwStartWritten, NULL);
					if(l_bOk)
					{
						//swprintf (m_strError,MAX_PATH, L"Query Cmd Send failed %d", l_bErrorFlag);
						//cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(L"IO-1-W-Fail");
					}
					else
					{
						//swprintf(m_strError, MAX_PATH, L"Query Cmd Send success %d", l_bErrorFlag);
						//cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(L"IO-1-W-Suc");
						m_bIOError = true;
					}
//					OutputDebugString(L"WriteFile\n");
				}
			}
			else
			{
				l_bFlag = true;
//				OutputDebugString(L"read\n");
				//polling read
				BOOL	l_bOk = ReadFile(l_Handle, l_cDataBytesReceive, dwBytesToReceive, &dwBytesReceived, NULL);
				if(l_bOk)
				{
					//if( l_cDataBytesReceive[0] == 0xaa && l_cDataBytesReceive[1] == 0xf0 && dwBytesReceived == dwBytesToReceive  )
					if( l_cDataBytesReceive[0] == 0xaa && l_cDataBytesReceive[1] == 0xf0 && 
							  l_cDataBytesReceive[IO_8051_BYTE_COUNT-2] == 0x68 && l_cDataBytesReceive[IO_8051_BYTE_COUNT-1] == 0x16)
					{
						unsigned char	l_cCheckSum = 0;
						for( int i=0;i<IO_8051_BYTE_COUNT-CHECK_SUM_COUNT;++i )
						{
							l_cCheckSum += l_cDataBytesReceive[i];
						}
						if( l_cCheckSum == l_cDataBytesReceive[IO_8051_BYTE_COUNT-CHECK_SUM_COUNT] )
						{
							memcpy(m_cDataBytesReceive[m_iBoardSelect],l_cDataBytesReceive,sizeof(l_cDataBytesReceive));
							//Bit Debug
							for( int k=0;k<m_iNumComport;++k )
							{
								for(int i=0;i<IO_8051_BYTE_COUNT;i++)
								{
									for(int j=0;j<8;j++)
									{
										m_bReadBit[k][i*8+j] = ((m_cDataBytesReceive[k][i] >> j) % 0x02)?true:false;
									}
								}
							}
							g_siErrorCount = 0;

							m_bIOError = false;
							int	l_iCount = IOSM::g_pIOSMDriverInterface->Count();
							for( int i=0;i<l_iCount;++i )
							{
								IOSM::g_pIOSMDriverInterface->GetObject(i)->Update(e_fElpaseTime);
							}
							for( int i = 0;i<MAX_PLAYER;++i )
							{
								if( g_iRefundSingnalStep[i] == 1 )
								{
									g_iRefundSingnalStep[i] = 2;
								}
							}
						}
						else
						{
							ErrorProcess(l_Handle);
//							OutputDebugString(L"fuck1");
						}
						this->m_NoIOSingnalTimeout.Start();
					}
					else
					{
						ErrorProcess(l_Handle);
//						OutputDebugString(L"fuck2");
							
					}
					//swprintf (m_strError,MAX_PATH, L"Query Cmd Receive failed %d", l_bErrorFlag);
					//cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(L"IO-1-R-Fail");
				}					
				else
				{
					//m_bIOError = true;
					ErrorProcess(l_Handle);
//					OutputDebugString(L"fuck3");
						//PurgeComm(l_Handle,PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
					//OutputDebugString(L"read file failed\n");
					//swprintf(m_strError, MAX_PATH, L"Query Cmd Receive success %d", l_bErrorFlag);
					//cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(L"IO-1-R-Suc");
				}
			}
		}
	}
}

void	cIO8051Setup::WorkingBordChange()
{
	++m_iBoardSelect;
	if( m_iBoardSelect>=m_iNumComport )
	{
		m_iBoardSelect = 0;
	}
}

bool	cIO8051Setup::RefundSingnalSending()
{
	//if( m_iIOstate[m_iBoardSelect] == 1 )
	{
		DWORD dwStartWritten = 0;//start bit value
		DWORD dwBytesReceived = 0;//start bit value
		//Hopper Request parameter
		DWORD dwHopperToWrite = m_iRefundSingnalSendDataSize;//Hopper request byte count
		HANDLE	l_Handle = l_Handle = this->m_pFileHandler[m_iBoardSelect];
		if(!IsCommStateOk(&this->m_DCB[m_iBoardSelect],l_Handle))
		{
			m_bIOError = true;
			return false;
		}
		BOOL l_bOk = WriteFile(l_Handle, m_cRefundDeviceSingnalData, dwHopperToWrite, &dwStartWritten, NULL);
		if(l_bOk)
		{
			//swprintf (m_strError,MAX_PATH, L"Query Cmd Receive failed %d", l_bErrorFlag);
			if( dwHopperToWrite == dwStartWritten )
			{
				g_bCoinRefundSingnal = false;
				for( int i = 0;i<MAX_PLAYER;++i )
				{
					if( g_bPlayerRefundIndex[i] == true )
					{
						g_iRefundSingnalStep[i] = 1;
						g_bPlayerRefundIndex[i] = false;
					}
				}
				return true;
			}
		}
		else
		{
			//swprintf(m_strError, MAX_PATH, L"Query Cmd Receive success %d", l_bErrorFlag);
			m_bIOError = true;
			return false;
		}
		//m_iIOstate[m_iBoardSelect] = 0;
	}
	return false;
}


void	cIO8051Setup::Update(float e_fElpaseTime)
{
	m_NoIOSingnalTimeout.Update(e_fElpaseTime);
	if( m_NoIOSingnalTimeout.bTragetTimrReached )
	{
		//IOSM::g_pIOSMDriverInterface->SetIOError(true);
	}

	if( e_fElpaseTime >= 0.016f )
		e_fElpaseTime = 0.016f;
	//avoid frame skip
	m_fSeconds += e_fElpaseTime;
	float	l_fTargetTime = REFRESH_8051_IO_TIME_OFFSET/this->m_iNumComport;
	if(m_fSeconds >= l_fTargetTime )
	{
		m_fSeconds = UT::GetFloatModulus(m_fSeconds,l_fTargetTime);
		if( m_pFileHandler[0] == 0 )
		{
			this->m_bIOError = true;
			this->Init(false);
			return;
		}
		SingnalProcess(e_fElpaseTime);
		WorkingBordChange();
	}
}
//std::wstring	g_str = L"";

int	g_iBitAddressValue[8] = {1<<0,1<<1,1<<2,1<<3,1<<4,1<<5,1<<6,1<<7};
bool cIO8051Setup::ReadInputData(int e_iAddress,bool*e_pbData)
{	
	//int	l_iValue = m_ExchangeIndexVector[e_iAddress];
	int	l_iValue = e_iAddress;
	int	l_iComPortValue = l_iValue/(IO_8051_BYTE_COUNT*BIT_COUNT);

	int	l_iByte = l_iValue/BIT_COUNT%IO_8051_BYTE_COUNT;

	int	l_iBit = l_iValue%BIT_COUNT;

	*e_pbData = (m_cDataBytesReceive[l_iComPortValue][l_iByte]&g_iBitAddressValue[l_iBit])?true:false;
	//std::string	l_str = UT::ComposeMsgByFormat("Address%d,On:%d",e_iAddress,*e_pbData?1:0);
	//cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(l_str.c_str());
	//if( g_iTestIOCount % 50 == 0 )
	//{
	//	g_str = L"Com:";
	//	g_str += ValueToStringW(l_iComPortValue);
	//	g_str += L",Address:";
	//	g_str += ValueToStringW(e_iAddress);
	//	g_str += L",Value:";
	//	g_str += ValueToStringW(l_iValue);
	//	g_str += L",Byte:";
	//	g_str += ValueToStringW(l_iByte);
	//	g_str += L",Bit:";
	//	g_str += ValueToStringW(l_iBit);
	//	g_str += L",BitValue";
	//	g_str += ValueToStringW(*e_pbData?1:0);
	//	g_str += L"\n";
	//	OutputDebugString(g_str.c_str());
	//}
	return true;
}

bool cIO8051Setup::ReadIOChar(int e_iAddress,char*e_pcValue)
{
	//int	l_iValue = m_ExchangeIndexVector[e_iAddress];
	int	l_iValue = e_iAddress;
	int	l_iComPortValue = l_iValue/(IO_8051_BYTE_COUNT);

	int	l_iByte = l_iValue%IO_8051_BYTE_COUNT;

	*e_pcValue = m_cDataBytesReceive[l_iComPortValue][l_iByte];
	return true;
}


bool	cIO8051Setup::WriteIOChar(int e_iAddress,char e_cValue)
{
	//                   1P¹ô 1P±m 2P¹ô 2P±m 3P¹ô 3P±m 4P¹ô 4P±m 5P¹ô 5P±m 6P¹ô 6P±m Åç½X
	//[17] = {0xAA, 0xF1,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x9A,0x68,0x16};
	//int	l_iValue = m_ExchangeIndexVector[e_iAddress];
	int	l_iValue = e_iAddress;
	int	l_iComPortValue = l_iValue/(REFUND_SINGNAL_BYTE_COUNT);
	//this->m_iIOstate[l_iComPortValue] = 1;
	int	l_iByte = l_iValue%REFUND_SINGNAL_BYTE_COUNT;
	memcpy(m_cRefundDeviceSingnalData,g_cRefundDeviceEmptySingnalData,sizeof(char)*REFUND_SINGNAL_BYTE_COUNT);
	m_cRefundDeviceSingnalData[l_iByte] = e_cValue;
	unsigned char	l_ucSecurityCode = 0;
	for(int i=0;i<REFUND_SINGNAL_BYTE_COUNT-CHECK_SUM_COUNT;i++)	{l_ucSecurityCode += m_cRefundDeviceSingnalData[i];}//Debug Åç½X = A5
	m_cRefundDeviceSingnalData[REFUND_SINGNAL_BYTE_COUNT-CHECK_SUM_COUNT] = l_ucSecurityCode;
	m_iRefundSingnalSendDataSize = REFUND_SINGNAL_BYTE_COUNT;
	g_bCoinRefundSingnal = true;
	return true;
}

bool	cIO8051Setup::WriteIOBit(int e_iAddress,bool e_bValue)
{
	//int	l_iValue = m_ExchangeIndexVector[e_iAddress];
	int	l_iValue = e_iAddress;
	int	l_iComPortValue = l_iValue/RESET_REFUND_SINGNAL_BYTE_COUNT;
	//this->m_iIOstate[l_iComPortValue] = 1;
	int	l_iByte = e_iAddress/8;
	m_iRefundSingnalSendDataSize = RESET_REFUND_SINGNAL_BYTE_COUNT;
	memcpy(m_cRefundDeviceSingnalData,g_DataNotEnoughCoinEmptyResetPollingWrite,sizeof(char)*RESET_REFUND_SINGNAL_BYTE_COUNT);
	m_cRefundDeviceSingnalData[l_iByte] = 03;
	unsigned char	l_ucSecurityCode = 0;
	for(int i=0;i<RESET_REFUND_SINGNAL_BYTE_COUNT-CHECK_SUM_COUNT;i++)	{l_ucSecurityCode += m_cRefundDeviceSingnalData[i];}//Debug Åç½X = A5
	m_cRefundDeviceSingnalData[RESET_REFUND_SINGNAL_BYTE_COUNT-CHECK_SUM_COUNT] = l_ucSecurityCode;
	//swprintf (m_strExchangeIndex,MAX_PATH, L"current= %d  exchange=%d \n", e_iAddress,l_iValue);
#ifdef DEBUG
#ifdef WIN32
		std::wstring g_str;
		g_str = L"Com:";
		g_str += ValueToStringW(l_iComPortValue);
		g_str += L",Address:";
		g_str += ValueToStringW(e_iAddress);
		g_str += L",Value:";
		g_str += ValueToStringW(l_iValue);
		g_str += L",Byte:";
		g_str += ValueToStringW(l_iByte);
		g_str += L",BitValue";
		g_str += ValueToStringW(e_bValue?1:0);
		g_str += L",FinalWriteValue";
		g_str += ValueToStringW(3);
		g_str += L"\n";
		OutputDebugString(g_str.c_str());
#endif
#endif
	g_bCoinRefundSingnal = true;
	return true;
}

void	cIO8051Setup::Render() 
{
	//if(!cGameApp::m_sbDeviceExist)
	//	return;
	swprintf(m_strReceiveDebug, MAX_PATH, L"NumCom:%d,BordID = %d,Send%d,Receving%d",this->m_iNumComport, m_iBoardSelect,m_iIOstate[m_iBoardSelect]==1,m_iIOstate[m_iBoardSelect]==0);
	cGameApp::m_spGlyphFontRender->RenderFont(10 , 280, m_strReceiveDebug);
	cGameApp::m_spGlyphFontRender->SetScale(0.5);
	WCHAR	l_str[MAX_PATH];
	int	l_LineChange = IO_8051_BYTE_COUNT/2;
	for(int i=0;i<MAX_COM_PORT_COUNT;i++)
	{
		for(int j=0;j<IO_8051_BYTE_COUNT;j++)
		{
			swprintf(l_str, MAX_PATH,L"%x",m_cDataBytesReceive[i][j] );
			cGameApp::m_spGlyphFontRender->RenderFont(j*30+130-(j/l_LineChange*(30*l_LineChange)) , i*30+330+((j/l_LineChange)*20), l_str);
		}
	}

	for(int i=0;i<REFUND_SINGNAL_BYTE_COUNT;i++)
	{
		swprintf(l_str, MAX_PATH,L"%x",m_cRefundDeviceSingnalData[i]);
		cGameApp::m_spGlyphFontRender->RenderFont(i*30+130 , 490, l_str);
	}

	cGameApp::m_spGlyphFontRender->SetScale(1.f);
}
cIO8051Setup*g_pIO8051Setup = 0;
namespace	IOSM
{
	bool	g_bSimulation = false;
	cIOSMDriverInterface*g_pIOSMDriverInterface = 0;
	std::vector<std::wstring>	g_AllDeviceNameVector;

	cIOSMDriverInterface::cIOSMDriverInterface()
	{
		m_bIOError = false;
		if( !g_pIO8051Setup )
			g_pIO8051Setup = new cIO8051Setup();
	}
	std::vector<std::wstring>	*cIOSMDriverInterface::GetAllDeviceNameVector(){ return &g_AllDeviceNameVector; }

	cIOSMDriverInterface::~cIOSMDriverInterface()
	{
		SAFE_DELETE(g_pIO8051Setup);
	}
	void	cIOSMDriverInterface::Destroy(){}
	void	cIOSMDriverInterface::Init()
	{
		//g_pIO8051Setup->Init(IO8051_DATA);
		g_pIO8051Setup->Init(true);
		//g_pIO8051Setup->Init(IO8051_DATA10);
		//g_pIO8051Setup->Init(IO8051_DATA14);
	}
	bool cIOSMDriverInterface::WriteIOBitAddress(int e_iAddress,int e_iBitAddress,bool e_bOn)
	{
		return false;
	}
	bool	cIOSMDriverInterface::ReadInputData(int e_iAddress,bool*e_pbData)
	{
		if(!cGameApp::m_sbDeviceExist)
			return false;
		g_pIO8051Setup->ReadInputData(e_iAddress,e_pbData);
		return false;
	}
	bool	cIOSMDriverInterface::ReadOutputData(int e_iAddress,bool*e_pbData)
	{		
		if(!cGameApp::m_sbDeviceExist)
			return false;
		g_pIO8051Setup->ReadInputData(e_iAddress,e_pbData);
		return false;
	}
	bool	cIOSMDriverInterface::WriteInputData(int e_iAddress,bool e_bOn)
	{
		if(!cGameApp::m_sbDeviceExist)
			return false;
		g_pIO8051Setup->WriteIOBit(e_iAddress,e_bOn);		
		return false;
	}
	bool	cIOSMDriverInterface::WriteOutputData(int e_iAddress,bool e_bOn)
	{
		return false;	
	}
	bool cIOSMDriverInterface::ReadIOBitAddress(int e_iAddress,int e_iBitAddress,bool*e_pbValue)
	{
		return false;
	}
	bool	cIOSMDriverInterface::ReadIOChar(int e_iAddress,char*e_pcValue)
	{
		if(!cGameApp::m_sbDeviceExist)
			return false;
		g_pIO8051Setup->ReadIOChar(e_iAddress,e_pcValue);
		return true;
	}
	bool	cIOSMDriverInterface::WriteIOChar(int e_iAddress,char e_cValue)
	{
		if(!cGameApp::m_sbDeviceExist)
			return false;
		g_pIO8051Setup->WriteIOChar(e_iAddress,e_cValue);
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
		if( !g_bMultihread  )
		{
			g_pIO8051Setup->Update(e_fElpaseTime);
		}
		this->m_bIOError = g_pIO8051Setup->IsIOError();
	}
	void	cIOSMDriverInterface::Render()
	{
//#ifdef DEBUG
		g_pIO8051Setup->Render();
//#endif
	}
	void	cIOSMDriverInterface::WriteGameKey()
	{
		//WriteIO4ByteOnce(0x3000,0xb3a29180);
		//WriteIO4ByteOnce(0x3004,0xf7e6d5c4);
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
//end namespace
}
#endif