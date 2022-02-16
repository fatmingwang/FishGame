#include "stdafx.h"
#include "SlotBetResult.h"
#include "SlotGamePhase.h"
#include "SlotRule.h"
#include "SlotProbabilityStatsticData.h"
#include "PlayerData.h"
#include "StageData.h"
#include "SlotObject.h"
#include "WorldTripSlotApp.h"
//from control panel

int	sSlotBetTableResult::NumSpinObjectInTable(int e_iSpinObjectIndex)
{
	int	l_iSize = (int)SpinObjectResultList.size();
	int	l_iNum = 0;
	for( int i =0;i<l_iSize;++i )
	{
		if( SpinObjectResultList[i] == e_iSpinObjectIndex )
			++l_iNum;
	}
	return l_iNum;
}

cSlotBetResult::cSlotBetResult(float e_fExpectProbability)
{
	m_fExpectProbability = e_fExpectProbability;
	m_iCurrentRoundBetMoney = 0;
	m_iCurrentRoundOutMoney = 0;

	m_pSlotProbabilityStatsticData = 0;
}

cSlotBetResult::~cSlotBetResult()
{
	ClearLastTableResult();
	SAFE_DELETE(m_pSlotProbabilityStatsticData);
}

float	cSlotBetResult::GetBalanceProbability()
{
	return (float)((double)(*m_pSlotProbabilityStatsticData->m_pi64OutMoney)/(*m_pSlotProbabilityStatsticData->m_pi64InMoney));
	//return (float)*m_piTotalOutMoney/(*m_piTotalInMoney); 
}

int	cSlotBetResult::GetCurrentRoundBetBalance()
{
	return m_iCurrentRoundBetMoney-m_iCurrentRoundOutMoney;
}


int64		cSlotBetResult::GetOutMoney()
{
	return *m_pSlotProbabilityStatsticData->m_pi64OutMoney;
}

int64		cSlotBetResult::GetInMoney()
{
	return *m_pSlotProbabilityStatsticData->m_pi64InMoney;
}

int64		cSlotBetResult::GetRevenue()
{
	return (*m_pSlotProbabilityStatsticData->m_pi64InMoney)-(*m_pSlotProbabilityStatsticData->m_pi64OutMoney);
}

int	cSlotBetResult::GetCurrentTablesWinMoney()
{
	size_t	l_uiSize = m_TableResultList.size();
	int	l_iWinMoney = 0;
	for( size_t i=0;i<l_uiSize;++i )
	{
		l_iWinMoney += m_TableResultList[i]->iWinMoney;
	}
	return l_iWinMoney;
}

void	cSlotBetResult::ClearLastTableResult()
{
	DELETE_VECTOR(m_TableResultList,sSlotBetTableResult*);
	if( !m_pSlotProbabilityStatsticData && g_pSlotGamePhase->m_pSlotRuleData )
		m_pSlotProbabilityStatsticData = new cSlotProbabilityStatsticData(g_pSlotGamePhase->m_pStageData->m_strStageName.c_str(),&g_pSlotGamePhase->m_pSlotRuleData->m_SpinObjectDataManager,g_pSlotGamePhase->m_pSlotRuleData);
}

void	cSlotBetResult::AssignTableData(int e_iTotalBetMoney)
{
	m_iCurrentRoundBetMoney = e_iTotalBetMoney;
	m_pSlotProbabilityStatsticData->UpdateData(&m_TableResultList,m_iCurrentRoundBetMoney,cWorldTripSlotApp::m_spPlayerData->m_i64Money);
}

void	cSlotBetResult::ExtraGameWinExtraMoney(int e_iOutMoney)
{
	m_pSlotProbabilityStatsticData->ExtraGameWinExtraMoney(e_iOutMoney);
}