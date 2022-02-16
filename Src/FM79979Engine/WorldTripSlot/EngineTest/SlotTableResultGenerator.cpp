#include "stdafx.h"
#include "SlotTableResultGenerator.h"
#include "SlotGamePhase.h"
#include "SlotRule.h"
#include "SlotBetBehavior.h"
#define	MAX_RECALCULATE_COUNT	5

#define	MIN_CONSECUTIVE_COUNT	3

#define	LINE_COMMUSE_POSSIBLE_COUNT	2

int	l_iErrorCount = 0;

void	DumpMessage(char*e_strFunctionName,int e_iMinScore,int e_iMaxScore,int e_iMultiplier,int e_iScore)
{
		cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(e_strFunctionName);
		printf(e_strFunctionName);
		std::string	l_str = ComposeMsgByFormat("MinScore:%d,MaxScore:%d,Multiplier%d,Score:%d",e_iMinScore,e_iMaxScore,e_iMultiplier,e_iScore);
		cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(l_str.c_str());
		printf(l_str.c_str());
		l_str = ComposeMsgByFormat("ErrorCount%d",++l_iErrorCount);
		cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(l_str.c_str());
}
void	DumpTable(sSlotBetTableResult*e_pTable,char*e_strDescription,bool e_bDumpLineInfo)
{
	return;
	string	l_str = e_strDescription;
	l_str += "\n";
	for( int i=0;i<(int)g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y;++i )
	{
		for( int j=0;j<(int)g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x;++j )
		{
			int	l_iIndex = i*g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x+j;
			l_str += ValueToString(l_iIndex);
			l_str += ":";
			l_str += ValueToString(e_pTable->SpinObjectResultList[l_iIndex]);
			l_str += "\t\t";
		}
		l_str += "\n";
	}
	if( e_bDumpLineInfo&&e_pTable->WinMoneyLineList.size() )
	{
		l_str += "WinMoneyIndex\n";
		for( int i=0;i<(int)e_pTable->WinMoneyLineList.size();++i )
		{
			l_str += "LineIndex:";
			l_str += ValueToString(e_pTable->WinMoneyLineIndexList[i]);
			l_str += "\n";
			l_str += "TableIndex:";
			sSlotBetTableResult::sLineResult l_Line = e_pTable->WinMoneyLineList[i];
			for( int j=0;j<(int)l_Line.WinMoneySpinObjectIndexList.size();++j )
			{
				l_str += ValueToString(l_Line.WinMoneySpinObjectIndexList[j]);
				l_str += ",";
			}
			l_str += "\n";
		}
	}
	
	cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(l_str.c_str());
}

bool	FreeGameCheck(bool e_bFreeGame,sSlotBetTableResult*e_pResult)
{
#ifdef _DEBUG
	int	l_iFreeGameIndex = g_pSlotGamePhase->m_pSlotRuleData->GetFreeSpinObjectIndex();
	int	l_iNumFreeGameIndex = e_pResult->NumSpinObjectInTable(l_iFreeGameIndex);
	if( l_iNumFreeGameIndex >= g_pSlotGamePhase->m_pSlotRuleData->m_iNumFreeGameToAchevied && !e_bFreeGame )
	{
		DumpMessage("Too much free SpinObject GenerateNoWinMoneyWithOldTable_1",0,0,1,0);
		return false;
	}
#endif
	return true;
}
//some times good table has wrong data...
int	GetMinScore(int e_iMultiplier)
{
	return g_pSlotGamePhase->m_pSlotRuleData->GetMinScore(e_iMultiplier);
}

int	GetMaxScore(int e_iMultiplier)
{
	return g_pSlotGamePhase->m_pSlotRuleData->GetMaxScore(e_iMultiplier);
}
bool	IsScoreLegal(int e_iMinScore,int e_iMaxScore,int e_iMultiplier)
{
	//should do something here but I am lazy to find out min and max score by program...fuck
	int	l_iMinScore = GetMinScore(e_iMultiplier);	//MIN_CONSECUTIVE_COUNT consecutive
	int	l_iMaxScore = GetMaxScore(e_iMultiplier);	//MAX_RECALCULATE_COUNT consecutive
	if(  e_iMaxScore < l_iMinScore || e_iMinScore > l_iMaxScore  )
		return false;
	return true;
}

std::vector<int>	StripNoneUseDataFromAllSpinObjectData(std::vector<int> e_AvoidData)
{
	int	l_iAllSpinObjectAmount = g_pSlotGamePhase->m_pSlotRuleData->m_SpinObjectDataManager.Count();
	std::vector<int>	l_SpinObjectData;
	for(int i=0;i<l_iAllSpinObjectAmount;++i)
	{
		bool	l_bAvoid = false;
		for( int j=0;j<(int)e_AvoidData.size();++j )
		{
			if( e_AvoidData[j] == i )
			{
				e_AvoidData.erase(e_AvoidData.begin()+j);
				l_bAvoid = true;
				break;
			}
		}
		if( !l_bAvoid )
			l_SpinObjectData.push_back(i);
	}
	return l_SpinObjectData;
}
std::vector<int>	StripNoneUseDataFromTwoVector(std::vector<int> e_AllData,std::vector<int> e_AvoidData)
{
	std::vector<int>l_DataList;
	while(e_AvoidData.size())
	{
		size_t	l_AllDataSize = e_AllData.size();
		int	l_iData = e_AvoidData[0];
		for(size_t i=0;i<l_AllDataSize;++i)
		{
			if(e_AllData[i] == l_iData)
			{
				e_AllData.erase(e_AllData.begin()+i);
				break;
			}
		}
		e_AvoidData.erase(e_AvoidData.begin());
	}
	return e_AllData;
}


std::vector<int>	GenerateRandomListWithoutAvoidData(std::vector<int> e_AllData,std::vector<int> e_AvoidData,int e_iCount)
{
	std::vector<int>l_StripDataList = StripNoneUseDataFromTwoVector(e_AllData,e_AvoidData);
	std::vector<int>l_DataList;
	for(int i=0;i<e_iCount;++i)
	{
		l_DataList.push_back(l_StripDataList[rand()%l_StripDataList.size()]);
	}
	return l_DataList;
}

std::vector<int>	GenerateNotSameRandomListWithoutAvoidData(std::vector<int> e_AllData,std::vector<int> e_AvoidData,int e_iCount)
{
	std::vector<int>l_StripDataList = StripNoneUseDataFromTwoVector(e_AllData,e_AvoidData);
	std::vector<int>l_DataList;
	if( (size_t)e_iCount > l_StripDataList.size() )
	{
		for( int i=0;i<e_iCount;++i )
		{
			l_DataList.push_back(0);
		}
		assert(0&&"avoid count and target data count is not match");
		return l_DataList;
	}
	while(l_DataList.size() != e_iCount )
	{
		int	l_iValue = GetRandValueFromVector<int>(&l_StripDataList);
		if(!UT::IsVectorContain<int>(l_iValue,&l_DataList))
		{
			l_DataList.push_back(l_iValue);
		}
	}
	return l_DataList;
}


bool	GenerateLocalJPTable(cSlotBetResult*e_pBetBehaviorResult,int e_iMultiplier)
{
	assert(e_pBetBehaviorResult->m_TableResultList.size() == 0);
	int	l_iMinScore = g_pSlotGamePhase->m_pSlotRuleData->GetMinScore(e_iMultiplier)*g_pSlotGamePhase->m_pSlotRuleData->m_LineRuleList.size();
	int	l_iScore = 0;
	sSlotBetTableResult*l_pTableResult = GenerateMultilineWinMoneyNewTableWithMatchCount(0,l_iMinScore*(rand()%e_iMultiplier),e_iMultiplier,&l_iScore,false);
	e_pBetBehaviorResult->m_TableResultList.push_back(l_pTableResult);
	if( l_pTableResult->iWinMoney == 0 )
		return false;
	for(int i=0;i<MAXIMUM_EXPLOSION_COUNT;++i )
	{
		sSlotBetTableResult*l_pLoacalTableResult = GenerateWinMoneyWithOldTable(0,l_iMinScore,l_pTableResult,e_iMultiplier,false);
		l_pTableResult = l_pLoacalTableResult;
		if( g_pSlotGamePhase->m_pBetBehavior->IsJPGameFilledCondition(l_pTableResult->SpinObjectResultList) )
		{//avoid network JP
			delete l_pLoacalTableResult;
			l_pLoacalTableResult = GenerateNoWinMoneyWithOldTable(l_pTableResult,false);
		}
		e_pBetBehaviorResult->m_TableResultList.push_back(l_pTableResult);
		if( l_pTableResult->iWinMoney == 0 )
			return false;
		
	}
	g_pSlotGamePhase->m_pBetBehavior->AssignLinesData(l_pTableResult);
	g_pSlotGamePhase->m_pBetBehavior->CalculateScoreConsecutive(l_pTableResult);	
	return true;
}

