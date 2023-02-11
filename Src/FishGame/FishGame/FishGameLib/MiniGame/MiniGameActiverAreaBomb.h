#ifndef _MINI_GAME_ACTIVER_AREA_BOMB_H_
#define _MINI_GAME_ACTIVER_AREA_BOMB_H_

#include "MiniGameBase.h"

class	cMiniGameActiverAreaBomb:public cMiniGameActiverBase
{
	float	m_fExplosionRadius;
public:
	DEFINE_TYPE_INFO();
	cMiniGameActiverAreaBomb(TiXmlElement*e_pXMLElement);
	cMiniGameActiverAreaBomb(cMiniGameActiverAreaBomb*e_pMiniGameActiverAreaBomb);
	virtual ~cMiniGameActiverAreaBomb();
	virtual	void			SetupShowData(int e_iPayRate,int e_iMoney,cPlayerBehaviorBase*e_pPlayer,cMonster*e_pMonster);
};

#endif