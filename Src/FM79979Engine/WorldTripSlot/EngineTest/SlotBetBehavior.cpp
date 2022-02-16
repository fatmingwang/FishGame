#include "stdafx.h"
#include "SlotRule.h"
#include "PlayerData.h"
#include "SlotBetBehavior.h"
#include "SlotObject.h"
#include "SlotGamePhase.h"
#include "AllPhaseDefine.h"
#include "GameNetwork.h"
#include "SlotTableResultGenerator.h"
#include "WorldTripSlotApp.h"
#include "SlotBetTableResult.h"
#include "SlotUI.h"
void	cSlotBetBehavior::sSpinObjectNameAndCount::SortIndex()
{
	std::list<int>	l_List;
	size_t	l_ui = IndexOfLine.size();
	for( size_t i=0;i<l_ui;++i )
	{
		l_List.push_back(IndexOfLine[i]);
	}
	l_List.sort();
	IndexOfLine.clear();
	while( l_List.size() )
	{
		IndexOfLine.push_back(l_List.front());
		l_List.pop_front();
	}
}

cSlotBetBehavior::cSlotBetBehavior()
{
	m_pButtonClick = 0;
	m_SlotGoodTables.ParseWithMyParse("WorldTripSlot/Probability.xml");
	m_iBonusMoneyRate = 1;
	m_iRound = 0;
	m_iNumLinesBet = 1;
	m_iTotalBet = 0;
	m_iWinMoney = 0;
	m_iPerLineBetMoney = 1;
#ifdef _DEBUG
	int	l_iMoney = 0;
	for( size_t i=0;i<g_pSlotGamePhase->m_pSlotRuleData->m_PerLineBetMoneyRule.size();++i )
	{
		assert(l_iMoney<g_pSlotGamePhase->m_pSlotRuleData->m_PerLineBetMoneyRule[i]);
		l_iMoney = g_pSlotGamePhase->m_pSlotRuleData->m_PerLineBetMoneyRule[i];
	}
#endif
}

cSlotBetBehavior::~cSlotBetBehavior()
{
}

void	cSlotBetBehavior::Init()
{
	int	l_iTotalSpinObjectCount = g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x*g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y;
	int	l_iLineRuleSize =  (int)g_pSlotGamePhase->m_pSlotRuleData->m_LineRuleList.size();
	m_iNumLinesBet = 1;
	m_iPerLineBetMoney = g_pSlotGamePhase->m_pSlotRuleData->m_PerLineBetMoneyRule[0];
	m_iTotalBet = m_iNumLinesBet*m_iPerLineBetMoney;
}

void	cSlotBetBehavior::SetBetData(int e_iNumLineBet,int e_iPerLineBetMoney)
{
	m_iNumLinesBet = e_iNumLineBet;
	m_iPerLineBetMoney = e_iPerLineBetMoney;
	m_iTotalBet = m_iNumLinesBet*this->m_iPerLineBetMoney;
}

void	cSlotBetBehavior::AddLineBet()
{
	if( m_pButtonClick )
		m_pButtonClick->Play(true);
	int	l_iSize =  (int)g_pSlotGamePhase->m_pSlotRuleData->m_LineRuleList.size();
	int	l_iNewLineBetCount = m_iNumLinesBet+1;
	if( l_iNewLineBetCount > l_iSize )
	{
		l_iNewLineBetCount = 1;
	}
	SetBetData(l_iNewLineBetCount,m_iPerLineBetMoney);
	g_pSlotGamePhase->m_pSlotUI->SetBetLines(m_iNumLinesBet);
}

int		cSlotBetBehavior::GetFocusBetMoneyIndexInVector()
{
	int	l_iSize = (int)g_pSlotGamePhase->m_pSlotRuleData->m_PerLineBetMoneyRule.size();
	for( int i=0;i<l_iSize;++i )
	{
		if( this->m_iPerLineBetMoney == g_pSlotGamePhase->m_pSlotRuleData->m_PerLineBetMoneyRule[i] )
			return i;
	}
	return -1;
}

void	cSlotBetBehavior::AddBetMoney()
{
	if( m_pButtonClick )
		m_pButtonClick->Play(true);
	int	l_iNewBetMoney = 0;
	int	l_iNewIndex = GetFocusBetMoneyIndexInVector()+1;
	int	l_iSize = (int)g_pSlotGamePhase->m_pSlotRuleData->m_PerLineBetMoneyRule.size();
	if( l_iNewIndex >= l_iSize )
		l_iNewIndex = 0;
	l_iNewBetMoney = g_pSlotGamePhase->m_pSlotRuleData->m_PerLineBetMoneyRule[l_iNewIndex];
	SetBetData(m_iNumLinesBet,l_iNewBetMoney);
}

