#ifndef _SLOT_TABLE_GENERATOR_H_
#define _SLOT_TABLE_GENERATOR_H_

#include "SpinObjectPrizeData.h"

template<class T>T	GetRandValueFromVector(std::vector<T>*e_pSrc)
{
	if( e_pSrc->size() < 1 )
		return 0;
	return (*e_pSrc)[rand()%e_pSrc->size()];
}

//some times good table has wrong data...
bool	IsScoreLegal(int e_iMinScore,int e_iMaxScore,int e_iMultiplier);

//there 2 vector and one for avoid anorther for all data,now return a vector only contain the data as expected
std::vector<int>	StripNoneUseDataFromTwoVector(std::vector<int> e_AllData,std::vector<int> e_AvoidData);
std::vector<int>	StripNoneUseDataFromAllSpinObjectData(std::vector<int> e_AvoidData);
//get a vector by indicate count with only expect data.
std::vector<int>	GenerateRandomListWithoutAvoidData(std::vector<int> e_AllData,std::vector<int> e_AvoidData,int e_iCount);
//get a vector and the result not repeat by indicate count with only expect data.
std::vector<int>	GenerateNotSameRandomListWithoutAvoidData(std::vector<int> e_AllData,std::vector<int> e_AvoidData,int e_iCount);

//sSlotBetTableResult*l_pTestTable = GenerateNoWinMoneyTable(true);
//CalculateScoreConsecutive(l_pTestTable);
//e_pBetBehaviorResult->m_TableResultList.push_back(l_pTestTable);

//for 10 explosion
bool	GenerateLocalJPTable(cSlotBetResult*e_pBetBehaviorResult,int e_iMultiplier);
//
bool	GenerateNetJPTable(cSlotBetResult*e_pBetBehaviorResult,int e_iMultiplier);

//a talbe without win any money,no free game no wild.
sSlotBetTableResult*	GenerateNoWinMoneyTable(bool e_bFreeGame,int e_iAvoidSpinObjectIndex);
sSlotBetTableResult*	GenerateWinMoneyNewTable(int e_iMinScore,int e_iMaxScore,int e_iMultiplier,int* e_piScore);
sSlotBetTableResult*	GenerateWinMoneyWithOldTable(int e_iMinScore,int e_iMaxScore,sSlotBetTableResult*e_pOldTable,int e_iMultiplier,bool e_bFreeGame);
//it's possible still win the money but small.
sSlotBetTableResult*	GenerateNoWinMoneyWithOldTable(sSlotBetTableResult*e_pOldTable,bool e_bFreeGame);


//
//
//DO NOT USE BELOW FUNCTION DIRECTLY
//
//
//for one line win monet setup as below
//1st	GetMaxScoreSpinObject
//2nd	GenerateOnelineWinMoneyNewTable or GenerateOnelineWinMoneyNewTableWithMatchCount
//3rd	GenerateScoreBySpinObject
//

//if e_piMatchCount is not null,it will call GenerateOnelineWinMoneyNewTableWithMatchCount or GenerateOnelineWinMoneyNewTable
int									GetMaxScoreSpinObject(int e_iMinScore,int e_iMaxScore,int e_iMultiplier,int* e_piScore,int *e_piMatchCount);
int									GetMinScoreSpinObject(int e_iMinScore,int e_iMaxScore,int e_iMultiplier,int* e_piScore,int *e_piMatchCount);
//some times out of control call this to force win score at accept range
sSlotBetTableResult*	GenerateOnelineWinMoneyNewTable(int e_iMinScore,int e_iMaxScore,int e_iMultiplier,int* e_piScore);
//if e_iMatchCount is bigger than 3 it's quite generat more than one line
sSlotBetTableResult*	GenerateOnelineWinMoneyNewTableWithMatchCount(int e_iMinScore,int e_iMaxScore,int e_iMultiplier,int e_iMatchCount,int* e_piScore);
//for multi line score
sSlotBetTableResult*	GenerateMultilineWinMoneyNewTableWithMatchCount(int e_iMinScore,int e_iMaxScore,int e_iMultiplier,int* e_piScore,bool e_bFreeGame);
//it might be one line but is possible more than one line depend on game design.
//
sSlotBetTableResult*	GenerateScoreBySpinObject(int e_iSpinObjectID,int e_iMinScore,int e_iMaxScore,int e_iMultiplier,int* e_piScore);


