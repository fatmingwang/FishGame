#ifndef _SLOT_PROBABILITY_STATSTIC_DATA_H_
#define _SLOT_PROBABILITY_STATSTIC_DATA_H_

//there are 2 files one for each symbol and line result 
//another player money and probability
//GameAppName/StageName/StageName_Statstic.xml
//GameAppName/StageName/StageName_Probability.xml
//

struct sSlotBetTableResult;
class	cSpinObjectPrizeDataManager;
class	cSlotRuleData;
class	cSlotBetResult;
class	cSlotProbabilityStatsticData
{
	friend class					cSlotBetResult;

	cSlotRuleData*					m_pSlotRuleData;
	//
	cBinaryFile						m_ProbabilityFile;
	cBinaryFile						m_BetResultFile;
	std::string						m_strEventVariableManagerFileName;
	cEventVariableManager*			m_pEventVariableManager;
	//
	int64							m_i64StartIncome;
	int64							m_i64StartOut;
	std::vector<int>				m_AllSpinObjectResultShowCount;
	//
	int64*							m_pi64InMoney;
	int64*							m_pi64OutMoney;
	//
	int								m_iColumn;
	int**							m_ppiConsecutiveScore;
	int**							m_ppiConsecutiveCount;

	int*							m_piPlayedRound;
	//
	//vector for how many spinobject
	//mapd for consecutive count
	//it will lost while game is close or into control panel
	std::vector<std::map<int,int> >	m_WinMoneySpinObjectMatchCount;
	//each line happen times
	std::vector<int>				m_LinesWinMoneyCount;
	void							UpdateData(std::vector<sSlotBetTableResult*>*e_TableResultList,int e_iBetMoney,int e_iPlayerMoney);
	//
	void							SetupVariable(const char*e_strEventVariableFileName);
public:
	cSlotProbabilityStatsticData(const WCHAR*e_strStageName,cSpinObjectPrizeDataManager*e_pSpinObjectDataManager,cSlotRuleData*e_pSlotRuleData);
	~cSlotProbabilityStatsticData();
	void							WriteProbabilityData(int e_iPlayerMoney);
	void							ExtraGameWinExtraMoney(int e_iOut);
};

#endif