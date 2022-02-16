#ifndef _SLOT_PROBABILITY_DATA_H_
#define _SLOT_PROBABILITY_DATA_H_

#include "PlayerData.h"
#include "SlotBetResult.h"
//============================
//spine object's odds
//============================
class	cSpinObjectPrizeData:public NamedTypedObject
{
	struct	sCountAndOdds
	{
		char	cCount;
		int		iOdds;
		sCountAndOdds(char	e_cCount,int e_iOdds)	{cCount = e_cCount;	iOdds = e_iOdds;}
	};
	std::vector<sCountAndOdds>	m_CountAndOddsList;
public:
	cSpinObjectPrizeData(TiXmlElement*e_pElement);
	int		GetOddsByCount(int e_iCount);
	int		m_iProbability;
	int		GetMaxOdds();
	int		GetMinOdds();
};

//=====================
//spin object's index to be a line.
//=====================

class	cSpinObjectPrizeDataManager:public cNamedTypedObjectVector<cSpinObjectPrizeData>
{
	int		m_iMinScore;
	int		m_iMaxScore;
	int		m_iMinScoreSpinObjectIndex;
	int		m_iMaxScoreSpinObjectIndex;
public:
	cSpinObjectPrizeDataManager();
	~cSpinObjectPrizeDataManager(){}
	int		m_iTotalProbabilityAmount;
	int		GetSpinObjectIndexByProbability(int e_iProbability);
	int		GetSpinObjectIndexByProbability();
	void	CalculateTotalProbabilityAmount();
	int		GetMaxScore(){ return m_iMaxScore; }
	int		GetMinScore(){ return m_iMinScore; }
	int		GetMinScoreSpinObjectIndex(){return m_iMinScoreSpinObjectIndex;}
	int		GetMaxScoreSpinObjectIndex(){return m_iMaxScoreSpinObjectIndex;}
};

#endif