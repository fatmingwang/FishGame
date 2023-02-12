#pragma once

#include "MiniGameBase.h"

class	cMiniGameActiverPowerUp:public cMiniGameActiverBase
{
	int					m_iMultiplier;
	UT::sTimeCounter	m_PowerUpTC;
public:
	DEFINE_TYPE_INFO();
	cMiniGameActiverPowerUp(TiXmlElement*e_pXMLElement);
	cMiniGameActiverPowerUp(cMiniGameActiverPowerUp*e_pMiniGameActiverPowerUp);
	virtual ~cMiniGameActiverPowerUp();
	virtual	void			Init();
	virtual	void			Update(float e_fElpaseTime);
	virtual	void			Render();
};