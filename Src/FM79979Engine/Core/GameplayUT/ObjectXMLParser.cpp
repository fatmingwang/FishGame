#include "../stdafx.h"
#include "../AllImageInclude.h"
#include "Mouse_Touch.h"
#include "ImageButton.h"
#include "AssembleImageButton.h"
#include "ObjectXMLParser.h"
namespace FATMING_CORE
{
	cRenderObjectBehavior*GetRenderObjectBehavior(TiXmlElement*e_pElement)
	{
		const WCHAR*l_strValue = e_pElement->Value();
		cRenderObjectBehavior*l_pRenderObjectBehavior = 0;
		COMPARE_VALUE_WITH_DEFINE( cMPDI::TypeID)
		{
			return cMPDI::GetMe(e_pElement);
		}
		else
		COMPARE_VALUE_WITH_DEFINE( cSubMPDI::TypeID)
		{
			return cSubMPDI::GetMe(e_pElement);
		}
		else
		COMPARE_VALUE_WITH_DEFINE( cImageButton::TypeID)
		{
			return cImageButton::GetMe(e_pElement);
		}
		COMPARE_VALUE_WITH_DEFINE( cPuzzleImageUnit::TypeID )
		{
			return cPuzzleImageUnit::GetMe(e_pElement);
		}
		else
		{
			UT::ErrorMsg(L"GetRenderObjectBehavior not support this type",l_strValue);
		}
		return l_pRenderObjectBehavior;
	}

	cClickMouseBehaviorList<cImageButton>*	GetClickMouseBehaviorList(TiXmlElement*e_pElement)
	{
		ELEMENT_VALUE_ASSERT_CHECK(e_pElement,L"cClickMouseBehaviorList");
		cPuzzleImage*l_pPI = 0;
		cMPDIList*l_pMPDIList = 0;
		cClickMouseBehaviorList<cImageButton>*l_pButtonList = new cClickMouseBehaviorList<cImageButton>();
		const WCHAR*l_strListName = e_pElement->Attribute(L"Name");
		l_pButtonList->SetName(l_strListName);
		e_pElement = e_pElement->FirstChildElement();
		while( e_pElement )
		{
			const WCHAR*l_strValue = e_pElement->Value();
			COMPARE_VALUE_WITH_DEFINE( cImageButton::TypeID)
			{
				cImageButton*l_pImageButton = cImageButton::GetMe(e_pElement);
				if( l_pImageButton )
				{
#ifdef _DEBUG
					if( l_pButtonList->GetObject(l_pImageButton->GetName()) )
					{
						UT::ErrorMsg(l_pImageButton->GetName(),L"thuis button name exists!");
					}
#endif
					l_pButtonList->AddObjectNeglectExist(l_pImageButton);
				}
			
			}
			else
			COMPARE_VALUE_WITH_DEFINE(cAssembleImageButton::TypeID)
			{
				cAssembleImageButton*l_pAssembleImageButton = cAssembleImageButton::GetMe(e_pElement);
				l_pButtonList->AddObjectNeglectExist(l_pAssembleImageButton);
			}
			e_pElement = e_pElement->NextSiblingElement();
		}
		return l_pButtonList;
	}
}