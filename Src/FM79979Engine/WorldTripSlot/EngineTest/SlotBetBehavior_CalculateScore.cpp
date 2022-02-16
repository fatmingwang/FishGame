#include "stdafx.h"
#include "SlotRule.h"
#include "PlayerData.h"
#include "SlotBetBehavior.h"
#include "SlotObject.h"
#include "SlotGamePhase.h"
#include "WinMoneyLine.h"
#include "AllPhaseDefine.h"
#include "GameNetwork.h"

std::vector<cSlotBetBehavior::sSpinObjectNameAndCount>	cSlotBetBehavior::GetNameAndCountByLineIndexByConsecutive(int e_iIndex,sSlotBetTableResult*e_pTableResult)
{
	std::vector<sSpinObjectNameAndCount>	l_NameAndCountList;
	int	l_iWinMoney = 0;
	sLineRule*l_pLineRule = g_pSlotGamePhase->m_pSlotRuleData->m_LineRuleList[e_iIndex];
	int	l_iLineObjectCount = l_pLineRule->cOneLineCount;
	//do a very quick search to find outeach object's count
	for( int j=0;j<l_iLineObjectCount;++j )
	{
		int	l_iSpinObjectIndex = l_pLineRule->pcLineObjectIndex[j];
		int	l_iSpinObjectResultIndex = e_pTableResult->SpinObjectResultList[l_iSpinObjectIndex];
		//-1 to skip.
		if( l_iSpinObjectResultIndex == -1 )
			continue;
		std::wstring	l_strTargetName = g_pSlotGamePhase->m_pSlotRuleData->m_SpinObjectDataManager.GetObject(l_iSpinObjectResultIndex)->GetName();
		int	l_iExistCount = (int)l_NameAndCountList.size();
		int	l_iSameCount = 0;
		for( int k=0;k<l_iExistCount;++k )
		{
			const WCHAR*l_str = l_NameAndCountList[k].strName.c_str();
			if(!wcscmp(l_str,l_strTargetName.c_str()))
			{
				++l_iSameCount;
				++l_NameAndCountList[k].iCount;
				l_NameAndCountList[k].WinMoneySpinObjectIndexOfTable.push_back(l_iSpinObjectIndex);
				l_NameAndCountList[k].IndexOfLine.push_back(j);
			}
		}
		if( l_iSameCount == 0 )
		{
			sSpinObjectNameAndCount	l_sSpinObjectNameAndCount;
			l_sSpinObjectNameAndCount.iCount = 1;
			l_sSpinObjectNameAndCount.strName = l_strTargetName;
			l_sSpinObjectNameAndCount.WinMoneySpinObjectIndexOfTable.push_back(l_iSpinObjectIndex);
			l_sSpinObjectNameAndCount.IndexOfLine.push_back(j);
			l_NameAndCountList.push_back(l_sSpinObjectNameAndCount);
		}
	}
	//ensure this line has bet the mony or skip this
	if( e_iIndex<this->m_iNumLinesBet )
	{
		//now check count and get win money
		int	l_iNameAndCountListSize  = (int)l_NameAndCountList.size();
		//do the special spingobject
		if( g_pSlotGamePhase->m_pSlotRuleData->m_strSpecilaSpinObjectName.length() )
		{
			sSpinObjectNameAndCount	l_SpecialSpinObjectameAndCount;
			const WCHAR*l_strSpecialSpinObject = g_pSlotGamePhase->m_pSlotRuleData->m_strSpecilaSpinObjectName.c_str();
			int	l_iNumSpecialSpinObject = 0;
			for( int j=0;j<l_iNameAndCountListSize;++j  )
			{
				if( !wcscmp(l_NameAndCountList[j].strName.c_str(),l_strSpecialSpinObject) )
				{
					l_SpecialSpinObjectameAndCount = l_NameAndCountList[j];
					l_iNumSpecialSpinObject = l_NameAndCountList[j].iCount;
					l_NameAndCountList.erase(l_NameAndCountList.begin()+j);
					l_iNameAndCountListSize  = (int)l_NameAndCountList.size();
					break;
				}
			}
			if( l_iNumSpecialSpinObject  )
			{
				for( int j=0;j<l_iNameAndCountListSize;++j  )
				{
					sSpinObjectNameAndCount*l_pNameAndCount = &l_NameAndCountList[j];
					l_pNameAndCount->iCount += l_iNumSpecialSpinObject;
					for(int k=0;k<(int)l_SpecialSpinObjectameAndCount.WinMoneySpinObjectIndexOfTable.size();++k)
					{
						l_pNameAndCount->WinMoneySpinObjectIndexOfTable.push_back(l_SpecialSpinObjectameAndCount.WinMoneySpinObjectIndexOfTable[k]);
						l_pNameAndCount->IndexOfLine.push_back(l_SpecialSpinObjectameAndCount.IndexOfLine[k]);
					}
					
				}
			}
		}
	}
	return l_NameAndCountList;
}

