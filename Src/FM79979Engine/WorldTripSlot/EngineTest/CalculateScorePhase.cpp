#include "stdafx.h"
#include "CalculateScorePhase.h"
#include "SlotObject.h"
#include "SlotGamePhase.h"
#include "WorldTripSlotApp.h"
extern int	g_iExplosionCount;
cCalculateScorePhase::cCalculateScorePhase()
{
	m_iShowMPDIMoney = 2000;
	m_pWinMoneyOver2000MPDI = 0;
	m_pConstantUI = 0;
	m_IdleTimeForShowHowManyMoneyWin.SetTargetTime(1.f);
	this->SetName(CALCULATE_SCORE_PHASE_NAME);
}

cCalculateScorePhase::~cCalculateScorePhase()
{
	SAFE_DELETE(m_pConstantUI);
}

void	cCalculateScorePhase::Init()
{
	if( !m_pConstantUI )
	{
		cNodeISAX	l_NodeISAX;
		bool	l_b = l_NodeISAX.ParseDataIntoXMLNode(CALCULATE_SCORE_PHASE_DATA);
		if( l_b )
		{
			TiXmlElement*l_pElement = l_NodeISAX.GetRootElement();
			const WCHAR*l_strShowTime = l_pElement->Attribute(L"ShowTime");
			const WCHAR*l_strShowMPDIMoney = l_pElement->Attribute(L"ShowMPDIMoney");
			const WCHAR*l_strShowWinMoneyIdleTime = l_pElement->Attribute(L"ShowWinMoneyIdleTime");
			if( !m_pConstantUI )
			{
				cMPDIList*l_pMPDIList = cGameApp::m_spAnimationParser->GetMPDIListByFileName(UI_MPDILIST_FILE_NAME);
				m_pConstantUI = new cMPDI(l_pMPDIList->GetObject(UPPER_UI));
				m_pConstantUI->Init();
				m_pConstantUI->Update(EPSIONAL);
				m_iShowMPDIMoney = _wtoi(l_strShowMPDIMoney);
			}
			int	l_iNumLineResult = g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y;
			if(l_iNumLineResult>100)
			{
				UT::ErrorMsg(L"total result is not allow over 100 lines",L"Error");
			}
			float	l_fTimeForWho = (float)_wtof(l_strShowTime);
			m_TimeForScoreToChangeUserMoney.SetTargetTime(l_fTimeForWho);
			if( l_strShowWinMoneyIdleTime )
			{
				float	l_fTime = (float)_wtof(l_strShowWinMoneyIdleTime);
				m_IdleTimeForShowHowManyMoneyWin.SetTargetTime(l_fTime);
			}
		}
	}
	m_iPlayerOriginalMoney = cWorldTripSlotApp::m_spPlayerData->m_i64Money;
	m_TimeForScoreToChangeUserMoney.Start();
	//g_pSlotGamePhase->m_pBetBehavior->CalculateScoreWithRound();
	m_iWinMoney = g_pSlotGamePhase->m_pBetBehavior->m_iWinMoney;
	m_iPlayerMoney = cWorldTripSlotApp::m_spPlayerData->m_i64Money;
	m_IdleTimeForShowHowManyMoneyWin.Start();
	g_pSlotGamePhase->m_pSlotLineObjectManagerList->Init();
	if( m_iWinMoney>0 )
	{//play win omney sound
	
	}
	else
	{
		this->m_bSatisfiedCondition = true;
	}
}

void	cCalculateScorePhase::Update(float e_fElpaseTime)
{
	if(m_IdleTimeForShowHowManyMoneyWin.bTragetTimrReached)
	{
		//m_pConstantUI->Update(e_fElpaseTime);
		m_TimeForScoreToChangeUserMoney.Update(e_fElpaseTime);
		if( m_TimeForScoreToChangeUserMoney.bTragetTimrReached )
		{
			g_pSlotGamePhase->m_pBetBehavior->m_iWinMoney = 0;
			cWorldTripSlotApp::m_spPlayerData->m_i64Money = m_iPlayerOriginalMoney+m_iWinMoney;
			g_iExplosionCount = 0;
			this->m_bSatisfiedCondition = true;
		}
		else
		if( m_iWinMoney >0 )
		{
			int	l_iMoney = (int)((1-m_TimeForScoreToChangeUserMoney.GetLERP())*m_iWinMoney);
			g_pSlotGamePhase->m_pBetBehavior->m_iWinMoney = l_iMoney;
			cWorldTripSlotApp::m_spPlayerData->m_i64Money = m_iPlayerMoney+(m_iWinMoney-g_pSlotGamePhase->m_pBetBehavior->m_iWinMoney);
			if( m_pWinMoneyOver2000MPDI )
				m_pWinMoneyOver2000MPDI->Update(e_fElpaseTime);
		}
	}
	else
	{
		m_IdleTimeForShowHowManyMoneyWin.Update(e_fElpaseTime);
		if(m_IdleTimeForShowHowManyMoneyWin.bTragetTimrReached)
		{
			if( m_pWinMoneyOver2000MPDI )
				m_pWinMoneyOver2000MPDI->Init();
		}
	}
}

void	cCalculateScorePhase::Render()
{
	m_pConstantUI->Render();
	g_pSlotGamePhase->m_pBetBehavior->Render();
	g_pSlotGamePhase->m_pSlotLineObjectManagerList->Render();
	if(m_IdleTimeForShowHowManyMoneyWin.bTragetTimrReached)
	{
		if( this->m_iWinMoney >m_iShowMPDIMoney )
		{
			if( m_pWinMoneyOver2000MPDI )
				m_pWinMoneyOver2000MPDI->Render();
		}
	}
	//just skip it 
#ifdef _DEBUG
	cGameApp::m_spGlyphFontRender->RenderFont(0,150,ValueToStringW(m_TimeForScoreToChangeUserMoney.fRestTime).c_str());
#endif
}

void	cCalculateScorePhase::DebugRender()
{

}

void	cCalculateScorePhase::MouseDown(int e_iX,int e_iY)
{

}

void	cCalculateScorePhase::MouseUp(int e_iX,int e_iY)
{

}

void	cCalculateScorePhase::MouseMove(int e_iX,int e_iY)
{

}

void	cCalculateScorePhase::KeyUp(char e_cKey)
{
	//this->m_bSatisfiedCondition = true;
}

const WCHAR*	cCalculateScorePhase::GetNextPhaseName()
{
#ifdef _DEBUG
	//OutputDebugString(L"leave cCalculateScorePhase\n");
#endif
	return BET_PHASE_NAME;
}