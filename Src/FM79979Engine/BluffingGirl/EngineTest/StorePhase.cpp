#include "stdafx.h"
#include "StorePhase.h"
#include "IABSetup.h"
#include "AllPhaseDefine.h"
cStorePhase::cStorePhase()
{
	m_bEnable = true;
	m_pIABSetup = 0;
	m_pIABPI = 0;
	Vector4	l_vCollideRange(0,0,cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);
	m_pScroller = new cScroller((cPuzzleImage*)0,l_vCollideRange,0,false);
	m_pScroller->SetOutOfWorking(false);
	this->SetName(STORE_PHASE_NAME);
}

cStorePhase::~cStorePhase()
{
	SAFE_DELETE(m_pScroller);
	SAFE_DELETE(m_pIABSetup);
	if( m_pIABPI )
		cGameApp::m_spImageParser->RemoveObject(m_pIABPI);
	m_pIABPI = 0;
}
//<cStorePhase PI="aabb.pi" IABSetupFile="">
//	<ProductData Image="AABB" ProductName="" />
//</cStorePhase>

void	cStorePhase::ProcessIABMenuData(TiXmlElement*e_pTiXmlElement)
{
	assert(m_pIABPI == 0);
	assert(m_pIABSetup== 0);
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("PI")
		{
			m_pIABPI = cGameApp::GetPuzzleImageByFileName(l_strValue);
		}
		else
		COMPARE_NAME("IABSetupFile")
		{
			m_pIABSetup = new cIABSetup(UT::WcharToChar(l_strValue).c_str());
		}
		else
		COMPARE_NAME("ScrollRect")
		{
			m_pScroller->SetCollisionRange(VALUE_TO_VECTOR4);
		}
		else
		COMPARE_NAME("ItemGap")
		{
			m_pScroller->SetObjectGap(VALUE_TO_FLOAT);
		}
	PARSE_NAME_VALUE_END
}

void	cStorePhase::ProcessScrollerData(TiXmlElement*e_pTiXmlElement)
{
	cPuzzleImageUnit*l_pPIUnit = 0;
	cPuzzleImageUnit*l_pPIUnitClone = 0;
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("Image")
		{
			if( m_pIABPI )
			{
				l_pPIUnit = m_pIABPI->GetObject(l_strValue);
				l_pPIUnitClone = new cPuzzleImageUnit(l_pPIUnit);
			}
		}
		else
		COMPARE_NAME("ProductName")
		{
			if( l_pPIUnitClone )
				l_pPIUnitClone->SetName(l_strValue);
		}
	PARSE_NAME_VALUE_END
	if( l_pPIUnitClone )
	{
		m_pScroller->AddObject(dynamic_cast<cBaseImage*>(l_pPIUnitClone),true);
	}
	else
	{//test code
		//cTextButton*l_pTextButton = new cTextButton(Vector2(l_vPos.x,l_vPos.y),cGameApp::m_spGlyphFontRender,Vector4(1,0,0,1),L"Test11111");
		//l_pTextButton->SetLocalPosition(l_vPos);
		//m_pScroller->AddData(l_pTextButton);
	}
}

void	cStorePhase::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	COMPARE_VALUE("IABMenu")
	{
		ProcessIABMenuData(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("ProductData")
	{
		ProcessScrollerData(e_pTiXmlElement);
	}
}


void    cStorePhase::Init()
{
	if( m_pScroller )
	{
		//reorganize position
		m_pScroller->Init();
		if( m_pIABPI )
		{
			m_pScroller->SetBG(m_pIABPI->GetObject(L"BG"));
			m_pScroller->SetCancelButton(m_pIABPI->GetObject(L"Back"),Vector3::Zero);
			//m_pScroller->SetOutOfWorking(true);
		}
	}
}

void	cStorePhase::SetEnable(bool e_bEnable)
{

}

void    cStorePhase::Update(float e_fElpaseTime)
{
	if( !m_bEnable )
		return;
	if( m_pScroller )
	{
		if( m_pScroller->IsSatisfiedCondition() || m_pScroller->IsChancelButtonSatisfied())
		{
			this->m_bSatisfiedCondition = true;
		}
		else
			m_pScroller->Update(e_fElpaseTime);
	}
}

bool	cStorePhase::IsPurchaseItem()
{
	return m_pScroller->IsSatisfiedCondition();
}

const	WCHAR*cStorePhase::GetPurchaseProductName()
{
	if( m_pScroller->IsSatisfiedCondition() )
		return m_pScroller->GetSelectedIndexName();
	return 0;
}

void    cStorePhase::Render()
{
	if( !m_bEnable )
		return;
	if( m_pScroller )
		m_pScroller->Render();
}

void    cStorePhase::DebugRender()
{
	if( !m_bEnable )
		return;
	if( m_pScroller )
		m_pScroller->DebugRender();
}

void    cStorePhase::MouseMove(int e_iPosX,int e_iPosY)
{
	if( !m_bEnable )
		return;
	if( m_pScroller )
		m_pScroller->MouseMove(e_iPosX,e_iPosY);
}

void    cStorePhase::MouseDown(int e_iPosX,int e_iPosY)
{
	if( !m_bEnable )
		return;
	if( m_pScroller )
		m_pScroller->MouseDown(e_iPosX,e_iPosY);
}

void    cStorePhase::MouseUp(int e_iPosX,int e_iPosY)
{
	if( !m_bEnable )
		return;
	if( m_pScroller )
		m_pScroller->MouseUp(e_iPosX,e_iPosY);
}

void	cStorePhase::FetchData(const WCHAR*e_strPhaseName,void*e_pData)
{
	m_strLastPhaseName = e_strPhaseName;
}

const   WCHAR*	cStorePhase::GetNextPhaseName()
{
	return m_strLastPhaseName.c_str();
}