bool	GenerateNetJPTable(cSlotBetResult*e_pBetBehaviorResult,int e_iMultiplier)
{
	assert(e_pBetBehaviorResult->m_TableResultList.size() == 0);
	sSlotBetTableResult*l_pTableResult = new sSlotBetTableResult();
	int	l_iTableAmount = g_pSlotGamePhase->m_pSlotRuleData->GetTableAmount();
	l_pTableResult->SpinObjectResultList.resize(l_iTableAmount);
	int	l_iWildIndex = g_pSlotGamePhase->m_pSlotRuleData->GetWildObjectIndex();
	int	l_iTargetLineIndex = rand()%g_pSlotGamePhase->m_pSlotRuleData->m_LineRuleList.size();
	sLineRule*l_pLine = g_pSlotGamePhase->m_pSlotRuleData->m_LineRuleList[l_iTargetLineIndex];
	for( int i=0;i<l_pLine->cOneLineCount;++i )
	{
		int	l_iIndex = l_pLine->pcLineObjectIndex[i];
		l_pTableResult->SpinObjectResultList[l_iIndex] = l_iWildIndex;
	}
	for( int i=0;i<l_iTableAmount;++i )
	{
		if( l_iWildIndex != l_pTableResult->SpinObjectResultList[i] )
			l_pTableResult->SpinObjectResultList[i] = g_pSlotGamePhase->m_pSlotRuleData->GetRandomSpinObject(true,true);
	}
	g_pSlotGamePhase->m_pBetBehavior->AssignLinesData(l_pTableResult);
	g_pSlotGamePhase->m_pBetBehavior->CalculateScoreConsecutive(l_pTableResult);
	e_pBetBehaviorResult->m_TableResultList.push_back(l_pTableResult);
	return true;
}

sSlotBetTableResult*	GenerateNoWinMoneyTable(bool e_bFreeGame,int e_iAvoidSpinObjectIndex)
{
	//filled 6 object to front of row line's spionobject
	#define	MINIMAX_NON_COSECUTIVE_COUNT	2
	int	l_iNeedCount = g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y*MINIMAX_NON_COSECUTIVE_COUNT;
	int	l_iTableAmount = g_pSlotGamePhase->m_pSlotRuleData->GetTableAmount();
	int	l_iAllSpinObjectAmount = g_pSlotGamePhase->m_pSlotRuleData->m_SpinObjectDataManager.Count();
	assert(l_iAllSpinObjectAmount-2>l_iNeedCount&&"spin object amount is not enough");
	int	l_iWildIndex = g_pSlotGamePhase->m_pSlotRuleData->GetWildObjectIndex();
	int	l_iFreeGameIndex = g_pSlotGamePhase->m_pSlotRuleData->GetFreeSpinObjectIndex();
	int	l_iSkipObjectIndex[] = {l_iWildIndex,l_iFreeGameIndex,e_iAvoidSpinObjectIndex};
	std::vector<int>	l_AvoidSpinObjectData;
	for(int i=0;i<sizeof(l_iSkipObjectIndex)/sizeof(int);++i)
	{
		l_AvoidSpinObjectData.push_back(l_iSkipObjectIndex[i]);
	}
	std::vector<int>	l_AllSpinObjectData;
	for(int i=0;i<l_iAllSpinObjectAmount;++i)
		l_AllSpinObjectData.push_back(i);
	//now we got none concecutive result.
	std::vector<int>l_NoSameData = GenerateNotSameRandomListWithoutAvoidData(l_AllSpinObjectData,l_AvoidSpinObjectData,l_iNeedCount);
	//filled rest data
	std::vector<int>l_RandomData = GenerateRandomListWithoutAvoidData(l_AllSpinObjectData,l_AvoidSpinObjectData,l_iTableAmount-l_iNeedCount);
	assert( l_iTableAmount == (int)(l_NoSameData.size()+l_RandomData.size()) );
	sSlotBetTableResult*l_pTableResult = new sSlotBetTableResult();
	l_pTableResult->SpinObjectResultList.resize(l_iTableAmount);
	//combine them and we got a none win money table
	for( int i=0;i<g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x;++i )
	{
		for( int j=0;j<g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y;++j )
		{
			int	l_iIndex = j*g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x+i;
			if( i<MINIMAX_NON_COSECUTIVE_COUNT )
			{
				l_pTableResult->SpinObjectResultList[l_iIndex] = l_NoSameData[0];
				l_NoSameData.erase(l_NoSameData.begin());
			}
			else
			{
				l_pTableResult->SpinObjectResultList[l_iIndex] = l_RandomData[0];
				l_RandomData.erase(l_RandomData.begin());				
			}
		}
	}
	if( e_bFreeGame )
	{//give a random free game
		ForceActiveFreeGame(l_pTableResult);
	}
	g_pSlotGamePhase->m_pBetBehavior->AssignLinesData(l_pTableResult);
	return l_pTableResult;
}

sSlotBetTableResult*	GenerateWinMoneyNewTable(int e_iMinScore,int e_iMaxScore,int e_iMultiplier,int* e_piScore)
{
//
//1st.	get a random line matched
//2nd.	get each match line's min and max score
//3rd.	find out all match spin object id and count,if there is no any object is filled condition,use min and max money to find suit one,or out of control
//4th.	if match object is bigger than row,rand a match object list of row.
//5th.	generate table and calculate score if score is small than min score rand 4 wild spin object to rise score
//6th.	repeat until score matched,if repeat count is bigger than MAX_RECALCULATE_COUNT,give a one line match with max score
	int	l_iNumLineExplosion = rand()%(g_pSlotGamePhase->m_pSlotRuleData->m_LineRuleList.size()/2)+1;
	std::vector<int>l_RandomLine = UT::GenerateRandomTable(l_iNumLineExplosion,g_pSlotGamePhase->m_pSlotRuleData->m_LineRuleList.size());
	std::vector<int>l_SpinObjectResultID;
	std::vector<int>l_MatchCountVector;
	int	l_iOneLineMinWinMoney = e_iMinScore/l_iNumLineExplosion;
	int	l_iOneLineMaxWinMoney = e_iMaxScore;
	//get matched row object and get score
	*e_piScore = 0;
	for( int i=0;i<g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y;++i )
	{
		int	l_iMatchedCount = 0;
		int	l_iSpinObjectIndex = GetRandomMatchedSpinObjectByScoreRange(l_iOneLineMinWinMoney,l_iOneLineMaxWinMoney,e_iMultiplier,e_piScore,&l_iMatchedCount);
		if( l_iSpinObjectIndex != -1 )
		{
			l_SpinObjectResultID.push_back(l_iSpinObjectIndex);
			l_MatchCountVector.push_back(l_iMatchedCount);
		}
		else
		//fuck....cannt find match spin object,give this a one line result
		{
			int	l_iSpinObjectIndex = GetRandomMatchedSpinObjectByScoreRange(e_iMinScore,e_iMaxScore,e_iMultiplier,e_piScore,&l_iMatchedCount);
			if( l_iSpinObjectIndex != -1 )
			{
				l_SpinObjectResultID.push_back(l_iSpinObjectIndex);
				l_MatchCountVector.push_back(l_iMatchedCount);
			}
			else//still can not find match score......out of control
			{
				//printf("out of control!!min score:%d,max score%d\n",e_iMinScore,e_iMaxScore);
				return GenerateOnelineWinMoneyNewTable(e_iMinScore,e_iMaxScore,e_iMultiplier,e_piScore);
			}
		}
	}
	sSlotBetTableResult*l_pTableResult = new sSlotBetTableResult();
	int	l_iReCalculateCount = 0;
	*e_piScore = 0;
	while( *e_piScore<e_iMinScore  && l_iReCalculateCount<MAX_RECALCULATE_COUNT )
	{
		++l_iReCalculateCount;
		std::vector<int>l_NewSpinObjectResultID = l_SpinObjectResultID;
		std::vector<int>l_NewMatchCountVector = l_MatchCountVector;
		if( (int)l_NewSpinObjectResultID.size() > g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y )
		{
			int	l_iNeeded = rand()%g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y;
			while(  (int)l_NewSpinObjectResultID.size() > l_iNeeded )
			{
				int	l_iEraseIndex = rand()%l_NewSpinObjectResultID.size();
				l_SpinObjectResultID.erase(l_NewSpinObjectResultID.begin()+l_iEraseIndex);
				l_NewMatchCountVector.erase(l_NewMatchCountVector.begin()+l_iEraseIndex);
			}
		}
		//now we have those spin object result ID
		for( int i=0;i<(int)l_SpinObjectResultID.size();++i )
		{
		
		}
		
		//assign all line data
		for( int i=0;i<(int)l_RandomLine.size();++i )
		{
			//l_pTableResult
		}
	}
	g_pSlotGamePhase->m_pBetBehavior->AssignLinesData(l_pTableResult);
	return l_pTableResult;
}

