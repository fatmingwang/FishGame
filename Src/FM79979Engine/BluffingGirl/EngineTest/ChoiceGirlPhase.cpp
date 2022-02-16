#include "stdafx.h"
#include "ChoiceGirlPhase.h"
//<ChoiceGirlPhase PI="Image/Girl/ChoiceGirl.pi">
//	<GirlData Name="1" Image="1"/>
//	<GirlData Name="2" Image="2"/>
//	<GirlData Name="3" Image="3"/>
//	<GirlData Name="4" Image="4"/>
//	<GirlData Name="5" Image="5"/>
//</ChoiceGirlPhase>
cChoiceGirlPhase::cChoiceGirlPhase(const char*e_strFileName)
{
	this->SetName(CHOICE_GIRL_PHASE_NAME);
	this->m_strFileName = e_strFileName;
	Vector4	l_vCollideRange(0,0,cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);
	m_pScroller = new cScroller((cPuzzleImage*)0,l_vCollideRange,0,true);
	m_pScroller->SetOutOfWorking(false);
	m_pChoiceGirlPI = 0;
	cMPDIList*l_pCommonMPDIList = cGameApp::GetMPDIListByFileName(L"Image/Common.mpdi");
	m_pScroller->SetCancelButton(l_pCommonMPDIList->GetObject(L"Back"));
	//m_pBackButton = new cImageButton(l_pCommonMPDIList->GetObject(L"Back"));
}

cChoiceGirlPhase::~cChoiceGirlPhase()
{
	SAFE_DELETE(m_pScroller);
}

void	cChoiceGirlPhase::Destroy()
{
	m_pScroller->Destroy();
	m_ResourceStamp.ReleaseNewResource();
	m_pChoiceGirlPI = 0;
}

void	cChoiceGirlPhase::ProcessChoiceGirlPhaseData(TiXmlElement*e_pTiXmlElement)
{
	assert(m_pChoiceGirlPI == 0);
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("PI")
		{
			m_pChoiceGirlPI = cGameApp::GetPuzzleImageByFileName(l_strValue);
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

void	cChoiceGirlPhase::ProcessGirlData(TiXmlElement*e_pTiXmlElement)
{
	cPuzzleImageUnit*l_pPIUnit = 0;
	cPuzzleImageUnit*l_pPIUnitClone = 0;
	std::wstring	l_strButtonName;
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("Image")
		{
			if( m_pChoiceGirlPI )
			{
				l_pPIUnit = m_pChoiceGirlPI->GetObject(l_strValue);
				l_pPIUnitClone = new cPuzzleImageUnit(l_pPIUnit);
			}
		}
		else
		COMPARE_NAME("Name")
		{
			l_strButtonName = l_strValue;
		}
	PARSE_NAME_VALUE_END
	if( l_pPIUnitClone )
	{
		l_pPIUnitClone->SetName(l_strButtonName.c_str());
		m_pScroller->AddObject(dynamic_cast<cBaseImage*>(l_pPIUnitClone),true);
	}
	else
	{//test code
		//cTextButton*l_pTextButton = new cTextButton(Vector2(l_vPos.x,l_vPos.y),cGameApp::m_spGlyphFontRender,Vector4(1,0,0,1),L"Test11111");
		//l_pTextButton->SetLocalPosition(l_vPos);
		//m_pScroller->AddData(l_pTextButton);
	}
}

void	cChoiceGirlPhase::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	COMPARE_VALUE("ChoiceGirlPhase")
	{
		ProcessChoiceGirlPhaseData(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("GirlData")
	{
		ProcessGirlData(e_pTiXmlElement);
	}
}

void	cChoiceGirlPhase::Init()
{
	m_ResourceStamp.StampResource1();
	m_bSatisfiedCondition = false;
	//
	if(!this->Parse(m_strFileName.c_str()))
	{
		UT::ErrorMsg(m_strFileName.c_str(),"parse failed!!");
	}
	if( m_pScroller )
		m_pScroller->Init();
	//
	m_ResourceStamp.StampResource2();
}

void	cChoiceGirlPhase::Update(float e_fElpaseTime)
{
	if( m_pScroller )
	{
		if( m_pScroller->IsSatisfiedCondition() || m_pScroller->IsChancelButtonSatisfied())
		{
			this->m_bSatisfiedCondition = true;
			if(m_pScroller->GetSelectedIndexName())
				m_strSelectedGirlName = m_pScroller->GetSelectedIndexName();
			this->Destroy();
		}
		else
			m_pScroller->Update(e_fElpaseTime);
	}
}

void	cChoiceGirlPhase::Render()
{
	if( m_pScroller )
		m_pScroller->Render();
}

void	cChoiceGirlPhase::DebugRender()
{
	if( m_pScroller )
		m_pScroller->DebugRender();
}

void	cChoiceGirlPhase::MouseDown(int e_iPosX,int e_iPosY)
{
	if( m_pScroller )
		m_pScroller->MouseDown(e_iPosX,e_iPosY);
}

void	cChoiceGirlPhase::MouseUp(int e_iPosX,int e_iPosY)
{
	if( m_pScroller )
		m_pScroller->MouseUp(e_iPosX,e_iPosY);
}

void	cChoiceGirlPhase::MouseMove(int e_iPosX,int e_iPosY)
{
	if( m_pScroller )
		m_pScroller->MouseMove(e_iPosX,e_iPosY);
}

const   WCHAR*	cChoiceGirlPhase::GetNextPhaseName()
{
	if(m_pScroller->IsChancelButtonSatisfied())
		return MAIN_PHASE_NAME;
	else
		return CHOICE_GIRL_STAGE_PHASE_NAME;
}

void*	cChoiceGirlPhase::GetData()
{
	return (void*)m_strSelectedGirlName.c_str();
}