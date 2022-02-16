#ifndef _BET_BEHAVIOR_H_
#define _BET_BEHAVIOR_H_

#include "SlotGoodTables.h"

class	cPlayerData;
class	cCalculateScorePhase;
class	cSlotObjectManager;
class	cFreeExplosionGame;
class	cFreeJPGame;
class	cSpinPhase;
class	cWinMoneySpinObjectMPDI;
class	cSlotUI;
struct	sSlotBetTableResult;
class	cSlotBetBehavior
{
	struct	sSpinObjectNameAndCount
	{
		std::wstring		strName;
		int					iCount;
		std::vector<int>	WinMoneySpinObjectIndexOfTable;	//the index of table
		std::vector<int>	IndexOfLine;					//check spinobject is in the first of line if not it doesnt match.
		void	SortIndex();
	};
	friend class	cWinMoneySpinObjectMPDI;
	friend class	cFreeExplosionGame;
	friend class	cCalculateScorePhase;
	friend class	cBetPhase;
	friend class	cSpinPhase;
	friend class	cWorldTripSlotApp;
	friend class	cFreeJPGame;
	friend class	cSlotGoodTables;
	friend class	cSlotUI;
	//===================================
	//====================================
	//find bet able money
	int										GetFocusBetMoneyIndexInVector();
	//cPlayerData*							m_pPlayerData;
	//
	int										m_iNumLinesBet;
	int										m_iPerLineBetMoney;
	int										m_iTotalBet;
	int										m_iWinMoney;
	//how many round spined
	int										m_iRound;
	void									SetBetData(int e_iNumLineBet,int e_iPerLineBetMoney);
	//
	std::vector<sSpinObjectNameAndCount>	GetNameAndCountByLineIndexByConsecutive(int e_iIndex,sSlotBetTableResult*e_pTableResult);
	sSpinObjectNameAndCount*				GetMatchCountAndNameAndCountData(std::vector<sSpinObjectNameAndCount>*e_pNameAndCountList,int e_iIndex);
	void									CalculateScoreByLine();
	//this one could be any of spin object,and if a line of sponbojects all is it,win JP
	//dynamic to change this value so win money could be more
	//it will be set at cBetPhase::Init()
	int										m_iBonusMoneyRate;
	cBasicSound*							m_pButtonClick;
	//cheat code
	sSlotBetTableResult*					GenerateCheatTableByCode();
	//a random result
	void									GenerateRandomSpinResult(cSlotBetResult*e_pBetBehaviorResult);
	void									GenerateProbabilitySpinResult(cSlotBetResult*e_pBetBehaviorResult);
	//
	sSlotBetTableResult*					ProcessMaxBetResultForProbabilityThenCalRealBetResultTable(int e_iMinScore,int e_iMaxScore,int e_iMultiplier,bool*e_pbWinJP,bool e_bFreeGame);
	sSlotBetTableResult*					ProcessMaxBetResultForProbabilityThenCalRealBetResultTableFromOldTable(sSlotBetTableResult*e_pOldTable,int e_iMinScore,int e_iMaxScore,int e_iMultiplier,bool*e_pbWinJP,bool e_bFreeGame);
	cSlotGoodTables							m_SlotGoodTables;
public:
	cSlotBetBehavior();
	~cSlotBetBehavior();
	void									BetMax();
	int										MaxBetMoney();
	void									AddLineBet();
	void									MinusLineBet();
	void									AddBetMoney();
	void									MinusBetMoney();
	void									ExchangeRateChange(int e_iCurrentPayRate,int e_iNewRate);
	//====================================
	void									Init();
	void									Update(float e_fElpaseTime);
	void									Render();

	void									DebugRender();
	void									MouseDown(int e_iX,int e_iY);
	void									MouseUp(int e_iX,int e_iY);
	void									MouseMove(int e_iX,int e_iY);
	//====================================
	//clear last round data
	void									CalculateScoreWithRound();
	//====================================
	//
	void									AddWinMoney(int e_iMoney);
	int										GetWinMoney(){return m_iWinMoney;}
	//assign table line data
	void									AssignLinesData(sSlotBetTableResult*e_pTableResult);
	//a random table generate from old table
	sSlotBetTableResult*					GenerateSpinExplosionResultWithRandom(sSlotBetTableResult*e_pTableResult);
	//a result table from old table with empty spinobject(-1),e_pEmptyIndexList for new empty indecies if need
	sSlotBetTableResult*					GenerateSpinExplosionResultWithEmptySpinObject(sSlotBetTableResult*e_pTableResult,std::vector<int>*e_pEmptyIndexList = 0);
	//calculate table result
	void									CalculateScoreConsecutive(sSlotBetTableResult*e_pTableResult);

	int										GetNumLinesBet(){return  m_iNumLinesBet;}
	int										GetPerLineBetMoney(){return m_iPerLineBetMoney;}
	int										GetRound(){return m_iRound;}
	//if JP is win do some internet check
	//bool									NetWorkJPCheck(std::vector<int>*e_pSpinObjectResultList);
	bool									IsJPGameFilledCondition(std::vector<int>	e_ResultList);
};

#endif