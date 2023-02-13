#include "stdafx.h"
#include "FileSaver.h"
#include "../GameApp/GameApp.h"
#include "../PlayerBehavior/PlayerData.h"
#include "FishProbability.h"
#include "../ControlPanel/ControlPanel.h"
#include "../FileNameDefine.h"
cFileSaver::cFileSaver(const char*e_strPlayerFileName,const char*e_strStaticFileName)
{
	m_pPlayerMoneyLogFile = 0;
	m_pi64PlayerRevenueLog = 0;
	m_pStaticsLogFile = 0;

	cEventVariable*l_pLatestIncome =		cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Latest_TotalIncome");
	cEventVariable*l_pLatestPay =			cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Latest_TotalPay");
	cEventVariable*l_pLatestRevenue =		cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Latest_Revenue");
	cEventVariable*l_pLatestExchangeIn =	cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Latest_ExchangeIn");
	cEventVariable*l_pLatestExchangeOut =	cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Latest_ExchangeOut");
	cEventVariable*l_pLatestToastCoin =		cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Latest_ToastCoin");
	cEventVariable*l_pLatestRefundCoin =	cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Latest_RefudCoin");

	m_pi64LatestInMoney = l_pLatestIncome->GetInt64();
	m_pi64LatestOutMoney = l_pLatestPay->GetInt64();
	m_pi64LatestRevenue = l_pLatestRevenue->GetInt64();
	m_pi64LatestExchangeIn = l_pLatestExchangeIn->GetInt64();
	m_pi64LatestExchangeOut = l_pLatestExchangeOut->GetInt64();
	m_pi64LatestCoinIn = l_pLatestToastCoin->GetInt64();
	m_pi64LatestCoinOut = l_pLatestRefundCoin->GetInt64();

	//m_WritePlayerMoneyTC.SetTargetTime( cGameApp::m_sbDeviceExist?0.05f:1.f );
	m_WritePlayerMoneyTC.SetTargetTime(1.f);
#ifdef DEBUG
	m_WritePlayerMoneyTC.SetTargetTime( 9 );
#endif
	//m_WritePlayerMoneyTC.SetTargetTime( 0.2f );
	m_WritePlayerMoneyTC.Start();
	OpenPlayerFile(e_strPlayerFileName);
}

cFileSaver::~cFileSaver()
{
	SAFE_DELETE(m_pStaticsLogFile);
	SAFE_DELETE(m_pi64PlayerRevenueLog);
	SAFE_DELETE(m_pPlayerMoneyLogFile);
}

void	cFileSaver::Update(float e_fElpaseTime)
{
	WriteStaticFile();
	WrtiePlayerFile(e_fElpaseTime);
}

void	cFileSaver::WrtiePlayerFile(float e_fElpaseTime)
{
	m_WritePlayerMoneyTC.Update(e_fElpaseTime);
	if( m_WritePlayerMoneyTC.bTragetTimrReached )
	{
		m_WritePlayerMoneyTC.Start();
		if( cFishApp::m_spPlayerManager && m_pPlayerMoneyLogFile )
		{
			m_pPlayerMoneyLogFile->SetFilePos(0);
			//6
			m_pPlayerMoneyLogFile->WriteToFile(*m_pi64LatestInMoney);
			m_pPlayerMoneyLogFile->WriteToFile(*m_pi64LatestOutMoney);
			m_pPlayerMoneyLogFile->WriteToFile(*m_pi64LatestExchangeIn);
			m_pPlayerMoneyLogFile->WriteToFile(*m_pi64LatestExchangeOut);
			m_pPlayerMoneyLogFile->WriteToFile(*m_pi64LatestCoinIn);
			m_pPlayerMoneyLogFile->WriteToFile(*m_pi64LatestCoinOut);
			int	l_iPlayerCount = cFishApp::m_spPlayerManager->Count();
			for( int i=0;i<l_iPlayerCount;++i )
			{
				m_pPlayerMoneyLogFile->WriteToFile(cFishApp::m_spPlayerManager->GetObject(i)->GetScore());
				 cPlayerBehaviorBase *pPlayer = ( cPlayerData* ) cFishApp::m_spPlayerManager->GetObject(i);
				 m_pPlayerMoneyLogFile->WriteToFile( (int64)pPlayer->GetWeapon()->GetPower() );
			}
			m_pPlayerMoneyLogFile->Flush();
		}
	}
}

