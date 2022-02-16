#include "../stdafx.h"
#include "PhaseControl.h"
#include "GameApp.h"
namespace FATMING_CORE
{

	//const wchar_t*         cSimplePhase::TypeID( L"cSimplePhase" );
 //   void    cSimplePhase::InternalMouseMove(int e_iPosX,int e_iPosY)
	//{
	//	int	l_iCount = this->m_AllMouseBehavior.Count();
	//	for( int i=0;i<l_iCount;++i )
	//		m_AllMouseBehavior[i]->MouseMove(e_iPosX,e_iPosY);
	//}
 //   void    cSimplePhase::InternalMouseDown(int e_iPosX,int e_iPosY)
	//{
	//	int	l_iCount = this->m_AllMouseBehavior.Count();
	//	for( int i=0;i<l_iCount;++i )
	//		m_AllMouseBehavior[i]->MouseMove(e_iPosX,e_iPosY);	
	//}
 //   void    cSimplePhase::InternalMouseUp(int e_iPosX,int e_iPosY)
	//{
	//	int	l_iCount = this->m_AllMouseBehavior.Count();
	//	for( int i=0;i<l_iCount;++i )
	//		m_AllMouseBehavior[i]->MouseMove(e_iPosX,e_iPosY);
	//}
 //   void    cSimplePhase::InternalMouseLeave(int e_iPosX,int e_iPosY)
	//{
	//	int	l_iCount = this->m_AllMouseBehavior.Count();
	//	for( int i=0;i<l_iCount;++i )
	//		m_AllMouseBehavior[i]->MouseMove(e_iPosX,e_iPosY);
	//}
 //   void    cSimplePhase::InternalMouseDoubleClickUp(int e_iPosX,int e_iPosY)
	//{

	//}

	cPhaseManager::cPhaseManager()
	{
		m_iCurrentPhase = -1;
	}

	cPhaseManager::cPhaseManager(cPhaseManager*e_pPhaseManager):cClickMouseBehaviorList<cSimplePhase>(e_pPhaseManager)
	{
	
	}

	const WCHAR*	cPhaseManager::GetCurrentPhaseName()
	{
		if( this->m_iCurrentPhase == -1)
			return NULL;
		return	this->m_ObjectList[m_iCurrentPhase]->GetName();
	}

	//char*	cPhaseManager::GetCurrentPhaseNameByChar()
	//{
	//	if( this->m_iCurrentPhase == -1)
	//		return NULL;
	//	return	UT::WcharToChar(this->m_ObjectList[m_iCurrentPhase]->GetName());
	//}

	bool	cPhaseManager::SetCurrentCurrentPhase(const WCHAR*e_strPhaseName)
	{
		int	l_i = this->GetObjectIndexByName(e_strPhaseName);
		if( l_i != -1 )
		{
			m_iCurrentPhase = l_i;
			(*this)[l_i]->Init();
			(*this)[l_i]->SetSatisfiedCondition(false);
			return true;
		}
		return false;
	}

	void	cPhaseManager::Update(float	e_fElpaseTime)
	{
		InternalUpdate(e_fElpaseTime);
		if( this->m_iCurrentPhase != -1 )
		{
			cSimplePhase *l_pSimplePhase = m_ObjectList[m_iCurrentPhase];
			l_pSimplePhase->Update(e_fElpaseTime);
			if( l_pSimplePhase->IsSatisfiedCondition() )
			{
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

	void	cPhaseManager::Destroy()
	{
		cClickMouseBehaviorList<cSimplePhase>::Destroy();
	}

	void	cPhaseManager::DebugRender()
	{
		if( this->m_iCurrentPhase != -1 )
			m_ObjectList[m_iCurrentPhase]->DebugRender();
	}

	void	cPhaseManager::Render()
	{
		InternalRender();
		if( this->m_iCurrentPhase != -1 )
			m_ObjectList[m_iCurrentPhase]->Render();
	}

	void	cPhaseManager::MouseDown(int e_iX,int e_iY)
	{
		if( this->m_iCurrentPhase != -1 )
			m_ObjectList[m_iCurrentPhase]->MouseDown(e_iX,e_iY);
	}

	void	cPhaseManager::MouseUp(int e_iX,int e_iY)
	{
		if( this->m_iCurrentPhase != -1 )
			m_ObjectList[m_iCurrentPhase]->MouseUp(e_iX,e_iY);
	}

	void	cPhaseManager::MouseMove(int e_iX,int e_iY)
	{
		if( this->m_iCurrentPhase != -1 )
			m_ObjectList[m_iCurrentPhase]->MouseMove(e_iX,e_iY);
	}

	void	cPhaseManager::KeyUp(char e_cKey)
	{
		if( this->m_iCurrentPhase != -1 )
			m_ObjectList[m_iCurrentPhase]->KeyUp(e_cKey);
	}

	void	cPhaseManager::KeyDown(char e_cKey)
	{
		if( this->m_iCurrentPhase != -1 )
			m_ObjectList[m_iCurrentPhase]->KeyDown(e_cKey);
	}

	void	cPhaseManager::KeyPress(char e_cKey)
	{
		if( this->m_iCurrentPhase != -1 )
			m_ObjectList[m_iCurrentPhase]->KeyPress(e_cKey);
	}
//end namespace
}