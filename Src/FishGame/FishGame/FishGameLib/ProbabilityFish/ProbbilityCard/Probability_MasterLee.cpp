#include "../stdafx.h"
#include "Probability_MasterLee.h"
//
#include "SerialIO_MasterLee.h"
#include "ComportFishData.h"
#include "SerialThread.h"
#include "Probability_MasterLee.h"
//
#include "../FishProbability.h"
//
#include "../../MiniGame/MiniGameManager.h"
#include "../../GameApp/GameApp.h"
#include "../../Monster/MonsterManager.h"
#include "../../Bullet/Bullet.h"
#include "../../PlayerBehavior/PlayerData.h"
int				g_iExchangeRateValueForSpecialVersion = 1;
bool			g_bSpecialVersion = false;
//
int				cSerialIO_MasterLee::m_siCom = 1;
float			cSerialIO_MasterLee::m_sfTimeOut = 1.f;
float			cSerialIO_MasterLee::m_sfComportIOTimeOutError = 3.f;
bool			cSerialIO_MasterLee::m_sbReceiveStartSingnal = true;
char			cSerialIO_MasterLee::m_sstrReSendPacket[MAX_SEND_BUFFER];
int				cSerialIO_MasterLee::m_siReSendSize;
bool			cSerialIO_MasterLee::m_sbWriteLog = false;
cBinaryFile*	cSerialIO_MasterLee::m_spErrorLogFile = 0;
bool			cSerialIO_MasterLee::m_sbWriteErrorLog = false;
unsigned short	cProbability_MasterLee::m_susCurrentID = 0;
//
//time out check
//
float			m_fIOTimeOutCheck = 0.f;
int				g_iNumPacketSendFailed = 0;
int				g_iNumIOTimeout = 0;//
int				g_iNumPacketSend = 0;
int				g_iNumPacketReceived = 0;
int				g_iNumPacketHitSend = 0;
int				g_iNumPacketHitReceived = 0;
int				g_iNumPacketEmptySend = 0;
int				g_iNumPacketEmptyReceived = 0;
int				g_iNumPacketResend = 0;
int				g_iNumFishesCouldBeSend = 99; //how MAX_SEND_BUFFER could store the fishes hitted
int				g_iWaitingProcessSingnalID = 79979;
//







cProbability_MasterLee::cProbability_MasterLee()
{
	m_fIOTimeOutCheck = 0.f;
	m_fFishDiedTimeOutCheck = 0.f;
	//
	m_bReceivedFishDataFromIO = false;
	//
	m_pSendDataWithMonster = 0;
	m_pSendDataWithMonsterList = 0;
	m_vReceiveFishData = 0;
	m_pSerialIO_MasterLee = 0;
	//
	m_pSendDataWithMonster	= new sSendDataWithMonster;
	m_pSendDataWithMonsterList	= new std::vector<sSendDataWithMonster*>;
	m_vReceiveFishData		= new cVectorDataSynchornized<sReceiveFishData>;
	//
	m_pSerialIO_MasterLee = new cSerialIO_MasterLee();
	//
}








cProbability_MasterLee::~cProbability_MasterLee()
{
	SAFE_DELETE( m_pSendDataWithMonster );
	SAFE_DELETE( m_pSendDataWithMonsterList );
	SAFE_DELETE (m_vReceiveFishData );
	SAFE_DELETE( m_pSerialIO_MasterLee );
}

