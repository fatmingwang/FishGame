#ifndef _PRIZE_EFFECT_H_
#define _PRIZE_EFFECT_H_

#include "WinMoneyEffect.h"

class PrizeEffect : public cWinMoneyEffectBase
{
public:

	PrizeEffect(){};
	~PrizeEffect(){};

	void Render();
	void InternalInit(){};
	void Update( float ElapseTime );
	void ModifyNumMPDIData( int PlayerID );
	void LoadWinMoneyEffectMPDIData( WinMoneyEffectMPDIData& MPDIData );

	NamedTypedObject*	Clone() { return 0; }
};

#endif