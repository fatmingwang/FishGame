#include "stdafx.h"
#include "ChoiceGirlStagePhase.h"
cChoiceGirlStagePhase::cChoiceGirlStagePhase()
{
	this->SetName(CHOICE_GIRL_STAGE_PHASE_NAME);
	Vector4	l_vCollideRange(0,0,cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);
	m_pScroller = new cScroller((cPuzzleImage*)0,l_vCollideRange,0,false);
	m_pScroller->SetOutOfWorking(false);
	m_pChoiceGirlStagePI = 0;
	cMPDIList*l_pCommonMPDIList = cGameApp::GetMPDIListByFileName(L"Image/Common.mpdi");
	m_pScroller->SetCancelButton(l_pCommonMPDIList->GetObject(L"Back"));
	//m_pBackButton = new cImageButton(l_pCommonMPDIList->GetObject(L"Back"));
}

cChoiceGirlStagePhase::~cChoiceGirlStagePhase()
{
	SAFE_DELETE(m_pScroller);
}

void	cChoiceGirlStagePhase::Destroy()
{
	m_pScroller->Destroy();
	m_ResourceStamp.ReleaseNewResource();
	m_pChoiceGirlStagePI = 0;
}

void	cChoiceGirlStagePhase::ProcessChoiceGirlPhaseData(TiXmlElement*e_pTiXmlElement)
{
	assert(m_pChoiceGirlStagePI == 0);
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("PI")
		{
			m_pChoiceGirlStagePI = cGameApp::GetPuzzleImageByFileName(l_strValue);
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
//	<StageData Name="Stage2" Image="Stage2" Video="Stage2" GameSetupFileName="Stage2"/>
void	cChoiceGirlStagePhase::ProcessStageData(TiXmlElement*e_pTiXmlElement)
{
	cStageData*l_pStageData = new cStageData();
	m_StageDataVector.AddObjectNeglectExist(l_pStageData);
	std::wstring	l_strStageName;
	cPuzzleImageUnit*l_pPIUnit = 0;
	cPuzzleImageUnit*l_pPIUnitClone = 0;
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("Image")
		{
			if( m_pChoiceGirlStagePI )
			{
				l_pPIUnit = m_pChoiceGirlStagePI->GetObject(l_strValue);
				if( l_pPIUnit )
					l_pPIUnitClone = new cPuzzleImageUnit(l_pPIUnit);
				else
				{
					UT::ErrorMsg(L"l_strValue",L"is not exists at pi");
				}
			}
		}
		else
		COMPARE_NAME("Name")
		{
			l_pStageData->m_strStageMame = l_strValue;
		}
		else
		COMPARE_NAME("Video")
		{
			l_pStageData->m_strVideoName = l_strValue;
		}
		else
		COMPARE_NAME("GameSetupFileName")
		{
			l_pStageData->m_strGameSetupFileName = l_strValue;
		}
	PARSE_NAME_VALUE_END
	if( l_pPIUnitClone )
	{
		l_pPIUnitClone->SetName(l_pStageData->m_strStageMame.c_str());
		m_pScroller->AddObject(dynamic_cast<cBaseImage*>(l_pPIUnitClone),true);
	}
	else
	{//test code
		//cTextButton*l_pTextButton = new cTextButton(Vector2(l_vPos.x,l_vPos.y),cGameApp::m_spGlyphFontRender,Vector4(1,0,0,1),L"Test11111");
		//l_pTextButton->SetLocalPosition(l_vPos);
		//m_pScroller->AddData(l_pTextButton);
	}
}
//<ChoiceGirlStagePhase PI="Image/Girl/ChoiceGirl.pi">
//	<StageData Name="Stage1" Image="Stage1" Video="Stage1" GameSetupFileName="Stage1"/>
//	<StageData Name="Stage2" Image="Stage2" Video="Stage2" GameSetupFileName="Stage2"/>
//	<StageData Name="Stage3" Image="Stage3" Video="Stage3" GameSetupFileName="Stage3"/>
//</ChoiceGirlStagePhase>
void	cChoiceGirlStagePhase::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	COMPARE_VALUE("ChoiceGirlStagePhase")
	{
		ProcessChoiceGirlPhaseData(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("StageData")
	{
		ProcessStageData(e_pTiXmlElement);
	}
}

void	cChoiceGirlStagePhase::Init()
{
	m_bSatisfiedCondition = false;
	m_ResourceStamp.StampResource1();
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

void	cChoiceGirlStagePhase::Update(float e_fElpaseTime)
{
	if( m_pScroller )
	{
		if( m_pScroller->IsSatisfiedCondition() || m_pScroller->IsChancelButtonSatisfied())
		{
			this->m_bSatisfiedCondition = true;
			m_strCurrentSelectedGorlName = L"";
			this->Destroy();
		}
		else
			m_pScroller->Update(e_fElpaseTime);
	}
}

void	cChoiceGirlStagePhase::Render()
{
	if( m_pScroller )
		m_pScroller->Render();
}

void	cChoiceGirlStagePhase::DebugRender()
{
	if( m_pScroller )
		m_pScroller->DebugRender();
}

void	cChoiceGirlStagePhase::MouseDown(int e_iPosX,int e_iPosY)
{
	if( m_pScroller )
		m_pScroller->MouseDown(e_iPosX,e_iPosY);
}

void	cChoiceGirlStagePhase::MouseUp(int e_iPosX,int e_iPosY)
{
	if( m_pScroller )
		m_pScroller->MouseUp(e_iPosX,e_iPosY);
}

void	cChoiceGirlStagePhase::MouseMove(int e_iPosX,int e_iPosY)
{
	if( m_pScroller )
		m_pScroller->MouseMove(e_iPosX,e_iPosY);
}

const   WCHAR*	cChoiceGirlStagePhase::GetNextPhaseName()
{
	if(m_pScroller->IsChancelButtonSatisfied())
		return CHOICE_GIRL_PHASE_NAME;
	else
		return BLUFFING_DICEGAME_PHASE_NAME;
}

void*	cChoiceGirlStagePhase::GetData()
{
	return (void*)m_pScroller->GetSelectedIndexName();
}

void	cChoiceGirlStagePhase::FetchData(const WCHAR*e_strPhaseName,void*e_pData)
{
	if( e_pData )
	{
		const WCHAR*l_strSelectedGirlName = (const WCHAR*)e_pData;
		m_strCurrentSelectedGorlName = l_strSelectedGirlName;
		m_strFileName = "PhaseData/GirlStage/";
		m_strFileName += UT::WcharToChar(m_strCurrentSelectedGorlName.c_str());
		m_strFileName += ".xml";
	}
}