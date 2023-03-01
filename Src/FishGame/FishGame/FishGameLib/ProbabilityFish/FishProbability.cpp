#include "stdafx.h"
#include "FishProbability.h"
#include "BulletBank/BankMode.h"
#include "FileSaver.h"
#include "BulletBank/BulletDataAndBankRepositorManager.h"
#include "../GameApp/GameApp.h"
#include "../Monster/MonsterManager.h"
#include "../GameBullet/Bullet.h"
#include "../PlayerBehavior/PlayerData.h"
#include "../PlayerBehavior/PlayerData.h"
#include "../MiniGame/MiniGameManager.h"
#include "../ControlPanel/ControlPanel.h"
#include "../FileNameDefine.h"

cProbabilityFish::cProbabilityFish()
{
	m_pBulletData = 0;
	m_iMaxScoreInOneFire = 0;
	m_fMachinRunTime = 0.f;
	m_bIsShow_RestartMachineMessage = false;
	m_pMPDI_MasterLeeIOErrorMessage = 0;
	m_fSafeToGiveProbabilityOffset = 0.01f;
	//
	//m_pProbability_MasterLee = 0;
	m_pFileSaver = 0;

	cEventVariable*l_pLatestIncome =		cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Latest_TotalIncome");
	cEventVariable*l_pLatestPay =			cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Latest_TotalPay");
	cEventVariable*l_pLatestRevenue =		cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Latest_Revenue");
	cEventVariable*l_pLatestExchangeIn =	cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Latest_ExchangeIn");
	cEventVariable*l_pLatestExchangeOut =	cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Latest_ExchangeOut");
	cEventVariable*l_pLatestToastCoin =		cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Latest_ToastCoin");
	cEventVariable*l_pLatestRefundCoin =	cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Latest_RefudCoin");
	cEventVariable*l_pProbability =			cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"RealProbability");

	m_pi64LatestInMoney = l_pLatestIncome->GetInt64();
	m_pi64LatestOutMoney = l_pLatestPay->GetInt64();
	m_pi64LatestRevenue = l_pLatestRevenue->GetInt64();
	m_pi64LatestExchangeIn = l_pLatestExchangeIn->GetInt64();
	m_pi64LatestExchangeOut = l_pLatestExchangeOut->GetInt64();
	m_pi64LatestCoinIn = l_pLatestToastCoin->GetInt64();
	m_pi64LatestCoinOut = l_pLatestRefundCoin->GetInt64();
	m_pfTargetProbability = l_pProbability->GetFloat();
}

cProbabilityFish::~cProbabilityFish()
{
	SAFE_DELETE(m_pBulletData);
	SAFE_DELETE(m_pFileSaver);
	//SAFE_DELETE( m_pProbability_MasterLee );
	SAFE_DELETE( m_pMPDI_MasterLeeIOErrorMessage );

}

bool	cProbabilityFish::MyParse( TiXmlElement *e_pRoot )
{
	e_pRoot = e_pRoot->FirstChildElement();
	const WCHAR*l_strName = 0;
	while( e_pRoot )
	{
		l_strName = e_pRoot->Value();
		//COMPARE_NAME("MasterLi")
		//{
		//	cFishApp::m_sbIsUsingMasterLeeProbability = true;
		//	PARSE_ELEMENT_START(e_pRoot)
		//		COMPARE_NAME("WriteLog")
		//		{
		//			cSerialIO_MasterLee::m_sbWriteLog = VALUE_TO_BOOLEAN;
		//		}
		//		else
		//		COMPARE_NAME("WriteErrorLog")
		//		{
		//			cSerialIO_MasterLee::m_sbWriteErrorLog = VALUE_TO_BOOLEAN;
		//		}
		//		else
		//		COMPARE_NAME("COM")
		//		{
		//			cSerialIO_MasterLee::m_siCom = VALUE_TO_INT;
		//		}
		//		else
		//		COMPARE_NAME("Timeout")
		//		{
		//			cSerialIO_MasterLee::m_sfTimeOut = VALUE_TO_FLOAT;
		//		}
		//		else
		//		COMPARE_NAME("ComportIOTimeOutError")
		//		{
		//			cSerialIO_MasterLee::m_sfComportIOTimeOutError = VALUE_TO_FLOAT;
		//		}
		//		else
		//		COMPARE_NAME("SendStartSingnal")
		//		{
		//			cSerialIO_MasterLee::m_sbReceiveStartSingnal = VALUE_TO_BOOLEAN;
		//		}
		//		else
		//		COMPARE_NAME("SingranlProcessGap")
		//		{
		//			cSerialThread::m_sfTargetTimeToWrite = VALUE_TO_FLOAT;
		//		}
		//		
		//	PARSE_NAME_VALUE_END
		//}
		e_pRoot = e_pRoot->NextSiblingElement();
	}
	return true;
}

