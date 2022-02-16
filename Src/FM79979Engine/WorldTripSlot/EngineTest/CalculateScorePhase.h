#ifndef _CALCULATE_SCORE_PHASE_H_
#define _CALCULATE_SCORE_PHASE_H_

#include "WinMoneyLine.h"

class	cCalculateScorePhase:public cSimplePhase	
{
	UT::sTimeCounter	m_IdleTimeForShowHowManyMoneyWin;
	//
	UT::sTimeCounter	m_TimeForScoreToChangeUserMoney;
	cMPDI*				m_pConstantUI;
	//
	int					m_iShowMPDIMoney;
	cMPDI*				m_pWinMoneyOver2000MPDI;
	int					m_iPlayerOriginalMoney;
	int					m_iWinMoney;
	int					m_iPlayerMoney;
public:
	cCalculateScorePhase();
	~cCalculateScorePhase();
	virtual	void	Init();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render();
	virtual	void	DebugRender();
	virtual	void	MouseDown(int e_iX,int e_iY);
	virtual	void	MouseUp(int e_iX,int e_iY);
	virtual	void	MouseMove(int e_iX,int e_iY);
	virtual	void	KeyUp(char e_cKey);
	virtual	const	WCHAR*	GetNextPhaseName();	
};


#endif