void cProbability_MasterLee::Initial()
{
	g_iExchangeRateValueForSpecialVersion = 1;
	g_bSpecialVersion = false;
	int	l_iBulletPayRateLimit = *cFishApp::m_spControlPanel->m_pEventManager->m_pEventVariableManager->GetObject(L"BulletPayRateLimit")->GetInt();
	cNodeISAX	l_NodeISAX;
	if(l_NodeISAX.ParseDataIntoXMLNode("Fish/Probability/ProbabilityRange.xml"))
	{
		TiXmlElement*l_pTiXmlElement = l_NodeISAX.GetRootElement();
		l_pTiXmlElement = l_pTiXmlElement->FirstChildElement();
		while( l_pTiXmlElement )
		{
			if(!wcscmp(l_pTiXmlElement->Value(),L"RangeWithProbability") )
			{
				const WCHAR*l_strRange = l_pTiXmlElement->Attribute(L"Range");
				POINT	l_Range = GetPoint(l_strRange);
				if( l_iBulletPayRateLimit >= l_Range.x && l_iBulletPayRateLimit<= l_Range.y  )
				{
					const WCHAR*l_strValue = l_pTiXmlElement->Attribute(L"Value");
					g_iExchangeRateValueForSpecialVersion = GetInt(l_strValue);
					if( g_iExchangeRateValueForSpecialVersion != 1 )
						g_bSpecialVersion = true;
				}
			}
			l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
		}
		//RangeWithProbability;
	}
	else
	{
		if( l_iBulletPayRateLimit != 1000 )
		{
			g_bSpecialVersion = true;
			if( l_iBulletPayRateLimit <= 10 )
			{
				l_iBulletPayRateLimit = 100;
			}
			else
			if( l_iBulletPayRateLimit <= 20 )
			{
				l_iBulletPayRateLimit = 50;
			}
			else
			if( l_iBulletPayRateLimit <= 40 )
			{
				l_iBulletPayRateLimit = 25;
			}
			else
			if( l_iBulletPayRateLimit <= 50 )
			{
				l_iBulletPayRateLimit = 20;
			}
			else
			if( l_iBulletPayRateLimit <= 100 )
			{
				l_iBulletPayRateLimit = 10;
			}
			else
			if( l_iBulletPayRateLimit <= 200 )
			{
				l_iBulletPayRateLimit = 5;
			}
			else
			if( l_iBulletPayRateLimit <= 500 )
			{
				l_iBulletPayRateLimit = 2;
			}
			else
			if( l_iBulletPayRateLimit <= 1000 )
			{
				l_iBulletPayRateLimit = 1;
			}
			g_iExchangeRateValueForSpecialVersion = l_iBulletPayRateLimit;
		}
	}
	Sleep(500);
}








void cProbability_MasterLee::Render()
{
	if( cSerialThread::serialCtrl() )
	{
		cSerialThread::serialCtrl()->DebugRender();
	}
}

void	cProbability_MasterLee::ProcessHittedFishes(float e_fElpaseTime)
{
	for( int i=0; i<cFishApp::m_spMonsterManager->m_iNumMonsterHitted; ++i )
	{
		sHittedFishData* l_pHittedFishData = &cFishApp::m_spMonsterManager->m_pHittedFish[i];
//		if( m_pSendDataWithMonster_MasterLee->m_SendData.cNumFishHitted < g_iNumFishesCouldBeSend )
		{
			sHittedFish_Send	l_sHittedFish_Send;
			l_sHittedFish_Send.ucBulletValue = l_pHittedFishData->m_pBullet->GetPayRate();
			l_sHittedFish_Send.ucPlayerID = l_pHittedFishData->m_pPlayer->GetPlayerID();
			l_sHittedFish_Send.uiFishOutMoney = l_pHittedFishData->m_pPlayer->GetPayRateMultier() * l_pHittedFishData->m_pMonster->GetPrize();
			l_sHittedFish_Send.usID = m_susCurrentID;

			sCompareData_MasterLee *l_pCompareData = new sCompareData_MasterLee(
					l_pHittedFishData->m_pMonster,
					l_pHittedFishData->m_pPlayer,
					l_pHittedFishData->m_pBullet,
					l_pHittedFishData->m_pMonster->IsMiniGameMonster()?l_pHittedFishData->m_pMonster->m_strMiniGameName.c_str():0 );

			if( l_pHittedFishData->m_pMonster->IsMiniGameMonster() )
			{
				int	l_iMiniGamePrize = cFishApp::m_spMiniGameManager->GetMiniGamePayRate(l_pHittedFishData->m_pMonster->m_strMiniGameName.c_str());
				if( l_iMiniGamePrize != 0 )
					l_sHittedFish_Send.uiFishOutMoney = l_iMiniGamePrize;
			}
			m_pSendDataWithMonster->m_SendData.HittedFish[m_pSendDataWithMonster->m_SendData.iNumFishHitted] = l_sHittedFish_Send;
			//
			m_pSendDataWithMonster->m_CompareDataList.push_back(l_pCompareData);
			++m_pSendDataWithMonster->m_SendData.iNumFishHitted;
			//
			++m_susCurrentID;
			if( m_susCurrentID > 254 )	//9999
			{
				m_susCurrentID = 0;
			}
		}
//		else
//		{//lost packet?
//			cFishApp::m_spProbabilityFish->MonsterToKeepGoing( l_pHittedFishData, 0.05f );
//			cSerialIO_MasterLee::WriteLog( "over than 110 fishes in one package", true );
//		}
	}
}