void	cProbabilityFish::Init()
{
	std::string	l_strLogFile = "Log/FishProbability";
	l_strLogFile += UT::WcharToChar(UT::GetSystemTimeForFile(true).c_str());
	l_strLogFile += ".txt";
	m_pFileSaver = new cFileSaver(PLAYER_MONEY_DATA,l_strLogFile.c_str());
	m_fSafeToGiveProbabilityOffset = 0.01f;
	if(!ParseWithMyParse(FISH_PROBABILITY))
	{
		UT::ErrorMsg(L"FISH_PROBABILITY",L"parse failed");
	}
	//
	//if( cFishApp::m_sbIsUsingMasterLeeProbability )
	//{
	//	SAFE_DELETE( m_pProbability_MasterLee );
	//	m_pProbability_MasterLee = new cProbability_MasterLee();
	//	m_pProbability_MasterLee->Initial();
	//}

	//Show IO Message
	cMPDIList*	l_pMessageMPDIList = cGameApp::GetMPDIListByFileName( MESSAGE_MPDILIST );
	m_pMPDI_MasterLeeIOErrorMessage = new cMultiPathDynamicImage( l_pMessageMPDIList->GetObject(L"RestartPlease_pos"));
	m_pMPDI_MasterLeeIOErrorMessage->Init();
	SAFE_DELETE(m_pBulletData);
	m_pBulletData = new cBulletDataAndBankRepositorManager();
	m_pBulletData->Init();
}

void	cProbabilityFish::Update(float e_fElpaseTime)
{
	m_fMachinRunTime += e_fElpaseTime;
	if( cFishApp::m_spMonsterManager)
	{
		//if( cFishApp::m_sbIsUsingMasterLeeProbability )
		//{
		//	m_pProbability_MasterLee->Update( e_fElpaseTime );
		//}
	}
	if( m_pBulletData )
		m_pBulletData->Update( e_fElpaseTime );
	if(m_pFileSaver)
		m_pFileSaver->Update(e_fElpaseTime);
	if(m_bIsShow_RestartMachineMessage)
	{
		m_pMPDI_MasterLeeIOErrorMessage->Update(e_fElpaseTime);
	}
}

void	cProbabilityFish::Render()
{
	//Show IO Message
	if(m_bIsShow_RestartMachineMessage)
	{
		m_pMPDI_MasterLeeIOErrorMessage->Render();
	}
}

void	cProbabilityFish::DebugRender()
{
	cGameApp::m_spGlyphFontRender->SetFontColor( Vector4( 1,1,0,1 ) );
	std::wstring	l_str;
	const int l_nCountWidth = 3;
	const int l_nStrokeKapa = 1;
	int l_nStartX = 0;
	int l_nStartY = 100;

	cEventVariable*l_pTotalIncomeEventVariable = cFishApp::m_spControlPanel->GetEventManager()->GetEventVariable(L"TotalIncome");
	cEventVariable*l_pTotalPayEventVariable = cFishApp::m_spControlPanel->GetEventManager()->GetEventVariable(L"TotalPay");
	cEventVariable*l_pTotalRevenueEventVariable = cFishApp::m_spControlPanel->GetEventManager()->GetEventVariable(L"TotalRevenue");

	int64 RevenueDist =  *m_pi64LatestInMoney - *m_pi64LatestOutMoney;
	double RevenueRate =  (RevenueDist / (double)*m_pi64LatestInMoney)*100.0f;
	l_str = UT::ComposeMsgByFormat( L"In: %I64d\nOut: %I64d\nRevenue: %I64d\n",
		*m_pi64LatestInMoney+*l_pTotalIncomeEventVariable->GetInt64(),
		*m_pi64LatestOutMoney+*l_pTotalPayEventVariable->GetInt64(),
		*m_pi64LatestRevenue+*l_pTotalRevenueEventVariable->GetInt64());

	cGameApp::m_spGlyphFontRender->RenderFont( l_nStartX, l_nStartY, l_str.c_str() );
	cGameApp::m_spGlyphFontRender->SetFontColor( Vector4( 1,1,1,1 ) );
	return;
	cGameApp::m_spGlyphFontRender->SetFontColor( Vector4( 0,0,0,1 ) );
	cGameApp::m_spGlyphFontRender->RenderFont( l_nStartX, l_nStartY, l_str.c_str() );
	cGameApp::m_spGlyphFontRender->SetFontColor(Vector4(1,1,1,1));

	if( cFishApp::m_sbIsUsingMasterLeeProbability )
	{
	}
	else
	{
		m_pBulletData->DebugRender();
	}
	//
	cGameApp::m_spGlyphFontRender->SetFontColor(Vector4(1,1,1,1));

	if( cFishApp::m_sbIsUsingMasterLeeProbability )
	{
		//m_pProbability_MasterLee->Render();
	}
	else
	{
	}
}

