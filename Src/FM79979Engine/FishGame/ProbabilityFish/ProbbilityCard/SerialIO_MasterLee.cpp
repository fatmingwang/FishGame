#include "../stdafx.h"
#include "SerialIO_MasterLee.h"
#include "ComportFishData.h"
#include "SendGameStartPacket.h"
#include "Probability_MasterLee.h"
#include "SerialThread.h"
#include "../../GameApp/GameApp.h"
#include "../../Monster/Monster.h"
#include "../../Bullet/Bullet.h"
#include "../../PlayerBehavior/PlayerData.h"
#include "../../ProbabilityFish/FishProbability.h"


cSerialIO_MasterLee::cSerialIO_MasterLee()
{
	if( !m_spErrorLogFile && m_sbWriteErrorLog )
	{
		m_spErrorLogFile = new cBinaryFile();
		std::wstring	l_strSystemTime = L"MasterLog";
		l_strSystemTime += UT::GetSystemTimeForFile(true);
		l_strSystemTime += L".txt";
		m_spErrorLogFile->Writefile( UT::WcharToChar(l_strSystemTime.c_str()).c_str(), false, false );
	}
	//
	m_strPortName = L"COM";
	m_strPortName += ValueToStringW( cSerialIO_MasterLee::m_siCom );
	//
	m_bControlPaneStartDataSend = false;
	m_pSendPacket_GameStart = new sSendGameStartPacket(this);
	//
}








cSerialIO_MasterLee::~cSerialIO_MasterLee()
{
	this->ClosePort();
	SAFE_DELETE(m_pSendPacket_GameStart);
	Sleep(100);
	SAFE_DELETE(m_spErrorLogFile);
}

bool	cSerialIO_MasterLee::IsDataMatch()
{
	cVectorDataSynchornized<sReceiveFishData> *l_pVector_ReceivedFishData_Last = cFishApp::m_spProbabilityFish->m_pProbability_MasterLee->GetReceivedFishData();
	cFUSynchronizedHold hold( l_pVector_ReceivedFishData_Last );
	//
	for( size_t i=0;i<l_pVector_ReceivedFishData_Last->size() ;++i )
	{
		sReceiveFishData *l_pReceivedFishData_0 = (*l_pVector_ReceivedFishData_Last)[i];
		if( cFishApp::m_spProbabilityFish->m_pProbability_MasterLee->m_pSendDataWithMonsterList->size() > 0 )
		{
			sSendDataWithMonster *l_pSendDataWithMonster_MasterLee = (*cFishApp::m_spProbabilityFish->m_pProbability_MasterLee->m_pSendDataWithMonsterList)[0];
			if( l_pReceivedFishData_0->cNumFishHitted != 0 && l_pSendDataWithMonster_MasterLee->m_SendData.iNumFishHitted == l_pReceivedFishData_0->cNumFishHitted )
			{
				for( int i=0; i<l_pReceivedFishData_0->cNumFishHitted; ++i )
				{
					int	l_iCurrentIndex = i;
					sHittedFish_Send	l_Sent_HittedFish		= l_pSendDataWithMonster_MasterLee->m_SendData.HittedFish[l_iCurrentIndex];
					sHittedFish			l_Received_HittedFish	= l_pReceivedFishData_0->HittedFish[l_iCurrentIndex];
					if( l_Sent_HittedFish.usID != l_Received_HittedFish.usID )
					{
						return false;
					}
				}
			}
			else
			{
				return false;
			}
		}
	}
	return true;
}