void	cSlotBetBehavior::MinusLineBet()
{
	if( m_pButtonClick )
		m_pButtonClick->Play(true);
	int	l_iNewLineBetCount = m_iNumLinesBet-1;
	if( l_iNewLineBetCount <= 0 )
	{
		l_iNewLineBetCount = (int)g_pSlotGamePhase->m_pSlotRuleData->m_LineRuleList.size();
	}
	SetBetData(l_iNewLineBetCount,m_iPerLineBetMoney);
	g_pSlotGamePhase->m_pSlotUI->SetBetLines(m_iNumLinesBet);
}

void	cSlotBetBehavior::MinusBetMoney()
{
	if( m_pButtonClick )
		m_pButtonClick->Play(true);
	int	l_iNewBetMoney = 0;
	int	l_iNewIndex = GetFocusBetMoneyIndexInVector()-1;
	int	l_iSize = (int)g_pSlotGamePhase->m_pSlotRuleData->m_PerLineBetMoneyRule.size();
	if( l_iNewIndex <0 )
	{
		l_iNewIndex = l_iSize-1;
	}
	l_iNewBetMoney = g_pSlotGamePhase->m_pSlotRuleData->m_PerLineBetMoneyRule[l_iNewIndex];
	SetBetData(m_iNumLinesBet,l_iNewBetMoney);
}

void	cSlotBetBehavior::ExchangeRateChange(int e_iCurrentPayRate,int e_iNewRate)
{
	cWorldTripSlotApp::m_spPlayerData->m_i64Money /= e_iCurrentPayRate;
	cWorldTripSlotApp::m_spPlayerData->m_i64Money *= e_iNewRate;
	g_pSlotGamePhase->m_iExchangeRate = e_iNewRate;
}

void	cSlotBetBehavior::BetMax()
{
	cGameApp::SoundPlay(MAX_SPIN_SOUND_NAME,true);
	int	l_iSize = (int)g_pSlotGamePhase->m_pSlotRuleData->m_PerLineBetMoneyRule.size();
	this->m_iNumLinesBet = (int)g_pSlotGamePhase->m_pSlotRuleData->m_LineRuleList.size();
	this->m_iPerLineBetMoney = g_pSlotGamePhase->m_pSlotRuleData->m_PerLineBetMoneyRule[l_iSize-1];
	this->m_iTotalBet = m_iPerLineBetMoney*m_iNumLinesBet;
	g_pSlotGamePhase->m_pSlotUI->SetBetLines(m_iNumLinesBet);
}

int		cSlotBetBehavior::MaxBetMoney()
{
	int	l_iSize = (int)g_pSlotGamePhase->m_pSlotRuleData->m_PerLineBetMoneyRule.size();
	int	l_iNumLinesBet = (int)g_pSlotGamePhase->m_pSlotRuleData->m_LineRuleList.size();
	int	l_iPerLineBetMoney = g_pSlotGamePhase->m_pSlotRuleData->m_PerLineBetMoneyRule[l_iSize-1];
	int	l_iTotalBet = m_iPerLineBetMoney*m_iNumLinesBet;
	return l_iTotalBet;
}

void	cSlotBetBehavior::Update(float e_fElpaseTime)
{

}

void	cSlotBetBehavior::Render()
{
	//g_pSlotGamePhase->m_pSlotUI->Render();
	m_SlotGoodTables.Render();
	//m_pNumerialImage->Draw(*g_pSlotGamePhase->m_pBetBehaviorResult->m_piTotalInMoney,100,400);
	//m_pNumerialImage->Draw(*g_pSlotGamePhase->m_pBetBehaviorResult->m_piTotalOutMoney,100,450);
	//m_pNumerialImage->Draw(*g_pSlotGamePhase->m_pBetBehaviorResult->m_piRevenue,100,500);
}


void	cSlotBetBehavior::DebugRender()
{

}

void	cSlotBetBehavior::MouseDown(int e_iX,int e_iY)
{

}

void	cSlotBetBehavior::MouseUp(int e_iX,int e_iY)
{

}