void	cProbability_MasterLee::ProcessSingnal(float e_fElpaseTime)
{
	if( m_pSerialIO_MasterLee->GetSendActivateValue() == false )
	{
		size_t l_uiSendDataWithMonsterListSize = m_pSendDataWithMonsterList->size();
		if( m_pSendDataWithMonster->m_SendData.iNumFishHitted == 0 ) //79979
		{//if fishes died message is not wait for process send empty message let comport keep connecting!
			if( m_fFishDiedTimeOutCheck == 0.f )
			{
				SendEmptyPacket( m_pSerialIO_MasterLee );
				++g_iNumPacketEmptySend;
			}
		}
		else
		if( m_pSendDataWithMonster->m_SendData.iNumFishHitted != 0 && l_uiSendDataWithMonsterListSize == 0 )
		{//write into comport if fishes hitted
			++g_iNumPacketHitSend;
			int l_iCompareDataListSize = m_pSendDataWithMonster->m_CompareDataList.size();
			//avoid upload too much fishes
			if( l_iCompareDataListSize > g_iNumFishesCouldBeSend )
			{
				l_iCompareDataListSize = g_iNumFishesCouldBeSend;
			}
			//
			unsigned char	l_cData[MAX_SEND_BUFFER];
			memset( l_cData, 0, MAX_SEND_BUFFER );
			int	l_iCurrentIndex = 0;
			l_cData[l_iCurrentIndex] = 0x55;			l_iCurrentIndex += 1;
			l_cData[l_iCurrentIndex] = 0xe1;			l_iCurrentIndex += 1;
			l_cData[l_iCurrentIndex] = 0xcc;			l_iCurrentIndex += 1;
			l_cData[l_iCurrentIndex] = l_iCompareDataListSize;	l_iCurrentIndex += 1;
			sSendDataWithMonster *l_pSendDataWithMonster_MasterLee = new sSendDataWithMonster;
			m_pSendDataWithMonsterList->push_back(l_pSendDataWithMonster_MasterLee);			//
			l_pSendDataWithMonster_MasterLee->Copy( m_pSendDataWithMonster, l_iCompareDataListSize );
			m_pSendDataWithMonster->Clear( l_iCompareDataListSize );
			//get firs fish ID
			for( int i=0; i<l_iCompareDataListSize; ++i )
			{
				sHittedFish_Send *l_pHittedFish = &l_pSendDataWithMonster_MasterLee->m_SendData.HittedFish[i];
				short *l_pID = (short*)&l_cData[l_iCurrentIndex];
				*l_pID = l_pHittedFish->usID;
				if( i == 0 )
					g_iWaitingProcessSingnalID = l_pHittedFish->usID;
				//
				l_iCurrentIndex += 2;
				l_cData[l_iCurrentIndex] = l_pHittedFish->ucPlayerID;
				//
				l_iCurrentIndex += 1;
				short*l_pBuletValue = (short*)&l_cData[l_iCurrentIndex];
				*l_pBuletValue = l_pHittedFish->ucBulletValue;
				if( g_bSpecialVersion )
					*l_pBuletValue *= g_iExchangeRateValueForSpecialVersion;
				l_iCurrentIndex += 2;
				int*l_pScore = (int*)&l_cData[l_iCurrentIndex];
				*l_pScore = l_pHittedFish->uiFishOutMoney;
				l_iCurrentIndex += 4;

			}
			l_cData[l_iCurrentIndex] = 0;
			for( int i=0;i<l_iCurrentIndex;++i )
			{
				l_cData[l_iCurrentIndex] += l_cData[i];
			}
			l_pSendDataWithMonster_MasterLee->m_SendData.cChecksum = l_cData[l_iCurrentIndex];
			++l_iCurrentIndex;
			l_cData[l_iCurrentIndex] = 0x68;
			l_iCurrentIndex += 1;
			l_cData[l_iCurrentIndex] = 0x16;
			l_iCurrentIndex += 1;
			m_pSerialIO_MasterLee->Write( (char*)&l_cData, l_iCurrentIndex );
		}
	}
}