cSlotBetBehavior::sSpinObjectNameAndCount*	cSlotBetBehavior::GetMatchCountAndNameAndCountData(std::vector<sSpinObjectNameAndCount>*e_pNameAndCountList,int e_iIndex)
{
	sLineRule*l_pLineRule = g_pSlotGamePhase->m_pSlotRuleData->m_LineRuleList[e_iIndex];
	//now check count and get win money
	int	l_iNameAndCountListSize  = (int)e_pNameAndCountList->size();
	for( int j=0;j<l_iNameAndCountListSize;++j  )
	{
		cSpinObjectPrizeData*l_pSpineObjectPrizeTable = g_pSlotGamePhase->m_pSlotRuleData->m_SpinObjectDataManager.GetObject((*e_pNameAndCountList)[j].strName.c_str());
		if( !l_pSpineObjectPrizeTable )
		{
			UT::ErrorMsg((WCHAR*)(*e_pNameAndCountList)[j].strName.c_str(),L"no this object name in prize table");
		}
		sSpinObjectNameAndCount*l_pNameAndCount = &(*e_pNameAndCountList)[j];
		l_pNameAndCount->SortIndex();
		//spinobject count could not same as consecutive count,and the consecutive are wee need.
		int	l_iMatchCount = 1;
		int	l_iStartIndex = l_pNameAndCount->IndexOfLine[0];
		if( l_iStartIndex !=0  )
			continue;
		int	l_iWinMoneySpinObjectIndexSize = l_pNameAndCount->iCount;
		for( int k=1;k<l_iWinMoneySpinObjectIndexSize;++k )
		{
			int	l_iIndex = l_pNameAndCount->IndexOfLine[k];
			if( l_iStartIndex +1 != l_iIndex)
			{
				break;
			}
			++l_iStartIndex;
			++l_iMatchCount;
		}
		int	l_iOdds = l_pSpineObjectPrizeTable->GetOddsByCount(l_iMatchCount);
		if( l_iOdds>0 )
		{
			l_pNameAndCount->WinMoneySpinObjectIndexOfTable.clear();
			for( int k=0;k<l_iMatchCount;++k )
			{
				int	l_iSpinObjectIndex = l_pLineRule->pcLineObjectIndex[k];
				l_pNameAndCount->WinMoneySpinObjectIndexOfTable.push_back(l_iSpinObjectIndex);
			}
			l_pNameAndCount->iCount = l_iMatchCount;
			//int	l_iLineWinMoney = (int)(l_iOdds*m_iBonusMoneyRate)*m_iPerLineBetMoney;
			//std::wstring	l_OutputData = L"Line:";
			//l_OutputData += ValueToStringW(e_iIndex);
			//l_OutputData += L",Name:";
			//l_OutputData += l_pNameAndCount->strName.c_str();
			//l_OutputData += L",MatchCount";
			//l_OutputData += ValueToStringW(l_pNameAndCount->iCount);
			//l_OutputData += L",Index:";
			//for( int k=0;k<l_pNameAndCount->iCount;++k )
			//{
			//	int	l_iSpinObjectIndex = l_pNameAndCount->WinMoneySpinObjectIndex[k];
			//	l_OutputData += ValueToStringW(l_iSpinObjectIndex);
			//	l_OutputData += L",";
			//}
			//printf(UT::WcharToChar(l_OutputData.c_str()));
			//printf(UT::WcharToChar(L"\n"));
			return l_pNameAndCount;
		}
	}
	return 0;
}