void	cFileSaver::WriteStaticFile()
{
	if( !m_pStaticsLogFile )
	{
		return;
	}
	if(1)
	{
		return;
	}
	int	l_iCount = cFishApp::m_spControlPanel->m_iPlayerCount;
	int64 l_i64TotalRevene = 0;
	for( int i=0; i<l_iCount; ++i )
	{
		l_i64TotalRevene += m_pi64PlayerRevenueLog[i];
		//revenue
		m_pStaticsLogFile->WriteToFile(m_pi64PlayerRevenueLog[i]);
	}
	for( int i=0; i<l_iCount; ++i )
	{
		//money
		m_pStaticsLogFile->WriteToFile(cFishApp::m_spPlayerManager->GetObject(i)->GetScore());
	}
	memset( m_pi64PlayerRevenueLog, 0, sizeof(int64)*l_iCount );
	//total in 
	//m_pStaticsLogFile->WriteToFile(*m_piTotalInMoney);
	//total out
	//m_pStaticsLogFile->WriteToFile(*m_piTotalOutMoney);

	float	l_fCurrentProbability = cFishApp::m_spProbabilityFish->GetCurrentRealProbability();
	m_pStaticsLogFile->WriteToFile(l_i64TotalRevene);
	m_pStaticsLogFile->WriteToFile(l_fCurrentProbability);
	//OutputDebugString(ValueToStringW(l_i64TotalRevene));
	//OutputDebugString(L",");
	//OutputDebugString(ValueToStringW(l_fCurrentProbability));
	//OutputDebugString(L"\n");
	m_pStaticsLogFile->Flush();
}

