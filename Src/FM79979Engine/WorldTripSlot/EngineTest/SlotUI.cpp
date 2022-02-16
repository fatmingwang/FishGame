#include "stdafx.h"
#include "SlotUI.h"
#include "WinMoneyLine.h"
#include "SlotGamePhase.h"
#include "PlayerData.h"
#include "WorldTripSlotApp.h"
cSlotUI::cSlotUI()
{
	m_iNumLinesBet = 1;
	m_pNumerialImage = 0;
	m_pWinmoneyLines = 0;
	for( int i=0;i<eIPL_MAX;++i )
		m_piInfoValue[i] = 0;

	m_piInfoValue[eIPL_TotalBetMoneyPos] = &g_pSlotGamePhase->m_pBetBehavior->m_iTotalBet;
	m_piInfoValue[eIPL_WinMoneyPos] = &g_pSlotGamePhase->m_pBetBehavior->m_iWinMoney;
	m_piInfoValue[eIPL_NumLineBetPos] = &g_pSlotGamePhase->m_pBetBehavior->m_iNumLinesBet;
	m_piInfoValue[eIPL_PerLineBetMoneyPos] = &g_pSlotGamePhase->m_pBetBehavior->m_iPerLineBetMoney;
	m_piInfoValue[eIPL_PlayerMoneyPos] = &cWorldTripSlotApp::m_spPlayerData->m_i64Money;
	m_piInfoValue[eIPL_JP1] = cWorldTripSlotApp::m_spPlayerData->GetJP(eJPT_NETWORK);
	m_piInfoValue[eIPL_JP2] = cWorldTripSlotApp::m_spPlayerData->GetJP(eJPT_LOCAL);
	
	if( !m_pNumerialImage )
	{
		cPuzzleImage*l_pPI = cGameApp::GetPuzzleImageByFileName(L"WorldTripSlot/ImageData/Num.pi");
		m_pNumerialImage = l_pPI->GetNumerialImageByName(L"but_0",L"but_9");
	}
	m_pWinmoneyLines = new cWinmoneyLines(BET_LINES_MPDI_FILE_NAME);
	//get from UI mpdi
	cMPDIList*l_pMPDIList = cGameApp::m_spAnimationParser->GetMPDIListByFileName(UI_MPDILIST_FILE_NAME);
	cMPDI*l_pMPDI = l_pMPDIList->GetObject(L"UIInfoPos");
	WCHAR*l_str[] = {
		L"TotalBetMoneyPos",
		L"WinMoneyPos",
		L"NumLineBetPos",
		L"PerLineBetMoneyPos",
		L"PlayerMoneyPos",
		L"JP1",
		L"JP2",
	};
	for( int i=0;i<eIPL_MAX;++i )
	{
		cSubMPDI*l_p = l_pMPDI->GetObject(l_str[i]);
		if( l_p )
			m_vInfoFontPos[i] = l_p->GetOriginalPointList()[0];
		//m_vInfoFontPos[i].x -= l_p->GetPointData(0)->Size.x/2.f;
		//m_vInfoFontPos[i].y -= l_p->GetPointData(0)->Size.y/2.f;
	}
}

cSlotUI::~cSlotUI()
{
	SAFE_DELETE(m_pNumerialImage);
	SAFE_DELETE(m_pWinmoneyLines);
}

void	cSlotUI::SetBetLines(int e_iNumLines)
{
	m_iNumLinesBet = e_iNumLines;
	m_pWinmoneyLines->StartLines(e_iNumLines);
}

void	cSlotUI::Render()
{
	for( int i=0;i<eIPL_MAX;++i )
	{
		//WCHAR*l_strValue = ValueToStringW(*m_piInfoValue[i]);
		//if( eIPL_WinMoneyPos == i )
		if( m_piInfoValue[i] )
			m_pNumerialImage->DrawOnCenter(*m_piInfoValue[i],(int)m_vInfoFontPos[i].x,(int)m_vInfoFontPos[i].y);
		//cGameApp::m_spGlyphFontRender->RenderFont(m_vInfoFontPos[i].x,m_vInfoFontPos[i].y,l_strValue);
	}
}

void	cSlotUI::DebugRender()
{
	//m_pNumerialImage->Draw(m_iRound,400,20);
}

void	cSlotUI::UpdateSelectedLines(float e_fElpaseTime)
{
	m_pWinmoneyLines->UpdateLines(m_iNumLinesBet,e_fElpaseTime);
}
void	cSlotUI::RenderBetLines()
{
	m_pWinmoneyLines->RenderBetLines(m_iNumLinesBet);
}