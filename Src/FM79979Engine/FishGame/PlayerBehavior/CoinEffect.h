#ifndef _COIN_EFFECT_H_
#define _COIN_EFFECT_H_

#include "WinMoneyEffect.h"
#include "SingleCoinEffectAnimation.h"

#define MAX_COINEFFECT_COUNT 5

class cCoinEffect : public cWinMoneyEffectBase
{
public:

	SingleCoinEffectAnimation   m_CoinEffect[ MAX_COINEFFECT_COUNT ];

	cCoinEffect(){};
	virtual ~cCoinEffect(){};

	void Render();
	void InternalInit(){};
	void Update( float ElapseTime );
	void ModifyNumMPDIData( int PlayerID );
	void SetMoneyMPDICoinCount( int MoneyMPDICoinCount );
	void LoadWinMoneyEffectMPDIData( WinMoneyEffectMPDIData& MPDIData );

	NamedTypedObject*	Clone() { return 0; }
};

#endif