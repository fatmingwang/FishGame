





#ifndef _cNegativeBankMode_H_
#define _cNegativeBankMode_H_

#include "BankMode.h"

//<PossitiveBankMode BulletDuration="6">







//<NegativeMode BigPrizeToOut="20" m_fFetchPercentageToRevenueFromBullet="0.3" BulletDuration="6"/>
class	cNegativeBankMode:public cBankMode
{
	//while revenue is positive fetch control's setup probability to balance revenue
	UT::sMinMaxData<float>			m_fFetchPercentageToRevenueFromBullet;
	//give a big money while store the revenue is processing
	int64			m_i64StoredBigMoneyToOut;
	//bool			m_bBigMoneyOut;
	UT::sMinMaxData<int>		m_iBigPrizeToOut;
	int							m_iThisRoundBigPrizeTarget;
	bool			IsBigMoneyOkOkay(int e_iMoney,int e_iPayRate);
	virtual void	ResetDataInit();
public:
	//cNegativeBankMode(float e_pTargetProbability,sBulletFireAverage*e_pBulletFireAverage,float e_fFetchPercentageToRevenueFromBullet);
	cNegativeBankMode(TiXmlElement*e_pTiXmlElement);
	virtual ~cNegativeBankMode();
	//store income(from control panel's probability) to keep revenue as positive
	virtual	void	AddRevenue(int64 e_i64Value);
	virtual	bool	IsModeSatisfied();
	bool			IsMoneyOutOk(int e_iMoney,int e_iPayRate);
	virtual	void	DebugRender( int e_iPosX,int e_iPosY );
};

#endif