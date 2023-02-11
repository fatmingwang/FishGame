#include "stdafx.h"
#include "MonsterGenerate.h"
#include "MonsterManager.h"
#include "../GameApp/GameApp.h"
#include "../GameApp/GameApp.h"
#include "../ControlPanel/ControlPanel.h"


sFishShowProbaility::sData::sData()
{
	iProbaility = 0;
	iFishIndex = -1;
	fFishSingleProbability = 1.f;
	fFishGroupMPDIProbability = 0.f;
	fFishGroupProbability = 0.f;
	iFishGroupCount.Max = 7;
	iFishGroupCount.Min = 7;
}

sFishShowProbaility::eFishGoType		sFishShowProbaility::sData::GetFishGoType()
{
	float	l_fProbabilirtSort[] = {fFishGroupMPDIProbability,fFishGroupProbability,fFishSingleProbability};
	float	l_fStartValue[3] = {fFishGroupMPDIProbability,
								fFishGroupMPDIProbability+fFishGroupProbability,
								fFishGroupMPDIProbability+fFishGroupProbability+fFishSingleProbability};
	float	l_fRandomValue = frand(0,l_fStartValue[2]);
	for( int i=0;i<eFGT_MAX;++i )
	{
		if( l_fStartValue[i]>l_fRandomValue )
		{
			if( l_fProbabilirtSort[i] != 0 )
			{
				return (eFishGoType)i;
			}
		}
	}
	return eFGT_SINGLE;
}

sFishShowProbaility::sFishShowProbaility()
{
	iTotalProbability = 0;
	bPauseGenerateMonster = false;
	TimeGapToGenerateMonster.SetTargetTime( 0.5f );
	fTimeToGenerateMonsters = 0.5f;
	FishShowProbabilityList.clear();
}
sFishShowProbaility::~sFishShowProbaility()
{

}


