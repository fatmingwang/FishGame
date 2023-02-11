#include "../stdafx.h"
#include "SerialIO_MasterLee.h"
#include "Probability_MasterLee.h"
#include "../../GameApp/GameApp.h"
#include "../../ProbabilityFish/FishProbability.h"


cSerialCtrl::cSerialCtrl():m_portStatus(FALSE),m_portHandle(NULL)
{
	m_portConfig.ByteSize = 8;            // Byte of the Data.
	m_portConfig.StopBits = ONESTOPBIT;   // Use one bit for stopbit.
	m_portConfig.Parity = NOPARITY;       // No parity bit
	m_portConfig.BaudRate = CBR_9600;     // Buadrate 9600 bit/sec
}

cSerialCtrl::~cSerialCtrl()
{
	if( !m_portHandle )
		ClosePort();
	m_portHandle = NULL;
}


void cSerialCtrl::SetPortStatus(bool bOnOff)
{
	m_portStatus = bOnOff;
}

bool cSerialCtrl::GetPortStatus()
{
	return m_portStatus;
}

HANDLE cSerialCtrl::GetPortHandle()
{
	return m_portHandle;
}

bool cSerialCtrl::OpenPort(DCB dcb, const WCHAR * portName)
{

	if (m_portStatus == FALSE)  // if port is opened already, not open port again.
	{
		m_portHandle = CreateFile(portName,  // Specify port device: default "COM1"
			GENERIC_READ | GENERIC_WRITE,       // Specify mode that open device.
			0,                                  // the devide isn't shared.
			NULL,                               // the object gets a default security.
			OPEN_EXISTING,                      // Specify which action to take on file. 
			0,                                  // default.
			NULL);                              // default.

		// Get current configuration of serial communication port.
		if (GetCommState(m_portHandle,&m_portConfig) == 0)
		{
			//AfxMessageBox("Get configuration port has problem.");
			return FALSE;
		}
		// Assign user parameter.
		m_portConfig.BaudRate = dcb.BaudRate;    // Specify buad rate of communicaiton.
		m_portConfig.StopBits = dcb.StopBits;    // Specify stopbit of communication.
		m_portConfig.Parity = dcb.Parity;        // Specify parity of communication.
		m_portConfig.ByteSize = dcb.ByteSize;    // Specify  byte of size of communication.

		// Set current configuration of serial communication port.
		if (SetCommState(m_portHandle,&m_portConfig) == 0)
		{
			//AfxMessageBox("Set configuration port has problem.");
			return FALSE;
		}

		// instance an object of COMMTIMEOUTS.
		COMMTIMEOUTS comTimeOut;                   
		comTimeOut.ReadIntervalTimeout = 3;
		comTimeOut.ReadTotalTimeoutMultiplier = 3;
		comTimeOut.ReadTotalTimeoutConstant = 2;
		comTimeOut.WriteTotalTimeoutMultiplier = 3;
		comTimeOut.WriteTotalTimeoutConstant = 2;
		SetCommTimeouts(m_portHandle,&comTimeOut);		// set the time-out parameter into device control.
		m_portStatus = TRUE; 
		return TRUE;      
	}
	return FALSE;
}

bool cSerialCtrl::OpenPort(const WCHAR * baudRate, const WCHAR * portName)
{
	DCB configSerial;
	configSerial.ByteSize = 8;
	configSerial.StopBits = ONESTOPBIT;
	configSerial.Parity = NOPARITY;
	switch(_wtoi(baudRate))
	{
	case 110:
		configSerial.BaudRate = CBR_110;
		break;
	case 300:
		configSerial.BaudRate = CBR_300;
		break;
	case 600:
		configSerial.BaudRate = CBR_600;
		break;
	case 1200:
		configSerial.BaudRate = CBR_1200;
		break;
	case 2400:
		configSerial.BaudRate = CBR_2400;
		break;
	case 4800:
		configSerial.BaudRate = CBR_4800;
		break;
	case 9600:
		configSerial.BaudRate = CBR_9600;
		break;
	case 14400:
		configSerial.BaudRate = CBR_14400;
		break;
	case 19200:
		configSerial.BaudRate = CBR_19200;
		break;
	case 38400:
		configSerial.BaudRate = CBR_38400;
		break;
	case 56000:
		configSerial.BaudRate = CBR_56000;
		break;
	case 57600:
		configSerial.BaudRate = CBR_57600;
		break;
	case 115200 :
		configSerial.BaudRate = CBR_115200;
		break;
	case 128000:
		configSerial.BaudRate = CBR_128000;
		break;
	case 256000:
		configSerial.BaudRate = CBR_256000;
		break;
	default:
		break;
	}

	return OpenPort(configSerial,portName);
}

