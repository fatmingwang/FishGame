#ifndef _SETUP_PHASE_H_
#define _SETUP_PHASE_H_

class	cSetupPhase:public cSimplePhase
{
public:
	cSetupPhase();
	~cSetupPhase();
	virtual	void	Destroy();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Init();							//it would be called if the stage is changed.
	virtual	void	Render();
	virtual	void	DebugRender();
	virtual	void	MouseDown(int e_iX,int e_iY);
	virtual	void	MouseUp(int e_iX,int e_iY);
	virtual	void	MouseMove(int e_iX,int e_iY);
	virtual	const   WCHAR*	GetNextPhaseName();
};

#endif