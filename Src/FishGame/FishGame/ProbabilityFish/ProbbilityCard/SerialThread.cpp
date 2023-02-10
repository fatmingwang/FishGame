#include "../stdafx.h"
#include "SerialThread.h"
#include "SerialIOBase.h"
#include "SerialIO_MasterLee.h"
#include "SendGameStartPacket.h"
#include "../../GameApp/GameApp.h"
#include "../../ProbabilityFish/FishProbability.h"
#include "../../Monster/MonsterManager.h"
//
float	cSerialThread::m_sfTargetTimeToWrite = 0.05f;
cSerialCtrl *cSerialThread::m_spSerialCtrl = 0;

cSerialThread::cSerialThread() :m_pSerialIO_Base(NULL)
{
	m_fCurrentTimeToWrite = 0.f;
	m_sfTargetTimeToWrite = 0.05f;
	if( !m_spSerialCtrl )
	{
		m_spSerialCtrl = new cSerialCtrl();
	}
	Spawn(1);
}

cSerialThread::~cSerialThread()
{
	m_pSerialIO_Base = NULL;
	WaitForIdle();
	Clear();
	SAFE_DELETE(m_spSerialCtrl);
}

void cSerialThread::Clear()
{
	cFUThreadPool::Clear();
}

bool cSerialThread::InitInstance()
{
	return TRUE;
}

int cSerialThread::Run()
{
	UT::sTimeAndFPS	l_sTimeAndFPS;
	l_sTimeAndFPS.Update();
	//send and read must be iterator
	bool	l_bSendSingnal = true;
	// Check signal controlling and status to open serial communication.
	cSerialIO_MasterLee*l_pLiSchoolMasterSerialIO = (cSerialIO_MasterLee*)m_pSerialIO_Base;
	int		l_iReadPacketFailed = 0;
	while(1)
	{
		while( m_pSerialIO_Base->GetProcessActivateValue() == TRUE )
		{
			l_sTimeAndFPS.Update();
			m_fCurrentTimeToWrite += l_sTimeAndFPS.fElpaseTime;
			//
			bool	l_bSignalGo = false;
			float	l_fTargetTime = m_sfTargetTimeToWrite;
			if( !l_pLiSchoolMasterSerialIO->m_bControlPaneStartDataSend && cSerialIO_MasterLee::m_sbReceiveStartSingnal )
			{
				if( m_pSerialIO_Base->GetSendActivateValue() == FALSE && !l_bSendSingnal )//wait a little longer
					l_fTargetTime *= 2.f;
			}
			if( m_fCurrentTimeToWrite >= l_fTargetTime )
			{
				//m_fCurrentTimeToWrite -= l_fTargetTime;
				m_fCurrentTimeToWrite = 0.f;
				l_bSignalGo = true;
			}
			//
			if ((serialCtrl()->GetPortStatus()==FALSE)&&m_pSerialIO_Base->GetPortActivateValue()==TRUE)
			{
				bool	l_bSuccess = false;
				if(serialCtrl()->OpenPort(m_pSerialIO_Base->m_DCB,m_pSerialIO_Base->m_strPortName.c_str())==TRUE)
				{
					l_bSuccess = true;
					m_pSerialIO_Base->OnEventOpen( TRUE );
					PurgeComm(serialCtrl()->GetPortHandle(),PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
				}
				if( !l_bSuccess )
				{
					for( int i=3;i<17;++i )
					{
						std::wstring	l_Com = L"COM";
						l_Com += ValueToStringW(i);
						//m_pSerialIO_Base->m_strPortName
						if(serialCtrl()->OpenPort(m_pSerialIO_Base->m_DCB,l_Com.c_str())==TRUE)
						{
							m_pSerialIO_Base->OnEventOpen( TRUE );
							PurgeComm(serialCtrl()->GetPortHandle(),PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
							i=79979;
							//OutputDebugString(l_Com.c_str());
							//OutputDebugString(L"\n");
							l_bSuccess = true;
						}
					}
				}
				if( !l_bSuccess )
				{
					m_pSerialIO_Base->OnEventOpen(FALSE);
					m_pSerialIO_Base->SetPortActivate(FALSE);
				}
			}
			//
			if( l_bSignalGo )
			{
				if( m_pSerialIO_Base->GetSendActivateValue() == TRUE && l_bSendSingnal == true )
				{
					l_bSendSingnal = false;
					//for start packet.
					if( !l_pLiSchoolMasterSerialIO->m_bControlPaneStartDataSend && cSerialIO_MasterLee::m_sbReceiveStartSingnal )
					{
						assert(l_pLiSchoolMasterSerialIO->m_pSendPacket_GameStart);
						if( l_pLiSchoolMasterSerialIO->m_pSendPacket_GameStart )
						{
							l_pLiSchoolMasterSerialIO->m_pSendPacket_GameStart->Send();
							if(l_pLiSchoolMasterSerialIO->m_pSendPacket_GameStart->IsDone())
							{
								l_pLiSchoolMasterSerialIO->m_bControlPaneStartDataSend = true;
								cFishApp::m_spMonsterManager->m_FishShowProbabilityList.bPauseGenerateMonster = false;
							}
						}
					}
					unsigned long nWritten;
					if( serialCtrl()->Write( m_pSerialIO_Base->m_sendBuffer, m_pSerialIO_Base->m_sendSize, nWritten ) == TRUE )
					{
						m_pSerialIO_Base->OnEventWrite( nWritten );
					}
					m_pSerialIO_Base->SetSendActivate( FALSE );
				}
				else
				if( m_pSerialIO_Base->GetPortActivateValue()==TRUE && l_bSendSingnal == false )
				{//give a fake send singnal
					l_bSendSingnal = true;
					char message[MAX_SEND_BUFFER]={0};
					//unsigned int lenBuff = MAX_SEND_BUFFER/2;
					unsigned int lenBuff = MAX_SEND_BUFFER;
					unsigned long lenMessage;
					if(serialCtrl()->Read(message,lenBuff,lenMessage)==TRUE)
					{
						if(lenMessage>0)
						{
						  m_pSerialIO_Base->OnEventRead(message,lenMessage);
						  l_iReadPacketFailed = 0;
						}
					}
					else
					{
						++l_iReadPacketFailed;
						if( l_iReadPacketFailed > 20 )
						{
							cFishApp::m_spProbabilityFish->SetShow_MasterLeeIOErrorMessage(true);
							l_iReadPacketFailed = 0;
						}
					}
				}
			}
			//
			if (m_pSerialIO_Base->m_bClosePort==TRUE)
			{
				if (serialCtrl()->ClosePort()==TRUE)
				{
					m_pSerialIO_Base->OnEventClose(TRUE);
				}
				else
				{
					m_pSerialIO_Base->OnEventClose(FALSE);
				}
				m_pSerialIO_Base->m_bClosePort=FALSE;
				//
				return 0;
			}
		}

	}
	return 0;
}

void cSerialThread::ClosePort()
{

	serialCtrl()->ClosePort();

}