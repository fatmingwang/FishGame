#ifndef _FREE_WHOLE_JEWELLS_TARGET_GAME_H_
#define _FREE_WHOLE_JEWELLS_TARGET_GAME_H_

#include "BaseExtraGame.h"
#include "JPEffect.h"
class	cWinmoneyLines;
class	cFreeJPGame:public cBaseExtraGame
{
	//so we know it's first time for free game or the last time for free game play
	//
	cJPEffect*			m_pJPEffect;
	cMPDI*				m_pConstantUI;
	cWinmoneyLines*		m_pWinmoneyLines;
	//
	int					m_iJPMoney;
	int					m_iFinalJPWinMoney;
	int					m_iStartJPWinMoney;
public:
	DEFINE_TYPE_INFO();
	cFreeJPGame(const WCHAR*e_strConditionName,int e_iConditionCount,int e_iJPMoney,cMPDI*e_pConstantUI);
	~cFreeJPGame();
	virtual	bool	InternalConditionCheck();
	virtual	void	Init();
	virtual	void	LoadResource();
	virtual	void	Destroy();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render();
};


#endif