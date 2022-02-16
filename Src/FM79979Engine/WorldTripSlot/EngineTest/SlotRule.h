#ifndef _SLOT_RULE_H_
#define _SLOT_RULE_H_

#include "SpinObjectPrizeData.h"

struct	sLineRule
{
	char	cOneLineCount;
	char*	pcLineObjectIndex;
	sLineRule(TiXmlElement*e_pElement);
	~sLineRule();
};

//======================================
//
//resulttable's spin object index was depend on m_SpinObjectDataManager's data.
//
//======================================

//<BetMoneyRule BetPerLine="1,5,10"/>
class	cSlotRuleData:public cNodeISAX
{
	virtual	bool	MyParse(TiXmlElement*e_pRoot);
	void	ProcessSpineObjectPrizeTable(TiXmlElement*e_pElement);
	void	ProcessLineRule(TiXmlElement*e_pElement);
	void	ProcessPerLineBetMoneyRule(TiXmlElement*e_pElement);
public:
	std::wstring								m_strFreeGameImageName;
	std::wstring								m_strSpecilaSpinObjectName;
	//all win money data
	cSpinObjectPrizeDataManager						m_SpinObjectDataManager;
	std::vector<sLineRule*>						m_LineRuleList;
	std::vector<int>							m_PerLineBetMoneyRule;
	//x equal m_SlotLineDataList.size(),y equal the count show on the screen
	//row and column
	POINT										m_TableCount;
	int											GetTableAmount(){ return m_TableCount.x*m_TableCount.y; }
	Vector4										m_vShowRect;
	cSlotRuleData();
	~cSlotRuleData();
	void	Destroy();
	//
	POINT										SpinObjectIndexToRowColumn(int e_iIndex);
	int											GetMinScore(int e_iMultiplier);
	int											GetMaxScore(int e_iMultiplier);
	//find out spin object who prize higest and multiple max lines
	int											GetMaxWinMoney(int e_iMultiplier);
	int											GetMinScoreSpinObjectIndex();
	int											GetMaxScoreSpinObjectIndex();
	int											GetFreeSpinObjectIndex();
	int											GetWildObjectIndex();
	int											GetRandomSpinObject(bool e_bAvoidFree,bool e_bAvoidJP);
	//will be set at FreeGameForPickGemsGame
	int											m_iNumFreeGameToAchevied;
};

#endif