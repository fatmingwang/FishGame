#pragma once

#include "MiniGameBase.h"

class	cMiniGameActiverFullScreenBomb:public cMiniGameActiverBase
{
public:
	DEFINE_TYPE_INFO();
	cMiniGameActiverFullScreenBomb(TiXmlElement*e_pXMLElement);
	cMiniGameActiverFullScreenBomb(cMiniGameActiverFullScreenBomb*e_pMiniGameActiverFullScreenBomb);
	virtual ~cMiniGameActiverFullScreenBomb();
	virtual	void			SetupShowData(int e_iPayRate,int e_iMoney,cPlayerBehaviorBase*e_pPlayer,cMonster*e_pMonster)override;
};
