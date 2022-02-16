#include "stdafx.h"
#include "FAIAnimationCharacter.h"
#include "../AllPhysicInclude.h"
//from Frame
//extern const FLOAT    FRAME_DIRTY_WORLD_CACHE;

namespace FATMING_AI
{
	//<cFAIAnimationCharacter >
	//	<Status Name="" MPDIList="" MPDI="" Loop="" />
	//	<Status Name="" MPDIList="" MPDI="" Loop="" />
	//	<Status Name="" MPDIList="" MPDI="" Loop="" />
	//	<Status Name="" MPDIList="" MPDI="" Loop="" />
	//</cFAIAnimationCharacter>
	const wchar_t*	cFAIAnimationCharacter::TypeID( L"cFAIAnimationCharacter" );
	cFAIAnimationCharacter::cFAIAnimationCharacter(TiXmlElement*e_pElement):cFAICharacterInterface(e_pElement)
	{
		m_ePreviousBehaviodStatus = eFAI_CBS_MAX;
		if( !e_pElement )
			return;
		//assert(wcscmp(e_pElement->Value(),cFAIAnimationCharacter::TypeID) == 0);
		if( e_pElement )
		{
			e_pElement = GetXmlElementByNameFromElement(cFAIAnimationCharacter::TypeID,e_pElement);			
			if( e_pElement )
				e_pElement = e_pElement->FirstChildElement();
			else
				return;
		}
		cMPDIList*l_pMPDIList = 0;
		cMPDI*l_pMPDI = 0;
		cFMAnimationRule*l_pFMAnimationRule = 0;
		bool	l_bLoop = false;
		while( e_pElement )
		{
			std::wstring	l_strBehaviorName;
			PARSE_ELEMENT_START(e_pElement)
				COMPARE_NAME("Name")
				{
					l_strBehaviorName = l_strValue;
				}
				else
				COMPARE_NAME("MPDIList")
				{
					l_pMPDIList = cGameApp::GetMPDIListByFileName(l_strValue);
				}
				else
				COMPARE_NAME("MPDI")
				{
					if( l_pMPDIList )
						l_pMPDI = l_pMPDIList->GetObject(l_strValue);
				}
				else
				COMPARE_NAME("Loop")
				{
					l_bLoop = VALUE_TO_BOOLEAN;
				}
			PARSE_NAME_VALUE_END
			assert( l_pMPDIList && l_pMPDI && "failed to get MPDI");
			if( l_pMPDIList && l_pMPDI )
			{
				cMPDI*l_pCloneMPDI = new cMPDI(l_pMPDI);
				l_pCloneMPDI->SetName(l_strBehaviorName.c_str());
				l_pCloneMPDI->SetAnimationLoop(l_bLoop);
				l_pFMAnimationRule = l_pCloneMPDI;
			}
			if( l_pFMAnimationRule )
			{
				l_pFMAnimationRule->SetParent(this);
				bool	l_b = this->AddObject(l_pFMAnimationRule);
				assert(l_b);
			}
			e_pElement = e_pElement->NextSiblingElement();
		}
	}

	cFAIAnimationCharacter::cFAIAnimationCharacter(cFAIAnimationCharacter*e_pFAIAnimationStatus)
	:cFAICharacterInterface(e_pFAIAnimationStatus),cFMWorkingObjectChanger<cFMAnimationRule>(e_pFAIAnimationStatus)
	{
		int	l_iCount = this->Count();
		for( int i=0;i<l_iCount;++i )
		{
			this->GetObject(i)->SetParent(this);
		}
	}

	cFAIAnimationCharacter::~cFAIAnimationCharacter()
	{
	
	}

	bool	cFAIAnimationCharacter::IsCharacterBehaviorStatusLock()
	{
		if( m_pAICharacterStatus && m_pAICharacterStatus->IsCharacterBehaviorStatusLock()  )
		{
			cFMAnimationRule*l_pFMAnimationRule = this->GetCurrentWorkingObject();
			if( !l_pFMAnimationRule || l_pFMAnimationRule->IsAnimationDone() )
				return true;
		}
		return false;
	}

	void*   cFAIAnimationCharacter::GetAnimationData()
	{
		//return (void*)(cFMWorkingObjectChanger<cRenderObjectBehavior>*)this; 
		return this; 
	}
	void	cFAIAnimationCharacter::Init()
	{
		cFMWorkingObjectChanger<cFMAnimationRule>::Init(); 
		cRenderObjectBehavior*l_pRenderObjectBehavior = this->GetCurrentWorkingObject();
		if( l_pRenderObjectBehavior )
			m_ePreviousBehaviodStatus = GetCharacterBehaviodStatus(l_pRenderObjectBehavior->GetName());
	}

	void	cFAIAnimationCharacter::Update(float e_fElpaseTime)
	{
		if( this->m_pAICharacterStatus )
		{
			if( m_pAICharacterStatus->GetFAICharacterBehaviorStatus() != m_ePreviousBehaviodStatus)
			{
				m_ePreviousBehaviodStatus = m_pAICharacterStatus->GetFAICharacterBehaviorStatus();
				int	l_iIndex = this->GetObjectIndexByName(g_strCharacterStatus[m_ePreviousBehaviodStatus]);
				if( l_iIndex != -1 )
				{
					this->SetCurrentWorkingObject(l_iIndex);
				}
			}
		}
		cFMWorkingObjectChanger<cFMAnimationRule>::Update(e_fElpaseTime); 
		//if(this->m_pCurrentWorkingObject && m_CachedWorldTransform._11 == FRAME_DIRTY_WORLD_CACHE )
		//if(this->m_pCurrentWorkingObject)
		//{
		//	cMatrix44	l_mat = this->GetWorldTransform();
		//	if( m_pCurrentWorkingObject->Type() == cMPDI::TypeID )
		//	{
		//		cMPDI*l_pMPDI = dynamic_cast<cMPDI*>(m_pCurrentWorkingObject);
		//		l_pMPDI->SetTransform(l_mat);
		//	}
		//	cbtConcaveShapeList*l_pbtConcaveShapeList = (cbtConcaveShapeList*)this->GetCollisionData();
		//	l_pbtConcaveShapeList->SetTransform(l_mat);
		//}
	}
	void	cFAIAnimationCharacter::Render()
	{
		cFMWorkingObjectChanger<cFMAnimationRule>::Render();
	}
}