void	cSlotBetBehavior::CalculateScoreConsecutive(sSlotBetTableResult*e_pTableResult)
{
	int	l_iLineRuleListSize = (int)g_pSlotGamePhase->m_pSlotRuleData->m_LineRuleList.size();
	int	l_iTotalSpinObjectCount = g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x*g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y;
	if( l_iLineRuleListSize )
	{
		for( int i=0;i<l_iLineRuleListSize;++i )
		{
			std::vector<sSpinObjectNameAndCount>	l_NameAndCountList = GetNameAndCountByLineIndexByConsecutive(i,e_pTableResult);
			sLineRule*l_pLineRule = g_pSlotGamePhase->m_pSlotRuleData->m_LineRuleList[i];
			sSlotBetTableResult::sLineResult*l_pLine = &e_pTableResult->AllLineList[i];
			l_pLine->iIndex = i;			
			//ensure this line has bet the mony or skip this
			if( i<this->m_iNumLinesBet )
			{
				int	l_iNameAndCountListSize  = (int)l_NameAndCountList.size();
				cSlotBetBehavior::sSpinObjectNameAndCount*l_pNameAndCount = GetMatchCountAndNameAndCountData(&l_NameAndCountList,i);
				if( !l_pNameAndCount )
					continue;
				int	l_iWinMoneySpinObjectID = g_pSlotGamePhase->m_pSlotRuleData->m_SpinObjectDataManager.GetObjectIndexByName(l_pNameAndCount->strName.c_str());
				cSpinObjectPrizeData*l_pSpineObjectPrizeTable = g_pSlotGamePhase->m_pSlotRuleData->m_SpinObjectDataManager.GetObject(l_iWinMoneySpinObjectID);
				int	l_iOdds = l_pSpineObjectPrizeTable->GetOddsByCount(l_pNameAndCount->iCount);
				int	l_iLineWinMoney = l_iOdds*m_iBonusMoneyRate*m_iPerLineBetMoney;
				l_pLine->iWinMoney = l_iLineWinMoney;
				e_pTableResult->iWinMoney += l_iLineWinMoney;
				for( int k=0;k<l_pNameAndCount->iCount;++k )
				{
					int	l_iSpinObjectIndex = l_pLineRule->pcLineObjectIndex[k];
					l_pLine->WinMoneySpinObjectIndexList.push_back(l_iSpinObjectIndex);
					e_pTableResult->pbWinMoneySpinObject[l_iSpinObjectIndex] = true;
				}
				l_pLine->iWinMoneySpinObjectID = l_iWinMoneySpinObjectID;
				l_pLine->iMatchCount = l_pNameAndCount->iCount;
				e_pTableResult->WinMoneyLineList.push_back(*l_pLine);
				e_pTableResult->WinMoneyLineIndexList.push_back(i);
				e_pTableResult->piConsecutiveCount[l_pNameAndCount->iCount] += 1;
				e_pTableResult->piConsecutiveScore[l_pNameAndCount->iCount] += l_iLineWinMoney;
			}
			else//play havent bet after this line.
			{
				return;
			}
		}
	}
}

