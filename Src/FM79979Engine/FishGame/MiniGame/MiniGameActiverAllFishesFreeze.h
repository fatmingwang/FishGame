#ifndef _MINI_GAME_ACTIVER_ALL_FISH_FREEZE_H_
#define _MINI_GAME_ACTIVER_ALL_FISH_FREEZE_H_

#include "MiniGameBase.h"

class	cMiniGameActiverAllFishesFreeze:public cMiniGameActiverBase
{
	UT::sTimeCounter	m_FreezeTC;
public:
	DEFINE_TYPE_INFO();
	cMiniGameActiverAllFishesFreeze(TiXmlElement*e_pXMLElement);
	cMiniGameActiverAllFishesFreeze(cMiniGameActiverAllFishesFreeze*e_pMiniGameActiverAllFishesFreeze);
	virtual ~cMiniGameActiverAllFishesFreeze();
	virtual	void			Init();
	virtual	void			Update(float e_fElpaseTime);
};

#endif