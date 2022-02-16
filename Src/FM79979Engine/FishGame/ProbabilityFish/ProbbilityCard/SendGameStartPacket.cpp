#include "../stdafx.h"
#include "SendGameStartPacket.h"
#include "SerialIO_MasterLee.h"
#include "../../GameApp/GameApp.h"
#include "../FishProbability.h"




void	GameStartSend( void *e_pData )
{
	cSerialIO_MasterLee*l_pLiSchoolMasterSerialIO = (cSerialIO_MasterLee*)e_pData;
	unsigned char	l_cData[MAX_SEND_BUFFER];
	int	l_iByteIndex = 0;
	l_cData[l_iByteIndex] = 0x55;++l_iByteIndex;//1
	l_cData[l_iByteIndex] = 0xe0;++l_iByteIndex;//1
	l_cData[l_iByteIndex] = 0xcc;++l_iByteIndex;//1

	SYSTEMTIME l_st;
	GetLocalTime(&l_st);
	l_cData[l_iByteIndex] = (unsigned char)(l_st.wYear%100);	++l_iByteIndex;//should be 2 but 1 as masterLi ask
	l_cData[l_iByteIndex] = (unsigned char)l_st.wMonth;	++l_iByteIndex;//1
	l_cData[l_iByteIndex] = (unsigned char)l_st.wDay;	++l_iByteIndex;//1

	cEventVariable*l_pLatest_TotalIncome = cFishApp::m_spControlPanel->GetEventManager()->GetEventVariable(L"Latest_TotalIncome");
	cEventVariable*l_pLatest_TotalPay = cFishApp::m_spControlPanel->GetEventManager()->GetEventVariable(L"Latest_TotalPay");
	cEventVariable*l_pHistoryTotalIncome =	cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"TotalIncome");
	cEventVariable*l_pHistoryTotalPay =		cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"TotalPay");

	int	l_iCoinToSore = cFishApp::m_spControlPanel->GetCoinToScore();

	int64	l_i64TotalIn = *l_pLatest_TotalIncome->GetInt64()+(*l_pHistoryTotalIncome->GetInt64()*l_iCoinToSore);
	int64	l_i64TotalOut = *l_pLatest_TotalPay->GetInt64()+(*l_pHistoryTotalPay->GetInt64()*l_iCoinToSore);

	memcpy(&l_cData[l_iByteIndex],&l_i64TotalIn,sizeof(int64));l_iByteIndex += sizeof(int64);//8
	memcpy(&l_cData[l_iByteIndex],&l_i64TotalOut,sizeof(int64));l_iByteIndex += sizeof(int64);//8
	l_cData[l_iByteIndex] = 0;//check sum
	for( int i=0;i<l_iByteIndex;++i )	l_cData[l_iByteIndex] += l_cData[i];
	l_iByteIndex += 1;
	l_cData[l_iByteIndex] = 0x68;
	l_iByteIndex += 1;
	l_cData[l_iByteIndex] = 0x16;
	l_iByteIndex += 1;
	//l_cData[l_iByteIndex] = 0x00;
	//l_iByteIndex += 1;
	l_pLiSchoolMasterSerialIO->Write( (char*)l_cData, l_iByteIndex );
}








bool	GameStartReceived( void *e_pData )
{
	static int GameStartFailTimes(0);

	unsigned char*l_pData = (unsigned char*)e_pData;
	if( l_pData[0] == 0xaa && l_pData[1] == 0xe0 && l_pData[2] == 0xcc )
	{
		if(l_pData[3] == 0x01)
		{
			g_pIOSMDriverInterface->Init();
			GameStartFailTimes = 0;
			return true;
		}
	}

	GameStartFailTimes++;

	if ( GameStartFailTimes >= 15 )
	{
		cFishApp::m_spProbabilityFish->SetShow_MasterLeeIOErrorMessage( true );
		GameStartFailTimes = 0;
	}

	return false;
}








void	SendControlPanelData1( void *e_pData )
{
	cSerialIO_MasterLee*l_pLiSchoolMasterSerialIO = (cSerialIO_MasterLee*)e_pData;
	unsigned char	l_cData[MAX_SEND_BUFFER];
	int	l_iByteIndex = 0;

	l_cData[l_iByteIndex] = 0x55;++l_iByteIndex;//1
	l_cData[l_iByteIndex] = 0xe4;++l_iByteIndex;//1
	l_cData[l_iByteIndex] = 0xcc;++l_iByteIndex;//1
	cEventVariable *l_pProbability = cFishApp::m_spControlPanel->GetEventManager()->GetEventVariable(L"Probability");
	int	l_Probability = *l_pProbability->GetInt();

	int	l_iTargetValue = l_Probability;
	sSendGameStartPacket::siGameMode = l_iTargetValue;
	//if( l_Probability >= 100 )		l_iTargetValue = 1;
	//if( l_Probability >= 98 )		l_iTargetValue = 2;
	//if( l_Probability >= 96 )		l_iTargetValue = 3;
	//if( l_Probability >= 94 )		l_iTargetValue = 4;
	//if( l_Probability >= 92 )		l_iTargetValue = 5;
	//if( l_Probability >= 90 )		l_iTargetValue = 6;
	//if( l_Probability >= 88 )		l_iTargetValue = 7;
	//if( l_Probability >= 86 )		l_iTargetValue = 8;
	//if( l_Probability >= 86 )		l_iTargetValue = 9;
	//if( l_Probability >= 86 )		l_iTargetValue = 10;
	//fuck
	/*l_iTargetValue = 5;*/

	l_cData[l_iByteIndex] = (unsigned char)l_iTargetValue; //Ãø«×
	++l_iByteIndex;

	l_cData[l_iByteIndex] = 0;//check sum
	for( int i=0;i<l_iByteIndex;++i )	l_cData[l_iByteIndex] += l_cData[i];
	l_iByteIndex += 1;
	l_cData[l_iByteIndex] = 0x68;
	l_iByteIndex += 1;
	l_cData[l_iByteIndex] = 0x16;
	l_iByteIndex += 1;
	//l_cData[l_iByteIndex] = 0x00;
	//l_iByteIndex += 1;
	l_pLiSchoolMasterSerialIO->Write( (char*)l_cData, l_iByteIndex );
}









