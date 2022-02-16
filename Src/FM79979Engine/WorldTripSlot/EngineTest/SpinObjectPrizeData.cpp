#include "stdafx.h"
#include "SpinObjectPrizeData.h"
#include "SlotGamePhase.h"
#include "SlotRule.h"

//<SpinObjectData Name="1" >
//	<Prize Count="2" Odds="1" />
//	<Prize Count="3" Odds="2" />
//	<Prize Count="4" Odds="3" />
//	<Prize Count="5" Odds="4" />
//</SpinObjectData>
cSpinObjectPrizeData::cSpinObjectPrizeData(TiXmlElement*e_pElement)
{
	m_iProbability = 100;
	assert(!wcscmp(e_pElement->Value(),L"SpinObjectData"));
	const WCHAR*l_strName = e_pElement->Attribute(L"Name");
	const WCHAR*l_strProbability = e_pElement->Attribute(L"Probability");
	if( l_strProbability )
	{
		m_iProbability = _wtoi(l_strProbability);
	}
	this->SetName(l_strName);
	TiXmlElement*l_pElement = e_pElement->FirstChildElement();
	while(l_pElement)
	{
		const WCHAR*l_strOdds = l_pElement->Attribute(L"Odds");
		const WCHAR*l_strCount = l_pElement->Attribute(L"Count");
		int	l_iCount = _wtoi(l_strCount);
#ifdef _DEBUG
		for( size_t i=0;i<m_CountAndOddsList.size();++i )
		{
			if( l_iCount == m_CountAndOddsList[i].cCount )
				assert(0&&"Prize Table Count exists!!");
		}
#endif
		sCountAndOdds	l_CountAndOdds(l_iCount,_wtoi(l_strOdds));
		m_CountAndOddsList.push_back(l_CountAndOdds);
		l_pElement = l_pElement->NextSiblingElement();
	}
}

int	cSpinObjectPrizeData::GetOddsByCount(int e_iCount)
{
	int	l_iSize = (int)m_CountAndOddsList.size();
	for( int i=0;i<l_iSize;++i )
	{
		if(m_CountAndOddsList[i].cCount == e_iCount)
			return m_CountAndOddsList[i].iOdds;
	}
	return 0;
}

int	cSpinObjectPrizeData::GetMaxOdds()
{
	int	l_iSize = (int)m_CountAndOddsList.size();
	int	l_iOdds = 0;
	for( int i=0;i<l_iSize;++i )
	{
		if(m_CountAndOddsList[i].iOdds > l_iOdds)
			l_iOdds = m_CountAndOddsList[i].iOdds;
	}
	return l_iOdds;
}

int	cSpinObjectPrizeData::GetMinOdds()
{
	int	l_iSize = (int)m_CountAndOddsList.size();
	int	l_iOdds = INT_MAX;
	for( int i=0;i<l_iSize;++i )
	{
		if(m_CountAndOddsList[i].iOdds < l_iOdds)
			l_iOdds = m_CountAndOddsList[i].iOdds;
	}
	return l_iOdds;
}

void	cSpinObjectPrizeDataManager::CalculateTotalProbabilityAmount()
{
	m_iTotalProbabilityAmount = 0;
	int	l_iCount = Count();
	m_iMaxScore = -1;
	m_iMinScore = INT_MAX;
	for( int i=0;i<l_iCount;++i )
	{
		m_iTotalProbabilityAmount += this->m_ObjectList[i]->m_iProbability;
		int	l_iOdds = this->m_ObjectList[i]->GetMaxOdds();
		if( m_iMaxScore<l_iOdds )
		{
			m_iMaxScore = l_iOdds;
			m_iMaxScoreSpinObjectIndex = i;
		}
		l_iOdds = this->m_ObjectList[i]->GetMinOdds();
		if( m_iMinScore>l_iOdds&& l_iOdds !=0 )
		{
			m_iMinScore = l_iOdds;
			m_iMinScoreSpinObjectIndex = i;
		}
	}
}

cSpinObjectPrizeDataManager::cSpinObjectPrizeDataManager()
{
	m_iMinScore = 0;
	m_iMaxScore = 0;
	m_iMinScoreSpinObjectIndex = -1;
	m_iMaxScoreSpinObjectIndex = -1;
	m_iTotalProbabilityAmount = 0;
}


int	cSpinObjectPrizeDataManager::GetSpinObjectIndexByProbability()
{
	int	l_iProbability = rand()%m_iTotalProbabilityAmount;
	return GetSpinObjectIndexByProbability(l_iProbability);
}

int		cSpinObjectPrizeDataManager::GetSpinObjectIndexByProbability(int e_iProbability)
{
	int	l_iCount = Count();
	int	l_iProbability = 0;
	for( int i=0;i<l_iCount;++i )
	{
		l_iProbability += this->m_ObjectList[i]->m_iProbability;
		if( l_iProbability >= e_iProbability )
		{
			return i;
		}
	}
	return 0;
}