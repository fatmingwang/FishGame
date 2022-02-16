#ifndef _BET_BEHAVIOR_RESULT_H_
#define _BET_BEHAVIOR_RESULT_H_


//#include "PlayerData.h"
class	cSlotBetBehavior;
class	cSlotProbabilityStatsticData;
struct	sSlotBetTableResult;
class	cSlotBetResult
{
	friend class						cSlotBetBehavior;
	//the money player has been bet for play
	//as same as g_pSlotGamePhase->m_pBetBehavior->m_iTotalBet but if free game it's zero
	int									m_iCurrentRoundBetMoney;
	//the money the machine has out.
	int									m_iCurrentRoundOutMoney;
	//
	float								m_fExpectProbability;
	cSlotProbabilityStatsticData*		m_pSlotProbabilityStatsticData;
public:
	cSlotBetResult(float e_fExpectProbability);
	~cSlotBetResult();
	//result tables,because game make result explosive,so it's possible has a lot TableResult
	std::vector<sSlotBetTableResult*>	m_TableResultList;
	//how much is this round win?
	int									GetCurrentTablesWinMoney();
	//total bet and total score
	float								GetBalanceProbability();
	//
	float								GetExpectProbability(){ return m_fExpectProbability; }
	int									GetCurrentRoundBetBalance();
	int64								GetRevenue();
	int64								GetOutMoney();
	int64								GetInMoney();
	int									GetCurrentRoundBetMoney(){ return m_iCurrentRoundBetMoney; }
	int									GetLastRoundBalance();
	void								ClearLastTableResult();
	//for probability
	//assign tables result into control penel,call this at BetPhase::SetupResult
	void								AssignTableData(int e_iTotalBetMoney);
	//extra game tfor out money
	//if extra game win extra money,call this to modify refund money.
	void								ExtraGameWinExtraMoney(int e_iOutMoney);
};

#endif