int	GetMaxScoreSpinObject(int e_iMinScore,int e_iMaxScore,int e_iMultiplier,int* e_piScore,int *e_piMatchCount)
{
	*e_piScore = 0;
	int	l_iMaxScoreSpinObjectIndex = -1;
	int	l_iMaxScoreMatchCount = -1;
	int	l_iSpinObjectAmount = g_pSlotGamePhase->m_pSlotRuleData->m_SpinObjectDataManager.Count();
	for( int i=0;i<l_iSpinObjectAmount;++i )
	{
		int	l_iMatchedCount = 0;
		int	l_iScore = 0;
		int	l_iSpinObjectIndex = -1;
		if(e_piMatchCount)
		{
			if(IsSpinObjectScoreMatchedWithCount(i,e_iMinScore,e_iMaxScore,*e_piMatchCount,e_iMultiplier,&l_iScore))
				l_iSpinObjectIndex = i;
		}
		else
		{
			if(IsSpinObjectScoreMatched(i,e_iMinScore,e_iMaxScore,e_iMultiplier,&l_iMatchedCount,&l_iScore))
				l_iSpinObjectIndex = i;
		}
		if( l_iSpinObjectIndex != -1 )
		{
			if( *e_piScore<l_iScore )
			{
				*e_piScore = l_iScore;
				l_iMaxScoreSpinObjectIndex = l_iSpinObjectIndex;
				l_iMaxScoreMatchCount = l_iMatchedCount;
			}
		}
	}
	return l_iMaxScoreSpinObjectIndex;
}

int	GetMinScoreSpinObject(int e_iMinScore,int e_iMaxScore,int e_iMultiplier,int* e_piScore,int *e_piMatchCount)
{
	*e_piScore = e_iMaxScore;
	int	l_iMinScoreSpinObjectIndex = -1;
	int	l_iMaxScoreMatchCount = -1;
	int	l_iSpinObjectAmount = g_pSlotGamePhase->m_pSlotRuleData->m_SpinObjectDataManager.Count();
	bool	l_bFind = false;
	for( int i=0;i<l_iSpinObjectAmount;++i )
	{
		int	l_iMatchedCount = 0;
		int	l_iScore = 0;
		int	l_iSpinObjectIndex = -1;
		if(e_piMatchCount)
		{
			if(IsSpinObjectScoreMatchedWithCount(i,e_iMinScore,e_iMaxScore,*e_piMatchCount,e_iMultiplier,&l_iScore))
				l_iSpinObjectIndex = i;
		}
		else
		{
			if(IsSpinObjectScoreMatched(i,e_iMinScore,e_iMaxScore,e_iMultiplier,&l_iMatchedCount,&l_iScore))
				l_iSpinObjectIndex = i;
		}
		if( l_iSpinObjectIndex != -1 )
		{
			l_bFind = true;
			if( *e_piScore>=l_iScore )
			{
				*e_piScore = l_iScore;
				l_iMinScoreSpinObjectIndex = l_iSpinObjectIndex;
				l_iMaxScoreMatchCount = l_iMatchedCount;
			}
		}
	}
	if(  !l_bFind )
	{
		*e_piScore = 0;
		DumpMessage("GetMinScoreSpinObject",e_iMinScore,e_iMaxScore,e_iMultiplier,*e_piScore);
	}
	return l_iMinScoreSpinObjectIndex;
}
//							  
sSlotBetTableResult*	GenerateOnelineWinMoneyNewTableWithMatchCount(int e_iMinScore,int e_iMaxScore,int e_iMultiplier,int e_iMatchCount,int* e_piScore)
{
	*e_piScore = 0;
	int	l_iMaxScoreSpinObjectIndex = GetMaxScoreSpinObject(e_iMinScore,e_iMaxScore,e_iMultiplier,e_piScore,&e_iMatchCount);
	int	l_iMaxScoreMatchCount = -1;
	return GenerateScoreBySpinObject(l_iMaxScoreSpinObjectIndex,e_iMinScore,e_iMaxScore,e_iMultiplier,e_piScore);
}
//now.
sSlotBetTableResult*	GenerateMultilineWinMoneyNewTableWithMatchCount(int e_iMinScore,int e_iMaxScore,int e_iMultiplier,int* e_piScore,bool e_bFreeGame)
{
	*e_piScore = 0;
	int	l_iFreeIndex = g_pSlotGamePhase->m_pSlotRuleData->GetFreeSpinObjectIndex();
	int	l_iWildIndex = g_pSlotGamePhase->m_pSlotRuleData->GetWildObjectIndex();
	int	l_iNumMatchLineObject = (rand()%g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y)+1;
	int	l_iRestTime = l_iNumMatchLineObject;
	sSlotBetTableResult*l_pTableResult = GenerateNoWinMoneyTable(false,l_iFreeIndex);
	//index should be random
	int	l_iRow = g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y;
	std::vector<int>l_RowRandIndex = UT::GenerateRandomTable(l_iRow,l_iRow);
	int	l_iLastSpinObjectIndex = -1;
	while( l_iRestTime )
	{
		sSlotBetTableResult l_CalScoreTableResult = *l_pTableResult;
		l_CalScoreTableResult.pbWinMoneySpinObject = 0;
		int	l_iStartIndex = l_RowRandIndex[0]*g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x;
		int	l_iScore = 0;
		int	l_iMatchCount = 0;
		//int	l_iSpinObjectID = rand()%g_pSlotGamePhase->m_pSlotRuleData->m_SpinObjectDataManager.Count();
		int	l_iSpinObjectID = GetRandomMatchedSpinObjectByScoreRange(e_iMinScore,e_iMaxScore,e_iMultiplier,&l_iScore,&l_iMatchCount);
		if( l_iSpinObjectID == l_iWildIndex )
		{//avoid NetWork JP
			l_iMatchCount -= 1;
		}
		//avoid all same spinobject table
		if( l_iMatchCount == g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x && l_iSpinObjectID == l_iLastSpinObjectIndex )
		{
			l_iMatchCount -= 1;
		}
		for( int i=0;i<l_iMatchCount;++i )
		{//if free at same position is okay.
			int	l_iIndex = l_iStartIndex+i;
			l_CalScoreTableResult.SpinObjectResultList[l_iIndex] = l_iSpinObjectID;
		}
		l_iLastSpinObjectIndex = l_iSpinObjectID;
		g_pSlotGamePhase->m_pBetBehavior->AssignLinesData(&l_CalScoreTableResult);
		g_pSlotGamePhase->m_pBetBehavior->CalculateScoreConsecutive(&l_CalScoreTableResult);
		//it's first time to here and win money is too big,ignore result and re generate again.
		if( l_CalScoreTableResult.iWinMoney > e_iMaxScore )
		{
			//first time to here ignore this result
			if( l_iNumMatchLineObject == l_iRestTime  )
			{
				--l_iRestTime;
				continue;
			}
			else
			{//okay this is not first time to here and win money is enough leave here
				break;
			}
		}
		l_RowRandIndex.erase(l_RowRandIndex.begin());
		l_pTableResult->SpinObjectResultList = l_CalScoreTableResult.SpinObjectResultList;
		//l_MatchedSpinObjectID
		--l_iRestTime;
	}
	//free game randomly give 3 dimonds ignorescore
	if( e_bFreeGame )
	{
		ForceActiveFreeGame(l_pTableResult);
	}
	g_pSlotGamePhase->m_pBetBehavior->AssignLinesData(l_pTableResult);
	g_pSlotGamePhase->m_pBetBehavior->CalculateScoreConsecutive(l_pTableResult);	
	//still cannot find result give it a one line reult
	if( l_pTableResult->iWinMoney < e_iMinScore || l_pTableResult->iWinMoney > e_iMaxScore )
	{
		delete l_pTableResult;
		return GenerateOnelineWinMoneyNewTable(e_iMinScore,e_iMaxScore,e_iMultiplier,e_piScore);
	}
	return l_pTableResult;
}