void	cSlotBetBehavior::MouseMove(int e_iX,int e_iY)
{

}

void	cSlotBetBehavior::CalculateScoreWithRound()
{
	++m_iRound;
	m_iWinMoney = 0;
	//if( g_bUseProbability )
		this->GenerateProbabilitySpinResult(g_pSlotGamePhase->m_pBetBehaviorResult);
	//else
	//	this->GenerateRandomSpinResult(g_pSlotGamePhase->m_pBetBehaviorResult);
}


void	cSlotBetBehavior::AddWinMoney(int e_iMoney)
{
	m_iWinMoney += e_iMoney;
}

bool	cSlotBetBehavior::IsJPGameFilledCondition(std::vector<int>	e_ResultList)
{
	size_t	l_iSize = g_pSlotGamePhase->m_pSlotRuleData->m_LineRuleList.size();
	if(m_iNumLinesBet != l_iSize)
	{
		return false;
	}
	const WCHAR*l_strJPName = g_pSlotGamePhase->m_pSlotRuleData->m_strSpecilaSpinObjectName.c_str();
	for( size_t i=0;i<l_iSize;++i )
	{
		bool	l_bSame = true;
		sLineRule*l_pLineRule = g_pSlotGamePhase->m_pSlotRuleData->m_LineRuleList[i];
		for(char j=0;j<l_pLineRule->cOneLineCount;++j)
		{
			int	l_iTableIndex = l_pLineRule->pcLineObjectIndex[j];
			int	l_iSpinObjectID = e_ResultList[l_iTableIndex];
			if( l_iSpinObjectID == -1 || l_iSpinObjectID>11 )
			{
				int a=0;
			}
			const WCHAR*l_strName = g_pSlotGamePhase->m_pSlotRuleData->m_SpinObjectDataManager.GetObject(l_iSpinObjectID)->GetName();
			if( wcscmp(l_strName,l_strJPName) )
			{
				l_bSame = false;
				break;
			}
		}
		if( l_bSame )
			return true;
	}
	return false;
}

//generate necessary random data for spin object's result
std::vector<int>	GenerateSpinResultVector(int e_iCount)
{
	std::vector<int>	l_SpinObjectResult;
	int	l_iSpinObjectMaximum = g_pSlotGamePhase->m_pSlotRuleData->m_SpinObjectDataManager.Count();
	int	l_iNumSpinObject = e_iCount;
	l_SpinObjectResult.resize(l_iNumSpinObject);

	//cheat code
	//int	l_iResult[] = {0,0,6,8,7,2,0,10,2,3,10,8,2,5,11};
	for( int i=0;i<l_iNumSpinObject;++i )
	{
		//l_SpinObjectResult[i] = l_iResult[i];
		int	l_iValue = rand()%g_pSlotGamePhase->m_pSlotRuleData->m_SpinObjectDataManager.m_iTotalProbabilityAmount;
		int	l_iIndex = g_pSlotGamePhase->m_pSlotRuleData->m_SpinObjectDataManager.GetSpinObjectIndexByProbability(l_iValue);
		l_SpinObjectResult[i] = l_iIndex;
		//cheat here
		//if( i%5 == 0 )
			//l_SpinObjectResult[i] = 10;
		//change probability data
	}
	return l_SpinObjectResult;
}

