#ifndef _PHASE_CONTROL_SAMPLE_H
#define _PHASE_CONTROL_SAMPLE_H

#include "PhaseControl.h"
namespace FATMING_CORE
{
	//===========================================================
	//a very simple phase to switch quickly to construction the switch phase
	//===========================================================
	class	cPhaseControlSample:public cSimplePhase
	{
		wstring	m_strNextPhaseName;
public:
		cPhaseControlSample(WCHAR*e_strHintText);
		virtual ~cPhaseControlSample();
		void	SetNextPhaseName(WCHAR*e_strNextPhaseName){ m_strNextPhaseName = e_strNextPhaseName; }
		virtual	void	Update(float e_fElpaseTime);
		virtual	void	Init();
		virtual	void	Render();
		virtual	void	MouseUp(int e_iX,int e_iY){}
		virtual	void	KeyUP(char e_cKey){ if( e_cKey == 'q' ) this->m_bSatisfiedCondition = true;  };
		virtual	const   WCHAR*	GetNextPhaseName(){ return m_strNextPhaseName.c_str(); }
	};
//end namespace
}
#endif