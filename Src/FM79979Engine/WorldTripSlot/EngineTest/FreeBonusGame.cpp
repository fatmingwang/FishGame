#include "stdafx.h"
#include "FreeBonusGame.h"
#include "SlotGamePhase.h"
#include "SlotObject.h"
#include "SlotRule.h"

cPickupObjectToWinMoney::cPickupObjectToWinMoney(cPuzzleImageUnit*e_pImage,Vector3 e_vPos,cMPDI*e_pNoWinMoneyMPDI,int e_iMoney)
:cImageButton(e_pImage,e_vPos)
{
	m_iMoney = e_iMoney;
	m_pNoWinMoneyMPDI = new cMPDI(e_pNoWinMoneyMPDI);
	m_pNoWinMoneyMPDI->Init();
	//m_pNoWinMoneyMPDI->SetPos(e_vPos);
	m_pNoWinMoneyMPDI->SetPos(e_vPos);
	m_pNoWinMoneyMPDI->SetStopAtLastFrame(true);
}

cPickupObjectToWinMoney::~cPickupObjectToWinMoney()
{
	SAFE_DELETE(m_pNoWinMoneyMPDI);
}

bool	cPickupObjectToWinMoney::InternalCollide(int e_iPosX,int e_iPosY)
{
	if(this->m_TC.bTragetTimrReached)
		return false;
	return cImageButton::InternalCollide(e_iPosX,e_iPosY);
}

bool    cPickupObjectToWinMoney::IsSatisfiedCondition()
{
	return m_pNoWinMoneyMPDI->IsAnimationDone();
}

void	cPickupObjectToWinMoney::Update(float e_fElpaseTime)
{
	if(!cImageButton::IsSatisfiedCondition())
	{
		cImageButton::Update(e_fElpaseTime);
	}
	else
	{
		m_pNoWinMoneyMPDI->Update(e_fElpaseTime);
	}
}

void	cPickupObjectToWinMoney::Render()
{
	if(!this->m_TC.bTragetTimrReached)
	{
		cImageButton::Render();
	}
	else
	{
		cPuzzleImageUnit::Render();
		if(this->m_iMoney)
		{
			Vector3	l_vPos = *this->m_pvPos;
			l_vPos.y-=30;
			cGameApp::m_spGlyphFontRender->RenderFont(l_vPos.x,l_vPos.y,ValueToStringW(m_iMoney));
		}
		else
		{
			m_pNoWinMoneyMPDI->Render();
		}
	}
}
//<PickGemsForFreeGameAndBonus KeyWord="9" ToNextFreeGameCount="6">
//	<GemsData FreeTimes="25" BonusRate="1,2,3,5"/>
//	<GemsData FreeTimes="20" BonusRate="1,3,5,8"/>
//	<GemsData FreeTimes="15" BonusRate="1,5,8,10"/>
//	<GemsData FreeTimes="13" BonusRate="1,8,10,15"/>
//	<GemsData FreeTimes="10" BonusRate="1,10,15,30"/>
//</PickGemsForFreeGameAndBonus>

const wchar_t*         cPickRockBonusGame::TypeID( L"cPickRockBonusGame" );
cPickRockBonusGame::cPickRockBonusGame(const WCHAR*e_strConditionName,int e_iConditionCount,int e_iRockShowRowIndex,WCHAR*e_strShowImage)
:cBaseExtraGame(e_strConditionName,e_iConditionCount)
{
	m_TimeForNoActionToLeave.SetTargetTime(5.f);
	m_eFreeGameList = eFGL_PICK_ROCK_BONUS;
	m_strImageName = e_strShowImage;
	m_bPickRockToWinMoneyDone = false;
	m_iRockShowRowIndex = e_iRockShowRowIndex;
	m_pEnteringMPDI = 0;
	m_pLeavingMPDI1 = 0;
	m_pLeavingMPDI2 = 0;
	m_pMPDIList = 0;
}

cPickRockBonusGame::~cPickRockBonusGame()
{

}

void	cPickRockBonusGame::MouseMove(int e_iPosX,int e_iPosY)
{
	if( m_pEnteringMPDI->IsAnimationDone() )
	{
		int	l_iCount = m_ImageButtonList.Count();
		for( int i=0;i<l_iCount;++i )
		{
			if( m_ImageButtonList[i]->m_iMoney == 0 && m_ImageButtonList[i]->IsButtonSatisfiedCondition() )
			{
				return;
			}
		}
		m_ImageButtonList.MouseMove(e_iPosX,e_iPosY);
	}
}

void	cPickRockBonusGame::MouseDown(int e_iPosX,int e_iPosY)
{
	if( m_pEnteringMPDI->IsAnimationDone() )
	{
		int	l_iCount = m_ImageButtonList.Count();
		for( int i=0;i<l_iCount;++i )
		{
			if( m_ImageButtonList[i]->m_iMoney == 0 && m_ImageButtonList[i]->IsButtonSatisfiedCondition() )
			{
				return;
			}
		}
		m_ImageButtonList.MouseDown(e_iPosX,e_iPosY);
	}
}

void    cPickRockBonusGame::MouseUp(int e_iPosX,int e_iPosY)
{
	if( m_pEnteringMPDI->IsAnimationDone() )
	{
		int	l_iCount = m_ImageButtonList.Count();
		for( int i=0;i<l_iCount;++i )
		{
			if( m_ImageButtonList[i]->m_iMoney == 0 && m_ImageButtonList[i]->IsButtonSatisfiedCondition() )
			{
				return;
			}
		}
		m_ImageButtonList.MouseUp(e_iPosX,e_iPosY);
	}
}
//return true if the condition of bonus is satisfied.
//bool	cPickRockBonusGame::InternalConditionCheck()
//{
//	if( cBaseExtraGame::IsConditionOk() )
//		return true;
//	return false;
//}


