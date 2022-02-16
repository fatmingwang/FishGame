#ifndef _FREE_PICKAXE_GAME_H_
#define _FREE_PICKAXE_GAME_H_

#include "BaseExtraGame.h"

class	cFreePickaxeGame:public cBaseExtraGame
{
	//so we know it's first time for free game or the last time for free game play
	//
	cMPDIList*	m_pMPDIList;
	cMPDI*		m_pPickaxeMPDI;
	int			FreeGameCountToAdd();
public:
	DEFINE_TYPE_INFO();
	cFreePickaxeGame(const WCHAR*e_strConditionName,int e_iConditionCount);
	~cFreePickaxeGame();
	virtual	bool	InternalConditionCheck();
	virtual	void	Init();
	virtual	void	LoadResource();
	virtual	void	Destroy(){}
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render();
};

#endif