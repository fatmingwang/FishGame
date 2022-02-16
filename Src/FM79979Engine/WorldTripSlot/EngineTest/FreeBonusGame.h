#ifndef _BONUS_GAME_H_
#define _BONUS_GAME_H_

#include "BaseExtraGame.h"

class	cPickRockBonusGame;

class	cPickupObjectToWinMoney:public cImageButton
{
	friend class		cPickRockBonusGame;
	int					m_iMoney;
	cMPDI*				m_pNoWinMoneyMPDI;
	virtual bool		InternalCollide(int e_iPosX,int e_iPosY);
public:
	cPickupObjectToWinMoney(cPuzzleImageUnit*e_pImage,Vector3 e_vPos,cMPDI*e_pNoWinMoneyMPDI,int e_iMoney);
	~cPickupObjectToWinMoney();
	bool				IsButtonSatisfiedCondition(){ return cImageButton::IsSatisfiedCondition(); }
	virtual bool		IsSatisfiedCondition();
	virtual	void		Update(float e_fElpaseTime);
	virtual	void	Render();
};

class	cPickRockBonusGame:public cBaseExtraGame
{
	//
	UT::sTimeCounter						m_TimeForNoActionToLeave;
	cMPDI*									m_pEnteringMPDI;
	cMPDI*									m_pLeavingMPDI1;
	cMPDI*									m_pLeavingMPDI2;
	//
	cMPDIList*								m_pMPDIList;
	//
	bool									m_bPickRockToWinMoneyDone;
	int										m_iRockShowRowIndex;
	//
	cDoAllMouseBehaviorList<cPickupObjectToWinMoney>	m_ImageButtonList;
	std::wstring							m_strImageName;
	//set result for play
	void									SetupResult();
	//
	//virtual	bool	InternalConditionCheck();
public:
	DEFINE_TYPE_INFO();
	cPickRockBonusGame(const WCHAR*e_strConditionName,int e_iConditionCount,int e_iRockShowRowIndex,WCHAR*e_strShowImage);
	~cPickRockBonusGame();
	virtual	void	LoadResource();
	virtual	void	Destroy();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render();
    virtual void	MouseMove(int e_iPosX,int e_iPosY);
    virtual void	MouseDown(int e_iPosX,int e_iPosY);
    virtual void    MouseUp(int e_iPosX,int e_iPosY);
};

#endif