void	cSerialIO_MasterLee::ProcessFishDied( char *e_szPacket, int e_nLength )
{
	++g_iNumPacketHitReceived;
	cVectorDataSynchornized<sReceiveFishData> *l_pVector_ReceivedFishData_Last = cFishApp::m_spProbabilityFish->m_pProbability_MasterLee->GetReceivedFishData();
	//is any buffer process?
	if( l_pVector_ReceivedFishData_Last->size() > 0 )
		return;
	//
	unsigned char	l_szDataPacket[ MAX_SEND_BUFFER ];
	memcpy( l_szDataPacket, e_szPacket, e_nLength);
	if(m_sbWriteLog)
	{
		std::string l_str2 = UT::ComposeMsgByFormat( "========================Received:%d=================", g_iNumPacketHitReceived );
		WriteLog( l_str2.c_str(), false );
	}
	//how many fish.
	int	l_nReceiveData_How_Many_Fishes = l_szDataPacket[ RECEIVE_HOW_MANY_FISHES ];
	unsigned char	l_szCheckSum = 0;
	int	l_iHeaderSize = RECEIVE_HEAD_SIZE;
	int	l_iNextFishSize = NEXT_FISH_SIZE; //2 for fish id,1 for player id,4 for fish score.
	int	l_iCheckSumPos = l_iHeaderSize + (l_nReceiveData_How_Many_Fishes * l_iNextFishSize);
	int	l_iPackEndCodeSize = 2;//0x68,0x16
	int	l_iTotalSize = l_iCheckSumPos+l_iPackEndCodeSize;
	std::wstring	l_Value;//for debug
	for( int i=0;i<l_iCheckSumPos;++i )
	{
		l_Value += ValueToStringW( (int)l_szDataPacket[i] );
		l_Value += L",";
		l_szCheckSum += l_szDataPacket[i];
	}

	int	l_iFirstFishID =  *(unsigned short*)&l_szDataPacket[RECEIVE_FIRST_MONSTER_ID_DATA_INDEX];
	bool	l_bCheckSumOk = true;
	if( l_szCheckSum != l_szDataPacket[l_iCheckSumPos] || g_iWaitingProcessSingnalID != l_iFirstFishID )
	{
		if( m_spErrorLogFile )
		{
			std::string	l_strErrorMsg;
			if(g_iWaitingProcessSingnalID != l_iFirstFishID)
				l_strErrorMsg = "ProcessFishDied ID error";
			else
				l_strErrorMsg = "ProcessFishDied checksum error";
			WriteLog( l_strErrorMsg.c_str(), true );
			WriteComportDataLog( e_szPacket, e_nLength, "data ",m_spErrorLogFile );
		}
		//if( l_szCheckSum != l_szDataPacket[l_iCheckSumPos] )
		//{
		//}
		//
		return;
	}
	else
	{//ensure we have received the same ID packet ir it will resend last buffer until we have get packet.
		if(!IsDataMatch())
			return;
	}
	if( l_iTotalSize+1 != e_nLength )
	{//
		if( m_spErrorLogFile )
			WriteComportDataLog( e_szPacket, e_nLength, "ProcessFishDied packet length not same",m_spErrorLogFile );
		PurgeComm(this->m_pSerialThread->serialCtrl()->GetPortHandle(),PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
	}
	else
	{
		int a=0;
	}
	//
	sReceiveFishData *l_pVector_ReceivedFishData_New = new sReceiveFishData();
	l_pVector_ReceivedFishData_New->cNumFishHitted = l_nReceiveData_How_Many_Fishes;
	int	l_iStartIndex = RECEIVE_HEAD_SIZE;

	//OutputDebugString(L"\nProcessFishDied:");
	for( int i=0; i<l_nReceiveData_How_Many_Fishes; ++i )
	{
		unsigned char *l_szDataTmp = &l_szDataPacket[ l_iStartIndex + (l_iNextFishSize*i) ];
		//3) 打中鱼返回信息： 2字节鱼信息编号 + 1字节玩家编号 + 4字节当前鱼得分信息
		memcpy( &l_pVector_ReceivedFishData_New->HittedFish[i].usID, l_szDataTmp, sizeof(unsigned short));		//魚的 ID
		l_pVector_ReceivedFishData_New->HittedFish[i].ucPlayerID = l_szDataTmp[2];								//玩家 ID
		memcpy( &l_pVector_ReceivedFishData_New->HittedFish[i].uiFishOutMoney, &l_szDataTmp[3], sizeof(int) );	//魚的分數
	}

	int l_nIndexID = -1;
	cFUSynchronizedHold hold( l_pVector_ReceivedFishData_Last );
	l_pVector_ReceivedFishData_Last->push_back( l_pVector_ReceivedFishData_New );
	cFishApp::m_spProbabilityFish->m_pProbability_MasterLee->SetReceivedFishDataFromIO( true );
}








void	cSerialIO_MasterLee::ProcessKey(char *inPacket,int inLength)
{
	int	l_iKeyCount = 5;
	if(cGameApp::m_sbDeviceExist)
	{//up,down,left,right,confirm
		char	l_pKeyData[] = {40,38,37,39,13};
		for( int i=0;i<l_iKeyCount;++i )
		{
			cGameApp::m_sucKeyData[l_pKeyData[i]] = false;
		}
	}
	else
	{
		char	l_pKeyData[] = {'I','A','Q','W','E'};
		for( int i=0;i<l_iKeyCount;++i )
		{
			cGameApp::m_sucKeyData[l_pKeyData[i]] = false;
		}
	}

	int	l_iKeyValue = inPacket[3];
	
}








void	cSerialIO_MasterLee::ProcessFishLuckyRoulette( char *inPacket, int inLength )
{
	//received data and send player data to master li.
}








void cSerialIO_MasterLee::OnEventClose( bool bSuccess )
{
	//UT::ErrorMsg(L"Comport cloase",bSuccess?L"okay":L"failed");
}








void	cSerialIO_MasterLee::WriteLog( const char*e_strError, bool e_bWriteToErrorLogFile )
{
	if( m_sbWriteLog )
	{
		cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine( e_strError );
	}
	//


	if( e_bWriteToErrorLogFile )
	{
		if( m_spErrorLogFile )
		{
			m_spErrorLogFile->WriteToFileImmediatelyWithLine( e_strError );
		}
	}
}








void cSerialIO_MasterLee::OnEventRead( char *e_szPacket, int e_nLength )
{
	cFishApp::m_spProbabilityFish->m_pProbability_MasterLee->m_fIOTimeOutCheck = 0;
	static UT::sTimeAndFPS	l_sTimegap;
	l_sTimegap.Update();
	if(l_sTimegap.fElpaseTime > 0.5f )
	{
		WriteLog( UT::ComposeMsgByFormat("receive data delay,past time%.3f", l_sTimegap.fElpaseTime).c_str(), true );
		if( m_spErrorLogFile )
			WriteComportDataLog(e_szPacket,e_nLength, "receive data delay:",m_spErrorLogFile);
	}
	//
	++g_iNumPacketReceived;
	//
	WriteLog( UT::ComposeMsgByFormat( "ReceivedTimes:%d", g_iNumPacketReceived ).c_str(), false );
	WriteComportDataLog( e_szPacket, e_nLength, "OnEventRead:",cGameApp::m_spLogFile );
	//
	if( !m_pSendPacket_GameStart )
		return;
	if( !m_pSendPacket_GameStart->IsDone() && cSerialIO_MasterLee::m_sbReceiveStartSingnal )
	{
		m_pSendPacket_GameStart->Received( e_szPacket );
		return;
	}
	//
	unsigned char l_szDataTmp1[ MAX_SEND_BUFFER ];
	memcpy( l_szDataTmp1, e_szPacket, e_nLength );
	if( l_szDataTmp1[0] == 0xaa && l_szDataTmp1[1] == 0xe1 && l_szDataTmp1[2] == 0xcc )//check header
	{
		ProcessFishDied( e_szPacket, e_nLength );
	}
	else
	if( l_szDataTmp1[0] == 0x77 && l_szDataTmp1[1] == 0x77 && l_szDataTmp1[2] == 0xcc )//check header
	{
		ProcessFishLuckyRoulette( e_szPacket, e_nLength );
	}
	else
	if( l_szDataTmp1[0] == 0xaa && l_szDataTmp1[1] == 0xe2 && l_szDataTmp1[2] == 0xcc )//check header
	{
		++g_iNumPacketEmptyReceived;
	}
	else
	{
		PurgeComm(this->m_pSerialThread->serialCtrl()->GetPortHandle(),PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
	}
}








void	cSerialIO_MasterLee::WriteComportDataLog( const char *e_pData, int e_iLength, char *e_strDescription,cBinaryFile*e_pcBinaryFile )
{
	if( e_pcBinaryFile )
	{
		std::string	l_strOutLog = e_strDescription;
		for( int i=0;i<e_iLength;++i )
		{
			char	l_Data[MAX_PATH];
			sprintf(l_Data,"%02X ",(unsigned char)e_pData[i]);
			l_strOutLog += l_Data;
		}
		e_pcBinaryFile->WriteToFileImmediatelyWithLine(l_strOutLog.c_str());
	}
}








bool	cSerialIO_MasterLee::ReSendLastPacket()
{
	if( GetSendActivateValue() == TRUE )
		return false;
	if( cFishApp::m_spProbabilityFish->m_pProbability_MasterLee->Get_SentData( (unsigned char*)m_sstrReSendPacket, &m_siReSendSize) )
	{
		Write( m_sstrReSendPacket, m_siReSendSize );
		if( m_spErrorLogFile )
		{
			WriteComportDataLog(m_sstrReSendPacket,m_siReSendSize, "ReSendLastPacket:",m_spErrorLogFile);
		}
		++g_iNumPacketResend;
		return true;
	}
	//
	if( m_spErrorLogFile )
		m_spErrorLogFile->WriteToFileImmediatelyWithLine("resend failed");
	return false;
}

void cSerialIO_MasterLee::OnEventWrite( int nWritten )
{
	static	int l_siTimes = 0;
	WriteLog( UT::ComposeMsgByFormat( "SendTimes:%d", l_siTimes ).c_str(), false );
	WriteComportDataLog( m_sendBuffer, m_sendSize, "OnEventWrite:",cGameApp::m_spLogFile );
	++l_siTimes;
}









void	SendEmptyPacket(cSerialIO_MasterLee*e_pLiSchoolMasterSerialIO)
{
	unsigned char	l_cData[MAX_SEND_BUFFER];
	l_cData[0] = 0x55;
	l_cData[1] = 0xe2;
	l_cData[2] = 0xcc;
	l_cData[3] = (unsigned char)(0x55+0xe2+0xcc);
	l_cData[4] = 0x68;
	l_cData[5] = 0x16;
	//l_cData[6] = 0x00;
	e_pLiSchoolMasterSerialIO->Write( (char*)l_cData, 6 );
}

void cSerialIO_MasterLee::OnEventOpen( bool bSuccess )
{
	if( m_sbWriteLog )
	{
		std::string	l_str = UT::ComposeMsgByFormat( "comport open %s", bSuccess ? "ok" : "failed" );
		WriteLog( l_str.c_str(), true );
	}
	//
	if( bSuccess )
	{
		//send control panel data to master li.
		//cFishApp::m_spControlPanel->m_fProbability;
		//cFishApp::m_spControlPanel->m_bLuckyTime;
		//SendEmptyPacket( cSerialIO_MasterLee * e_pLiSchoolMasterSerialIO );
	}
	else
	{
		cFishApp::m_spProbabilityFish->SetShow_MasterLeeIOErrorMessage(true);
		//g_pIOSMDriverInterface->SetIOError(true);
	}
}