sSlotBetTableResult*cSlotBetBehavior::ProcessMaxBetResultForProbabilityThenCalRealBetResultTable(int e_iMinScore,int e_iMaxScore,int e_iMultiplier,bool*e_pbWinJP,bool e_bFreeGame)
{
	if( e_iMinScore <= 0 && e_iMaxScore <= 0 )
	{
		sSlotBetTableResult*l_pResult = GenerateNoWinMoneyTable(e_bFreeGame,79979);
		CalculateScoreConsecutive(l_pResult);
		return l_pResult;
	}
	//1.first maxima bet data and store original data
	//2.then calculate score and write log
	//3.then  set it back then calculate real score
	//1.
	int	l_iScore = 0;
	int	l_iOriginalNumLinesBet = this->m_iNumLinesBet;	int	l_iOriginalPerLineBetMoney = this->m_iPerLineBetMoney;	int	l_iOriginalTotalBet = m_iTotalBet;
	this->BetMax();
	bool	l_bBetMax = false;;
	if( l_iOriginalNumLinesBet == this->m_iNumLinesBet && 
		l_iOriginalPerLineBetMoney == this->m_iPerLineBetMoney &&
		l_iOriginalTotalBet == m_iTotalBet)
	{
		l_bBetMax = true;
	}
	//2.
	sSlotBetTableResult*l_pLoacalTableResult = GenerateMultilineWinMoneyNewTableWithMatchCount(e_iMinScore,e_iMaxScore,e_iMultiplier,&l_iScore,e_bFreeGame);
	*e_pbWinJP = IsJPGameFilledCondition(l_pLoacalTableResult->SpinObjectResultList);
	if( l_bBetMax )
		return l_pLoacalTableResult;
	//3.
	m_iNumLinesBet = l_iOriginalNumLinesBet;	m_iPerLineBetMoney = l_iOriginalPerLineBetMoney;	m_iTotalBet = l_iOriginalTotalBet;
	sSlotBetTableResult*l_pRealTableResult = new sSlotBetTableResult();
	l_pRealTableResult->SpinObjectResultList = l_pLoacalTableResult->SpinObjectResultList;
	*e_pbWinJP = IsJPGameFilledCondition(l_pLoacalTableResult->SpinObjectResultList);
	AssignLinesData(l_pRealTableResult);
	//second calculate result
	CalculateScoreConsecutive(l_pRealTableResult);
	delete l_pLoacalTableResult;
	return l_pRealTableResult;
}

sSlotBetTableResult*cSlotBetBehavior::ProcessMaxBetResultForProbabilityThenCalRealBetResultTableFromOldTable(sSlotBetTableResult*e_pOldTable,int e_iMinScore,int e_iMaxScore,int e_iMultiplier,bool*e_pbWinJP,bool e_bFreeGame)
{
	if( e_iMinScore <= 0 && e_iMaxScore <= 0 )
	{
		return GenerateNoWinMoneyWithOldTable(e_pOldTable,e_bFreeGame);
	}
	//1.
	int	l_iScore = 0;
	int	l_iOriginalNumLinesBet = this->m_iNumLinesBet;	int	l_iOriginalPerLineBetMoney = this->m_iPerLineBetMoney;	int	l_iOriginalTotalBet = m_iTotalBet;
	this->BetMax();
	bool	l_bBetMax = false;;
	if( l_iOriginalNumLinesBet == this->m_iNumLinesBet && 
		l_iOriginalPerLineBetMoney == this->m_iPerLineBetMoney &&
		l_iOriginalTotalBet == m_iTotalBet)
	{
		l_bBetMax = true;
	}
	//2.
	sSlotBetTableResult*l_pLoacalTableResult = GenerateWinMoneyWithOldTable(e_iMinScore,e_iMaxScore,e_pOldTable,e_iMultiplier,e_bFreeGame);
	*e_pbWinJP = IsJPGameFilledCondition(l_pLoacalTableResult->SpinObjectResultList);
	if( l_bBetMax )
		return l_pLoacalTableResult;
	//3.
	m_iNumLinesBet = l_iOriginalNumLinesBet;	m_iPerLineBetMoney = l_iOriginalPerLineBetMoney;	m_iTotalBet = l_iOriginalTotalBet;
	sSlotBetTableResult*l_pRealTableResult = new sSlotBetTableResult();
	l_pRealTableResult->SpinObjectResultList = l_pLoacalTableResult->SpinObjectResultList;
	AssignLinesData(l_pRealTableResult);
	//second calculate result
	CalculateScoreConsecutive(l_pRealTableResult);
	delete l_pLoacalTableResult;
	return l_pRealTableResult;
}

int	GetComboRestScore(int e_iScore)
{
	if( e_iScore <= 0 )
		return 0;
	int	l_iWantedValue = e_iScore/3+1;
	return (rand()%(l_iWantedValue))+l_iWantedValue;
}

