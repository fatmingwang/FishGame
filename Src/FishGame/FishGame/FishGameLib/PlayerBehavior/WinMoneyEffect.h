#ifndef WINMONEY_EFFECT
#define	WINMONEY_EFFECT

#include "WinMoneyEffectBase.h"
#include "SingleCoinEffectAnimation.h"

enum eWinMoneyEffectCoinCount
{
	eWinMoneyEffectCoinCount_Triple = 3,
	eWinMoneyEffectCoinCount_Penta  = 5,
	eWinMoneyEffectCoinCount_Max    = 6
};

class cWinMoneyEffectManager : public cNamedTypedObjectVector< cWinMoneyEffectBase >
{
	int	m_TotalEffectAmount;
	int	m_iNumWorkingWinMoneyEffect;

	cWinMoneyEffectBase**											m_ppWorkingWinMoneyEffect;
	cNamedTypedObjectVector< cNamedTypedObjectVector< cWinMoneyEffectBase > >	m_AllWinMoneyEffectList;

public:

	cWinMoneyEffectManager();
	virtual ~cWinMoneyEffectManager();

	cWinMoneyEffectBase* RequirePrizeEffect( const wchar_t* EffectTypeName, cMatrix44 AniMatrix, int Money, cPlayerData* pPlayer, bool e_bForceRequire = false );
	cWinMoneyEffectBase* RequireWinMoneyEffectBase(const wchar_t* EffectTypeName, cMatrix44& AniMatrix, int Money, cPlayerData* pPlayer, eWinMoneyEffectCoinCount MoneyCount, bool e_bForceRequire = false );

	void Init();
	void Render();
	void Update( float ElapseTime );
	void LoadWinMoneyEffectXMLData();
};

#endif