#pragma once

#include "MiniGameBase.h"

class	cMiniGameActiverAllFishesFreeze:public cMiniGameActiverBase
{
	UT::sTimeCounter	m_FreezeTC;
public:
	DEFINE_TYPE_INFO();
	cMiniGameActiverAllFishesFreeze(TiXmlElement*e_pXMLElement);
	cMiniGameActiverAllFishesFreeze(cMiniGameActiverAllFishesFreeze*e_pMiniGameActiverAllFishesFreeze);
	virtual ~cMiniGameActiverAllFishesFreeze();
	virtual	void			Init()override;
	virtual	void			Update(float e_fElpaseTime)override;
};
