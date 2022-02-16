#ifndef PHASE_CONTROL_H
#define PHASE_CONTROL_H
#include "Mouse_Touch.h"
namespace FATMING_CORE
{
//=====================
//here is the phase control
//we perhap have more than one phase in the game.(game over ,game intro,and so on)
//the phase have some common function,we always call update and render if we need more extract data,we have get it external,like mouse position or else
//
//before cSimplePhase update must call SetCondition!!the condition could be a pointer object or else,we could use this condition to determine what we need to do
//ex: when stage clear,we could using condition to determine what we need to draw
//
//while we created a lot phase we wanted,we could put whole phase into cPhaseManager,
//the manager will change pahse,while phase was satisified the condition,
//
//we only need to add object by cPhaseManager,and cPhaseManager will control everything,unless game is over will leave the whold game
//
//if u do not allow m_bSatisfiedCondition to be false again,set it as true in the Init function if u wanted.
//=====================
	class	cSimplePhase:virtual public cClickMouseBehavior
	{
	protected:
		//if u do not allow m_bSatisfiedCondition to be false again,set it as true in the Init function if u wanted.
		bool			m_bSatisfiedCondition;
		GET_SET_DEC(bool,m_bKillAfterSatisfiedCondition,IsKillAfterSatisfiedCondition,SetKillAfterSatisfiedCondition);
		virtual bool    InternalCollide(int e_iPosX,int e_iPosY){ return true; }
	public:
		//DEFINE_TYPE_INFO();
		cSimplePhase(){ m_bSatisfiedCondition = false; m_bKillAfterSatisfiedCondition = false;  }
		//virtual	void				Start(float e_fElpaseTime = EPSIONAL){}
		virtual	void	Init() = 0;//it would be called if the stage is changed,set m_bSatisfiedCondition as false!?
		virtual	void	Update(float e_fElpaseTime) = 0;
		virtual	void	Render() = 0;
		virtual	void	Destroy(){}
		virtual	void	DebugRender(){}
		virtual	void	KeyUp(char e_cKey){};
		virtual	void	KeyDown(char e_cKey){};
		virtual	void	KeyPress(char e_cKey){};
		//not necessary,some times we need to read data by multithread to prevent screen empty
		//must call after SetCondition via SetCondition we could know what the data is needed
		virtual	void	ReadDataByMultiThread(){};
		virtual	const   WCHAR*	GetNextPhaseName() = 0;
		//if the condition is full,sometimes we need to know the data we want use in this phase,ex: the menu final select object index or else...
		//u could release memory here if u ensure this will called while phase condition if filled.
		virtual	void*	GetData(){return 0;}
		//get the data from previous phase
		virtual	void	FetchData(const WCHAR*e_strPhaseName,void*e_pData){}
		virtual bool	IsSatisfiedCondition(){ return m_bSatisfiedCondition; }
		void			SetSatisfiedCondition(bool e_b){ m_bSatisfiedCondition = e_b;}
	};
//====================
//
//====================
	class	cPhaseManager: public cClickMouseBehaviorList<cSimplePhase>,public cSimplePhase
	{
		GET_SET_DEC(int,m_iCurrentPhase,GetCurrentPhase,SetCurrentPhase);
		//for internal use if u have default render or update override it,ex background image
		virtual	void	InternalRender(){}
		virtual	void	InternalUpdate(float e_fElpaseTime){}
		virtual bool    InternalCollide(int e_iPosX,int e_iPosY){ return false; }
	public:
		cPhaseManager();
		cPhaseManager(cPhaseManager*e_pPhaseManager);
		CLONE_MYSELF(cPhaseManager);
		const	WCHAR*	GetCurrentPhaseName();
		//char*	GetCurrentPhaseNameByChar();
		bool	SetCurrentCurrentPhase(const WCHAR*e_strPhaseNAme);
		virtual	bool	IsSatisfiedCondition(){ return this->m_iCurrentPhase == -1; }
		void			SetSatisfiedCondition(bool e_b){}
		virtual	void	Init(){ cClickMouseBehaviorList<cSimplePhase>::Init(); }
		virtual	void	Update(float	e_fElpaseTime);
		virtual	void	Render();
		virtual	void	Destroy();
		virtual	void	DebugRender();
		virtual	void	MouseDown(int e_iX,int e_iY);
		virtual	void	MouseUp(int e_iX,int e_iY);
		virtual	void	MouseMove(int e_iX,int e_iY);
		virtual	void	KeyUp(char e_cKey);
		virtual	void	KeyDown(char e_cKey);
		virtual	void	KeyPress(char e_cKey);
		//
		virtual	const   WCHAR*	GetNextPhaseName(){ return 0;}
		virtual	bool	Load(){ return false; }
		virtual	void	Start(float e_fElpastTime){}
		virtual void	SingnalProcess(unsigned char e_usKey){ cClickMouseBehaviorList<cSimplePhase>::SingnalProcess(e_usKey); }
		virtual void	SingnalProcess(){ cClickMouseBehaviorList<cSimplePhase>::SingnalProcess(); }
		virtual void    SetRelativePosition( Vector3 e_vPos ){}
	};
//for quickly copy

//class	cGamePhase:public cSimplePhase
//{
//public:
//	DEFINE_TYPE_INFO();
//	cPhase();
//	virtual	void	Update(float e_fElpaseTime);
//	virtual	void	Init();
//	virtual	void	Render();
//	virtual	void	DebugRender();
//	virtual	void	MouseDown(int e_iX,int e_iY);
//	virtual	void	MouseUp(int e_iX,int e_iY);
//	virtual	void	MouseMove(int e_iX,int e_iY);
//	virtual	void	KeyUP(char e_cKey);
//	virtual	WCHAR*	GetNextPhaseName();	
//};

//cGamePhase::cGamePhase()
//{
//
//}
//
//void	cGamePhase::Update(float e_fElpaseTime)
//{
//
//}
//
//void	cGamePhase::Init()
//{
//
//}
//
//void	cGamePhase::Render()
//{
//
//}
//
//void	cGamePhase::DebugRender()
//{
//
//}
//
//void	cGamePhase::MouseDown(int e_iX,int e_iY)
//{
//
//}
//
//void	cGamePhase::MouseUp(int e_iX,int e_iY)
//{
//
//}
//
//void	cGamePhase::MouseMove(int e_iX,int e_iY)
//{
//
//}
//
//void	cGamePhase::KeyUP(char e_cKey)
//{
//
//}
//
//WCHAR*	cGamePhase::GetNextPhaseName()
//{
//
//}
//end namespace
}

#endif