//some times out of control call this to force win score at accept range
sSlotBetTableResult*	GenerateOnelineWinMoneyNewTable(int e_iMinScore,int e_iMaxScore,int e_iMultiplier,int* e_piScore)
{
	*e_piScore = 0;
	sSlotBetTableResult*l_pResult = 0;
	int	l_iScoreSpinObjectIndex = 0;
	if( IsScoreLegal(e_iMinScore,e_iMaxScore,e_iMultiplier) )
	{
		//give a max or a random
		if(rand()%e_iMaxScore>e_iMaxScore/2)
			l_iScoreSpinObjectIndex = GetMaxScoreSpinObject(e_iMinScore,e_iMaxScore,e_iMultiplier,e_piScore,0);
		else
		{
			int	l_iMatchCount = 0;
			l_iScoreSpinObjectIndex = GetRandomMatchedSpinObjectByScoreRange(e_iMinScore,e_iMaxScore,e_iMultiplier,e_piScore,&l_iMatchCount);
		}
		int	l_iMaxScoreMatchCount = -1;
		l_pResult = GenerateScoreBySpinObject(l_iScoreSpinObjectIndex,e_iMinScore,e_iMaxScore,e_iMultiplier,e_piScore);
		//if max and random both not matched give a minimam
		if( *e_piScore<e_iMinScore || *e_piScore>e_iMaxScore )
		{
			SAFE_DELETE(l_pResult);
			l_iScoreSpinObjectIndex = GetMinScoreSpinObject(e_iMinScore,e_iMaxScore,e_iMultiplier,e_piScore,0);
			l_pResult = GenerateScoreBySpinObject(l_iScoreSpinObjectIndex,e_iMinScore,e_iMaxScore,e_iMultiplier,e_piScore);
		}
	}
	if( !l_pResult || l_pResult->iWinMoney<e_iMinScore||l_pResult->iWinMoney>e_iMaxScore )
	{
		SAFE_DELETE(l_pResult);
		l_pResult = GenerateNoWinMoneyTable(false,79979);
		DumpMessage("GenerateOnelineWinMoneyNewTable",e_iMinScore,e_iMaxScore,e_iMultiplier,*e_piScore);
	}
	return l_pResult;
}

sSlotBetTableResult*	GenerateScoreBySpinObject(int e_iSpinObjectID,int e_iMinScore,int e_iMaxScore,int e_iMultiplier,int* e_piScore)
{
	if( e_iSpinObjectID == -1 )
		return  0;
	sSlotBetTableResult*l_pTableResult = 0;
	int	l_iReCalculateCount = 0;
	int	l_iWinMoney = 0;
	int	l_iMaxCombo = g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x;
	int	l_iTargetLineIndex = 0;
	while( (l_iWinMoney<e_iMinScore || l_iWinMoney >e_iMaxScore) && l_iReCalculateCount<MAX_RECALCULATE_COUNT )
	{
		SAFE_DELETE(l_pTableResult);
		l_pTableResult = GenerateNoWinMoneyTable(false,e_iSpinObjectID);
		int	l_iFreeGameIndex = g_pSlotGamePhase->m_pSlotRuleData->GetFreeSpinObjectIndex();
		l_iTargetLineIndex = rand()%g_pSlotGamePhase->m_pSlotRuleData->m_LineRuleList.size();
		sLineRule*l_pLineRule = g_pSlotGamePhase->m_pSlotRuleData->m_LineRuleList[l_iTargetLineIndex];
		for(int i=0;i<l_pLineRule->cOneLineCount;++i)
		{
			if( l_iMaxCombo <= i )
			{
				l_pTableResult->SpinObjectResultList[l_pLineRule->pcLineObjectIndex[i]] = l_iFreeGameIndex;
			}
			else
				l_pTableResult->SpinObjectResultList[l_pLineRule->pcLineObjectIndex[i]] = e_iSpinObjectID;
		}
		g_pSlotGamePhase->m_pBetBehavior->CalculateScoreConsecutive(l_pTableResult);
		*e_piScore = l_pTableResult->iWinMoney;
		l_iWinMoney = l_pTableResult->iWinMoney;
		if( l_iWinMoney > e_iMaxScore )
		{
			l_iMaxCombo -= 1;
		}
		++l_iReCalculateCount;
	}
	return l_pTableResult;
}

sSlotBetTableResult*	GenerateWinMoneyWithOldTable(int e_iMinScore,int e_iMaxScore,sSlotBetTableResult*e_pOldTable,int e_iMultiplier,bool e_bFreeGame)
{
	std::vector<int>l_EmptyIndexList;
	std::vector<int>l_FirstColumnSpinObjectIndex;
	int	l_iColumn = g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x;
	sSlotBetTableResult*l_pTableResult = g_pSlotGamePhase->m_pBetBehavior->GenerateSpinExplosionResultWithEmptySpinObject(e_pOldTable,&l_EmptyIndexList);
#ifdef _DEBUG
	sSlotBetTableResult l_DebugTableResult;
	l_DebugTableResult.SpinObjectResultList = l_pTableResult->SpinObjectResultList;
#endif
	//find empty lines
	//calculate exist result and find score.
	//if score is matched rand a condition and give new score or not depend on condition
	//assign a small score then calculate score
	//if score is not matched...it's fatal error
	//if small score is matched,got all possible result for each lines.
	sSlotBetTableResult l_TableResultForCalScore;
	l_TableResultForCalScore.SpinObjectResultList = l_pTableResult->SpinObjectResultList;
	//
	{
		//for new talbe with empty object
		g_pSlotGamePhase->m_pBetBehavior->AssignLinesData(&l_TableResultForCalScore);
		g_pSlotGamePhase->m_pBetBehavior->CalculateScoreConsecutive(&l_TableResultForCalScore);
		//win too much money or money is enough
		if( l_TableResultForCalScore.iWinMoney > e_iMinScore || l_TableResultForCalScore.iWinMoney > e_iMaxScore )
		{
			if(rand()%2)
			{//money is enough now filled no win money table
				delete l_pTableResult;
				l_pTableResult = GenerateNoWinMoneyWithOldTable(e_pOldTable,e_bFreeGame);
				return l_pTableResult;
			}
		}
	}
	FreeGameCheck(e_bFreeGame,l_pTableResult);
	int	l_iLineSize = (int)g_pSlotGamePhase->m_pSlotRuleData->m_LineRuleList.size();
	int	l_iWinMoney = 0;
	std::vector<int>l_iNewLineNeedToMatch;
	l_iNewLineNeedToMatch.reserve(l_iLineSize);
	for(int i=0;i<l_iLineSize;++i)
	{
		sLineRule*l_pLineRule = g_pSlotGamePhase->m_pSlotRuleData->m_LineRuleList[i];
		bool	l_bNewLine = false;
		for( int j=0;j<l_pLineRule->cOneLineCount;++j )
		{
			int	l_iIndex = l_pLineRule->pcLineObjectIndex[j];
			if( l_pTableResult->SpinObjectResultList[l_iIndex] == -1 )
			{//new line
				l_iNewLineNeedToMatch.push_back(i);
				j = l_pLineRule->cOneLineCount;
				l_bNewLine = true;
			}
		}
	}
	assert(l_iNewLineNeedToMatch.size());
	//assign random result to force line win money
	int	l_iNewLineSize = l_iNewLineNeedToMatch.size();
	l_iNewLineNeedToMatch = UT::GenerateRandomTable<int>(&l_iNewLineNeedToMatch);
	//force target lindex to first
	if( 1 )
	{
		int	l_SwapLinedex = 1;
		int	l_iIndex = UT::IndexOfVector<int>(l_SwapLinedex,&l_iNewLineNeedToMatch);
		if( l_iIndex != -1 )
		{
			int	l_iFirstIndex = l_iNewLineNeedToMatch[0];
			l_iNewLineNeedToMatch[0] = l_SwapLinedex;
			l_iNewLineNeedToMatch[l_iIndex] = l_iFirstIndex;
		}
	}
	int	l_iMinScore = e_iMinScore-l_TableResultForCalScore.iWinMoney;
	int	l_iMaxScore = e_iMaxScore-l_TableResultForCalScore.iWinMoney;
	//if no one is matched get closeset high score
	int	l_iClosestScore = INT_MAX;
	int	l_iClosestScoreScoreSpinObjectID = -1;
	bool	l_bAssignScore = false;
	for(int i=0;i<l_iNewLineSize;++i)
	{
		int	l_iLineIndex = l_iNewLineNeedToMatch[i];
		if( l_iMinScore<0 )
			l_iMinScore = 0;
		if( l_iMaxScore<0 )
			l_iMaxScore = 0;
		int	l_iScore = 0;
		sSlotBetTableResult l_TableResultForCheckScore;
		l_TableResultForCheckScore.SpinObjectResultList = l_TableResultForCalScore.SpinObjectResultList;
		int	l_iSpinObjectID = -1;
		std::vector<int>l_FilledTableIndices;
		bool	l_bFilled = FilledLineScore(&l_TableResultForCheckScore,l_iLineIndex,l_iMinScore,l_iMaxScore,e_iMultiplier,&l_iScore,&l_iSpinObjectID,&l_FilledTableIndices);
		if(l_bFilled)
		{//matched new score calculate new score
			g_pSlotGamePhase->m_pBetBehavior->AssignLinesData(&l_TableResultForCheckScore);
			g_pSlotGamePhase->m_pBetBehavior->CalculateScoreConsecutive(&l_TableResultForCheckScore);
			int	l_iOffset = abs(l_TableResultForCheckScore.iWinMoney-e_iMinScore);
			if( l_iClosestScore>l_iOffset )
			{
				l_iClosestScore = l_iOffset;
				l_iClosestScoreScoreSpinObjectID = l_iSpinObjectID;
			}
			if( l_TableResultForCheckScore.iWinMoney > e_iMaxScore )
			{//fuck win too much money skip this one.
				continue;
			}
			if( l_TableResultForCheckScore.iWinMoney < e_iMinScore )
			{//

				continue;
			}
			l_bAssignScore = true;
			l_TableResultForCalScore.SpinObjectResultList = l_TableResultForCheckScore.SpinObjectResultList;
		}
#ifdef _DEBUG
		if( l_iLineIndex == 1 && !l_bFilled )
		{
			int a=0;
		}
#endif
	}
	if( !l_bAssignScore && l_iMinScore*LINE_COMMUSE_POSSIBLE_COUNT<e_iMaxScore && l_iClosestScoreScoreSpinObjectID != -1 )
	{
		int	l_iLineIndex = l_iNewLineNeedToMatch[0];
		sLineRule*l_pLineRule = g_pSlotGamePhase->m_pSlotRuleData->m_LineRuleList[l_iLineIndex];
		for(int i=0;i<l_pLineRule->cOneLineCount;++i)
		{
			l_TableResultForCalScore.SpinObjectResultList[l_pLineRule->pcLineObjectIndex[i]] = l_iClosestScoreScoreSpinObjectID;
		}
	}
	//if money is still smaller than min money force to add money
	l_pTableResult->SpinObjectResultList = l_TableResultForCalScore.SpinObjectResultList;
	int	l_iSize = l_pTableResult->SpinObjectResultList.size();
	int	l_iFreeGameIndex = g_pSlotGamePhase->m_pSlotRuleData->GetFreeSpinObjectIndex();
	int	l_iWildIndex = g_pSlotGamePhase->m_pSlotRuleData->GetWildObjectIndex();
	for(int i=0;i<l_iSize;++i)
	{
		if(l_pTableResult->SpinObjectResultList[i] == -1)
		{
			if(IsIndexOkForWild(l_pTableResult->SpinObjectResultList[i],l_pTableResult,e_iMinScore,e_iMaxScore,e_iMultiplier))
			{
				l_pTableResult->SpinObjectResultList[i] = l_iWildIndex;
			}
			else
			{
				int	l_iTargetl_iIndex = GetSpinObjectIDNotMatchNeighbor(l_pTableResult->SpinObjectResultList[i],l_pTableResult,e_iMinScore,e_iMaxScore,e_iMultiplier);
				l_pTableResult->SpinObjectResultList[i] = l_iTargetl_iIndex;
			}
		}
	}
	g_pSlotGamePhase->m_pBetBehavior->AssignLinesData(l_pTableResult);
	g_pSlotGamePhase->m_pBetBehavior->CalculateScoreConsecutive(l_pTableResult);
	FreeGameCheck(e_bFreeGame,l_pTableResult);
	if(l_pTableResult->iWinMoney>e_iMaxScore||l_pTableResult->iWinMoney<e_iMinScore)
	{
		//fuck fatal error!,give a smallest answer
		DumpMessage("GenerateWinMoneyWithOldTable",e_iMinScore,e_iMaxScore,e_iMultiplier,l_pTableResult->iWinMoney);
		DumpTable(e_pOldTable,"OldTable",false);
		DumpTable(l_pTableResult,"NewTable",true);
		delete l_pTableResult;
		l_pTableResult = GenerateNoWinMoneyWithOldTable(e_pOldTable,e_bFreeGame);
	}
	return l_pTableResult;
}

