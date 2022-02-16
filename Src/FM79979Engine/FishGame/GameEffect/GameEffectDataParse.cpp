#include "stdafx.h"
#include "GameEffectBase.h"
#include "GameEffectCardMarquee.h"
#include "../PlayerBehavior/PlayerBehaviorBase.h"
#include "../GameApp/GameApp.h"
void	cGameEffectManager::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	COMPARE_VALUE("GameEffect")
	{
		ProvessGameEffect(e_pTiXmlElement);
	}
}

void	cGameEffectManager::ProvessGameEffect(TiXmlElement*e_pTiXmlElement)
{
	cGameEffectBase*l_pGameEffectBase = 0;
	const WCHAR*l_strCount = ATTRIBUT_VALUE(e_pTiXmlElement,"Count");
	const WCHAR*l_strAlwaysRun = ATTRIBUT_VALUE(e_pTiXmlElement,"AlwaysRun");
	e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	COMPARE_VALUE_WITH_DEFINE(cGameEffectCardMarqueeManager::TypeID)
	{
		l_pGameEffectBase = ProvessGameEffectCardMarqueeManager(e_pTiXmlElement);
	}
	else
	{
		if( m_pGameEffectParseFunction )
		{
			l_pGameEffectBase = (*m_pGameEffectParseFunction)(e_pTiXmlElement);
		}
		else
			UT::ErrorMsg(l_strValue,L"not suport");
	}
	if( l_pGameEffectBase )
	{
		if( l_strAlwaysRun )
		{
			l_pGameEffectBase->SetName(l_strValue);
			this->m_AlwaysRunGameEffectBaseList.AddObjectNeglectExist(l_pGameEffectBase);
		}
		else
		{
			cNamedTypedObjectVector< cGameEffectBase >*l_pData = new cNamedTypedObjectVector< cGameEffectBase >;
			l_pData->SetName(l_strValue);
			l_pData->AddObject(l_pGameEffectBase);
			m_AllGameEffectBaseList.AddObject(l_pData);
			if( l_strCount )
			{
				int	l_iCount = GetInt(l_strCount)-1;
				for( int i=0;i<l_iCount;++i )
				{
					l_pData->AddObject(dynamic_cast<cGameEffectBase*>(l_pGameEffectBase->Clone()));
				}
			}
		}
	}
}
//<GameEffectManager>
//	<cGameEffectCardMarqueeManager Count="8" PI="Fish/Image/CoinEffect/ShowGetMoney.pi" MaximumCount="10" ViewableCount="4" StartPos="0,0,0" EndPos="70,0,0" HoldTime="0.51" >
//		<cNumerialImage PI="Fish/Image/CoinEffect/ShowGetMoney.pi" />
//		<Matrix Data="......."/>
//	</cGameEffectCardMarqueeManager>
//</GameEffectManager>
cGameEffectBase*	cGameEffectManager::ProvessGameEffectCardMarqueeManager(TiXmlElement*e_pTiXmlElement)
{
	int	l_iCount = -1;
	cPuzzleImage*l_pPuzzleImage = 0;
	int	l_iMaximumCount = -1;
	int	l_iViewableCount = -1;
	Vector3	l_vStartPos,l_vEndPos;
	float	l_fHoldTime,l_fTimeToDestination = 0.f;
	cNumerialImage*		l_pNumerialImage = 0;
	cMatrix44	l_mat;
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("Count")
		{
			l_iCount = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("PI")
		{
			l_pPuzzleImage = cGameApp::GetPuzzleImageByFileName(l_strValue);
		}
		else
		COMPARE_NAME("MaximumCount")
		{
			l_iMaximumCount = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("ViewableCount")
		{
			l_iViewableCount = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("StartPos")
		{
			l_vStartPos = VALUE_TO_VECTOR3;
		}
		else
		COMPARE_NAME("EndPos")
		{
			l_vEndPos = VALUE_TO_VECTOR3;
		}
		else
		COMPARE_NAME("HoldTime")
		{
			l_fHoldTime = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("TimeToDestination")
		{
			l_fTimeToDestination = VALUE_TO_FLOAT;
		}
	PARSE_NAME_VALUE_END
	e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while( e_pTiXmlElement )
	{
		COMPARE_TARGET_ELEMENT_VALUE_WITH_DEFINE(e_pTiXmlElement,cNumerialImage::TypeID)
		{
			l_pNumerialImage = cGameApp::GetNumerialImageByXmlElement(e_pTiXmlElement);
		}
		e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
	}
	cGameEffectCardMarqueeManager*l_pcGameEffectCardMarqueeManager = new cGameEffectCardMarqueeManager(l_pNumerialImage);
	if( l_iCount > 0 )
	{
		for( int i=0;i<l_iCount;++i )
		{
			cPlayerBehaviorBase*l_pPlayerBehaviorBase = cFishApp::m_spPlayerManager->GetObject(i);
			if( l_pPlayerBehaviorBase )
			{
				l_mat = l_pPlayerBehaviorBase->GetPlayerMatrix();
				cGameEffectPlayerCardMarquee*l_pGameEffectPlayerCardMarquee = new cGameEffectPlayerCardMarquee(l_pPuzzleImage,l_iMaximumCount,l_fTimeToDestination,l_vStartPos,l_vEndPos,l_fHoldTime,l_iViewableCount,l_mat,l_pNumerialImage);
				l_pcGameEffectCardMarqueeManager->m_GameEffectPlayerCardMarqueeList.AddObjectNeglectExist(l_pGameEffectPlayerCardMarquee);
			}
		}
	}
	return l_pcGameEffectCardMarqueeManager;
}