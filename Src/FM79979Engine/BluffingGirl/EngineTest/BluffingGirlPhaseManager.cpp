#include "stdafx.h"
#include "BluffingGirlPhaseManager.h"
#include "StorePhase.h"
#include "AllPhaseDefine.h"
void	cBluffingGirlPhaseManager::Update(float	e_fElpaseTime)
{
	InternalUpdate(e_fElpaseTime);
	if( this->m_iCurrentPhase != -1 )
	{
		cSimplePhase *l_pSimplePhase = m_ObjectList[m_iCurrentPhase];
		l_pSimplePhase->Update(e_fElpaseTime);
		if( l_pSimplePhase->IsSatisfiedCondition() )
		{
			if( !wcscmp(l_pSimplePhase->GetName(),STORE_PHASE_NAME) )
			{
				cStorePhase*l_pIABMenu = (cStorePhase*)l_pSimplePhase;
				const WCHAR*	l_strLastPhaseName = l_pIABMenu->GetNextPhaseName();
				m_iCurrentPhase = GetObjectIndexByName(l_strLastPhaseName);
				l_pSimplePhase = m_ObjectList[m_iCurrentPhase];
				l_pSimplePhase->SetSatisfiedCondition(false);
				return;
			}
			int	l_iIndex = GetObjectIndexByName(l_pSimplePhase->GetNextPhaseName());
			if( l_iIndex == -1  )
			{
#ifdef WIN32
				cGameApp::OutputDebugInfoString(L"error Phase or end phase?? ");
#endif
			}
			m_iCurrentPhase = l_iIndex;
			cSimplePhase*l_pTargetPhase = 0;
			if( l_iIndex != -1 )
			{
			    l_pTargetPhase = m_ObjectList[l_iIndex];
				l_pTargetPhase->SetSatisfiedCondition(false);//if u do not allow m_bSatisfiedCondition to be false again,set it as true in the Init function if u wanted.
				l_pTargetPhase->FetchData(l_pSimplePhase->GetName(),l_pSimplePhase->GetData());
				l_pTargetPhase->Init();
				l_pTargetPhase->Update(0.001f);
			}
			if(l_pSimplePhase->IsKillAfterSatisfiedCondition())
			    this->RemoveObject(l_pSimplePhase);
			if( l_pTargetPhase )
			{
				m_iCurrentPhase = this->GetObjectIndexByPointer(l_pTargetPhase);
				//try to avoid empty phase render...!?
				//l_pTargetPhase->Update(0.00f);
			}
			cGameApp::m_sTimeAndFPS.Update();
		}
	}
}