void	cProbabilityFish::RevenueMinus( int e_iMoney, int e_iPlayerID)
{
	*m_pi64LatestOutMoney			+= e_iMoney;
	*m_pi64LatestRevenue =			*m_pi64LatestInMoney-*m_pi64LatestOutMoney;
	if( m_pFileSaver )
		this->m_pFileSaver->RevenueMinus(e_iMoney, e_iPlayerID);
}

void	cProbabilityFish::RevenueAdd( int e_iMoney, int e_iPlayerID )
{
	if(e_iMoney == 0)
		return;
	if( m_pFileSaver )
		this->m_pFileSaver->RevenueAdd(e_iMoney, e_iPlayerID);
	*m_pi64LatestInMoney +=			e_iMoney;
	*m_pi64LatestRevenue =			*m_pi64LatestInMoney-*m_pi64LatestOutMoney;

	if(e_iMoney == 0)
		return;
	if( !cFishApp::m_sbIsUsingMasterLeeProbability )
	{
		float l_fMiniGameStoreMoney = 0;
		if( l_fMiniGameStoreMoney -(int) l_fMiniGameStoreMoney >0 )
		{
			l_fMiniGameStoreMoney += 1.f;
		}
		m_pBulletData->RevenueAdd( e_iMoney - (int)l_fMiniGameStoreMoney, e_iPlayerID, e_iMoney );
	}
}

void	cProbabilityFish::WrtiePlayerMoney(float e_fElpaseTime)
{	
}
void	ClearLatestRecord(bool e_bClearPlayerMoney)
{
	cFishApp::m_spProbabilityFish->ClearLatestRecord(e_bClearPlayerMoney);
}

void	cProbabilityFish::ClearLatestRecord(bool e_bClearPlayerMoney)
{
	//if( !this )
		//return;
	m_pi64LatestInMoney  = 0;
	m_pi64LatestOutMoney = 0;
	m_pi64LatestCoinIn  = 0;
	m_pi64LatestCoinOut = 0;
	m_pi64LatestRevenue     = 0;
	m_pi64LatestExchangeIn  = 0;
	m_pi64LatestExchangeOut = 0;
	if( this->m_pFileSaver )
		this->m_pFileSaver->ClearLatestRecord(e_bClearPlayerMoney);
}

void	cProbabilityFish::ExchangeIn( int64 e_i64Value )
{
	*m_pi64LatestExchangeIn += e_i64Value;
}

void	cProbabilityFish::ExchangeOut( int64 e_i64Value )
{
	*m_pi64LatestExchangeOut += e_i64Value;
}

void	cProbabilityFish::ToastCoin( int64 e_i64Value )
{
	*m_pi64LatestCoinIn += e_i64Value;
}

void	cProbabilityFish::RefundCoin( int64 e_i64Value )
{
	*m_pi64LatestCoinOut += e_i64Value;
}




extern bool g_bEasyMonsterDied;



bool cProbabilityFish::MonsterToDied( sHittedFishData *e_pHittedFishData, int e_iOutMoney, bool e_bForceToDied, bool e_bMasterLi )
{
	if( e_iOutMoney == 0 )
	{
		int a=0;
	}
	if( !e_bForceToDied && !e_pHittedFishData->m_pMonster->IsStatusAllowToDied() )
	{
		return false;
	}
	//
	e_pHittedFishData->m_pPlayer->AddScore( e_pHittedFishData->m_pMonster , e_iOutMoney );
	e_pHittedFishData->m_pMonster->SetMonsterStatus( eMS_DIED_SHOW) ;
	e_pHittedFishData->m_pMonster->m_pHittedPlayer = e_pHittedFishData->m_pPlayer;
	e_pHittedFishData->m_pMonster->m_pBullet = e_pHittedFishData->m_pBullet;
	RevenueMinus( e_iOutMoney, e_pHittedFishData->m_pPlayer->m_iPlayerID);
	if( m_pFileSaver )
		this->m_pFileSaver->RevenueMinus(e_iOutMoney, e_pHittedFishData->m_pPlayer->m_iPlayerID);
	
	if( e_pHittedFishData->m_pMonster->IsMiniGameMonster() )
	{
		int	l_iPayrate = e_iOutMoney/e_pHittedFishData->m_pBullet->GetPayRate();
		cFishApp::m_spMiniGameManager->RequireMiniGameActiverBase(e_pHittedFishData->m_pMonster->m_strMiniGameName.c_str(),l_iPayrate,e_iOutMoney,e_pHittedFishData->m_pPlayer,e_pHittedFishData->m_pMonster);
	}

	if( m_iMaxScoreInOneFire < e_iOutMoney )
	{
		m_iMaxScoreInOneFire = e_iOutMoney;
	}
	return true;
}


