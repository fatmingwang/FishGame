#ifndef _SLOT_GOOD_TABLES_H_
#define _SLOT_GOOD_TABLES_H_

#include "SlotTimeStamp.h"
#include "SlotBetTableResult.h"

//<GoodTablesList>
	//<GoodTable MinMoney="500"  MaxMoney="1000"  ConditionScore="500" Type="Normal">
	//<GoodTable MinMoney="900"  MaxMoney="1500"  ConditionScore="900" Type="Combo" ComboCount="3" >
	//<GoodTable MinMoney="900"  MaxMoney="1500"  ConditionScore="1200" Type="FreeGame" >
	//<GoodTable MinMoney="900"  MaxMoney="1500"  ConditionScore="1200" Type="JPNetWork" >
	//<GoodTable MinMoney="900"  MaxMoney="1500"  ConditionScore="1200" Type="JPLocal" >
//</GoodTablesList>

enum	eGoodTableType
{
	eGTT_RANDOM,		//if random min score and max score will ignore.
	eGTT_NORMAL,		//go as normal
	eGTT_FREE_GAME,		//go as free game
	eGTT_JP_NETWORK,	//go as JP network
	eGTT_JP_LOCAL,		//go as JP local
	eGTT_MAX,
};

eGoodTableType	GeteGoodTableType(const WCHAR*e_strName);
//============================
//if no win money iComboCount is 0
//
//============================
struct	sGoodTable
{
	int					iConditionScore;
	int					iMinMoney;
	int					iMaxMoney;
	eGoodTableType	GoodTableCategory;
	int					iComboCount;
	sGoodTable();
	~sGoodTable(){}
};

//ensure data is put from small to big
struct	sGoodTableList
{
	sGoodTableList();
	~sGoodTableList(){}
	sGoodTable				m_JPWinMoneyTable[eJPT_MAX];
	std::vector<sGoodTable>	m_WinMoneyTableList;
	bool						IsDataVaild();
	sGoodTable				*GetWinMoneyTable(int e_iScore);
	sGoodTable				*GetWinMoneyTableByMoneyNeed(int e_iMaxMoney);
	//JP data is not in file I have to add it in the code
	sGoodTable*				GetJPTable(eJPType e_eJPType);
};

class	cSlotTemperatureScore;
class	cSlotProbability;
class	cSlotGoodTables:public cNodeISAX
{
	friend class				cSlotProbability;
//
	void						ProcessSlotTemperatureScore(TiXmlElement*e_pTiXmlElement);
	void						ProcessTimeStamp(TiXmlElement*e_pTiXmlElement);
	void						ProcessScroeMultiplier(TiXmlElement*e_pTiXmlElement);
	void						ProcessGoodTablesList(TiXmlElement*e_pTiXmlElement);
	void						MoneyChangeCondition(TiXmlElement*e_pTiXmlElement);
	void						HappyAndUnHappyRate(TiXmlElement*e_pTiXmlElement);
	virtual	bool				MyParse(TiXmlElement*e_pRoot);
	//
	sGoodTable*					IsJPOpen();
	//
	cSlotTemperatureScore*		m_pSlotTemperatureScore;
	sGoodTableList				m_WinMoneyTableList;
	//set by sTimeStampWithScore list
	int*						m_piCurrentScore;
	sGoodTable					m_CurrentWinMoneyTable;
	std::vector<sGoodTable>		m_StatusScoreList;
	//let player feel happy rate
	int							m_iHappyMonneyRate;
	float						m_fHappyMonneyMultiple;
	//lost too money or win too much money
	int							m_iMoneyStrangleRate;
	//because different rule,maxbet money is different,but we only has aone table file,so do divitor to get new money of tables
	int							m_iMaxBetMoney;
	//
	float						m_fSecondFreeGameProbability;
public:
	cSlotGoodTables();
	~cSlotGoodTables();
	void						Init(char*e_strProbabilityFileName);
	void						Update(cSlotBetResult*e_pBetBehaviorResult);
	void						Render();
	void						GetGoodTable(cSlotBetResult*e_pBetBehaviorResult);
	sGoodTableList*				GetWinMoneyTableList(){return &m_WinMoneyTableList;}
	sGoodTable*					GetCurrentWinMoneyTable(){return &m_CurrentWinMoneyTable;}
	int							GetCurrentScore();
};

#endif