bool cSerialCtrl::Read(char * inputData, const unsigned int & sizeBuffer, unsigned long & length)
{
	static UT::sTimeAndFPS	l_sTimegap;
	l_sTimegap.Update();
	if (ReadFile(m_portHandle,  // handle of file to read
		inputData,               // handle of file to read
		sizeBuffer,              // number of bytes to read
		&length,                 // pointer to number of bytes read
		NULL) == 0)              // pointer to structure for data
	{
		m_strDebugString = L"read failed";
		l_sTimegap.Update();
//		cSerialIO_MasterLee::WriteLog(UT::ComposeMsgByFormat("HW-read failed,read data past time%.3f",l_sTimegap.fElpaseTime).c_str(),true);
		return FALSE;
	}

	if (length > 0)
	{
		m_strDebugString = L"read ok";
		m_strDebugString += ValueToStringW((int)length);
		m_strDebugString += L" ok";
		inputData[length] = NULL; // Assign end flag of message.

		l_sTimegap.Update();
//		cSerialIO_MasterLee::WriteLog(UT::ComposeMsgByFormat("HW-read ok,read data past time%.3f",l_sTimegap.fElpaseTime).c_str(),true);
		return TRUE;  
	}
	m_strDebugString = L"read failed";
	l_sTimegap.Update();
//	cSerialIO_MasterLee::WriteLog(UT::ComposeMsgByFormat("HW-read failed,read data past time%.3f",l_sTimegap.fElpaseTime).c_str(),true);
	return false;
}


bool cSerialCtrl::Write( const char * outputData, const unsigned int & sizeBuffer, unsigned long & length )
{	
	++g_iNumPacketSend;
	static UT::sTimeAndFPS	l_sTimegap;
	l_sTimegap.Update();
	if( sizeBuffer > 0 )
	{
		if (WriteFile(m_portHandle, // handle to file to write to
			outputData,              // pointer to data to write to file
			sizeBuffer,              // number of bytes to write
			&length,NULL) == 0)      // pointer to number of bytes written
		{
			m_strDebugString = L"wrtie failed";
			++g_iNumPacketSendFailed;
			l_sTimegap.Update();
//			cSerialIO_MasterLee::WriteLog( UT::ComposeMsgByFormat("HW-send data failed time%.3f",l_sTimegap.fElpaseTime).c_str(), true );
			return FALSE;
		}
		m_strDebugString = L"wrtie ok size::";
		m_strDebugString += ValueToStringW(sizeBuffer);
		l_sTimegap.Update();
//		cSerialIO_MasterLee::WriteLog( UT::ComposeMsgByFormat("HW-send data ok time%.3f",l_sTimegap.fElpaseTime).c_str(), true );
		return TRUE;
	}
	l_sTimegap.Update();
//	cSerialIO_MasterLee::WriteLog(UT::ComposeMsgByFormat("HW-send data failed time%.3f",l_sTimegap.fElpaseTime).c_str(),true);
	m_strDebugString  = L"write failed ";
	return FALSE;
}


void	cSerialCtrl::DebugRender()
{
	cGameApp::m_spGlyphFontRender->SetScale( 0.5f );
	cGameApp::m_spGlyphFontRender->RenderFont( 5, 50, m_strDebugString.c_str() );
		//
	//std::wstring	l_str = UT::ComposeMsgByFormat( 
	//	L"Com:%d\nSend:%d\nReceive:%d\nEmptySend:%d\nEmptyReceive:%d\nHitSend:%d\nHitReceive:%d\nTimeoutForFishDied:%d\nSendFailed:%d\nCheckSumError:%d\nLastSendPacketFirstFishID:(%d)->%d\nCurrentReceivePacketFirstFishID:%d\nLastSendPacketFirstFishID_No_Match:%d\nResend:%d\nTimeout:%.2f",
	//	cSerialIO_MasterLee::m_siCom,
	//	g_iNumPacketSend,
	//	g_iNumPacketReceived,
	//	g_iNumPacketEmptySend,
	//	g_iNumPacketEmptyReceived,
	//	g_iNumPacketHitSend,
	//	g_iNumPacketHitReceived,
	//	g_pnCount_TimeoutForFishDied[0],
	//	g_iNumPacketSendFailed,
	//	g_pnNumPacketChecksumError[0],
	//	g_nRoundFishID,
	//	g_pnLastSendPacketFirstFishID[0],
	//	g_pnCurrentReceivePacketFirstFishID[0],
	//	g_nCount_LastSendPacketFirstFishID_No_Match,
	//	g_iNumPacketResend,
	//	g_fTimeOutForFishDied);
	//cGameApp::m_spGlyphFontRender->RenderFont(550,170,l_str.c_str());
	//
	cGameApp::m_spGlyphFontRender->SetScale(1.f);
}


bool cSerialCtrl::ClosePort()
{
	if (m_portStatus == TRUE)               // Port need to be open before.
	{
		m_portStatus = FALSE;                 // Update status
		if(CloseHandle(m_portHandle) == 0)    // Call this function to close port.
		{
			//AfxMessageBox("Port Closeing isn't successed.");
			return FALSE;
		}    
		return TRUE;
	}
	return FALSE;
}