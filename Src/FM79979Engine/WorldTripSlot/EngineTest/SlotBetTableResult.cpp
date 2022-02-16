#include "stdafx.h"
#include "SlotBetTableResult.h"
#include "SlotGamePhase.h"
#include "SlotRule.h"

sSlotBetTableResult::sSlotBetTableResult()
{
	pbWinMoneySpinObject = 0;
	iWinMoney = 0;
	int	l_iColumn = g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x;
	assert(l_iColumn<10&&"call fatming because he is lazy to optmize this");
	//piConsecutiveScore = new int[l_iColumn];
	//piConsecutiveCount = new int[l_iColumn];
	memset(piConsecutiveScore,0,sizeof(int)*l_iColumn);
	memset(piConsecutiveCount,0,sizeof(int)*l_iColumn);
}

sSlotBetTableResult::~sSlotBetTableResult()
{
	SAFE_DELETE(pbWinMoneySpinObject);
	//SAFE_DELETE_ARRAY(piConsecutiveScore);
	//SAFE_DELETE_ARRAY(piConsecutiveCount);
}

void	sSlotBetTableResult::ClearData(int e_iNumLines)
{
	int	l_iColumn = g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x;
	memset(piConsecutiveScore,0,sizeof(int)*l_iColumn);
	memset(piConsecutiveCount,0,sizeof(int)*l_iColumn);
	AllLineList.clear();
	AllLineList.reserve(e_iNumLines);
	WinMoneyLineList.clear();
	WinMoneyLineIndexList.clear();
	iWinMoney = 0;
}