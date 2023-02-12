#pragma once
//
#include "FakeRevenue.h"
//
class	cBankRunner;
//
//<cBankMode BulletDuration="6" />







class	cBankMode:public NamedTypedObject
{
private:
	friend class		cBankRunner;
	//
protected:
	int					m_iNumRevenueAddAndNoRevenueMinus;//how many shot
	//
	int64				m_i64TotalBulletValue;
	int64				m_i64Aeverage;
	int					m_iBulletCount;
	void				BulletDataUpdate(int64 e_iValue);
	//
	int64	m_i64CurrentIn;
	int64	m_i64CurrentOut;
	int64	m_i64Revenue;
	virtual	float	GetProbability();
	virtual	float	GetNewProbability(int e_iMoney);
	//while condition is satisifed,call ModeAssign and auto to call this.
	virtual void	ResetDataInit() = 0;
	//from control panel
	float			m_TargetProbability;
	//
public:
	cBankMode( float e_pTargetProbability);
	cBankMode( TiXmlElement *e_pTiXmlElement );
	virtual ~cBankMode();
	//
	virtual	bool	IsModeSatisfied() = 0;
	//positive to negative and invert
	void			ModeAssign(cBankMode*e_pBankMode);
	virtual	void	AddRevenue(int64 e_i64Value);
	virtual	void	MinusRevenue(int64 e_i64Value);
	virtual	bool	IsMoneyOutOk(int e_iMoney,int e_iPayRate);
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	DebugRender( int e_iPosX,int e_iPosY ) = 0;
	virtual	bool	IsFakeMoneyEnoughToPay(int e_iMoney){ return false; }
};
