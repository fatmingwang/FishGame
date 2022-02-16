#ifndef WINMONEY_EFFECT_BASE
#define	WINMONEY_EFFECT_BASE

struct WinMoneyEffectMPDIData
{
	const WCHAR* MPDIListName;
	const WCHAR* EffectMPDIName;
	const WCHAR* NumerialMPDIListName;
	const WCHAR* NumerialMPDIName;
};

class cPlayerData;

class cWinMoneyEffectBase : public NamedTypedObject
{
public:

	int	          m_iMoney;

	bool          m_bIsDone;
	bool          m_bIsUsing;

	cMPDI*		  m_pNumMPDI;
	cMPDI*		  m_pCoinMPDI;

	Frame		  m_Frame;
	cMatrix44	  m_PlayerMatrix;

	cPlayerData*  m_pPlayer;

	cWinMoneyEffectBase();
	virtual ~cWinMoneyEffectBase();

	void CommonInitData();

	virtual	bool IsDone() { return m_bIsDone; }
	virtual bool IsUsing() { return m_bIsUsing; }

	virtual	void	Init();
	virtual	void	Destroy(){};
	virtual void	SetNumMPDIPos( Vector3 NewPos );
	virtual void	ModifyNumMPDIData( int PlayerID ){};
	virtual void	SetMoneyMPDICoinCount( int MoneyMPDICoinCount ){};
	virtual	void	SetData( cMatrix44 AniMatrix, int Money, cPlayerData* pPlayer );
	virtual	void	Update(float e_fElpaseTime) = 0;
	virtual	NamedTypedObject*	Clone() = 0;
	virtual void                InternalInit() = 0;
	virtual void                LoadWinMoneyEffectMPDIData( WinMoneyEffectMPDIData& MPDIData ) = 0;
};

#endif