sSlotBetTableResult*	cSlotBetBehavior::GenerateCheatTableByCode()
{
	sSlotBetTableResult*l_pLoacalTableResult = new sSlotBetTableResult;
	l_pLoacalTableResult->SpinObjectResultList.resize(15);
	l_pLoacalTableResult->SpinObjectResultList[0] = 2;
	l_pLoacalTableResult->SpinObjectResultList[1] = 2;
	l_pLoacalTableResult->SpinObjectResultList[2] = 2;
	l_pLoacalTableResult->SpinObjectResultList[3] = 2;
	l_pLoacalTableResult->SpinObjectResultList[4] = 2;

	l_pLoacalTableResult->SpinObjectResultList[5] = 1;
	l_pLoacalTableResult->SpinObjectResultList[6] = 0;
	l_pLoacalTableResult->SpinObjectResultList[7] = 4;
	l_pLoacalTableResult->SpinObjectResultList[8] = 1;
	l_pLoacalTableResult->SpinObjectResultList[9] = 8;

	l_pLoacalTableResult->SpinObjectResultList[10] = 0;
	l_pLoacalTableResult->SpinObjectResultList[11] = 0;
	l_pLoacalTableResult->SpinObjectResultList[12] = 0;
	l_pLoacalTableResult->SpinObjectResultList[13] = 0;
	l_pLoacalTableResult->SpinObjectResultList[14] = 0;

	AssignLinesData(l_pLoacalTableResult);
	//second calculate result
	CalculateScoreConsecutive(l_pLoacalTableResult);
	return l_pLoacalTableResult;
}