sSlotBetTableResult*	GenerateNoWinMoneyWithOldTable(sSlotBetTableResult*e_pOldTable,bool e_bFreeGame)
{
	FreeGameCheck(e_bFreeGame,e_pOldTable);
	std::vector<int>l_EmptyIndexList;
	//assign old table to new one with empty data
	sSlotBetTableResult*l_pTableResult = g_pSlotGamePhase->m_pBetBehavior->GenerateSpinExplosionResultWithEmptySpinObject(e_pOldTable,&l_EmptyIndexList);
#ifdef _DEBUG
	sSlotBetTableResult l_OldTableResultWithEmptySpinObject;
	l_OldTableResultWithEmptySpinObject.SpinObjectResultList = l_pTableResult->SpinObjectResultList;
#endif

	FreeGameCheck(e_bFreeGame,l_pTableResult);
	assert(l_EmptyIndexList.size()&&"GenerateNoWinMoneyWithOldTable");
	//-1 for not acheviedfree game.
	int	l_iFreeGameIndex = g_pSlotGamePhase->m_pSlotRuleData->GetFreeSpinObjectIndex();
	int	l_iNumFreeGameIndex = l_pTableResult->NumSpinObjectInTable(l_iFreeGameIndex);
	int	l_iAddableFreeGame = g_pSlotGamePhase->m_pSlotRuleData->m_iNumFreeGameToAchevied-l_iNumFreeGameIndex;
	if( l_iAddableFreeGame>0 )
	{
		int	l_iRestFreeGameCouldBeUse = rand()%(l_iAddableFreeGame);
		AddFreeGameFromOldTable(l_pTableResult,l_iRestFreeGameCouldBeUse);
	}
	int	l_iColumnCount = g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x;
	int	l_iRowCount = g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y;
	int	l_iEmptySize = (int)l_EmptyIndexList.size();
	int	l_iTableAmountCount = g_pSlotGamePhase->m_pSlotRuleData->GetTableAmount();
	for(int i=0;i<l_iEmptySize;++i)
	{
		std::vector<int>l_RelatedSpinObjectIndex;
		//find out left spin object and give it a non consecutive spin object
		assert(g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y<g_pSlotGamePhase->m_pSlotRuleData->m_SpinObjectDataManager.Count()&&"row is bigger than spin object count...and I am lazy to analize further result FUCK!!");
		std::vector<int>l_LeftSpinObjectList;
		int	l_iIndex = l_EmptyIndexList[i];
		if( l_pTableResult->SpinObjectResultList[l_iIndex] != -1 )
			continue;
		if(  ( l_iIndex % l_iColumnCount) == 0  )
		{//this is most left one
			//find above
			int	l_iAboveIndex = l_iIndex-l_iColumnCount;
			if( l_iAboveIndex>=0 )
			{
				int	l_iSpinObjectIndex = l_pTableResult->SpinObjectResultList[l_iAboveIndex];
				l_RelatedSpinObjectIndex.push_back(l_iSpinObjectIndex);
			}
			//find below
			int	l_iDown = l_iIndex+l_iColumnCount;
			if( l_iDown<l_iTableAmountCount )
			{
				int	l_iSpinObjectIndex = l_pTableResult->SpinObjectResultList[l_iDown];
				l_RelatedSpinObjectIndex.push_back(l_iSpinObjectIndex);
			}
			
		}
		else
		{//find all left object
			//find above
			int	l_iLeftTopIndex = (l_iIndex-1);
			while( l_iLeftTopIndex>=l_iColumnCount )
			{
				l_iLeftTopIndex -= l_iColumnCount;
			}
			for( int j=0;j<l_iRowCount;++j )
			{
				int	l_iSpinObjectIndex = l_pTableResult->SpinObjectResultList[l_iLeftTopIndex+(l_iColumnCount*j)];
				l_RelatedSpinObjectIndex.push_back(l_iSpinObjectIndex);
			}
		}
		int	l_iWildIndex = g_pSlotGamePhase->m_pSlotRuleData->GetWildObjectIndex();
		int	l_iFreeGameIndex = g_pSlotGamePhase->m_pSlotRuleData->GetFreeSpinObjectIndex();
		l_RelatedSpinObjectIndex.push_back(l_iWildIndex);
		l_RelatedSpinObjectIndex.push_back(l_iFreeGameIndex);
		//generate mutex spin object for final result
		std::vector<int>l_ResultIndexVector	= StripNoneUseDataFromAllSpinObjectData(l_RelatedSpinObjectIndex);
		int	l_iResultIndex = l_ResultIndexVector[rand()%l_ResultIndexVector.size()];
		l_pTableResult->SpinObjectResultList[l_EmptyIndexList[i]] = l_iResultIndex;
	}
	FreeGameCheck(e_bFreeGame,l_pTableResult);
	g_pSlotGamePhase->m_pBetBehavior->AssignLinesData(l_pTableResult);
	g_pSlotGamePhase->m_pBetBehavior->CalculateScoreConsecutive(l_pTableResult);
	if( l_pTableResult->iWinMoney )
	{
		DumpMessage("GenerateNoWinMoneyWithOldTableno still give money because explosion table has condition give money",0,0,1,l_pTableResult->iWinMoney);
		DumpTable(e_pOldTable,"old table",true);
#ifdef _DEBUG
		DumpTable(&l_OldTableResultWithEmptySpinObject,"old empty table",false);
#endif
		DumpTable(l_pTableResult,"new table",true);
	}
	return l_pTableResult;
}


