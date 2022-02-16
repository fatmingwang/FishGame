#include "stdafx.h"
#include "ExtraGamePhase.h"
#include "SlotObject.h"
#include "SlotGamePhase.h"
#include "WinMoneyLine.h"

#include "BaseExtraGame.h"
#include "FreeExplosionGame.h"
#include "FreeBonusGame.h"
#include "FreeMiningGame.h"
#include "FreePickaxeGame.h"
#include "FreeJP.h"
#include "FreeGameForPickGemsGame.h"


cExtraGamePhase::cExtraGamePhase()
{
	this->SetName(EXTRA_GAME_PHASE_NAME);
	m_pConstantUI = 0;
	m_pCurrentBaseExtraGame = 0;
}

cExtraGamePhase::~cExtraGamePhase()
{
	SAFE_DELETE(m_pConstantUI);
}

void	cExtraGamePhase::Init()
{
	cMPDIList*l_pMPDIList = cGameApp::m_spAnimationParser->GetMPDIListByFileName(UI_MPDILIST_FILE_NAME);
	if( !m_pConstantUI )
	{
		//m_pConstantUI = new cMPDI(l_pMPDIList->GetObject(l_strUIMPDIName));
		m_pConstantUI = new cMPDI(l_pMPDIList->GetObject(UPPER_UI));
		m_pConstantUI->Init();
		m_pConstantUI->Update(EPSIONAL);
		m_pConstantUI->RefreshTotalPlayTime();
	}
	if(m_BaseExtraGameList.Count() == 0)
	{
		//<FreeGamePhase>
		//	<FreeGame1 KeyWord="0" ToNextFreeGameCount="0" />
		//	<FreeGame2 KeyWord="1" ToNextFreeGameCount="3" />
		//	<FreeGame3 KeyWord="1" ToNextFreeGameCount="1" />
		//	<FreeGame4 KeyWord="2" ToNextFreeGameCount="3" />
		//	<BonusGame KeyWord="3" ToNextFreeGameCount="3" />
		//</FreeGamePhase>
		cNodeISAX	l_NodeISAX;
		bool	l_b = l_NodeISAX.ParseDataIntoXMLNode(EXTRA_GAME_PHASE_DATA);
		if( l_b )
		{
			TiXmlElement*l_pElement = l_NodeISAX.GetRootElement();
			l_pElement = l_pElement->FirstChildElement();
			while( l_pElement )
			{
				cBaseExtraGame*l_pBaseExtraGame = 0;
				const WCHAR*l_strKeyWord = l_pElement->Attribute(L"KeyWord");
				const WCHAR*l_strCount =  l_pElement->Attribute(L"ToNextFreeGameCount");
				int	l_iCount = _wtoi(l_strCount);
				if( !wcscmp(L"ExplosionFreeGame",l_pElement->Value()))
				{
					const WCHAR*l_strSpeed =  l_pElement->Attribute(L"FallDownSpeed");
					float	l_fSpeed = (float)_wtof(l_strSpeed);
					const WCHAR*l_strTenComboExtraMoneyMin =  l_pElement->Attribute(L"TenComboExtraMoneyMin");
					const WCHAR*l_strTenComboExtraMoneyMax =  l_pElement->Attribute(L"TenComboExtraMoneyMax");
					const WCHAR*l_strGameSpeed =			  l_pElement->Attribute(L"GameSpeed");
					float	l_fGameSpeed = 1.f;
					if( l_strGameSpeed )
						l_fGameSpeed = (float)_wtof(l_strGameSpeed);
					int	l_iTenComboExtraMoneyMin = 80000;
					int	l_iTenComboExtraMoneyMax = 120000;
					if( l_strTenComboExtraMoneyMin )
						l_iTenComboExtraMoneyMin = _wtoi(l_strTenComboExtraMoneyMin);
					if( l_strTenComboExtraMoneyMax )
						l_iTenComboExtraMoneyMax = _wtoi(l_strTenComboExtraMoneyMax);
					l_pBaseExtraGame = new cFreeExplosionGame(l_strKeyWord,l_iCount,l_fSpeed,m_pConstantUI,l_iTenComboExtraMoneyMin,l_iTenComboExtraMoneyMax,l_fGameSpeed);
				}
				else
				if( !wcscmp(L"FreePickaxeGame",l_pElement->Value()))
				{
					l_pBaseExtraGame = new cFreePickaxeGame(l_strKeyWord,l_iCount);
				}
				else
				if( !wcscmp(L"FreeMiningGame",l_pElement->Value()))
				{
					const WCHAR*l_strFallDownSpeed = l_pElement->Attribute(L"FallDownSpeed");
					const WCHAR*l_strTrainMovingTime =  l_pElement->Attribute(L"TrainMovingTime");
					float	l_fSpeed = (float)_wtof(l_strFallDownSpeed);
					float	l_fTime = (float)_wtof(l_strTrainMovingTime);
					l_pBaseExtraGame = new cFreeMiningGame_Mining(l_strKeyWord,l_iCount,l_fSpeed,l_fTime);
				}
				else
				if( !wcscmp(L"PickRockBonusGame",l_pElement->Value()))
				{
					const WCHAR*l_strRockShowRowIndex =  l_pElement->Attribute(L"RockShowRowIndex");
					const WCHAR*l_strRockImageName =  l_pElement->Attribute(L"RockImageName");
					int	l_iIndex = _wtoi(l_strRockShowRowIndex);
					l_pBaseExtraGame = new cPickRockBonusGame(l_strKeyWord,l_iCount,l_iIndex,(WCHAR*)l_strRockImageName);
				}
				else
				if( !wcscmp(L"JPGame",l_pElement->Value()))
				{
					const WCHAR*l_strJPMoney =  l_pElement->Attribute(L"JPMoney");
					int	l_iJPMoney = _wtoi(l_strJPMoney);
					l_pBaseExtraGame = new cFreeJPGame(l_strKeyWord,l_iCount,l_iJPMoney,this->m_pConstantUI);
				}
				else
				if( !wcscmp(L"PickGemsForFreeGameAndBonus",l_pElement->Value()))
				{
					l_pBaseExtraGame = new cFreeGameForPickGemsGame(l_strKeyWord,l_iCount,l_pElement,m_pConstantUI);
				}
				l_pElement = l_pElement->NextSiblingElement();
				if( l_pBaseExtraGame )
				{
					m_BaseExtraGameList.AddObjectNeglectExist(l_pBaseExtraGame);
					l_pBaseExtraGame->SetOwner(this);
				}
			}		
		}
	}
	//=====================
	if( m_BaseExtraGameList.Count() )
	{
		m_BaseExtraGameList.Init();
		m_pCurrentBaseExtraGame = m_BaseExtraGameList[0];
		if(m_pCurrentBaseExtraGame->IsConditionOk())
			m_pCurrentBaseExtraGame->LoadResource();
		else
		{
			m_pCurrentBaseExtraGame->SetDone(true);
			CheckFreeGameCondition();
		}
	}
}