void	cSlotBetBehavior::GenerateProbabilitySpinResult(cSlotBetResult*e_pBetBehaviorResult)
{
	e_pBetBehaviorResult->ClearLastTableResult();
	//so filled all spin object as random
	bool	l_bWinJP = false;
	int	l_iMultiplier = this->m_iBonusMoneyRate*this->m_iPerLineBetMoney;
	m_SlotGoodTables.GetGoodTable(e_pBetBehaviorResult);
	sGoodTable*l_pWinMoneyTable = m_SlotGoodTables.GetCurrentWinMoneyTable();
	if( l_pWinMoneyTable->GoodTableCategory == eGTT_JP_LOCAL )
	{
		if(!GenerateLocalJPTable(g_pSlotGamePhase->m_pBetBehaviorResult,l_iMultiplier))
		{

		}
		return;
	}
	if( l_pWinMoneyTable->GoodTableCategory == eGTT_JP_NETWORK )
	{
		if(!GenerateNetJPTable(g_pSlotGamePhase->m_pBetBehaviorResult,l_iMultiplier))
		{

		}
		return;
	}
	int	l_iMaxScore = l_pWinMoneyTable->iMaxMoney;
	int	l_iMinScore = l_pWinMoneyTable->iMinMoney;
	int	l_iRestMaxScore = l_iMaxScore;
	int	l_iRestMinScore = l_iMinScore;
	int	l_iMaxSpinObjectScore = g_pSlotGamePhase->m_pSlotRuleData->GetMaxScore(l_iMultiplier);
	if( l_iRestMinScore > l_iMaxSpinObjectScore  )
	{
		l_iRestMinScore = l_iMaxSpinObjectScore/((rand()%3)+1);
	}
	int	l_iCombo = l_pWinMoneyTable->iComboCount;
	//at least for one table.
	int	l_iCurrentIndex = 1;
	--l_iCombo;
	UT::sTimeAndFPS	l_FPS;
	sSlotBetTableResult*l_pTableResult = 0;
	//while( !l_pTableResult || (l_pTableResult->iWinMoney && l_iCurrentIndex < MAXIMUM_EXPLOSION_COUNT+1 && !g_pSlotGamePhase->m_bFreeToPlay) )
	while( !l_pTableResult || (l_pTableResult->iWinMoney && l_iCurrentIndex < MAXIMUM_EXPLOSION_COUNT+1 ) )
	{
		sSlotBetTableResult*l_pLoacalTableResult = 0;
		l_FPS.Update();
		int	l_iTargetScore = GetComboRestScore(l_iRestMaxScore);
		if( !l_pTableResult )
		{
			l_pLoacalTableResult = ProcessMaxBetResultForProbabilityThenCalRealBetResultTable(l_iRestMinScore,l_iTargetScore,l_iMultiplier,&l_bWinJP,l_pWinMoneyTable->GoodTableCategory==eGTT_FREE_GAME?true:false);
			if( l_bWinJP )
			{//avoid win JP
				delete l_pLoacalTableResult;
				l_pLoacalTableResult = ProcessMaxBetResultForProbabilityThenCalRealBetResultTable(0,0,1,&l_bWinJP,false);
			}
			//cheat:force to show result
			if(0)
			{
				delete l_pLoacalTableResult;
				l_pLoacalTableResult = GenerateCheatTableByCode();
			}
		}
		else
		{
			if( l_iCombo>0 )
				l_pLoacalTableResult = ProcessMaxBetResultForProbabilityThenCalRealBetResultTableFromOldTable(l_pTableResult,l_iRestMinScore,l_iTargetScore,l_iMultiplier,&l_bWinJP,false);
			else
				l_pLoacalTableResult = GenerateNoWinMoneyWithOldTable(l_pTableResult,false);
			++l_iCurrentIndex;
			if( l_bWinJP )
			{//avoid win JP
				delete l_pLoacalTableResult;
				l_pLoacalTableResult = GenerateNoWinMoneyWithOldTable(l_pTableResult,false);
			}
		}
		l_FPS.Update();
		//std::string	l_str = UT::ComposeMsgByFormat("1 table generate time%.3f",l_FPS.fElpaseTime);
		//cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(l_str.c_str());
		//l_pLoacalTableResult = 	GenerateOnelineWinMoneyNewTable(50,1000,this->m_iBonusMoneyRate*this->m_iPerLineBetMoney,&l_iScore);
		l_pTableResult = l_pLoacalTableResult;
		e_pBetBehaviorResult->m_TableResultList.push_back(l_pLoacalTableResult);
		//lazy doing
		{
			{
				//std::string	l_str = UT::ComposeMsgByFormat("Table1:Score%d,MaxScore:%d,MinScore:%d",l_pTableResult->iWinMoney,l_iRestMaxScore,l_iRestMinScore);
				//cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(l_str.c_str());
			}
			l_iRestMaxScore -= l_pTableResult->iWinMoney;
			l_iRestMinScore -= l_pTableResult->iWinMoney;
			if( l_iRestMinScore<0 )
				l_iRestMinScore = 0;
			--l_iCombo;
		}
	}
	int	l_iWinMoney = g_pSlotGamePhase->m_pBetBehaviorResult->GetCurrentTablesWinMoney();
	if( l_iWinMoney < l_pWinMoneyTable->iMinMoney || l_iWinMoney >l_pWinMoneyTable->iMaxMoney )
		cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine("Result not match!!!");
	std::string	l_str = UT::ComposeMsgByFormat("ROUND  %d,Temp:%d,Table1Result:Combo:%d,Score%d",GetRound(),m_SlotGoodTables.GetCurrentScore(),g_pSlotGamePhase->m_pBetBehaviorResult->m_TableResultList.size(),l_iWinMoney);
	cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(l_str.c_str());
	//for control panel record
	if( l_iWinMoney>1000 )
	{
		//EarningOver1000Score(l_iWinMoney);
	}
}
//
void	cSlotBetBehavior::GenerateRandomSpinResult(cSlotBetResult*e_pBetBehaviorResult)
{
	e_pBetBehaviorResult->ClearLastTableResult();
	//so filled all spin object as random
	int	l_iNumSpinObject = g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x*g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y;
	//at least for one table.
	sSlotBetTableResult*l_pTableResult = 0;
	{
		sSlotBetTableResult*l_pLoacalTableResult = 0;	
		//now assign spinobject as line to fill sSlotBetTableResult::sLineResult
		l_pLoacalTableResult = new sSlotBetTableResult;
		l_pLoacalTableResult->SpinObjectResultList = GenerateSpinResultVector(l_iNumSpinObject);
		AssignLinesData(l_pLoacalTableResult);
		//second calculate result
		CalculateScoreConsecutive(l_pLoacalTableResult);
		l_pTableResult = l_pLoacalTableResult;
		e_pBetBehaviorResult->m_TableResultList.push_back(l_pLoacalTableResult);
	}
	//win JP.
	//if( NetWorkJPCheck(&l_pTableResult->SpinObjectResultList) )
	//	return;
	int	l_iCurrentIndex = 1;
	//check if combo is not over maximum
	//while(	l_pTableResult->iWinMoney && l_iCurrentIndex < MAXIMUM_EXPLOSION_COUNT+1 && !g_pSlotGamePhase->m_bFreeToPlay )
	while(	l_pTableResult->iWinMoney && l_iCurrentIndex < MAXIMUM_EXPLOSION_COUNT+1 )
	{
		//third do regenerate new spin object result
		sSlotBetTableResult*l_pLoacalTableResult = 0;
		l_pLoacalTableResult = GenerateSpinExplosionResultWithRandom(l_pTableResult);
		//avoid win JP
		if(IsJPGameFilledCondition(l_pLoacalTableResult->SpinObjectResultList))
		{
			size_t	l_siSize = l_pLoacalTableResult->SpinObjectResultList.size();
			//make first colum is not wild to avoid JP
			for(int i=0;i<g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y;++i)
			{
				int	l_iOriginalIndex = l_pLoacalTableResult->SpinObjectResultList[g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x*i];
				l_pLoacalTableResult->SpinObjectResultList[g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x*i] = 0;
			}
		}
		AssignLinesData(l_pLoacalTableResult);
		CalculateScoreConsecutive(l_pLoacalTableResult);
		e_pBetBehaviorResult->m_TableResultList.push_back(l_pLoacalTableResult);
		l_pTableResult = l_pLoacalTableResult;
		++l_iCurrentIndex;
	}
}