bool	IsSpinObjectScoreMatchedWithCount(int e_iSpinObjectIndex,int e_iMinScore,int e_iMaxScore,int e_iForceMatchedCount,int e_iMultiplier,int* e_piSocre)
{
	*e_piSocre = 0;
	cSpinObjectPrizeData*l_pSpineObjectPrizeTable = g_pSlotGamePhase->m_pSlotRuleData->m_SpinObjectDataManager.GetObject(e_iSpinObjectIndex);
	int	l_iScore = (int)l_pSpineObjectPrizeTable->GetOddsByCount(e_iForceMatchedCount)*e_iMultiplier;
	if( l_iScore>=e_iMinScore && l_iScore<=e_iMaxScore )
	{//find maximum count
		*e_piSocre = l_iScore;
		return true;
	}
	return false;
}

//find out max score consecutive count
bool	IsSpinObjectScoreMatched(int e_iSpinObjectIndex,int e_iMinScore,int e_iMaxScore,int e_iMultiplier,int*e_piMatchedCount,int* e_piSocre)
{
	*e_piSocre = 0;
	//cSpinObjectPrizeDataManager
	cSpinObjectPrizeData*l_pSpineObjectPrizeTable = g_pSlotGamePhase->m_pSlotRuleData->m_SpinObjectDataManager.GetObject(e_iSpinObjectIndex);
	*e_piMatchedCount = -1;
	//how many consecutive depend how many column
	//+1 for 1 start
	for( int i=0;i<g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x+1;++i )
	{
		int	l_iScore = (int)l_pSpineObjectPrizeTable->GetOddsByCount(i)*e_iMultiplier;
		if( l_iScore>=e_iMinScore && l_iScore<=e_iMaxScore )
		{//find maximum count
			*e_piMatchedCount = i;
			*e_piSocre = l_iScore;
		}
	}
	return *e_piMatchedCount != -1?true:false;
}

//find suitable spin object and got score
int	GetRandomMatchedSpinObjectByScoreRange(int e_iMinScore,int e_iMaxScroe,int e_iMultiplier,int *e_pScore,int*e_piMatchCount)
{
	int	l_iMultiplier = e_iMultiplier;
	int	l_iSpineObjectPrizeTableListCount = g_pSlotGamePhase->m_pSlotRuleData->m_SpinObjectDataManager.Count();
	int	l_iMatchCount = -1;
	std::vector<int>	l_MatchedIndex;
	std::vector<int>	l_MatchedCount;
	int	l_iScore = 0;
	int	l_iFreeGameIndex = g_pSlotGamePhase->m_pSlotRuleData->GetFreeSpinObjectIndex();
	for( int i=0;i<l_iSpineObjectPrizeTableListCount;++i )
	{
		if(IsSpinObjectScoreMatched(i,e_iMinScore,e_iMaxScroe,l_iMultiplier,&l_iMatchCount,&l_iScore))
		{
			if( l_iFreeGameIndex != i )
			{
				l_MatchedCount.push_back(l_iMatchCount);
				l_MatchedIndex.push_back(i);
			}
		}
	}
	int	l_iFinalIndex = -1;
	if( l_MatchedIndex.size() )
	{
		l_iFinalIndex = rand()%l_MatchedIndex.size();
		cSpinObjectPrizeData*l_pSpineObjectPrizeTable = g_pSlotGamePhase->m_pSlotRuleData->m_SpinObjectDataManager.GetObject(l_MatchedIndex[l_iFinalIndex]);
		*e_piMatchCount = l_MatchedCount[l_iFinalIndex];
		*e_pScore = (int)l_pSpineObjectPrizeTable->GetOddsByCount(*e_piMatchCount)*l_iMultiplier;
	}
	return l_iFinalIndex;
}

int		GetRandomMatchedSpinObjectByScoreRangeWithMatchCount(int e_iMinScore,int e_iMaxScroe,int e_iMultiplier,int e_iMatchCount,int *e_pScore,int*e_piAvoidSpinObjectIndex)
{
	int	l_iMultiplier = e_iMultiplier;
	int	l_iSpineObjectPrizeTableListCount = g_pSlotGamePhase->m_pSlotRuleData->m_SpinObjectDataManager.Count();
	std::vector<int>	l_MatchedIndex;
	l_MatchedIndex.reserve(l_iSpineObjectPrizeTableListCount);
	int	l_iScore = 0;
	for( int i=0;i<l_iSpineObjectPrizeTableListCount;++i )
	{
		if(IsSpinObjectScoreMatchedWithCount(i,e_iMinScore,e_iMaxScroe,e_iMatchCount,l_iMultiplier,&l_iScore))
		{
			l_MatchedIndex.push_back(i);
		}
	}
	int	l_iFinalIndex = -1;
	if( l_MatchedIndex.size() )
	{
		l_iFinalIndex = rand()%l_MatchedIndex.size();
		l_iFinalIndex = l_MatchedIndex[l_iFinalIndex];
		//skip avoid data
		if( e_piAvoidSpinObjectIndex && *e_piAvoidSpinObjectIndex == l_iFinalIndex )
		{//if bigger set to avoid one or return -1
			if(l_MatchedIndex.size() > 1 )
			{
				if( l_MatchedIndex[0] == *e_piAvoidSpinObjectIndex )
					l_iFinalIndex = l_MatchedIndex[1];
				else
					l_iFinalIndex = l_MatchedIndex[0];
			}
			else
				return -1;
		}
		cSpinObjectPrizeData*l_pSpineObjectPrizeTable = g_pSlotGamePhase->m_pSlotRuleData->m_SpinObjectDataManager.GetObject(l_iFinalIndex);
		*e_pScore = (int)l_pSpineObjectPrizeTable->GetOddsByCount(e_iMatchCount)*l_iMultiplier;
	}
	return l_iFinalIndex;
}

bool	IsSpinObjectPossibleWinTooMuch(int e_iMinScore,int e_iMaxScore,int e_iMultiplier,sSlotBetTableResult*e_pTableResult,int e_iTableIndex,int e_iMatchSpinObjectID,int e_iMatchCount)
{
	int	l_iWildIndex = g_pSlotGamePhase->m_pSlotRuleData->GetWildObjectIndex();
	int	l_iFreeGameIndex = g_pSlotGamePhase->m_pSlotRuleData->GetFreeSpinObjectIndex();
	if( e_iMatchSpinObjectID == -1 ||
		e_iMatchSpinObjectID == l_iWildIndex ||
		e_iMatchSpinObjectID == l_iFreeGameIndex)
		return true;
	int	l_iScore = g_pSlotGamePhase->m_pSlotRuleData->m_SpinObjectDataManager.GetObject(e_iMatchSpinObjectID)->GetOddsByCount(e_iMatchCount)*e_iMultiplier;
	if( l_iScore*LINE_COMMUSE_POSSIBLE_COUNT >e_iMaxScore  )
	{
		int	l_iWildIndex = g_pSlotGamePhase->m_pSlotRuleData->GetWildObjectIndex();
		int	l_iColumn = g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x;
		//find closest 2 row and get first object
		int	l_iAdjency[2] = {e_iTableIndex-l_iColumn,e_iTableIndex+l_iColumn};
		//closest line are 2.
		for( int i=0;i<2;++i )
		{
			if( l_iAdjency[i] >= 0  && l_iAdjency[i]<g_pSlotGamePhase->m_pSlotRuleData->GetTableAmount() )
			{
				int	l_iSpinObjectID = e_pTableResult->SpinObjectResultList[l_iAdjency[i]];
				if(	e_iMatchSpinObjectID == l_iSpinObjectID ||
					e_iMatchSpinObjectID == l_iWildIndex ||
					e_iMatchSpinObjectID == -1 )
				{
					//possible win too manuy score skip it,fuck
					return true;
				}
			}
		}
	}
	return false;
}