void	cProbability_MasterLee::ProcessTimeout(float e_fElpaseTime)
{
	//
	//time out check
	if( m_pSendDataWithMonsterList->size() > 0 )
	{
		m_fFishDiedTimeOutCheck += e_fElpaseTime;
	}
	m_fIOTimeOutCheck += e_fElpaseTime;
	//
	//time out
	if(m_pSendDataWithMonsterList->size() > 0 && m_fFishDiedTimeOutCheck > cSerialIO_MasterLee::m_sfTimeOut )
	{//clear all data
		if(m_pSerialIO_MasterLee->ReSendLastPacket())
			m_fFishDiedTimeOutCheck = 0.f;
	}
	//
	if( m_fIOTimeOutCheck > cSerialIO_MasterLee::m_sfComportIOTimeOutError )
	{
		g_iNumIOTimeout = (int)(m_fIOTimeOutCheck/cSerialIO_MasterLee::m_sfComportIOTimeOutError);
		cFishApp::m_spProbabilityFish->SetShow_MasterLeeIOErrorMessage(true);
		cSerialIO_MasterLee::WriteLog( "cSerialIO_MasterLee::m_sfComportIOTimeOutError error", true );
	}
}

void	cProbability_MasterLee::Update( float e_fElpaseTime )
{
	//
	if( m_bReceivedFishDataFromIO )
	{
		bool l_bResult = ProcessFishesDied();
		m_fIOTimeOutCheck = 0.f;
		m_fFishDiedTimeOutCheck = 0.f;
	}
	ProcessHittedFishes(e_fElpaseTime);
	ProcessSingnal(e_fElpaseTime);
	ProcessTimeout(e_fElpaseTime);
}








bool	cProbability_MasterLee::ProcessFishesDied()
{
	cVectorDataSynchornized<sReceiveFishData>	*l_pVector_ReceivedFishData_Last  = GetReceivedFishData();
	cFUSynchronizedHold hold( l_pVector_ReceivedFishData_Last );
	//
	while( l_pVector_ReceivedFishData_Last->size() )
	{
		sReceiveFishData *l_pReceivedFishData_0 = (*l_pVector_ReceivedFishData_Last)[0];
		l_pVector_ReceivedFishData_Last->erase( l_pVector_ReceivedFishData_Last->begin() );
		//
		if( m_pSendDataWithMonsterList->size() > 0 )
		{
			sSendDataWithMonster *l_pSendDataWithMonster_MasterLee = (*m_pSendDataWithMonsterList)[0];
			m_pSendDataWithMonsterList->erase( m_pSendDataWithMonsterList->begin() );
			//
			if( l_pReceivedFishData_0->cNumFishHitted != 0 && 
				l_pSendDataWithMonster_MasterLee->m_SendData.iNumFishHitted == l_pReceivedFishData_0->cNumFishHitted )
			{
				for( int i=0; i<l_pReceivedFishData_0->cNumFishHitted; ++i )
				{
					int	l_iCurrentIndex = i;
					sHittedFish_Send	l_Sent_HittedFish		= l_pSendDataWithMonster_MasterLee->m_SendData.HittedFish[l_iCurrentIndex];
					sHittedFish			l_Received_HittedFish	= l_pReceivedFishData_0->HittedFish[l_iCurrentIndex];
					if( l_Sent_HittedFish.usID == l_Received_HittedFish.usID )
					{
						sHittedFishData	l_sHittedFishData;
						sCompareData_MasterLee *l_pCompareData = l_pSendDataWithMonster_MasterLee->m_CompareDataList[l_iCurrentIndex];
						l_sHittedFishData.m_pMonster = l_pCompareData->m_pMonster;//l_pSendDataWithMonster_MasterLee->CompareMonster[l_iCurrentIndex];
						l_sHittedFishData.m_pBullet = l_pCompareData->m_pBullet;//l_pSendDataWithMonster_MasterLee->CompareBullet[l_iCurrentIndex];
						l_sHittedFishData.m_pPlayer = l_pCompareData->m_pPlayerBehaviorBase;//l_pSendDataWithMonster_MasterLee->ComparePlayer[l_iCurrentIndex];
						if( m_pSerialIO_MasterLee->m_sbWriteLog )
						{
							std::string l_str2 = UT::ComposeMsgByFormat(
								"Real:\t\tID:%d\t\tPlayerID:%d\t\tMoney:%d",
								l_Received_HittedFish.usID,
								l_sHittedFishData.m_pPlayer->GetPlayerID(),
								l_Received_HittedFish.uiFishOutMoney );
							cSerialIO_MasterLee::WriteLog( l_str2.c_str(), false );
						}
						//
						if( l_Received_HittedFish.uiFishOutMoney > 0 )
						{
							if( g_bSpecialVersion )
								l_Received_HittedFish.uiFishOutMoney /= g_iExchangeRateValueForSpecialVersion;
							bool l_bResult = cFishApp::m_spProbabilityFish->MonsterToDied( &l_sHittedFishData, l_Received_HittedFish.uiFishOutMoney, true, true );
							if( !l_bResult )
							{
								int a=0;
							}
						}
						else
						{
							cFishApp::m_spProbabilityFish->MonsterToKeepGoing( &l_sHittedFishData, 0.05f );
						}
					}
					else
					{
						//UT::ErrorMsg( L"Received and sent Data Count is not match!", L"please check comport data" );
						cSerialIO_MasterLee::WriteLog( "===>Received and sent Data Count is not match!", true );
						break;
					}
				}
			}
			else
			{
				//UT::ErrorMsg( L"fish died packet data is not match", L"fish died packet data is not match" );
				cSerialIO_MasterLee::WriteLog( "===>Fish died packet data is not match", true );
			}
			//
			delete l_pSendDataWithMonster_MasterLee;
			//
		}
		delete l_pReceivedFishData_0;
	}
	if( m_pSendDataWithMonsterList->size() )
	{
		cSerialIO_MasterLee::WriteLog( "===>Fish didn't clear all.", true );
	}
	else
		g_iWaitingProcessSingnalID = 79979;//set ID to incorrect
	//
	m_bReceivedFishDataFromIO = false;
	return true;
}








