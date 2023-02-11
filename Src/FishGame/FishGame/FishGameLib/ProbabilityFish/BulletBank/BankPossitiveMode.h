





#ifndef _cPossitiveBankMode_H_
#define _cPossitiveBankMode_H_

#include "BankMode.h"








class	cPossitiveBankMode:public cBankMode
{
private:
	sFakeRevenue	m_FakeRevenue;
	virtual void	ResetDataInit();
public:
	cPossitiveBankMode(float e_pTargetProbability);
	cPossitiveBankMode(TiXmlElement*e_pTiXmlElement);
	virtual ~cPossitiveBankMode();
	//switch condition:fake revenue is zero and revenue is negative
	virtual	bool	IsModeSatisfied();
	virtual	void	MinusRevenue(int64 e_i64Value);
	bool			IsMoneyOutOk(int e_iMoney,int e_iPayRate);
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	DebugRender( int e_iPosX,int e_iPosY );
	virtual	bool	IsFakeMoneyEnoughToPay(int e_iMoney);
};

#endif