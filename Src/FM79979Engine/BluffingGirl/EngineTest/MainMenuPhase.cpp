#include "stdafx.h"
//#include <jni.h>
#include "MainMenuPhase.h"
#include "AllPhaseDefine.h"
#include "IABSetup.h"

cMainMenuPhase::cMainMenuPhase(const char*e_strMainMenuFileName)
{
	this->SetName(MAIN_PHASE_NAME);
	m_pMPFIList = 0;
	m_pBGMPDI = 0;
	m_strFileName = e_strMainMenuFileName;
	m_SelectionButton.SetAllObjectsDoneToSatisfied(false);
}

cMainMenuPhase::~cMainMenuPhase()
{
	Destroy();
}

void	cMainMenuPhase::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	COMPARE_VALUE("PhaseData")
	{
		ProcessPhaseData(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("Selection")
	{
		ProcessSelection(e_pTiXmlElement);
	}
}

void	cMainMenuPhase::ProcessPhaseData(TiXmlElement*e_pTiXmlElement)
{
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("MPDIListFileName")
		{
			m_pMPFIList = cGameApp::GetMPDIListByFileName(l_strValue);
			if( !m_pMPFIList )
			{
				UT::ErrorMsg(l_strValue,L"MPDIFile parse failed!");
			}
		}
		else
		COMPARE_NAME("BG")
		{
			if( m_pMPFIList )
			{
				m_pBGMPDI = m_pMPFIList->GetObject(l_strValue);
			}
		}
		else
		COMPARE_NAME("ClickSound")
		{
			m_pClickSound = cGameApp::GetSoundByFileName(l_strValue);
		}
	PARSE_NAME_VALUE_END
}

void	cMainMenuPhase::ProcessSelection(TiXmlElement*e_pTiXmlElement)
{
	if( !m_pMPFIList )
		return;
	cMPDI*l_MPDI = 0;
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("MPDI")
		{
			l_MPDI = m_pMPFIList->GetObject(l_strValue);
		}
		else
		COMPARE_NAME("SubMPDI")
		{
			if( l_MPDI )
			{
				cSubMPDI*l_pSubMPDI = l_MPDI->GetObject(l_strValue);
				if( l_pSubMPDI )
				{
					cImageButton*l_pImageButton = new cImageButton(l_pSubMPDI);
					m_SelectionButton.AddObjectNeglectExist(l_pImageButton);
				}
			}
		}
	PARSE_NAME_VALUE_END
}

void	cMainMenuPhase::Init()
{
	m_bSatisfiedCondition = false;
	Destroy();
	m_ResourceStamp.StampResource1();
	m_pMPFIList = 0;
	m_pBGMPDI = 0;
	if(!this->Parse(m_strFileName.c_str()))
	{
		UT::ErrorMsg(m_strFileName.c_str(),"parse failed!!");
	}
	m_SelectionButton.Init();
	if( m_pBGMPDI )
		m_pBGMPDI->Init();
	m_ResourceStamp.StampResource2();
	//m_ResourceStamp.ReleaseNewResource();
}
void	cMainMenuPhase::Destroy()
{
	m_SelectionButton.Destroy();
	m_ResourceStamp.ReleaseNewResource();
	m_pBGMPDI = 0;
	m_pMPFIList = 0;
}
bool g_bFristTimeInto = true;
void	cMainMenuPhase::Update(float e_fElpaseTime)
{
	if( m_pBGMPDI )
		m_pBGMPDI->Update(e_fElpaseTime);
	m_SelectionButton.Update(e_fElpaseTime);
	if( m_SelectionButton.IsSatisfiedCondition() )
	{
		if( !m_SelectionButton.GetCurrentWorkingObject() )
		{
			return;
		}
		bool	l_bIntoStore = false;
		const WCHAR*l_strName = m_SelectionButton.GetCurrentWorkingObject()->GetName();
		if( !wcscmp(l_strName,CHOICE_GIRL_PHASE_NAME) )
		{
			m_strNextPhaseName = CHOICE_GIRL_PHASE_NAME;
		}
		else
		if( !wcscmp(l_strName,FB_PHASE_NAME) )
		{
			m_strNextPhaseName = FB_PHASE_NAME;
		}
		else
		if( !wcscmp(l_strName,SETUP_PHASE_NAME) )
		{
			m_strNextPhaseName = SETUP_PHASE_NAME;
		}
		else
		if( !wcscmp(l_strName,STORE_PHASE_NAME) )
		{
			l_bIntoStore  =true;
			m_strNextPhaseName = STORE_PHASE_NAME;
		}
		if( !l_bIntoStore  )
		{
			Destroy();
		}
		m_SelectionButton.Init();			
		this->m_bSatisfiedCondition = true;
	}
}
//it would be called if the stage is changed.
void	cMainMenuPhase::Render()
{
	glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);
	if( m_pBGMPDI )
		m_pBGMPDI->Render();
	m_SelectionButton.Render();
	GLRender::glDisable2D();
}

void	cMainMenuPhase::DebugRender()
{
	m_SelectionButton.DebugRender();
}

void	cMainMenuPhase::MouseDown(int e_iX,int e_iY)
{
	m_SelectionButton.MouseDown(e_iX,e_iY);
}

void	cMainMenuPhase::MouseUp(int e_iX,int e_iY)
{
	m_SelectionButton.MouseUp(e_iX,e_iY);
	if( m_SelectionButton.GetCurrentWorkingObject() )
	{
		if( m_pClickSound )m_pClickSound->Play(true);
	}
}

void	cMainMenuPhase::MouseMove(int e_iX,int e_iY)
{
	m_SelectionButton.MouseMove(e_iX,e_iY);
}

const   WCHAR*	cMainMenuPhase::GetNextPhaseName()
{
	return m_strNextPhaseName.c_str();
}

void*	cMainMenuPhase::GetData()
{
	return 0;
}

void	cMainMenuPhase::KeyDown(char e_char)
{
	switch(e_char)
	{
		case 37://left

			break;
		case 38://up

			break;
		case 39://right

			break;
		case 40://down

			break;
	}

}