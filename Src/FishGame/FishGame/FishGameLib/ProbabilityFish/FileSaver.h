#pragma once

class	cFileSaver
{
	int64*								m_pi64LatestInMoney;
	int64*								m_pi64LatestOutMoney;
	int64*								m_pi64LatestRevenue;
	int64*								m_pi64LatestExchangeIn;
	int64*								m_pi64LatestExchangeOut;
	int64*								m_pi64LatestCoinIn;
	int64*								m_pi64LatestCoinOut;
	//
	UT::sTimeCounter		m_WritePlayerMoneyTC;
	void					OpenPlayerFile(const char*e_strPlayerFileName);
	void					WrtiePlayerFile(float e_fElpaseTime);
	void					WriteStaticFile();
	//6 for total,6 for latest else for player money.
	cBinaryFile*			m_pStaticsLogFile;
	int64					*m_pi64PlayerRevenueLog;
	//need to write player data every 1 second,so save player data here,not store in controller panel data.
	cBinaryFile*			m_pPlayerMoneyLogFile;
public:
	cFileSaver(const char*e_strPlayerFileName,const char*e_strStaticFileName);
	virtual ~cFileSaver();
	void					Update(float e_fElpaseTime);
	void					RevenueMinus( int e_iMoney, int e_iPlayerID);
	void					RevenueAdd( int e_iMoney, int e_iPlayerID );
	void					ClearLatestRecord(bool e_bClearPlayerMoney);
};