bool	MatchCountOfEmptyLine(int e_iMinScore,int e_iMaxScore,int e_iMultiplier,sSlotBetTableResult*e_pTableResult,int e_iLineIndex,int*e_pMatchSpinObjectID,int*e_piMatchCount,int*e_iFirstMatchedSpinObjectIndex)
{
	//find first match spinobject
	//1:3 object is same
	//2:3 object is same with wild
	//3:3 object is same with empty spinobject id
	//4:3 object is same with with wild and empty spinobject id
	*e_piMatchCount = 0;
	sLineRule*l_pLineRule = g_pSlotGamePhase->m_pSlotRuleData->m_LineRuleList[e_iLineIndex];
	bool	l_bEmptyIndex = false;
	int	l_iFirstSpinObjectIndex = -1;
	int	l_iWildIndex = g_pSlotGamePhase->m_pSlotRuleData->GetWildObjectIndex();
#ifdef _DEBUG
	std::vector<int>l_TargetIndices;
	for( int i=0;i<l_pLineRule->cOneLineCount;++i )
	{
		int	l_iCurrentIndex = l_pLineRule->pcLineObjectIndex[i];
		int	l_iCurrentID = e_pTableResult->SpinObjectResultList[l_iCurrentIndex];
		l_TargetIndices.push_back(l_iCurrentID);
	}
#endif
	bool	l_bConsecutive = true;
	for( int i=0;i<l_pLineRule->cOneLineCount;++i )
	{
		int	l_iCurrentIndex = l_pLineRule->pcLineObjectIndex[i];
		int	l_iCurrentID = e_pTableResult->SpinObjectResultList[l_iCurrentIndex];
		if( l_iCurrentID == -1  )
		{
			l_bEmptyIndex = true;
		}
		if( l_iFirstSpinObjectIndex == -1 )
		{
			if( l_iCurrentID != -1 && l_iCurrentID != l_iWildIndex )
			{
				l_iFirstSpinObjectIndex = l_iCurrentID;
				*e_iFirstMatchedSpinObjectIndex = i;
			}
			*e_piMatchCount += 1;
		}
		else
		{
			if( l_iCurrentID != l_iWildIndex || l_iFirstSpinObjectIndex != l_iCurrentID )
				l_bConsecutive = false;
			if( l_bConsecutive )
				*e_piMatchCount += 1;
		}
	}
	if( !l_bEmptyIndex)
		return false;
	int	l_iFreeGameIndex = g_pSlotGamePhase->m_pSlotRuleData->GetFreeSpinObjectIndex();
	if( l_iFirstSpinObjectIndex == l_iFreeGameIndex )
	{
		if(*e_iFirstMatchedSpinObjectIndex >= MIN_CONSECUTIVE_COUNT )
		{
			*e_pMatchSpinObjectID = -1;
			*e_piMatchCount = *e_iFirstMatchedSpinObjectIndex;
			*e_iFirstMatchedSpinObjectIndex -=1;
		}
		else
			return false;
	}
	*e_pMatchSpinObjectID = l_iFirstSpinObjectIndex;
	//got score and avoid adjency spin
	//find left column spin object
	if( *e_pMatchSpinObjectID != -1 )
	{
		return IsSpinObjectPossibleWinTooMuch(e_iMinScore,e_iMaxScore,e_iMultiplier,
			e_pTableResult,l_pLineRule->pcLineObjectIndex[*e_iFirstMatchedSpinObjectIndex],
			*e_pMatchSpinObjectID,*e_piMatchCount)?false:true;
	}
	//if( *e_iFirstMatchedSpinObjectIndex )
	return true;
}

bool	FilledLineScore(sSlotBetTableResult*e_pTableResult,int e_iLineIndex,int e_iMinScore,int e_iMaxScore,int e_iMultiplier,int *e_piScore,int*e_piSpinObjectID,std::vector<int>*e_pFilledTableIndices)
{
	//find needed to matched spin object and match count and find score if score is enought assign data
	//if no matched spin object,find match count and give a suitable spinobject to fill score
	*e_piScore = 0;
	//no contain empty index.
	sLineRule*l_pLineRule = g_pSlotGamePhase->m_pSlotRuleData->m_LineRuleList[e_iLineIndex];
	int	l_iScore = 0;
	int	l_iWildIndex = g_pSlotGamePhase->m_pSlotRuleData->GetWildObjectIndex();
	int	l_iFreeGameIndex = g_pSlotGamePhase->m_pSlotRuleData->GetFreeSpinObjectIndex();
	int	l_iMatchCount = 0;
	int	l_iMatchSpinObject = 0;
	int	l_iFirstMatchedSpinObjectIndex = -1;
	if(!MatchCountOfEmptyLine(e_iMinScore,e_iMaxScore,e_iMultiplier,e_pTableResult,e_iLineIndex,&l_iMatchSpinObject,&l_iMatchCount,&l_iFirstMatchedSpinObjectIndex))
	{
		return false;
	}
	//cannt filled consecutive
	if( l_iMatchCount < MIN_CONSECUTIVE_COUNT )
	{
		return false;
	}
	int	l_iAvoidIndexBecauseScoreIsTooBigOrTooSmall = -1;
	if( l_iMatchSpinObject != -1 )
	{
		cSpinObjectPrizeData*l_pSpineObjectPrizeTable = g_pSlotGamePhase->m_pSlotRuleData->m_SpinObjectDataManager.GetObject(l_iMatchSpinObject);
		for( int i=l_iMatchCount;i>=MIN_CONSECUTIVE_COUNT;--i )
		{
			*e_piScore = l_pSpineObjectPrizeTable->GetOddsByCount(i)*e_iMultiplier;
			if( *e_piScore *LINE_COMMUSE_POSSIBLE_COUNT< e_iMinScore )
				break;
			if( *e_piScore *LINE_COMMUSE_POSSIBLE_COUNT> e_iMaxScore )
				continue;
			int	l_iTableIndex = l_pLineRule->pcLineObjectIndex[i-1];
			int	l_iSpinObjectID = e_pTableResult->SpinObjectResultList[l_iTableIndex];
			if( l_iSpinObjectID == -1 )
				continue;
			goto SUCCESS;
		}
		if( l_iFirstMatchedSpinObjectIndex >= MIN_CONSECUTIVE_COUNT )
		{
			l_iAvoidIndexBecauseScoreIsTooBigOrTooSmall = l_iMatchSpinObject;
			l_iMatchCount = l_iFirstMatchedSpinObjectIndex;
			l_iMatchSpinObject = -1;
			//back a step to skip this object.
			l_iFirstMatchedSpinObjectIndex -= 1;
			int	l_iAllSpinObjectAmount = g_pSlotGamePhase->m_pSlotRuleData->m_SpinObjectDataManager.Count();
			for( int i=0;i<l_iAllSpinObjectAmount;++i )
			{
				int	l_iScore = g_pSlotGamePhase->m_pSlotRuleData->m_SpinObjectDataManager.GetObject(i)->GetOddsByCount(l_iMatchCount)*e_iMultiplier;
				if( l_iScore*LINE_COMMUSE_POSSIBLE_COUNT>=e_iMinScore )
				{
					if(!IsSpinObjectPossibleWinTooMuch(e_iMinScore,e_iMaxScore,e_iMultiplier,e_pTableResult,l_pLineRule->pcLineObjectIndex[l_iFirstMatchedSpinObjectIndex],l_iMatchSpinObject,l_iMatchCount))
					{
						goto SUCCESS;
					}
				}
			}
		}
	}
	//if empty is not from exist give it a random id who filled score condition andfind out match count
	if( l_iMatchSpinObject == -1 || l_iMatchSpinObject == l_iFreeGameIndex  )
	{
		//ignore try to speed up cpu calculate.FUCK
		l_iMatchSpinObject = GetRandomMatchedSpinObjectByScoreRangeWithMatchCount(e_iMinScore,e_iMaxScore,e_iMultiplier,l_iMatchCount,e_piScore,&l_iAvoidIndexBecauseScoreIsTooBigOrTooSmall);
		if(IsSpinObjectPossibleWinTooMuch(e_iMinScore,e_iMaxScore,e_iMultiplier,e_pTableResult,l_pLineRule->pcLineObjectIndex[l_iFirstMatchedSpinObjectIndex],l_iMatchSpinObject,l_iMatchCount))
		{
			l_iMatchSpinObject = -1;
		}
		//fuck count is not match change match count and refind.
		if( l_iMatchSpinObject == -1 || l_iMatchSpinObject == l_iFreeGameIndex )
		{
			l_iMatchCount = MIN_CONSECUTIVE_COUNT;
			l_iMatchSpinObject = GetRandomMatchedSpinObjectByScoreRangeWithMatchCount(e_iMinScore,e_iMaxScore,e_iMultiplier,l_iMatchCount,e_piScore);
			if( l_iMatchSpinObject == l_iFreeGameIndex )
				return false;
			if(!IsSpinObjectPossibleWinTooMuch(e_iMinScore,e_iMaxScore,e_iMultiplier,e_pTableResult,l_pLineRule->pcLineObjectIndex[l_iFirstMatchedSpinObjectIndex],l_iMatchSpinObject,l_iMatchCount))
			{
				return false;
			}
			//l_iSpinObjectID = GetRandomMatchedSpinObjectByScoreRange(e_iMinScore,e_iMaxScore,e_iMultiplier,e_piScore,&l_iMatchCount);
		}
	}
	if( l_iMatchSpinObject == -1 )
		return false;
	l_iScore = (int)(g_pSlotGamePhase->m_pSlotRuleData->m_SpinObjectDataManager.GetObject(l_iMatchSpinObject)->GetOddsByCount(l_iMatchCount)*e_iMultiplier);
	if( l_iScore <e_iMinScore || l_iScore>e_iMaxScore )
	{
		return false;
	}
SUCCESS:
	//幹沒連續當成有連續...gy
	for( int i=0;i<l_pLineRule->cOneLineCount;++i )
	{
		int	l_iIndex = l_pLineRule->pcLineObjectIndex[i];
		int	l_iCurrentSpinObjectID = e_pTableResult->SpinObjectResultList[l_iIndex];
		if( e_pTableResult->SpinObjectResultList[l_iIndex] != -1 )
		{
			if( l_iCurrentSpinObjectID == l_iMatchSpinObject || 
				l_iCurrentSpinObjectID == l_iWildIndex )
			{
				--l_iMatchCount;
			}
			continue;
		}
		if( l_iMatchCount )
		{
			e_pTableResult->SpinObjectResultList[l_iIndex] = l_iMatchSpinObject;
			e_pFilledTableIndices->push_back(l_iIndex);
		}
		else
		{
			e_pTableResult->SpinObjectResultList[l_iIndex] = g_pSlotGamePhase->m_pSlotRuleData->GetRandomSpinObject(true,true);
		}
		--l_iMatchCount;
	}
	*e_piSpinObjectID = l_iMatchSpinObject;
	return true;
}