void	cSlotBetBehavior::CalculateScoreByLine()
{
	//1st:find out all items on the result
	//2nd:check slot lint rule
	//3rd calculate score
	//
	int	l_iSize = (int)g_pSlotGamePhase->m_pSlotRuleData->m_LineRuleList.size();
	int	l_iTotalSpinObjectCount = g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x*g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y;
	if( l_iSize )
	{
		for( int i=0;i<l_iSize;++i )
		{
			std::vector<sSpinObjectNameAndCount>	l_NameAndCountList;
			int	l_iWinMoney = 0;
			sLineRule*l_pLineRule = g_pSlotGamePhase->m_pSlotRuleData->m_LineRuleList[i];
			int	l_iLineObjectCount = l_pLineRule->cOneLineCount;
			//do a very quick search to find outeach object's count
			for( int j=0;j<l_iLineObjectCount;++j )
			{
				int	l_iSpinObjectIndex = l_pLineRule->pcLineObjectIndex[j];
				std::wstring	l_strTargetName = GetSpinObjectNameByIndex(l_iSpinObjectIndex);
				int	l_iExistCount = (int)l_NameAndCountList.size();
				int	l_iSameCount = 0;
				for( int k=0;k<l_iExistCount;++k )
				{
					const WCHAR*l_str = l_NameAndCountList[k].strName.c_str();
					if(!wcscmp(l_str,l_strTargetName.c_str()))
					{
						++l_iSameCount;
						++l_NameAndCountList[k].iCount;
						l_NameAndCountList[k].WinMoneySpinObjectIndexOfTable.push_back(l_iSpinObjectIndex);
					}
				}
				if( l_iSameCount == 0 )
				{
					sSpinObjectNameAndCount	l_sSpinObjectNameAndCount;
					l_sSpinObjectNameAndCount.iCount = 1;
					l_sSpinObjectNameAndCount.strName = l_strTargetName;
					l_sSpinObjectNameAndCount.WinMoneySpinObjectIndexOfTable.push_back(l_iSpinObjectIndex);
					l_NameAndCountList.push_back(l_sSpinObjectNameAndCount);
				}
			}
			//ensure this line has bet the mony or skip this
			if( i<this->m_iNumLinesBet )
			{
				//now check count and get win money
				int	l_iNameAndCountListSize  = (int)l_NameAndCountList.size();

				//do the special spingobject
				if( g_pSlotGamePhase->m_pSlotRuleData->m_strSpecilaSpinObjectName.length() )
				{
					sSpinObjectNameAndCount	l_SpecialSpinObjectameAndCount;
					const WCHAR*l_strSpecialSpinObject = g_pSlotGamePhase->m_pSlotRuleData->m_strSpecilaSpinObjectName.c_str();
					int	l_iNumSpecialSpinObject = 0;
					for( int j=0;j<l_iNameAndCountListSize;++j  )
					{
						if( !wcscmp(l_NameAndCountList[j].strName.c_str(),l_strSpecialSpinObject) )
						{
							l_SpecialSpinObjectameAndCount = l_NameAndCountList[j];
							l_iNumSpecialSpinObject = l_NameAndCountList[j].iCount;
							l_NameAndCountList.erase(l_NameAndCountList.begin()+j);
							l_iNameAndCountListSize  = (int)l_NameAndCountList.size();
							break;
						}
					}
					if( l_iNumSpecialSpinObject  )
					{
						for( int j=0;j<l_iNameAndCountListSize;++j  )
						{
							sSpinObjectNameAndCount*l_pNameAndCount = &l_NameAndCountList[j];
							l_pNameAndCount->iCount += l_iNumSpecialSpinObject;
							for(int k=0;k<(int)l_SpecialSpinObjectameAndCount.WinMoneySpinObjectIndexOfTable.size();++k)
								l_pNameAndCount->WinMoneySpinObjectIndexOfTable.push_back(l_SpecialSpinObjectameAndCount.WinMoneySpinObjectIndexOfTable[k]);
							
						}
					}
				}

				for( int j=0;j<l_iNameAndCountListSize;++j  )
				{
					cSpinObjectPrizeData*l_pSpineObjectPrizeTable = g_pSlotGamePhase->m_pSlotRuleData->m_SpinObjectDataManager.GetObject(l_NameAndCountList[j].strName.c_str());
					if( !l_pSpineObjectPrizeTable )
					{
						UT::ErrorMsg((WCHAR*)l_NameAndCountList[j].strName.c_str(),L"no this object name in prize table");
					}

					int	l_iOdds = l_pSpineObjectPrizeTable->GetOddsByCount(l_NameAndCountList[j].iCount);
					l_iWinMoney += l_iOdds*m_iBonusMoneyRate;
				}
				//now we have know witch line index got money
				if( l_iWinMoney !=0 )
				{
					l_iWinMoney *= m_iPerLineBetMoney;
				}
			}
			else
			//play havent bet after this line.
			{
				return;
			}
		}
	}
}

void	cSlotBetBehavior::AssignLinesData(sSlotBetTableResult*e_pTableResult)
{
	int	l_iNumSpinObject = e_pTableResult->SpinObjectResultList.size();
	size_t	l_iLines = g_pSlotGamePhase->m_pSlotRuleData->m_LineRuleList.size();
	if(!e_pTableResult->pbWinMoneySpinObject)
		e_pTableResult->pbWinMoneySpinObject = new bool[l_iNumSpinObject];
	memset(e_pTableResult->pbWinMoneySpinObject,0,sizeof(bool)*l_iNumSpinObject);
	e_pTableResult->ClearData(l_iLines);
	for( size_t i = 0;i<l_iLines;++i )
	{
		sSlotBetTableResult::sLineResult	l_Line;
		l_Line.iIndex = i;
		sLineRule*l_pLineRule = g_pSlotGamePhase->m_pSlotRuleData->m_LineRuleList[i];
		char	l_cNum = l_pLineRule->cOneLineCount;
		for( char j = 0;j<l_cNum;++j )
		{
			assert(l_pLineRule->pcLineObjectIndex[j]<=l_iNumSpinObject&&"table index out of range");
			int	l_iIndex = e_pTableResult->SpinObjectResultList[l_pLineRule->pcLineObjectIndex[j]];
			if( l_iIndex >= 11 )
			{
				int a=0;
			}
			l_Line.SpinObjectResultList.push_back(l_iIndex);
		}
		e_pTableResult->AllLineList.push_back(l_Line);
	}
}
