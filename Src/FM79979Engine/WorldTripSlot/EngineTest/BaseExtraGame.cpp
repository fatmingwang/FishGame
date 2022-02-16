#include "stdafx.h"
#include "BaseExtraGame.h"
#include "SlotObject.h"
#include "SlotGamePhase.h"

const wchar_t*         cBaseExtraGame::TypeID( L"cBaseExtraGame" );
cBaseExtraGame::cBaseExtraGame(const WCHAR*e_strConditionName,int e_iConditionCount)
{
	m_eFreeGameList = eFGL_MAX;
	m_strConditionName = e_strConditionName;
	m_iConditionCount = e_iConditionCount;
	m_bStart = false; 
	m_bDone = true; 
}

cBaseExtraGame::~cBaseExtraGame()
{

}

bool	cBaseExtraGame::IsDone()
{
	return m_bDone;
}

void			cBaseExtraGame::Start()
{
	m_bStart = true;
	LoadResource();
	m_bDone = false;
}

void	cBaseExtraGame::Init()
{
	m_bStart = false; 
	m_bDone = false;
}

bool	cBaseExtraGame::InternalConditionCheck()
{
	int	l_iCount = g_pSlotGamePhase->m_pSlotLineObjectManagerList->GetNameCount(this->m_strConditionName.c_str());
	if(this->m_iConditionCount<=l_iCount)
		return true;
	return false;
}

bool	cBaseExtraGame::IsConditionOk()
{
	int	l_iData = m_eFreeGameList;
	if(g_pSlotGamePhase->m_pbForceIntoFreeGame[this->m_eFreeGameList])
	{
		g_pSlotGamePhase->m_pbForceIntoFreeGame[this->m_eFreeGameList] = false;
		return true;
	}
	return InternalConditionCheck();
}