//1280:720 = 17 :10,but 27 is not even so make it as 26
//const int			g_iTotalDirectionOrder = 26;//17+10
//const int			g_iDirectionOrder[26] = {0,1,2,3,1,3,//6
//											 0,1,2,3,1,3,//6
//											 0,1,2,3,1,	 //5
//											 0,1,2,3,3,	 //5
//											 0,1,2,3};	 //4

const int			g_iTotalDirectionOrder = 8;
//const int			g_iDirectionOrder[12] = {0,2,0,2,0,2,1,3,1,3,1,3};	 //8
//const int			g_iDirectionOrder[12] = {1,3,1,3,1,3,1,3,1,3,1,3};	 //8
const int			g_iDirectionOrder[8] = {2,2,2,1,0,0,0,3};	 //8

UINT				g_suiDirection = 0;








Vector3		cProbabilityFish::GetRandomPos( float e_fRadius )
{
	sMinMaxData<float>	l_fPosX;
	sMinMaxData<float>	l_fPosY;
	l_fPosX.Max = cGameApp::m_spOpenGLRender->m_vGameResolution.x;		l_fPosX.Min = 0.f;
	l_fPosY.Max = cGameApp::m_spOpenGLRender->m_vGameResolution.y;		l_fPosY.Min = 0.f;
	//
	++g_suiDirection ;
	//
	if( g_suiDirection >= g_iTotalDirectionOrder )
	{
		g_suiDirection = 0;
	}
	int	l_iDirection = g_iDirectionOrder[g_suiDirection];
	Vector3	l_vPos;

	switch( l_iDirection )
	{
	case 0://eD_LEFT
		l_vPos.x = -e_fRadius;
		l_vPos.y = l_fPosY.Rand();
		break;
	case 1://eD_UP
		l_vPos.x = l_fPosX.Rand();
		l_vPos.y = -e_fRadius;
		break;
	case 2://eD_RIGHT:
		l_vPos.x = cGameApp::m_spOpenGLRender->m_vGameResolution.x+e_fRadius;
		l_vPos.y = l_fPosY.Rand();
		break;
	case 3://eD_DOWN:
		l_vPos.x = l_fPosX.Rand();
		l_vPos.y = cGameApp::m_spOpenGLRender->m_vGameResolution.y+e_fRadius;
		break;
	} //end switch
	l_vPos.z = 0.f;
	return l_vPos;
}

Vector3		cProbabilityFish::GetRandomPos( cMonster*e_pMonster )
{
	return GetRandomPos( e_pMonster->m_fRadius );
}

void	cProbabilityFish::MonsterToKeepGoing( sHittedFishData*e_pHittedFishData, float e_fAddProbability )
{
	if( e_pHittedFishData->m_pMonster->m_fRestProbabilityValueToDied >= e_fAddProbability )
	{
		e_pHittedFishData->m_pMonster->m_fRestProbabilityValueToDied -= e_fAddProbability;
	}
	//e_pHittedFishData->m_pMonster->SetMonsterStatus(eMS_ALIVE);
}

float	cProbabilityFish::GetCurrentRealProbability()
{
	if( *m_pi64LatestInMoney != 0 )
	{
		return (float)((double)(*m_pi64LatestOutMoney)/(*m_pi64LatestInMoney));
	}
	return 0.f;
}

float	cProbabilityFish::GetCurrentProbability()
{
	int64	l_iTotalInMoney = GetLeatestInMoney();
	return (float)((double)*m_pi64LatestOutMoney/l_iTotalInMoney);
}
float	cProbabilityFish::GetTargetProbability()
{
	return *m_pfTargetProbability/100.f;
}
float	cProbabilityFish::GetOffsetProbability()
{
	float l_fTargetProbability = GetTargetProbability();
	float l_fProbability = GetCurrentRealProbability();
	if( l_fProbability < 0 )
	{
		return 0;
	}
	return l_fTargetProbability - l_fProbability;
}

int64	cProbabilityFish::GetLeatestInMoney()
{
	int64	l_i64Value = *m_pi64LatestInMoney;
	if( l_i64Value < 0 )
	{
		l_i64Value = 0;
	}
	//avoid total in is zero
	return l_i64Value+1;
}

float	cProbabilityFish::GetNewProbailityByAddMoney(int e_iOutMoney)
{
	int64	l_iNewTotalOutMoney = *m_pi64LatestOutMoney+e_iOutMoney;
	return (float)((double)l_iNewTotalOutMoney/(double)GetLeatestInMoney());
}

bool	cProbabilityFish::IsProbailitySafeToOut(float e_fProbaility)
{
	if(e_fProbaility<GetTargetProbability())
	{
		return true;
	}
	return false;
}