bool	ReceiveControlPanelData1 (void *e_pData )
{
	unsigned char*l_pData = (unsigned char*)e_pData;
	if( l_pData[0] == 0xaa && l_pData[1] == 0xe4 && l_pData[2] == 0xcc )
	{
		if( sSendGameStartPacket::siGameMode == l_pData[3] || ++sSendGameStartPacket::siErrorCount>= 10 )
		{
			sSendGameStartPacket::siErrorCount = 0;
#ifdef DEBUG
			OutputDebugString(L"game mode ok\n");
#endif
			return true;
		}
		++sSendGameStartPacket::siErrorCount;
	}
	return false;
}








void	SendControlPanelData2( void *e_pData )
{
	cSerialIO_MasterLee*l_pLiSchoolMasterSerialIO = (cSerialIO_MasterLee*)e_pData;
	unsigned char	l_cData[MAX_SEND_BUFFER];
	int	l_iByteIndex = 0;

	l_cData[l_iByteIndex] = 0x55;++l_iByteIndex;//1
	l_cData[l_iByteIndex] = 0xe6;++l_iByteIndex;//1
	l_cData[l_iByteIndex] = 0xcc;++l_iByteIndex;//1

	int	l_iTargetValue = *cFishApp::m_spControlPanel->m_pEventManager->m_pEventVariableManager->GetObject( L"PlaceType" )->GetInt();
	sSendGameStartPacket::siPlace = l_iTargetValue;
	l_cData[l_iByteIndex] = (unsigned char)l_iTargetValue;++l_iByteIndex;
	l_cData[l_iByteIndex] = 0;//check sum
	for( int i=0;i<l_iByteIndex;++i )	l_cData[l_iByteIndex] += l_cData[i];
	l_iByteIndex += 1;
	l_cData[l_iByteIndex] = 0x68;
	l_iByteIndex += 1;
	l_cData[l_iByteIndex] = 0x16;
	l_iByteIndex += 1;
	//l_cData[l_iByteIndex] = 0x00;
	//l_iByteIndex += 1;
	l_pLiSchoolMasterSerialIO->Write( (char*)l_cData, l_iByteIndex );
}








bool	ReceiveControlPanelData2( void *e_pData )
{
	unsigned char*l_pData = (unsigned char*)e_pData;
	if(	l_pData[0] == 0xaa && l_pData[1] == 0xe6 && l_pData[2] == 0xcc )
	{
		if(sSendGameStartPacket::siPlace == l_pData[3] || sSendGameStartPacket::siErrorCount >= 10 )
		{
			sSendGameStartPacket::siErrorCount = 0;
#ifdef DEBUG
			OutputDebugString(L"place ok\n");
#endif
			return true;
		}
		++sSendGameStartPacket::siErrorCount;
	}
	return false;
}

int		sSendGameStartPacket::siGameMode = 6;//0~9
int		sSendGameStartPacket::siPlace = 0;//0~3
int		sSendGameStartPacket::siErrorCount = 0;

sSendGameStartPacket::sSendGameStartPacket( cSerialIO_MasterLee *e_pLiSchoolMasterSerialIO )
{
	siErrorCount = 0;
	siGameMode = 6;//0~9
	siPlace = 0;//0~3
	m_pSerialIO_MasterLee = e_pLiSchoolMasterSerialIO;
	//
	m_iCurrentStep = 0;
	{
		sPacketSendAndReceivedProcess*l_pPacketSendAndReceivedProcess = new sPacketSendAndReceivedProcess(GameStartSend,GameStartReceived);
		m_StartSendDataList.push_back(l_pPacketSendAndReceivedProcess);
	}
	{
		sPacketSendAndReceivedProcess*l_pPacketSendAndReceivedProcess = new sPacketSendAndReceivedProcess(SendControlPanelData1,ReceiveControlPanelData1);
		m_StartSendDataList.push_back(l_pPacketSendAndReceivedProcess);
	}
	{
		sPacketSendAndReceivedProcess*l_pPacketSendAndReceivedProcess = new sPacketSendAndReceivedProcess(SendControlPanelData2,ReceiveControlPanelData2);
		m_StartSendDataList.push_back(l_pPacketSendAndReceivedProcess);
	}
}








sSendGameStartPacket::~sSendGameStartPacket()
{
	DELETE_VECTOR(m_StartSendDataList,sPacketSendAndReceivedProcess*);
}








bool	sSendGameStartPacket::IsDone()
{
	return m_iCurrentStep == (int)m_StartSendDataList.size() ? true : false;
}








void	sSendGameStartPacket::Received( char *e_pData )
{
	if( m_iCurrentStep <= (int)m_StartSendDataList.size() )
	{
		if( m_StartSendDataList[m_iCurrentStep]->Received( e_pData ) )
		{
			++m_iCurrentStep;
		}
	}
}








void	sSendGameStartPacket::Send()
{
	if( m_iCurrentStep < (int)m_StartSendDataList.size()  )
	{
		m_StartSendDataList[m_iCurrentStep]->Send( m_pSerialIO_MasterLee );
	}
}








