#pragma once

#include "MiniGameBase.h"

class	cMiniGameActiverAreaBomb:public cMiniGameActiverBase
{
	float	m_fExplosionRadius;
public:
	DEFINE_TYPE_INFO();
	cMiniGameActiverAreaBomb(TiXmlElement*e_pXMLElement);
	cMiniGameActiverAreaBomb(cMiniGameActiverAreaBomb*e_pMiniGameActiverAreaBomb);
	virtual ~cMiniGameActiverAreaBomb();
	virtual	void			SetupShowData(int e_iPayRate,int e_iMoney,cPlayerBehaviorBase*e_pPlayer,cMonster*e_pMonster)override;
};
