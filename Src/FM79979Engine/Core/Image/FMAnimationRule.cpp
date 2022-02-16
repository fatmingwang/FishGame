#include "../stdafx.h"
#include "FMAnimationRule.h"
namespace FATMING_CORE
{
	cFMAnimationRule::cFMAnimationRule()
    {
	    m_fStartTime = 0.f;
	    m_bStart = false;
	    m_fPastTime = 0.f; 
	    m_bAnimationDone = true;
	    m_bAnimationLoop = false;
		m_bStayAtLastFrame = false;
    }
	cFMAnimationRule::cFMAnimationRule(cFMAnimationRule*e_pFatmingAnimationRule):cRenderObjectBehavior(e_pFatmingAnimationRule)
    {
		m_bStayAtLastFrame = e_pFatmingAnimationRule->m_bStayAtLastFrame;
	    m_fStartTime = e_pFatmingAnimationRule->m_fStartTime;
	    m_bStart = e_pFatmingAnimationRule->m_bStart;
	    m_fPastTime = e_pFatmingAnimationRule->m_fPastTime;
	    m_bAnimationDone = e_pFatmingAnimationRule->m_bAnimationDone;
	    m_bAnimationLoop = e_pFatmingAnimationRule->m_bAnimationLoop;
    }
    cFMAnimationRule::~cFMAnimationRule()
	{
		//OutputDebugString(L"cFMAnimationRule\n");
	}
    void	cFMAnimationRule::Init()
    {
	    m_bStart = false;
	    m_bAnimationDone = false;
	    m_fPastTime = 0.f;
	    InternalInit();
	    //if( e_fElpaseTime !=0.f )
		  //  Update(e_fElpaseTime);
    }

    void	cFMAnimationRule::Update(float e_fElpaseTime)
    {
		if( m_bAnimationDone && !this->m_bAnimationLoop )
		    return;
	    m_fPastTime += e_fElpaseTime;
	    if( !m_bStart )
	    {
		    if(m_fPastTime>m_fStartTime)
		    {
			    m_bStart = true;
			    InternalUpdate(m_fPastTime-m_fStartTime);
		    }
	    }
	    else
	    {
		    InternalUpdate(e_fElpaseTime);
	    }
    }

    void	cFMAnimationRule::Render()
    {
		if( !m_bStart )
			return;
		if( m_bAnimationDone )
		{
			if( !m_bAnimationLoop )
				return;
		}
	    InternalRender();
    }
    bool	cFMAnimationRule::IsAnimationLoop(){ return m_bAnimationLoop;}
    bool	cFMAnimationRule::IsAnimationDone(){ return m_bAnimationDone; }
    float	cFMAnimationRule::GetPastTime(){ return m_fPastTime; }
    void	cFMAnimationRule::SetAnimationLoop(bool e_bLoop){ m_bAnimationLoop = e_bLoop; }
    void	cFMAnimationRule::SetAnimationDone(bool e_bAnimationDone){ m_bAnimationDone = e_bAnimationDone;m_fPastTime = this->GetEndTime()+0.0001f; }
//end namespace FATMING_CORE
}