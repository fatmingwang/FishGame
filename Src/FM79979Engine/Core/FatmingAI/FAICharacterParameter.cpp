#include "stdafx.h"
#include "FAICharacterParameter.h"
#include "../AllPhysicInclude.h"
namespace FATMING_AI
{
	const wchar_t*	cFAICharacterInterface::TypeID( L"cFAICharacterInterface" );
    //const wchar_t*cFAICharacterInterface::TypeID( L"cFAICharacterInterface" );
	cFAICharacterInterface::cFAICharacterInterface(cFAICharacterInterface*e_pAICharacterInterface)
	{
		m_pBound = 0;
		m_fAnimationSpeed = e_pAICharacterInterface->m_fAnimationSpeed;
		if( e_pAICharacterInterface->m_pBound )
		{
			m_pBound = new cBound(*e_pAICharacterInterface->m_pBound);
		}
		m_v2DAngle = Vector3::Zero;
		m_fSpeed = e_pAICharacterInterface->m_fSpeed;
		m_fRunAwaySpeed = e_pAICharacterInterface->m_fRunAwaySpeed;
		this->m_pAICharacterStatus = (cFAICharacterStatus*)e_pAICharacterInterface->m_pAICharacterStatus->Clone();
	}

    cFAICharacterInterface::cFAICharacterInterface(TiXmlElement*e_pElement)
    {
		m_pBound = 0;
		m_v2DAngle = Vector3::Zero;
		m_fSpeed = 10.f;
		m_fAnimationSpeed = 1.f;
		m_pAICharacterStatus = 0;

		if( e_pElement )
		{
			TiXmlElement*l_pCharacterStatusElement = GetXmlElementByNameFromElement(cFAICharacterStatus::TypeID,e_pElement);
			if( l_pCharacterStatusElement )
				m_pAICharacterStatus = new cFAICharacterStatus(l_pCharacterStatusElement);
		}

		if( e_pElement )
		{
			const WCHAR*l_strSpeed = e_pElement->Attribute(L"Speed");
			if( l_strSpeed )
				m_fSpeed = (float)_wtof(l_strSpeed);
			const WCHAR*l_strRunAwaySpeed = e_pElement->Attribute(L"RunAwaySpeed");
			if( l_strRunAwaySpeed )
				m_fRunAwaySpeed = (float)_wtof(l_strRunAwaySpeed);
			else
				m_fRunAwaySpeed = m_fSpeed;
		}
    }

	cFAICharacterInterface::~cFAICharacterInterface()
	{
		SAFE_DELETE(m_pBound);
		SAFE_DELETE(m_pAICharacterStatus);
	}

    Vector3* cFAICharacterInterface::GetPosition()
    {
        return (Vector3*)(size_t)(m_LocalTransform.m[MyMath::W]);
    }

	void	cFAICharacterInterface::DebugRender()
	{
		//NamedTypedObject*l_pNamedTypedObject = (NamedTypedObject*)this->GetCollisionData();
		//if( l_pNamedTypedObject )
		//{
		//	if( l_pNamedTypedObject->Type() == cbtConcaveShapeList::TypeID )
		//	{
		//		cbtConcaveShapeList*l_pbtConcaveShapeList = (cbtConcaveShapeList*)this->GetCollisionData();
		//		l_pbtConcaveShapeList->Render();
		//	}
		//}
	}

	void	 cFAICharacterInterface::SetBound(cBound*e_pData)
	{
		if(m_pBound)
		{
			*m_pBound = *e_pData;
		}
		else
			m_pBound = new cBound(*e_pData);
	}

	bool	cFAICharacterInterface::IsCharacterBehaviorStatusLock()
	{
		if(this->m_pAICharacterStatus)
		{
			return m_pAICharacterStatus->IsCharacterBehaviorStatusLock();
		}
		return false;
	}

	bool	cFAICharacterInterface::IsCharacterBehaviorStatusLock(eFAICharacterBehaviorStatus	e_eFAICharacterBehaviorStatus)
	{
		if(this->m_pAICharacterStatus)
		{
			return m_pAICharacterStatus->IsCharacterBehaviorStatusLock(e_eFAICharacterBehaviorStatus);
		}
		return false;	
	}
}