void	cExtraGamePhase::CheckFreeGameCondition()
{
	if( m_pCurrentBaseExtraGame && m_pCurrentBaseExtraGame->IsDone() )
	{
		m_pCurrentBaseExtraGame->Destroy();
		int	l_iCount = m_BaseExtraGameList.Count();
		int	l_iIndex = m_BaseExtraGameList.GetObjectIndexByPointer(m_pCurrentBaseExtraGame)+1;
		for( int i=l_iIndex;i<l_iCount;++i )
		{
			bool	l_bConditionOK = false;
			m_pCurrentBaseExtraGame = m_BaseExtraGameList[i];
			if(m_pCurrentBaseExtraGame->IsConditionOk()  && !m_pCurrentBaseExtraGame->IsDone() )
			{
				if( m_pCurrentBaseExtraGame->Type() == cFreeGameForPickGemsGame::TypeID )
				{//fucking stupid.
					if( g_pSlotGamePhase->m_iFreeGame != 0 )
					{
						cFreeGameForPickGemsGame*l_pFreeGameForPickGemsGame = (cFreeGameForPickGemsGame*)m_pCurrentBaseExtraGame;
						l_pFreeGameForPickGemsGame->m_FreeGameData.iTimeForPickupGems += 1;
						WRITELOGFILE("Free Game Add 1");
						continue;
					}
				}
				l_bConditionOK  =true;
			}

			if( !l_bConditionOK  &&  g_pSlotGamePhase->m_iFreeGame ==0  )
			{
				if(m_pCurrentBaseExtraGame->Type() == cFreeGameForPickGemsGame::TypeID)
				{
					cFreeGameForPickGemsGame*l_pFreeGameForPickGemsGame = (cFreeGameForPickGemsGame*)m_pCurrentBaseExtraGame;
					if( l_pFreeGameForPickGemsGame->m_FreeGameData.iTimeForPickupGems > 0 )
						l_bConditionOK = true;
				}
			}

			if( l_bConditionOK )
			{
				m_pCurrentBaseExtraGame->LoadResource();
				return;
			}
		}
		//all free game is done or conidtion is not satisfied
		m_pCurrentBaseExtraGame = 0;
		this->m_bSatisfiedCondition = true;
	}
}