void	cPickRockBonusGame::SetupResult()
{
	cMPDI*l_pNoWinMoneyMPDI = m_pMPDIList->GetObject(L"symbol_bonuse");
	int	l_iMoneyToGive[] = {500,0,1000,0,0};
	cSlotObjectManager*l_pSlotObjectManager = g_pSlotGamePhase->m_pSlotLineObjectManagerList->GetObject(0);
	cPuzzleImage*l_pPI = l_pSlotObjectManager->GetAllSpinObjectPI();
	cPuzzleImageUnit*l_pImage = l_pPI->GetObject(m_strImageName.c_str());		
	vector<int>	l_uiAllMagicIDVector1 = UT::GenerateRandomTable(5,5);
	for( int i=0;i<g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x;++i )
	{
		cSlotObjectManager*l_pSlotObjectManager = g_pSlotGamePhase->m_pSlotLineObjectManagerList->GetObject(i);
		Vector3	l_vPos = l_pSlotObjectManager->IndexToResultShowPos(m_iRockShowRowIndex);
		int	l_iMoneyToWin = l_iMoneyToGive[l_uiAllMagicIDVector1[i]];

		cPickupObjectToWinMoney*l_pPickupObjectToWinMoney = new cPickupObjectToWinMoney(l_pImage,l_vPos,l_pNoWinMoneyMPDI,l_iMoneyToWin);
		m_ImageButtonList.AddObjectNeglectExist(l_pPickupObjectToWinMoney);
	}
}

void	cPickRockBonusGame::LoadResource()
{
	if( !m_pMPDIList )
	{
		m_pMPDIList = cGameApp::GetMPDIListByFileName(FREE_GAME_BONUS_MPDILIST_FILE_NAME);
		m_pEnteringMPDI = m_pMPDIList->GetObject(L"bounsegame_mov");
		m_pLeavingMPDI1 = m_pMPDIList->GetObject(L"black");
		m_pLeavingMPDI2 = m_pMPDIList->GetObject(L"light");
		m_pEnteringMPDI->SetAnimationLoop(false);
		m_pLeavingMPDI1->SetAnimationLoop(false);
		m_pLeavingMPDI2->SetAnimationLoop(false);
	}
	m_bPickRockToWinMoneyDone = false;
	m_pEnteringMPDI->Init();
	m_pLeavingMPDI1->Init();
	m_pLeavingMPDI2->Init();
	SetupResult();
	m_TimeForNoActionToLeave.Start();
}

void	cPickRockBonusGame::Destroy()
{
	if( m_pMPDIList )
	{
		//cGameApp::m_spAnimationParser->RemoveObject(m_pMPDIList->GetName());
		//m_pMPDIList = 0;
	}
	m_ImageButtonList.Destroy();
}

void	cPickRockBonusGame::Update(float e_fElpaseTime)
{
	
	if( !m_TimeForNoActionToLeave.bTragetTimrReached )
	{
		m_TimeForNoActionToLeave.Update(e_fElpaseTime);
		if(m_TimeForNoActionToLeave.bTragetTimrReached )
		{
			m_bPickRockToWinMoneyDone = true;
		}
	}
	if( m_bPickRockToWinMoneyDone )
	{
		if( m_pLeavingMPDI1->IsAnimationDone() )
		{
			if( m_pLeavingMPDI2->IsAnimationDone() )
			{
				this->m_bDone = true;
			}
			else
				m_pLeavingMPDI2->Update(e_fElpaseTime);
		}
		else
			m_pLeavingMPDI1->Update(e_fElpaseTime);
	}
	else
	{
		if( m_pEnteringMPDI->IsAnimationDone() )
		{
			m_ImageButtonList.Update(e_fElpaseTime);
			int	l_iCount = m_ImageButtonList.Count();
			for( int i=0;i<l_iCount;++i )
			{
				if( m_ImageButtonList[i]->m_iMoney == 0 && m_ImageButtonList[i]->IsSatisfiedCondition() )
				{
					m_bPickRockToWinMoneyDone = true;
					return;
				}
			}
		}
		else
			m_pEnteringMPDI->Update(e_fElpaseTime);
	}
}

void	cPickRockBonusGame::Render()
{
	if( m_bPickRockToWinMoneyDone )
	{
		if( m_pLeavingMPDI1->IsAnimationDone() )
		{
			g_pSlotGamePhase->m_pSlotLineObjectManagerList->Render();
			m_pLeavingMPDI2->Render();
		}
		else
		{
			//g_pSlotGamePhase->m_pSlotLineObjectManagerList->RenderFrame();
			m_ImageButtonList.Render();
			m_pLeavingMPDI1->Render();
		}
		
	}
	else
	{
		if( m_pEnteringMPDI->IsAnimationDone() )
		{
			//g_pSlotGamePhase->m_pSlotLineObjectManagerList->RenderFrame();
			m_ImageButtonList.Render();
		}
		else
		{
			if( m_pEnteringMPDI->GetCurrentProgress() >0.5f )
			{
				//g_pSlotGamePhase->m_pSlotLineObjectManagerList->RenderFrame();
				m_ImageButtonList.Render();
			}
			else
				g_pSlotGamePhase->m_pSlotLineObjectManagerList->Render();
			m_pEnteringMPDI->Render();
		}
		
	}
}