#ifndef COINSTACK_H
#define COINSTACK_H

class CoinStack : public NamedTypedObject
{
public:

	int				  m_Money;
	int				  m_FishID;

	bool			  m_bSetData;
	bool			  m_bUseGrayColor;
	bool			  m_bCalculatorPos;	

	float			  m_PosRot;
	float			  m_MoveRemainTm;

	cMPDI*			  m_pFishCard;

	Vector3			  m_TargetPos;
	Vector3			  m_DrawMoneyPos;

	Frame			  m_ShowFrame;

	cNumeralImage*	  m_pStackScoreNumerial;

	CoinStack();
	virtual ~CoinStack(){};

	void Init(){};
	void Render();
	void Destroy(){};
	void Update( float ElapseTime );
	void SetCoinStackData( cMPDI* pFishCardImage, cNumeralImage* pNumerialImage, float MoveRemainTime );
};

#endif