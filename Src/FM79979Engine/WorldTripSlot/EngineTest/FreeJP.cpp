#include "stdafx.h"
#include "FreeJP.h"
#include "SlotGamePhase.h"
#include "SlotObject.h"
#include "WinMoneyLine.h"
#include "GameNetwork.h"
#include "JPEffect.h"
#include "WorldTripSlotApp.h"
const wchar_t*         cFreeJPGame::TypeID( L"cFreeJPGame" );

cFreeJPGame::cFreeJPGame(const WCHAR*e_strConditionName,int e_iConditionCount,int e_iJPMoney,cMPDI*e_pConstantUI)
:cBaseExtraGame(e_strConditionName,e_iConditionCount)
{
	m_pConstantUI = e_pConstantUI;
	m_iJPMoney = e_iJPMoney;
	m_eFreeGameList = eFGL_WHOLE_TARGET_SAME;
	m_pWinmoneyLines = 0;
	m_pJPEffect = 0;
}

cFreeJPGame::~cFreeJPGame()
{
	SAFE_DELETE(m_pJPEffect);
	SAFE_DELETE(m_pWinmoneyLines);
}

bool	cFreeJPGame::InternalConditionCheck()
{
	int	l_iSize = (int)g_pSlotGamePhase->m_pSlotRuleData->m_LineRuleList.size();
	if( l_iSize )
	{
		//int	l_iCount = g_pSlotGamePhase->m_pSlotLineObjectManagerList->Count();
		std::vector<wstring>	l_strInSearchLineObjectName;
		//int	l_iSize = (int)cWorldTripSlotApp::->m_LineRuleList.size();
		int	l_iConditionImageIndex = g_pSlotGamePhase->m_pSlotLineObjectManagerList->GetObject(0)->GetAllSpinObjectPI()->GetObjectIndexByName(this->m_strConditionName.c_str());
		for( int i=0;i<l_iSize;++i )
		{
			sLineRule*l_pLineRule = g_pSlotGamePhase->m_pSlotRuleData->m_LineRuleList[i];
			char*l_pcLineIndex = l_pLineRule->pcLineObjectIndex;
			int	l_iCount = l_pLineRule->cOneLineCount;
			bool	l_bSame = true;
			for( int i=0;i<l_iCount;++i )
			{
				if( GetLoopMovingObjectByIndex(l_pcLineIndex[i])->iImageIndex != l_iConditionImageIndex )
				{
					l_bSame = false;
				}
			}
			if( l_bSame )
			{
				return true;
			}
		}
	}
	return false;
}

void	cFreeJPGame::Destroy()
{
#ifdef _DEBUG
//	OutputDebugString(L"leave cFreeJPGame\n");
#endif
}

void	cFreeJPGame::LoadResource()
{
	if( !m_pJPEffect )
	{
		m_pJPEffect = new cJPEffect(1);
	}
	m_pJPEffect->Init(*cWorldTripSlotApp::m_spPlayerData->GetJP(eJPT_NETWORK));
	//m_pJPEffect->Init(99999);
	sSlotBetTableResult*l_pTableResult = g_pSlotGamePhase->m_pBetBehaviorResult->m_TableResultList[0];
	m_pWinmoneyLines->Init(l_pTableResult);
	m_pWinmoneyLines->SetDone();
}

void	cFreeJPGame::Init()
{
	cBaseExtraGame::Init();
	if( !m_pWinmoneyLines )
	m_pWinmoneyLines = new cWinmoneyLines(BET_LINES_MPDI_FILE_NAME);
	int*l_pJP = cWorldTripSlotApp::m_spPlayerData->GetJP(eJPT_NETWORK);
	m_iJPMoney = *l_pJP;
	m_iStartJPWinMoney = g_pSlotGamePhase->m_pBetBehaviorResult->GetCurrentTablesWinMoney();
	m_iFinalJPWinMoney = *cWorldTripSlotApp::m_spPlayerData->GetJP(eJPT_NETWORK)+m_iStartJPWinMoney;
}

void	cFreeJPGame::Update(float e_fElpaseTime)
{
	if( !m_pWinmoneyLines->IsDone() )
	{
		m_pWinmoneyLines->UpdateWinMoneyLinesByStep(e_fElpaseTime);
	}
	else
	{
		if( !m_pJPEffect->IsDone() )
		{
			m_pJPEffect->Update(e_fElpaseTime);
			float	l_fProgress = m_pJPEffect->GetCurrentProgress();
			if( l_fProgress >1.f )
				l_fProgress = 1.f;
			g_pSlotGamePhase->m_pBetBehavior->m_iWinMoney = UT::LERP(m_iStartJPWinMoney,m_iFinalJPWinMoney,l_fProgress);
			int*l_pJP = cWorldTripSlotApp::m_spPlayerData->GetJP(eJPT_NETWORK);
			*l_pJP = (int)((1-l_fProgress)*m_iJPMoney);
			m_pJPEffect->SetNumerial(*l_pJP);
		}
		if( m_pJPEffect->IsDone() )
		{
			m_bDone = true;
			g_pSlotGamePhase->m_pBetBehavior->m_iWinMoney = m_iFinalJPWinMoney;

			//assign refund money
			g_pSlotGamePhase->m_pBetBehaviorResult->ExtraGameWinExtraMoney(*cWorldTripSlotApp::m_spPlayerData->GetJP(eJPT_NETWORK));
			cWorldTripSlotApp::m_spPlayerData->SetJP(eJPT_NETWORK,0);
			int*l_piCount = cWorldTripSlotApp::m_spPlayerData->GetJPOpenCount(eJPT_NETWORK);
			if( l_piCount )
				*l_piCount += 1;
			WRITELOGFILE("NETWORK	JP OPEN!!!!!");
			cWorldTripSlotApp::m_sfDebugValue = 1.f;
		}
	}
}

void	cFreeJPGame::Render()
{
	m_pConstantUI->Render();
	g_pSlotGamePhase->m_pSlotLineObjectManagerList->Render();
	m_pWinmoneyLines->Render(0);
	if( m_pWinmoneyLines->IsDone() )
		m_pJPEffect->Render();
}