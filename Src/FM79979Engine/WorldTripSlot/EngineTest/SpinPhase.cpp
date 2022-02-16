#include "stdafx.h"
#include "SpinPhase.h"
#include "SlotObject.h"
#include "SlotGamePhase.h"
#include "WinMoneyLine.h"

#include "GameMessage.h"

cSpinPhase::cSpinPhase()
{
	this->SetName(SPIN_PHASE_NAME);
	m_pConstantUI = 0;
}

cSpinPhase::~cSpinPhase()

{
	SAFE_DELETE(m_pConstantUI);
}


void	cSpinPhase::Init()
{
	if( !m_pConstantUI )
	{
		cMPDIList*l_pMPDIList = cGameApp::m_spAnimationParser->GetMPDIListByFileName(UI_MPDILIST_FILE_NAME);
		m_pConstantUI = new cMPDI(l_pMPDIList->GetObject(UPPER_UI));
		m_pConstantUI->Init();
		m_pConstantUI->Update(EPSIONAL);
		m_pConstantUI->RefreshTotalPlayTime();
	}
	if( g_pSlotGamePhase->m_pBetBehavior->m_iBonusMoneyRate != 1 )
	{
		if(g_pSlotGamePhase->m_pBetBehaviorResult->GetCurrentTablesWinMoney())
		{
		}
	}
	else
	{
	}
	//m_bCheckSpinBurronIsUp = false;
	if(!g_pSlotGamePhase->m_bFreeToPlay)
	{

	}

	for( int i=0;i<50;++i )
		m_fSpeedUp[i] = 1.f;
}

void	cSpinPhase::Update(float e_fElpaseTime)
{
	//if( !m_bCheckSpinBurronIsUp )
	//{
	//	if( !cGameApp::m_sucKeyData['S'] )
	//		m_bCheckSpinBurronIsUp  = true;
	//}
	if( cGameApp::m_sucKeyData['S'] )
		e_fElpaseTime *= 2.5f;
	//unsigned char	l_cKEyData[5] = {'C','V','B','N','M'};
	//for( int i=0;i<5;++i )
	//{
	//	if(cGameApp::m_sucKeyData[l_cKEyData[i]])
	//	{
	//		m_fSpeedUp[i] = 3.f;
	//	}
	//}

    int l_iCount = g_pSlotGamePhase->m_pSlotLineObjectManagerList->Count();
    for(int i=0;i<l_iCount;++i)
    {
		g_pSlotGamePhase->m_pSlotLineObjectManagerList->GetObject(i)->Update(m_fSpeedUp[i]*e_fElpaseTime);
    }
	//g_pSlotGamePhase->m_pSlotLineObjectManagerList->Update(e_fElpaseTime);
	//m_pConstantUI->Update(e_fElpaseTime);
	l_iCount = g_pSlotGamePhase->m_pSlotLineObjectManagerList->Count();
	if( g_pSlotGamePhase->m_pSlotLineObjectManagerList->IsSpinedDone() )
	{
		this->m_bSatisfiedCondition = true;
	}
}

void	cSpinPhase::Render()
{
	m_pConstantUI->Render();
	//WCHAR*l_str = ValueToStringW(g_pSlotGamePhase->m_pSlotLineObjectManagerList->GetObject(g_pSlotGamePhase->m_pSlotLineObjectManagerList->Count()-1)->GetRestTime());
#ifdef _DEBUG
	cGameApp::m_spGlyphFontRender->RenderFont(0,150,ValueToStringW(g_pSlotGamePhase->m_pSlotLineObjectManagerList->GetObject(0)->GetRestTime()).c_str());
#endif
	g_pSlotGamePhase->m_pBetBehavior->Render();
	//if( g_pSlotGamePhase->m_pSlotLineObjectManagerList->IsSpinedDone() )
	//{

	//}
	g_pSlotGamePhase->m_pSlotLineObjectManagerList->Render();
}

void	cSpinPhase::DebugRender()
{

}

void	cSpinPhase::MouseDown(int e_iX,int e_iY)
{

}

void	cSpinPhase::MouseUp(int e_iX,int e_iY)
{

}

void	cSpinPhase::MouseMove(int e_iX,int e_iY)
{

}

void	cSpinPhase::KeyUp(char e_cKey)
{
	//this->m_bSatisfiedCondition = true;
}

void*	cSpinPhase::GetData()
{
	return 0;
}

const WCHAR*	cSpinPhase::GetNextPhaseName()
{
#ifdef _DEBUG
	//OutputDebugString(L"leave cSpinPhase\n");
#endif
	//return CALCULATE_SCORE_PHASE_NAME;
	//if( g_pSlotGamePhase->m_pBetBehavior->m_WinMoneyLineIndex.size() == 0 )
	//	return BET_PHASE_NAME;
	return EXTRA_GAME_PHASE_NAME;
}