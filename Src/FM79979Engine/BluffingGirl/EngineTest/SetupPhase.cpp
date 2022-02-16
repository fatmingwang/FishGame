#include "stdafx.h"
#include "SetupPhase.h"

cSetupPhase::cSetupPhase()
{
	this->SetName(SETUP_PHASE_NAME);
}

cSetupPhase::~cSetupPhase()
{

}

void	cSetupPhase::Destroy()
{

}

void	cSetupPhase::Update(float e_fElpaseTime)
{
	this->m_bSatisfiedCondition = true;
}

void	cSetupPhase::Init()
{

}
							//it would be called if the stage is changed.
void	cSetupPhase::Render()
{

}

void	cSetupPhase::DebugRender()
{

}

void	cSetupPhase::MouseDown(int e_iX,int e_iY)
{

}

void	cSetupPhase::MouseUp(int e_iX,int e_iY)
{

}

void	cSetupPhase::MouseMove(int e_iX,int e_iY)
{

}

const   WCHAR*	cSetupPhase::GetNextPhaseName()
{
	return MAIN_PHASE_NAME;
}