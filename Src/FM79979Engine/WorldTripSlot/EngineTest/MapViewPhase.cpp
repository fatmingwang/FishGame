#include "stdafx.h"
#include "MapViewPhase.h"
#include "AllPhaseDefine.h"

cMapViewPhase::cMapViewPhase(const char*e_strMainMenuFileName)
{
	this->SetName(MAP_VIEW_PHASE);
	m_pMPFIList = 0;
	m_pBGMPDI = 0;
	m_strFileName = e_strMainMenuFileName;
	m_SelectionButton.SetAllObjectsDoneToSatisfied(false);
}

cMapViewPhase::~cMapViewPhase()
{
	if( m_pMPFIList )
		cGameApp::m_spAnimationParser->RemoveObject(m_pMPFIList->GetName());
}

void	cMapViewPhase::HandleElementData(TiXmlElement*e_pTiXmlElement)
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

void	cMapViewPhase::ProcessPhaseData(TiXmlElement*e_pTiXmlElement)
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

void	cMapViewPhase::ProcessSelection(TiXmlElement*e_pTiXmlElement)
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

void	cMapViewPhase::Init()
{
	m_strSelectedStageName = L"";
	Destroy();
	m_pBGMPDI = 0;
	if(!this->Parse(m_strFileName.c_str()))
	{
		UT::ErrorMsg(m_strFileName.c_str(),"parse failed!!");
	}
	m_SelectionButton.Init();
	if( m_pBGMPDI )
		m_pBGMPDI->Init();
}
void	cMapViewPhase::Destroy()
{
	if( m_pMPFIList )
	{
		cGameApp::m_spAnimationParser->RemoveObject(m_pMPFIList->GetName());
		m_pMPFIList = 0;
	}
}
void	cMapViewPhase::Update(float e_fElpaseTime)
{
	if( m_pBGMPDI )
		m_pBGMPDI->Update(e_fElpaseTime);
	m_SelectionButton.Update(e_fElpaseTime);
	if( m_SelectionButton.IsSatisfiedCondition() )
	{
		int	l_iIndex = m_SelectionButton.GetCurrentWorkingObjectIndex();
		m_strSelectedStageName = m_SelectionButton.GetCurrentWorkingObject()->GetName();
		m_SelectionButton.Init();
		Destroy();
		this->m_bSatisfiedCondition = true;
	}
}
//it would be called if the stage is changed.
void	cMapViewPhase::Render()
{
	glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);
	if( m_pBGMPDI )
		m_pBGMPDI->Render();
	m_SelectionButton.Render();
	GLRender::glDisable2D();
}

void	cMapViewPhase::DebugRender()
{
	m_SelectionButton.DebugRender();
}

void	cMapViewPhase::MouseDown(int e_iX,int e_iY)
{
	m_SelectionButton.MouseDown(e_iX,e_iY);
}

void	cMapViewPhase::MouseUp(int e_iX,int e_iY)
{
	m_SelectionButton.MouseUp(e_iX,e_iY);
	if( m_SelectionButton.GetCurrentWorkingObject() )
	{
		if( m_pClickSound )m_pClickSound->Play(true);
	}
}

void	cMapViewPhase::MouseMove(int e_iX,int e_iY)
{
	m_SelectionButton.MouseMove(e_iX,e_iY);
}

const   WCHAR*	cMapViewPhase::GetNextPhaseName()
{
	return SLOT_GAME_PHASE;
}

void*	cMapViewPhase::GetData()
{
	return (void*)&m_strSelectedStageName;
}

void	cMapViewPhase::KeyDown(char e_char)
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