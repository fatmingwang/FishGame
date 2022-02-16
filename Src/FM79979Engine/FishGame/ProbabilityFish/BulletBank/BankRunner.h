





#ifndef _cBankRunner_H_
#define _cBankRunner_H_

class	cBankMode;
class	cPossitiveBankMode;
class	cNegativeBankMode;
class	cBankRunnerManager;







class	cBankRunner:public NamedTypedObject
{
private:
	friend class	cBankRunnerManager;
	friend class	cPossitiveBankMode;
	friend class	cNegativeBankMode;
	friend class	cBankMode;
	Vector2				m_vTargetPercentValueRange;		//small 0~30,Midum 31~70,big71~100
	int64				m_i64LeastMoney;				//
	cBankMode			*m_pCurrentMode;
	cPossitiveBankMode	*m_pPossitiveBankMode;
	cNegativeBankMode	*m_pNegativeBankMode;
	//
public:
	cBankRunner(TiXmlElement*e_pXmlElement);
	virtual ~cBankRunner();
	//
	void	Update(float e_fElpaseTime);
	void	DebugRender( int e_iPosX,int e_iPosY );
	void	AddRevenue(int64 e_i64Value);
	void	MinusRevenue(int64 e_i64Value);
	bool	IsMoneyOutOk(int e_iMoney,int e_iPayRate);
	bool	IsFakeMoneyEnoughToPay(int e_iMoney);
	Vector2	GetTargetPercentValueRange(){return m_vTargetPercentValueRange;}
	bool	IsLeastMoneyOk(int64 e_i64Money);
};

#endif