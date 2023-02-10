#include "../stdafx.h"
#include "SerialIOBase.h"
#include "SerialThread.h"
#include "../../GameApp/GameApp.h"
#include "../../ProbabilityFish/FishProbability.h"


void	ComportWorkThread( size_t _workParameter, size_t _pUri )
{
	cSerialIOBase *l_pSerialThread = (cSerialIOBase*)_workParameter;
	l_pSerialThread->m_pSerialThread->Run();	//±Ò°Ê¦hºü
}

void	ComportWorkThreadDone(size_t _workParameter, size_t _pUri)
{
}

cSerialIOBase::cSerialIOBase():m_pSerialThread(NULL), m_strPortName(L"COM1"), m_strBaudRate(L"115200"), m_sendSize(0)
{
	m_pSerialThread = new cSerialThread();	//¦hºü
	//
	m_bProccessActivate=FALSE;
	m_bPortActivate=FALSE;
	m_bSendActivate=FALSE;
	m_bClosePort=FALSE;
	//
	Init();
	//
	m_bPortActivate=TRUE;	
}

cSerialIOBase::~cSerialIOBase()
{
	UnInit();
	SAFE_DELETE(m_pSerialThread);
}

bool cSerialIOBase::Init()
{
	m_pSerialThread->setOwner(this);

	m_DCB.ByteSize = 8;
	m_DCB.StopBits = ONESTOPBIT;
	m_DCB.Parity = NOPARITY;
	switch(_wtoi(m_strBaudRate.c_str()))
	{
	case 110:
		m_DCB.BaudRate = CBR_110;
		break;
	case 300:
		m_DCB.BaudRate = CBR_300;
		break;
	case 600:
		m_DCB.BaudRate = CBR_600;
		break;
	case 1200:
		m_DCB.BaudRate = CBR_1200;
		break;
	case 2400:
		m_DCB.BaudRate = CBR_2400;
		break;
	case 4800:
		m_DCB.BaudRate = CBR_4800;
		break;
	case 9600:
		m_DCB.BaudRate = CBR_9600;
		break;
	case 14400:
		m_DCB.BaudRate = CBR_14400;
		break;
	case 19200:
		m_DCB.BaudRate = CBR_19200;
		break;
	case 38400:
		m_DCB.BaudRate = CBR_38400;
		break;
	case 56000:
		m_DCB.BaudRate = CBR_56000;
		break;
	case 57600:
		m_DCB.BaudRate = CBR_57600;
		break;
	case 115200 :
		m_DCB.BaudRate = CBR_115200;
		break;
	case 128000:
		m_DCB.BaudRate = CBR_128000;
		break;
	case 256000:
		m_DCB.BaudRate = CBR_256000;
		break;
	default:
		break;
	}
	m_bProccessActivate=TRUE;
	//create thread
	FUStaticFunctor2<size_t, size_t, void>* workFunctor = new FUStaticFunctor2<size_t, size_t, void>(&ComportWorkThread);
	FUStaticFunctor2<size_t, size_t, void>* doneFunctor = new FUStaticFunctor2<size_t, size_t, void>(&ComportWorkThreadDone);
	m_pSerialThread->ExecuteWork(workFunctor,doneFunctor,(size_t)this,0);
	return TRUE;

}

void cSerialIOBase::UnInit()
{
	m_bPortActivate= FALSE;
	if(m_pSerialThread)
	{
		m_pSerialThread->ClosePort();
		m_pSerialThread->Clear();
	}
}

void cSerialIOBase::OpenPort(WCHAR*strPortName,WCHAR*strBaudRate)
 {
	 m_strBaudRate=strBaudRate;
	 m_strPortName=strPortName;
	 m_bPortActivate=TRUE;
	 
 }

void cSerialIOBase::OnEventOpen(bool bSuccess)
{
	if( bSuccess )
	{
		printf("port open successfully\n");
	}
	else
	{
		printf("port open failed\n");
	}
}

void cSerialIOBase::ClosePort()
{
	m_bClosePort=TRUE;
	SetPortActivate(FALSE);
}

void cSerialIOBase::OnEventClose(bool bSuccess)
{
	if(bSuccess)
		printf("port close successfully\n");
	else
		printf("port close failed\n");
}

void cSerialIOBase::OnEventRead(char *inPacket,int inLength)
{
    
	return;
}

void cSerialIOBase::Write( char *outPacket, int outLength )
{
	if( outLength <= MAX_SEND_BUFFER )
	{
		memcpy( m_sendBuffer, outPacket, outLength);
		m_sendSize = outLength;
		SetSendActivate( TRUE );
	}
	else
	{
		assert(0&&"buffer over flow");
		OnEventWrite(-1);
	}
	return ;
}

void cSerialIOBase::OnEventWrite( int nWritten )
{
	if(nWritten<0)
	{
		printf( "write error\n" );
	}
	else
	{
		printf( "%d bytes send\n", nWritten );
	}
}

bool cSerialIOBase::GetSendActivateValue()
{
	return m_bSendActivate;
}

void cSerialIOBase::SetSendActivate(bool bSendActivate)
{
	m_bSendActivate = bSendActivate;
}

bool cSerialIOBase::GetPortActivateValue()
{
	return m_bPortActivate;
}

void cSerialIOBase::SetPortActivate(bool bPortActivate)
{
	m_bPortActivate=bPortActivate;
}

bool cSerialIOBase::GetProcessActivateValue()
{
	return m_bProccessActivate;
}

void cSerialIOBase::SetProcessActivate(bool bProcessActivate)
{
	m_bProccessActivate=bProcessActivate;
}