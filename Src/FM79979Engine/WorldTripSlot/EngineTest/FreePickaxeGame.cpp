#include "stdafx.h"
#include "FreePickaxeGame.h"
#include "SlotGamePhase.h"
#include "SlotObject.h"

const wchar_t*         cFreePickaxeGame::TypeID( L"cFreePickaxeGame" );

cFreePickaxeGame::cFreePickaxeGame(const WCHAR*e_strConditionName,int e_iConditionCount)
:cBaseExtraGame(e_strConditionName,e_iConditionCount)
{
	m_eFreeGameList = eFGL_PICKAXE;
	m_pMPDIList = 0;
}

cFreePickaxeGame::~cFreePickaxeGame()
{
}

bool	cFreePickaxeGame::InternalConditionCheck()
{
	int	l_iCount = g_pSlotGamePhase->m_pSlotLineObjectManagerList->GetNameCount(this->m_strConditionName.c_str());
	//it's already on free game2
	if( g_pSlotGamePhase->m_bFreeToPlay && l_iCount>0 )
		return true;
	if(this->m_iConditionCount<=l_iCount)
		return true;
	return false;
}

void	cFreePickaxeGame::LoadResource()
{
	if( !m_pMPDIList )
	{
		m_pMPDIList = cGameApp::GetMPDIListByFileName(ANIMATED_MOVE_SPIN_OBJECT_MPDILIST_FILE_NAME);
		m_pPickaxeMPDI = m_pMPDIList->GetObject(FREE_GAME_PICKAXE_MPDI_NAME);
	}
	m_pPickaxeMPDI->Init();
}

void	cFreePickaxeGame::Init()
{
	cBaseExtraGame::Init();
}

int	cFreePickaxeGame::FreeGameCountToAdd()
{
	int	l_iCount = g_pSlotGamePhase->m_pSlotLineObjectManagerList->GetNameCount(this->m_strConditionName.c_str());
	if( !g_pSlotGamePhase->m_bFreeToPlay)
	{
		l_iCount = l_iCount+1-this->m_iConditionCount;
	}
	return l_iCount;
}

void	cFreePickaxeGame::Update(float e_fElpaseTime)
{
	m_pPickaxeMPDI->Update(e_fElpaseTime);
	if( m_pPickaxeMPDI->IsAnimationDone() )
	{
		//g_pSlotGamePhase->m_iFreeGame += FreeGameCountToAdd();
		m_bDone = true;
	}
}

void	cFreePickaxeGame::Render()
{
	g_pSlotGamePhase->m_pSlotLineObjectManagerList->Render();
	cPuzzleImageUnit*l_pPIUnit = g_pSlotGamePhase->m_pSlotLineObjectManagerList->GetObject(0)->GetAllSpinObjectPI()->GetObject(0);
	Vector2	l_vSize((float)*l_pPIUnit->GetWidth(),(float)*l_pPIUnit->GetHeight());
	for(int i=0;i<g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x;++i)
	{
		cSlotObjectManager*l_pSlotObjectManager = g_pSlotGamePhase->m_pSlotLineObjectManagerList->GetObject(i);
		for(int j=0;j<g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y;++j)
		{
			if(!wcscmp(l_pSlotObjectManager->GetNameByResultIndex(j),m_strConditionName.c_str()))
			{

				sLoopMovingObject*l_pLoopMovingObject = l_pSlotObjectManager->GetLoopMovingObject(j);
				Vector3	l_vPos = l_pLoopMovingObject->vCurrentPos;
				l_vPos.x += l_vSize.x/2.f;
				l_vPos.y += l_vSize.y/2.f;
				m_pPickaxeMPDI->SetPos(l_vPos);
				m_pPickaxeMPDI->Render();
			}
		}
	}
	int	l_iCount = FreeGameCountToAdd();
	std::wstring	l_str = ValueToStringW(g_pSlotGamePhase->m_iFreeGame);
	l_str += L" + ";
	l_str += ValueToStringW(l_iCount);
	cGameApp::m_spGlyphFontRender->RenderFont(200,200,(WCHAR*)l_str.c_str());
}