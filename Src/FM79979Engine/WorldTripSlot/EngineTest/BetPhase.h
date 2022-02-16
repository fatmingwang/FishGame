#ifndef _BET_PHASE_H_
#define _BET_PHASE_H_

#include "SlotRule.h"
class	cBetDataButtonManaget;
class	cGameMoneyLimitError;

class	cBetPhase:public cSimplePhase
{
	//0,1,2
	//0 for none
	//1 for Menu1
	//2 for menu2
	cMPDI*	m_pMenu;
	int		m_iReadMeProgress;
	//if money is enough player is oging to minus money at same time
	bool	ChecMoneyEncoughToBet(bool e_bMinusPlayerMoney);
	//
	friend class cBetDataButtonManaget;
	cBetDataButtonManaget*		m_pBetDataButtonManaget;
	cMPDI*						m_pConstantUI;
	void						SetupResult();
	//
	GET_SET_DEC(bool,m_bAutoSpin,IsAutoSpin,SetAutoSpin);
	//
	std::vector<int>			m_ExchangeRateVector;
	int							m_iCurrentFocusExchangeRate;
public:
	cBetPhase();
	~cBetPhase();
	virtual	void	Init();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render();
	virtual	void	DebugRender();
	virtual	void	MouseDown(int e_iX,int e_iY);
	virtual	void	MouseUp(int e_iX,int e_iY);
	virtual	void	MouseMove(int e_iX,int e_iY);
	virtual	void	KeyUp(char e_cKey);
	virtual	void	KeyDown(char e_cKey);
	virtual	void	KeyPress(char e_cKey);
	virtual	const	WCHAR*	GetNextPhaseName();	
	void			RenderReadMeMenu();
};


class	cBetDataButtonManaget:public cClickMouseBehaviorList<cClickMouseBehavior>
{
	enum	eButtonList
	{
		eBT_BetMaxButton = 0,
		eBT_AutoSpinButton,
		eBT_AddLineBetButton,
		eBT_MinusLineBetButton,
		eBT_AddBetMoneyButton,
		eBT_MinusBetMoneyButton,
		eBT_SpinButton,
		eBT_MenuButton,
		eBT_ExchangeButton,
		eBT_MAX,

	};
	cImageButton*	m_pAutoSpinYesImageButton;
	void			ProcessButtEvent(eButtonList e_eButtonList);
	cBetPhase*		m_pBetPhase;
public:
	cBetDataButtonManaget(cBetPhase*e_pBetPhase);
	~cBetDataButtonManaget();
	virtual	void	Init();
    virtual void    Update(float e_fElpaseTime);
};


#endif