//1st find out score table
//2nd min and max score and score type
//3rd if no win money return a no win money table,if win money how many combo is it or
//
//

//if u want to match a line is totall new or a empty spinobject in it
//call	FilledLineScore and this function will call GetRandomMatchedSpinObjectByScoreRange then IsSpinObjectScoreMatched
//so u knoe which spinobject is match and get a match line with score if it's possible to filled line.
//
//if u want find a random spinobject match score condition call
//GetRandomMatchedSpinObjectByScoreRange					or
//GetRandomMatchedSpinObjectByScoreRangeWithMatchCount
//
//if u want to know is spinobject filled condition call
//IsSpinObjectScoreMatched							or
//IsSpinObjectScoreMatchedWithCount
//
//
//

//find out max score consecutive count
bool	IsSpinObjectScoreMatched(int e_iSpinObjectIndex,int e_iMinScore,int e_iMaxScore,int e_iMultiplier,int*e_piMatchedCount,int* e_piSocre);
//quite similar above but indicate a count to find out match one.
bool	IsSpinObjectScoreMatchedWithCount(int e_iSpinObjectIndex,int e_iMinScore,int e_iMaxScore,int e_iForceMatchedCount,int e_iMultiplier,int* e_piSocre);
//find suitable spin object and got score
int		GetRandomMatchedSpinObjectByScoreRange(int e_iMinScore,int e_iMaxScroe,int e_iMultiplier,int *e_pScore,int*e_piMatchCount);
//e_piAvoidSpinObjectIndex if satisfied spinobject is e_piAvoidSpinObjectIndex avoid it
int		GetRandomMatchedSpinObjectByScoreRangeWithMatchCount(int e_iMinScore,int e_iMaxScroe,int e_iMultiplier,int e_iMatchCount,int *e_pScore,int*e_piAvoidSpinObjectIndex = 0);
//if line has empty object and cannt win any money return false
//no fre game setup here if u want to force into free game please setup result before give score
bool	FilledLineScore(sSlotBetTableResult*e_pTableResult,int e_iLineIndex,int e_iMinScore,int e_iMaxScore,int e_iMultiplier,int *e_piScore,int*e_piSpinObjectID,std::vector<int>*e_pFilledTableIndices);
//find out empty could be matched and return match count,if e_pSpinObjectID is -1,please assign a data to it.
//e_iFirstMatchedSpinObjectIndex mean e_pMatchSpinObjectID's index of line
//also avoid score is too big from first column adjency spinobject
bool	MatchCountOfEmptyLine(int e_iMinScore,int e_iMaxScore,int e_iMultiplier,sSlotBetTableResult*e_pTableResult,int e_iLineIndex,int*e_pMatchSpinObjectID,int*e_piMatchCount,int*e_iFirstMatchedSpinObjectIndex);
bool	IsSpinObjectPossibleWinTooMuch(int e_iMinScore,int e_iMaxScore,int e_iMultiplier,sSlotBetTableResult*e_pTableResult,int e_iTableIndex,int e_iMatchSpinObjectID,int e_iMatchCount);
//return a no match spinobject id
int		GetSpinObjectIDNotMatchNeighbor(int e_iIndex,sSlotBetTableResult*e_pTableResult,int e_iMinScore,int e_iMaxScore,int e_iMultiplier);
//
void	ForceActiveFreeGame(sSlotBetTableResult*e_pTableResult);
void	AddFreeGameFromOldTable(sSlotBetTableResult*e_pTableResult,int e_iCount);
//to add a wild let table result to be good while win money is not enough
bool	IsIndexOkForWild(int e_iIndex,sSlotBetTableResult*e_pTableResult,int e_iMinScore,int e_iMaxScore,int e_iMultiplier);
#endif