void	sFishShowProbaility::ProcessShowProbability(TiXmlElement*e_pTiXmlElement,const WCHAR*e_strMonsterName,int e_iMonsterID)
{
	sFishShowProbaility::sData	l_sFishShowProbaility;
	l_sFishShowProbaility.iFishIndex = e_iMonsterID;
	l_sFishShowProbaility.strFishName = e_strMonsterName;
	PARSE_ELEMENT_START(e_pTiXmlElement)	
		COMPARE_NAME("ShowProbability")
		{
			l_sFishShowProbaility.iProbaility = VALUE_TO_INT;
			iTotalProbability += l_sFishShowProbaility.iProbaility;
		}
		else
		COMPARE_NAME("FishGroupProbability")
		{
			l_sFishShowProbaility.fFishGroupProbability = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("FishGroupCount")
		{
			l_sFishShowProbaility.iFishGroupCount = VALUE_TO_VECTOR2;
		}
		else
		COMPARE_NAME("FishGroupMPDIProbability")
		{
			l_sFishShowProbaility.fFishGroupMPDIProbability = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("SingleFishProbability")
		{
			l_sFishShowProbaility.fFishSingleProbability = VALUE_TO_FLOAT;
		}
	PARSE_NAME_VALUE_END
	FishShowProbabilityList.push_back(l_sFishShowProbaility);
}

void	sFishShowProbaility::ProcessFishData(TiXmlElement*e_pTiXmlElement)
{
	iTotalProbability = 0;
	const WCHAR*l_strFishGroupProbability = e_pTiXmlElement->Attribute(L"FishGroupProbability");
	if( l_strFishGroupProbability )
	{
		fFishGroupProbability = (float)_wtof(l_strFishGroupProbability);
	}
	e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while( e_pTiXmlElement )
	{
		if(!wcscmp(e_pTiXmlElement->Value(),L"FishGenerateFactor"))
		{
			int	l_iIndex = 0;
			sFishShowProbaility::sFishOutData	l_FishOutData;
			PARSE_ELEMENT_START(e_pTiXmlElement)
				COMPARE_NAME("PlayerCount")
				{
					l_iIndex = VALUE_TO_INT;
				}
				else
				COMPARE_NAME("NumFishOutAtSameTime")
				{
					l_FishOutData.iOutFishCount = VALUE_TO_VECTOR2;
				}
				else
				COMPARE_NAME("TimeToGenerateMonsters")
				{
					l_FishOutData.fTimeCircle = VALUE_TO_VECTOR2;
				}
			PARSE_NAME_VALUE_END
			PlayerCountWithFish[l_iIndex] = l_FishOutData;
			
		}
		e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
	}
	if(PlayerCountWithFish.size() != 6)
	{
		UT::ErrorMsg(L"m_PlayerCountWithFish not matched", ValueToStringW(PlayerCountWithFish.size()));
	}
	if(cFishApp::m_spControlPanel->m_iPlayerCount != 1 &&
		cFishApp::m_spControlPanel->m_iPlayerCount != 4 &&
		cFishApp::m_spControlPanel->m_iPlayerCount != 6 &&
		cFishApp::m_spControlPanel->m_iPlayerCount != 8 &&
		cFishApp::m_spControlPanel->m_iPlayerCount != 10 &&
		cFishApp::m_spControlPanel->m_iPlayerCount != 14)
	{
		UT::ErrorMsg(L"cFishApp::m_spControlPanel->m_iPlayerCount not matched not matched",ValueToStringW(cFishApp::m_spControlPanel->m_iPlayerCount));
	}
}

cMonster*	sFishShowProbaility::MonsterGo( int e_iMonsterID )
{
	int	l_iIndex = e_iMonsterID;
	//cMonster*l_pMonster = cFishApp::m_spMonsterManager->Require(rand()%cFishApp::m_spMonsterManager->Count());
	cMonster *l_pMonster = cFishApp::m_spMonsterManager->Require(l_iIndex);
	return l_pMonster;
}

void	sFishShowProbaility::MonsterGenerate( float e_fElpaseTime )
{
	if( bPauseGenerateMonster )
	{
		return;
	}
	//
	TimeGapToGenerateMonster.Update( e_fElpaseTime );
	if( !TimeGapToGenerateMonster.bTragetTimrReached )
	{
		return;
	}
	TimeGapToGenerateMonster.SetTargetTime( fTimeToGenerateMonsters );
	//
	//check for new fish
	//fucking game designer
	int	l_iRest = cFishApp::m_spMonsterManager->GetMaxMonsterAlive() - cFishApp::m_spMonsterManager->GetCountMonsterAlive();
	if( iNumMonsterOutAtSameTime > 0 && l_iRest>0 )
	{
		int	l_iNumMonster = rand() % l_iRest % iNumMonsterOutAtSameTime;
		//int	l_iNumMonster = l_iRest;
		//
		#define	SMALLEST_MOMSTER_AMOUNT_DIVISION	4
		//
		if( cFishApp::m_spMonsterManager->GetCountMonsterAlive() < (cFishApp::m_spMonsterManager->GetMaxMonsterAlive()/SMALLEST_MOMSTER_AMOUNT_DIVISION) )
		{
			//l_iNumMonster = cFishApp::m_spMonsterManager->m_iMaxMonsterAlive/SMALLEST_MOMSTER_AMOUNT_DIVISION;
			TimeGapToGenerateMonster.fRestTime = 0.f;
		}
		//
		if( l_iNumMonster )
		{
			float	l_fMultiFishGroupProbability = frand(0,1);
			if( l_fMultiFishGroupProbability <= fFishGroupProbability)
			{
				cFishApp::m_spMonsterManager->FishGroupGo();
				return;
			}
			
			for(int i=0;i<l_iNumMonster;++i )
			{
				int	l_iIndex = GetRandomFishID();
				sFishShowProbaility::eFishGoType l_eFishGoType = FishShowProbabilityList[l_iIndex].GetFishGoType();
				if( l_eFishGoType == sFishShowProbaility::eFGT_GROUP )
				{//fish group
					int	l_iFishCount = FishShowProbabilityList[l_iIndex].iFishGroupCount.Rand();
					if( l_iNumMonster > l_iFishCount )
					{
						l_iNumMonster -= cFishApp::m_spMonsterManager->FishGroupGo(l_iIndex,l_iFishCount)+1;
					}
					else
					{
						break;
					}
				}
				else
				if( l_eFishGoType == sFishShowProbaility::eFGT_GROUP_MPDI )
				{
					cMonster*l_pMonster = cFishApp::m_spMonsterManager->GetObject(l_iIndex);
					if( l_pMonster )
					{
						int	l_iNumOut = cFishApp::m_spMonsterManager->FishGroupGo( (WCHAR*)l_pMonster->GetName(), l_iNumMonster );
						if( l_iNumOut != 0 )
						{
							l_iNumMonster -= (l_iNumOut+1);
						}
						else
						{
							break;
						}
					}
				}
				else
				{
					MonsterGo( l_iIndex );
				}
			}
		}
	}
}


int		sFishShowProbaility::GetRandomFishID()
{
	assert( iTotalProbability>0 );
	//
	int	l_iValue = rand() % iTotalProbability;
	int	l_iSize = (int)FishShowProbabilityList.size();
	int	l_CurrentValue = 0;
	for(int i=0;i<l_iSize;++i)
	{
		int l_nShowProbability = FishShowProbabilityList[i].iProbaility;
		l_CurrentValue += l_nShowProbability;
		if( l_nShowProbability!=0 && l_CurrentValue >= l_iValue )
		{
//			if( i>=17 )
//			{
//				int a=0;
//			}
			return i;
		}
	}
	return -1;
}

void	sFishShowProbaility::Init()
{
	TimeGapToGenerateMonster.SetTargetTime( fTimeToGenerateMonsters );
	bPauseGenerateMonster = false;
	if( cFishApp::m_sbIsUsingMasterLeeProbability )
		bPauseGenerateMonster = true;
}

void	sFishShowProbaility::Update(float e_fElpaseTime)
{
	sFishShowProbaility::sFishOutData l_FishOutData = PlayerCountWithFish[cFishApp::m_spControlPanel->m_iPlayerCount];
	iNumMonsterOutAtSameTime = l_FishOutData.iOutFishCount.Rand();
	fTimeToGenerateMonsters = l_FishOutData.fTimeCircle.Rand();
	if( !cFishApp::m_sbSceneChange )
	{
		MonsterGenerate( e_fElpaseTime );
	}
}