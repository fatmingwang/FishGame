#ifndef _EXTRA_GAME_PHASE_H_
#define _EXTRA_GAME_PHASE_H_

class	cBaseExtraGame;

class	cExtraGamePhase:public cSimplePhase
{
	cBaseExtraGame*						m_pCurrentBaseExtraGame;
	cBehaviorObjectList<cBaseExtraGame>	m_BaseExtraGameList;
	cMPDI*								m_pConstantUI;
	//
	void								CheckFreeGameCondition();
public:
	cExtraGamePhase();
	~cExtraGamePhase();
	virtual	void	Init();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render();
	virtual	void	DebugRender();
	virtual	void	MouseDown(int e_iX,int e_iY);
	virtual	void	MouseUp(int e_iX,int e_iY);
	virtual	void	MouseMove(int e_iX,int e_iY);
	virtual	void	KeyUP(char e_cKey);
	virtual	const	WCHAR*	GetNextPhaseName();	
	const   WCHAR*	GetCurrentExtraGameName();
};

#endif