void	cExtraGamePhase::Update(float e_fElpaseTime)
{
	if( !m_pCurrentBaseExtraGame )
	{
		this->m_bSatisfiedCondition = true;
		return;
	}
	m_pCurrentBaseExtraGame->Update(e_fElpaseTime);
	//win jp no more extra game
	if( m_pCurrentBaseExtraGame->Type() != cFreeJPGame::TypeID )
		CheckFreeGameCondition();
	else
	{
		if( m_pCurrentBaseExtraGame->IsDone() )
		{
			m_pCurrentBaseExtraGame = 0;
			this->m_bSatisfiedCondition = true;
		}
	}
			
}

void	cExtraGamePhase::Render()
{
	//if(!m_pFreeGame->m_pExploreMPDI->IsAnimationDone())
	//	g_pSlotGamePhase->m_pSlotLineObjectManagerList->Render();
	//else
	//	g_pSlotGamePhase->m_pSlotLineObjectManagerList->RenderFrame();
	if( m_pCurrentBaseExtraGame )
		m_pCurrentBaseExtraGame->Render();
	else
	{
		if( m_pConstantUI )
			m_pConstantUI->Render();
		g_pSlotGamePhase->m_pSlotLineObjectManagerList->Render();
	}
	g_pSlotGamePhase->m_pBetBehavior->Render();
}

void	cExtraGamePhase::DebugRender()
{

}

void	cExtraGamePhase::MouseDown(int e_iX,int e_iY)
{
	if( m_pCurrentBaseExtraGame )
		m_pCurrentBaseExtraGame->MouseDown(e_iX,e_iY);
}

void	cExtraGamePhase::MouseUp(int e_iX,int e_iY)
{
	if( m_pCurrentBaseExtraGame )
		m_pCurrentBaseExtraGame->MouseUp(e_iX,e_iY);
}

void	cExtraGamePhase::MouseMove(int e_iX,int e_iY)
{
	if( m_pCurrentBaseExtraGame )
		m_pCurrentBaseExtraGame->MouseMove(e_iX,e_iY);
}

void	cExtraGamePhase::KeyUP(char e_cKey)
{

}

const WCHAR*	cExtraGamePhase::GetNextPhaseName()
{
#ifdef _DEBUG
	//OutputDebugString(L"leave cExtraGamePhase\n");
#endif
	return CALCULATE_SCORE_PHASE_NAME;
}

const WCHAR*	cExtraGamePhase::GetCurrentExtraGameName()
{
	if( !m_pCurrentBaseExtraGame )
		return 0;
	return m_pCurrentBaseExtraGame->Type();
}