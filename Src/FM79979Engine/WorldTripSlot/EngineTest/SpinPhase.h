#ifndef _SPIN_PHASE_H_
#define _SPIN_PHASE_H_

#include "SlotObject.h"
#include "FallDownNewSpinObject.h"

class	cExtraGame;
class	cSpinPhase:public cSimplePhase
{
	//bool						m_bCheckSpinBurronIsUp;
	cMPDI*						m_pConstantUI;
	cExtraGame*					m_pFreeGame;
	bool						m_bFreeGame;
	float						m_fSpeedUp[50];//assume 50is big enough
public:
	cSpinPhase();
	~cSpinPhase();
	virtual	void	Init();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render();
	virtual	void	DebugRender();
	virtual	void	MouseDown(int e_iX,int e_iY);
	virtual	void	MouseUp(int e_iX,int e_iY);
	virtual	void	MouseMove(int e_iX,int e_iY);
	virtual	void	KeyUp(char e_cKey);
	virtual	const	WCHAR*	GetNextPhaseName();	
	virtual	void*	GetData();
};

#endif