sSlotBetTableResult*	cSlotBetBehavior::GenerateSpinExplosionResultWithEmptySpinObject(sSlotBetTableResult*e_pTableResult,std::vector<int>*e_pEmptyIndexList)
{
	//find out explosion object
	int	l_iNumSpinObject = g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x*g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y;
	sSlotBetTableResult*l_psSlotBetTableResult = new sSlotBetTableResult;
	std::vector<int>	l_NewSpinObjectResult;
	l_NewSpinObjectResult.resize(l_iNumSpinObject);
	//fin out last index where explosion,and count
	int				l_iTargetIndex = 0;
	//cPuzzleImage*	l_pPI = g_pSlotGamePhase->m_pSlotLineObjectManagerList->GetObject(0)->GetAllSpinObjectPI();
	//
	int	l_iTestValue = 0;
	int	l_iNewResultIndex = 0;
	for( int i=0;i<g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x;++i )
	{
		int	l_iNonExplosionIndex = g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y-1;
		int	l_iNextSpinObjectIndex = g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x;
		int	l_iImageIndex = -1;
		for( int j=g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y-1;j>-1;--j )
		{
			l_iTargetIndex = j*g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x+i;
			//start index from unexplosion 
			if(!e_pTableResult->pbWinMoneySpinObject[l_iTargetIndex])
			{
				int	l_iFinalSpinObjectIndex = l_iNonExplosionIndex*l_iNextSpinObjectIndex+i;
				l_NewSpinObjectResult[l_iFinalSpinObjectIndex] = e_pTableResult->SpinObjectResultList[l_iTargetIndex];
				--l_iNonExplosionIndex;
			}
		}
		int	l_iStartIndex = i;
		for( int j=g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y-1;j>-1;--j )
		{
			l_iTargetIndex = j*g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x+i;
			if(e_pTableResult->pbWinMoneySpinObject[l_iTargetIndex])
			{
				++l_iTestValue;
				int	l_iFinalSpinObjectIndex = l_iStartIndex+(l_iNonExplosionIndex*l_iNextSpinObjectIndex);
				//fuck here is wonrg.
				l_NewSpinObjectResult[l_iFinalSpinObjectIndex] = -1;
				if( e_pEmptyIndexList )
					e_pEmptyIndexList->push_back(l_iFinalSpinObjectIndex);
				--l_iNonExplosionIndex;
				++l_iNewResultIndex;
			}
		}
	}
	l_psSlotBetTableResult->SpinObjectResultList = l_NewSpinObjectResult;
	return l_psSlotBetTableResult;
}

sSlotBetTableResult*	cSlotBetBehavior::GenerateSpinExplosionResultWithRandom(sSlotBetTableResult*e_pTableResult)
{
	int	l_iNumSpinObject = g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x*g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y;
	int	l_iNumNewSpinObject = 0;
	for( int i=0;i<l_iNumSpinObject;++i )
	{
		if(e_pTableResult->pbWinMoneySpinObject[i])
			++l_iNumNewSpinObject;
	}
	sSlotBetTableResult*l_psSlotBetTableResult = GenerateSpinExplosionResultWithEmptySpinObject(e_pTableResult);
	std::vector<int>	l_NeddedNewSpinObjectResult = GenerateSpinResultVector(l_iNumNewSpinObject);
	for( int i=0;i<l_iNumSpinObject;++i )
	{
		if(l_psSlotBetTableResult->SpinObjectResultList[i] == -1 )
		{
			l_psSlotBetTableResult->SpinObjectResultList[i] = l_NeddedNewSpinObjectResult[--l_iNumNewSpinObject];
		}
	}
	assert(l_iNumNewSpinObject == 0);
	return l_psSlotBetTableResult;
}