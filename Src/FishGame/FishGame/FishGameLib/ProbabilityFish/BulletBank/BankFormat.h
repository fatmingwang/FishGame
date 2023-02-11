





#ifndef _BANK_FORMAT_H_
#define _BANK_FORMAT_H_

#include "BankMode.h"
//<Root>
//	<SubRepositoryBank Range="100,70">
//		<PossitiveBankMode >
//			<FakeRevenue FakeRevenue="2000" FakeRevenueCircle="30,1000" HappyRevenue="100,10000" FakeRevenueFetchProbability="0.3" FakeRevenueGiveDivision="4" HappyTimeScale="1"/>
//		</PossitiveBankMode>
//		<NegativeMode BigPrizeToOut="20" m_fFetchPercentageToRevenueFromBullet="0.5" />
//	</SubRepositoryBank>
//	<SubRepositoryBank Range="69,31">
//		<PossitiveBankMode >
//			<FakeRevenue FakeRevenue="500" FakeRevenueCircle="30,1000" HappyRevenue="100,1000" FakeRevenueFetchProbability="0.3" FakeRevenueGiveDivision="4" HappyTimeScale="1"/>
//		</PossitiveBankMode>
//		<NegativeMode BigPrizeToOut="20" m_fFetchPercentageToRevenueFromBullet="0.2" />
//	</SubRepositoryBank>
//	<SubRepositoryBank Range="30,0">
//		<PossitiveBankMode >
//			<FakeRevenue FakeRevenue="300" FakeRevenueCircle="30,1000" HappyRevenue="10,100" FakeRevenueFetchProbability="0.3" FakeRevenueGiveDivision="4" HappyTimeScale="1"/>
//		</PossitiveBankMode>
//		<NegativeMode BigPrizeToOut="20" m_fFetchPercentageToRevenueFromBullet="0.1" />
//	</SubRepositoryBank>
//</Root>








class	cBankRunnerManager:public cNamedTypedObjectVector<cBankRunner>,public cNodeISAX
{
private:
	int		m_iCurrentWorkingIndex;
	//
	void	ProcessePositiveMode(TiXmlElement*e_pTiXmlElement);
	void	ProcesseNegativeMode(TiXmlElement*e_pTiXmlElement);
	void	ProcessBankFormat(TiXmlElement*e_pTiXmlElement);
	virtual	bool	MyParse(TiXmlElement*e_pRoot);
	//
public:
	int		GetIndexByPerenctage(float e_fPercentage,int e_iPayRate,int e_iPlayerID);
	void	Update(float e_fElpaseTime);
	void	DebugRender( int e_nPosX, int e_nPosY );
};

#endif