void	cFileSaver::OpenPlayerFile(const char*e_strPlayerFileName)
{
	cBinaryFile l_BinaryFile( true );
	if(l_BinaryFile.Openfile( e_strPlayerFileName) )
	{
		int	l_iSize = l_BinaryFile.GetDataFileSize();
		if( l_iSize )
		{
			//LatestIn,LatestOut,LatestRevenue,LatestExchangeIn,LatestExchangeOut,LatestCoinIn,LatestCoinOut
			//8			8			0				8				8				8				8
			//6*8=56
			//4*8=32
			int	l_iPlayerCountSaved = (l_iSize-(6*sizeof(int64)))/(sizeof(int64)*2);
			//avoid spuid guy open as text and save it.
			unsigned char*l_pData = (unsigned char*)l_BinaryFile.GetDataFile(0);
			*m_pi64LatestInMoney = l_BinaryFile.GetInt64(l_pData);l_pData += sizeof(int64);
			*m_pi64LatestOutMoney = l_BinaryFile.GetInt64(l_pData);l_pData += sizeof(int64);
			*m_pi64LatestRevenue = *m_pi64LatestInMoney-*m_pi64LatestOutMoney;
			*m_pi64LatestExchangeIn = l_BinaryFile.GetInt64(l_pData);l_pData += sizeof(int64);
			*m_pi64LatestExchangeOut = l_BinaryFile.GetInt64(l_pData);l_pData += sizeof(int64);
			*m_pi64LatestCoinIn = l_BinaryFile.GetInt64(l_pData);l_pData += sizeof(int64);
			*m_pi64LatestCoinOut = l_BinaryFile.GetInt64(l_pData);l_pData += sizeof(int64);
			//
			int	l_iMinBulletValue = cFishApp::m_spControlPanel->m_iBulletPayRateMinPoint;
			int	l_iMaxBulletValue = cFishApp::m_spControlPanel->m_iBulletPayRateLimit;
			int	l_iAddBulletValue = cFishApp::m_spControlPanel->m_iBulletPayRateAddPoint;
			if( cFishApp::m_spPlayerManager&&cFishApp::m_spPlayerManager->Count() == l_iPlayerCountSaved )

			{
				//if(!cFishApp::m_spPlayerManager->GetObject(0)->m_bAutoPlay)
				{
					int	l_iPlayerCount = cFishApp::m_spPlayerManager->Count();
					//player count is same so give money or cler
					for ( int i = 0; i < l_iPlayerCount; ++i )
					{
						 int64	l_i64Value = l_BinaryFile.GetInt64(l_pData);
						 cPlayerData *l_pPlayer = (cPlayerData*)cFishApp::m_spPlayerManager->GetObject(i);
						 l_pPlayer->m_i64Score = l_i64Value;				l_pData += sizeof(int64);
						 l_i64Value = l_BinaryFile.GetInt64(l_pData);		l_pData += sizeof(int64);
						 int	l_iPower = (int)l_i64Value;
						 if( l_iPower < l_iMinBulletValue )
						 {
							l_iPower = l_iMinBulletValue;
						 }
						 else
						 if( l_iPower > l_iMaxBulletValue )
						 {
							l_iPower = l_iMaxBulletValue;
						 }
						 if( l_iPower % l_iAddBulletValue )
						 {
							l_iPower = l_iMinBulletValue;
						 }
						 l_pPlayer->m_pWeapon->SetPower(l_iPower);
						 l_pPlayer->WeaponLVImageCheck();
					}
				}
			}
		}
	}
	//
	l_BinaryFile.CloseFile();
	//
	//m_pPlayerMoneyLogFile
	SAFE_DELETE(m_pPlayerMoneyLogFile);
	m_pPlayerMoneyLogFile = new cBinaryFile();
	//m_pPlayerMoneyLogFile->Writefile(PLAYER_MONEY_DATA,true,cGameApp::m_sbDeviceExist);
	m_pPlayerMoneyLogFile->Writefile(PLAYER_MONEY_DATA, true, false);
	WrtiePlayerFile(99.f);
}

void	cFileSaver::RevenueAdd( int e_iMoney, int e_iPlayerID )
{
	if( m_pi64PlayerRevenueLog )
		m_pi64PlayerRevenueLog[e_iPlayerID] -= e_iMoney;
}

void	cFileSaver::RevenueMinus( int e_iMoney, int e_iPlayerID )
{
	if( m_pi64PlayerRevenueLog )
		m_pi64PlayerRevenueLog[e_iPlayerID] += e_iMoney;
}

void	cFileSaver::ClearLatestRecord(bool e_bClearPlayerMoney)
{
	m_pPlayerMoneyLogFile->SetFilePos(  0 );
	m_pPlayerMoneyLogFile->WriteToFile( 0 );
	m_pPlayerMoneyLogFile->WriteToFile( 0 );
	m_pPlayerMoneyLogFile->WriteToFile( 0 );
	m_pPlayerMoneyLogFile->WriteToFile( 0 );
	m_pPlayerMoneyLogFile->WriteToFile( 0 );
	m_pPlayerMoneyLogFile->WriteToFile( 0 );
	if( e_bClearPlayerMoney )
	{
		int	l_iCount = cFishApp::m_spPlayerManager->Count();
		for ( int i = 0; i < l_iCount; i++ )
		{
			cPlayerData *l_pPlayer = ( cPlayerData* ) cFishApp::m_spPlayerManager->GetObject(i);
			l_pPlayer->m_pWeapon->SetPower(0);
			l_pPlayer->m_i64Score = 0;
			m_pPlayerMoneyLogFile->WriteToFile( ( int64 ) 0 );
			m_pPlayerMoneyLogFile->WriteToFile( ( int64 ) 0 );
		}
	}
	m_pPlayerMoneyLogFile->Flush();
}