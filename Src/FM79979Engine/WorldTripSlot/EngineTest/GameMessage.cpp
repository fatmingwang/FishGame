#include "stdafx.h"
#include "GameMessage.h"


cGameMessage::cGameMessage()
{
	WCHAR*l_str[] = {L"GameMessageIdle",L"GameMessageFreeTime",L"GameMessageSpin",L"GameMessageBigWin",L"GameMessagePleaseInsertCoin"};
	cMPDIList*l_pMPDIList = cGameApp::GetMPDIListByFileName(UI_MPDILIST_FILE_NAME);
	m_Status.SetFromResource(true);
	for( int i=0;i<eGM_MAX;++i )
	{
		cMPDI*l_pSubMPDI = l_pMPDIList->GetObject(l_str[i]);
		assert(l_pSubMPDI);
		m_Status.AddObjectNeglectExist(l_pSubMPDI);
	}
	SetIdle();
}
cGameMessage::~cGameMessage()
{

}

void		cGameMessage::SetInsertCoin()
{
	m_eGameMessage = eGM_INSERT_COIN;
	m_Status.SetCurrentWorkingObject((int)m_eGameMessage);
}

void		cGameMessage::SetIdle()
{
	m_eGameMessage = eGM_IDLE;
	m_Status.SetCurrentWorkingObject((int)m_eGameMessage);
}

void		cGameMessage::SetFreeTime(int e_iTime)
{
	m_eGameMessage = eGM_FREE_TIME;
	m_Status.SetCurrentWorkingObject((int)m_eGameMessage);
	m_Status.GetCurrentWorkingObject()->Update( e_iTime*0.1f );
}

void		cGameMessage::SetBigWin()
{
	m_eGameMessage = eGM_BIG_WIN;
	m_Status.SetCurrentWorkingObject((int)m_eGameMessage);
}

void		cGameMessage::SetSpin()
{
	m_eGameMessage = eGM_SPIN;
	m_Status.SetCurrentWorkingObject((int)m_eGameMessage);
}

void		cGameMessage::Update(float e_fElpaseTime)
{
	if(m_eGameMessage == eGM_IDLE)
		m_Status.Update(e_fElpaseTime);
}

void		cGameMessage::Render()
{
	m_Status.Render();
}