bool	cProbability_MasterLee::Get_SentData( unsigned char *e_pData, int *e_piLength )
{
	//ensure only one packet is wating
	assert(m_pSendDataWithMonsterList->size()<2&&"m_pSendDataWithMonsterList->size()<2");
	if( m_pSendDataWithMonsterList->size() == 0 )
	{
		return false;
	}
	//
	//unsigned char	*l_cData = e_pData;
	unsigned char	l_cData[MAX_SEND_BUFFER];
	memset( l_cData, 0, MAX_SEND_BUFFER );
	int	l_iCurrentIndex = 0;
	l_cData[l_iCurrentIndex] = 0x55;			l_iCurrentIndex += 1;
	l_cData[l_iCurrentIndex] = 0xe1;			l_iCurrentIndex += 1;
	l_cData[l_iCurrentIndex] = 0xcc;			l_iCurrentIndex += 1;

	sSendDataWithMonster *l_pSendDataWithMonster_MasterLee = (*m_pSendDataWithMonsterList)[0];
	l_cData[l_iCurrentIndex] = l_pSendDataWithMonster_MasterLee->m_CompareDataList.size();
	if( l_cData[l_iCurrentIndex] > g_iNumFishesCouldBeSend )
	{
		l_cData[l_iCurrentIndex] = g_iNumFishesCouldBeSend;
	}
	l_iCurrentIndex += 1;
	for( int i=0; i<l_pSendDataWithMonster_MasterLee->m_SendData.iNumFishHitted; ++i )
	{
		sHittedFish_Send *l_pHittedFish = &l_pSendDataWithMonster_MasterLee->m_SendData.HittedFish[i];
		short *l_pID = (short*)&l_cData[l_iCurrentIndex];
		*l_pID = l_pHittedFish->usID;
		//
		l_iCurrentIndex += 2;
		l_cData[l_iCurrentIndex] = l_pHittedFish->ucPlayerID;
		//
		l_iCurrentIndex += 1;
		short*l_pBuletValue = (short*)&l_cData[l_iCurrentIndex];
		*l_pBuletValue = l_pHittedFish->ucBulletValue;
		if( g_bSpecialVersion )
			*l_pBuletValue *= g_iExchangeRateValueForSpecialVersion;
		l_iCurrentIndex += 2;
		int*l_pScore = (int*)&l_cData[l_iCurrentIndex];
		*l_pScore = l_pHittedFish->uiFishOutMoney;
		l_iCurrentIndex += 4;
	}	
	l_cData[l_iCurrentIndex] = 0;
	for( int i=0;i<l_iCurrentIndex;++i )
	{
		l_cData[l_iCurrentIndex] += l_cData[i];
	}
	++l_iCurrentIndex;
	l_cData[l_iCurrentIndex] = 0x68;
	l_iCurrentIndex += 1;
	l_cData[l_iCurrentIndex] = 0x16;
	l_iCurrentIndex += 1;
	*e_piLength = l_iCurrentIndex;
	memcpy(e_pData,l_cData,sizeof(char)*MAX_SEND_BUFFER);
	return true;
}