int		GetSpinObjectIDNotMatchNeighbor(int e_iIndex,sSlotBetTableResult*e_pTableResult,int e_iMinScore,int e_iMaxScore,int e_iMultiplier)
{
	int	l_iAvoidRow = g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y;
	int	l_iColumn = g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x;
	int	l_Amount = g_pSlotGamePhase->m_pSlotRuleData->GetTableAmount();
	int	l_iLeftUpIndex = e_iIndex-1;
	while( l_iLeftUpIndex >= l_iColumn )
	{
		l_iLeftUpIndex -= g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x;
	}
	std::vector<int>l_AvoidIndex;
	l_AvoidIndex.reserve(9);
	for( int i=0;i<l_iAvoidRow;++i )
	{
		int	l_TableIndex = l_iLeftUpIndex+(i*l_iColumn);
		if( l_TableIndex < 0 || l_TableIndex >= l_Amount )
			continue;
		int	l_SpinObjectIndex = e_pTableResult->SpinObjectResultList[l_TableIndex];
		l_AvoidIndex.push_back(l_SpinObjectIndex);
	}
	int	l_iWildIndex = g_pSlotGamePhase->m_pSlotRuleData->GetWildObjectIndex();
	int	l_iFreeGameIndex = g_pSlotGamePhase->m_pSlotRuleData->GetFreeSpinObjectIndex();
	l_AvoidIndex.push_back(l_iWildIndex);
	l_AvoidIndex.push_back(l_iFreeGameIndex);
	std::vector<int>l_Result = StripNoneUseDataFromAllSpinObjectData(l_AvoidIndex);
	assert(l_Result.size()>0);
#ifdef _DEBUG
	if( l_Result.size() == 0 )
	{
		cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine("GetSpinObjectIDNotMatchNeighbor error");
	}
#endif
	return l_Result[rand()%l_Result.size()];
}

void	ForceActiveFreeGame(sSlotBetTableResult*e_pTableResult)
{
	//1st remove all free spin object and assign min score to it
	int	l_iFreeGameIndex = g_pSlotGamePhase->m_pSlotRuleData->GetFreeSpinObjectIndex();
	int	l_iIndex = g_pSlotGamePhase->m_pSlotRuleData->GetMinScoreSpinObjectIndex();
	int	l_iSpinObjectAmount = g_pSlotGamePhase->m_pSlotRuleData->GetTableAmount();
	for( int i=0;i<l_iSpinObjectAmount;++i )
	{
		if( e_pTableResult->SpinObjectResultList[l_iIndex] == l_iFreeGameIndex )
			e_pTableResult->SpinObjectResultList[l_iIndex] = l_iIndex;
	}
	//2nd assign free game
	int	l_Value = rand()%g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x+1;
	if( l_Value < g_pSlotGamePhase->m_pSlotRuleData->m_iNumFreeGameToAchevied )
		l_Value = g_pSlotGamePhase->m_pSlotRuleData->m_iNumFreeGameToAchevied;
	std::vector<int>l_Result = GenerateRandomTable(l_Value,g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x);
	int	l_iResultCount = (int)l_Result.size();

	for( int i=0;i<l_iResultCount;++i )
	{
		int	l_iTableIndex = rand()%g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y;
		l_iTableIndex = (g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x*l_iTableIndex)+l_Result[i];
		e_pTableResult->SpinObjectResultList[l_iTableIndex] = l_iFreeGameIndex;
	}
	FreeGameCheck(true,e_pTableResult);
}

void	AddFreeGameFromOldTable(sSlotBetTableResult*e_pTableResult,int e_iCount)
{
	if( e_iCount<=0 )
		return;
	int	l_iFreeGameIndex = g_pSlotGamePhase->m_pSlotRuleData->GetFreeSpinObjectIndex();
	int	l_iNumFreeGameIndex = e_pTableResult->NumSpinObjectInTable(l_iFreeGameIndex);
	if( l_iNumFreeGameIndex >0 )
		return;
	//2nd assign free game
	std::vector<int>l_Result = GenerateRandomTable(e_iCount,g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x);
	int	l_iResultCount = (int)l_Result.size();
	for( int i=0;i<l_iResultCount;++i )
	{
		int	l_iIndex = l_Result[i];
		if( e_pTableResult->SpinObjectResultList[l_iIndex] == -1 )
		{
			e_pTableResult->SpinObjectResultList[l_iIndex] = l_iFreeGameIndex;
		}
	}
}

bool	IsIndexOkForWild(int e_iIndex,sSlotBetTableResult*e_pTableResult,int e_iMinScore,int e_iMaxScore,int e_iMultiplier)
{
	int	l_iAvoidRow = g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y;
	int	l_iColumn = g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x;
	int	l_Amount = g_pSlotGamePhase->m_pSlotRuleData->GetTableAmount();
	int	l_iLeftUpIndex = e_iIndex-1;
	while( l_iLeftUpIndex >= l_iColumn )
	{
		l_iLeftUpIndex -= g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x;
	}
	int	l_TotalScroe = 0;
	for( int i=0;i<l_iAvoidRow;++i )
	{
		int	l_TableIndex = l_iLeftUpIndex+(i*l_iColumn);
		if( l_TableIndex < 0 || l_TableIndex >= l_Amount )
			continue;
		int	l_SpinObjectIndex = e_pTableResult->SpinObjectResultList[l_TableIndex];
		cSpinObjectPrizeData*l_pSpineObjectPrizeTable = g_pSlotGamePhase->m_pSlotRuleData->m_SpinObjectDataManager.GetObject(l_SpinObjectIndex);
		if( l_pSpineObjectPrizeTable )
		{
			int	l_iScore = l_pSpineObjectPrizeTable->GetMaxOdds()*e_iMultiplier;
			l_TotalScroe += l_iScore;
			if( l_TotalScroe > e_iMaxScore)
				return false;
		}
	}
	return true;
}