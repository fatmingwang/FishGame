#include "stdafx.h"
#include "JPEffect.h"
#include "NumerialRollingEffect.h"

cJPEffect::cJPEffect(int e_iJPIndex)
{
	cMPDIList*l_pMPDIList = cGameApp::GetMPDIListByFileName(FREE_GAME_JP_MPDILIST_FILE_NAME);
	if( e_iJPIndex == 1 )
	{
		m_pJPIn = l_pMPDIList->GetObject(FREE_GAME_JP_1_IN_MPDILIST_NAME);
		m_pJPAddMoney = l_pMPDIList->GetObject(FREE_GAME_JP_1_IDLE_MPDILIST_NAME);
	}
	else
	if( e_iJPIndex == 2 )
	{
		m_pJPIn = l_pMPDIList->GetObject(FREE_GAME_JP_2_IN_MPDILIST_NAME);
		m_pJPAddMoney = l_pMPDIList->GetObject(FREE_GAME_JP_2_IDLE_MPDILIST_NAME);
	}
	cPuzzleImage*l_pPI = m_pJPIn->GetPuzzleImage();
	m_pNumerialImage = l_pPI->GetNumerialImageByName(L"BG_00",L"BG_09");
	cMPDI*l_pNumerial = l_pMPDIList->GetObject(L"JackpotNum");
	m_vNumerialPos = Vector2(365,1160);
	if( l_pNumerial->GetObject(0) )
	{
		Vector3	l_vNumerialPos = l_pNumerial->GetObject(0)->GetOriginalPointList()[0];
		m_vNumerialPos.x = l_vNumerialPos.x;
		m_vNumerialPos.y = l_vNumerialPos.y;
	}
	m_bStart = false;
	m_pFixedRollingObjectManagerList = 0;
	//m_pFixedRollingObjectManagerList = new cFixedRollingObjectManagerList("JewelsKing/ImageData/JPNum.pi",Vector2(300,1095));
}

cJPEffect::~cJPEffect()
{
	SAFE_DELETE(m_pNumerialImage);
	SAFE_DELETE(m_pFixedRollingObjectManagerList);
}

void	cJPEffect::Init(int e_iMoney)
{
	if( m_pJPIn )
	{
		m_pJPIn->Init();
	}
	if( m_pJPAddMoney )
	{
		m_pJPAddMoney->Init();
	}
	this->m_iMoney = e_iMoney;
	m_bStart = true;
	cGameApp::m_spSoundParser->GetObject(JP_SOUND_NAME)->Play(true);
	if( m_pFixedRollingObjectManagerList )
		m_pFixedRollingObjectManagerList->SetStopResult(m_iMoney);
}

float	cJPEffect::GetCurrentProgress()
{
	return m_pJPAddMoney->GetCurrentProgress();
}

void	cJPEffect::Update(float e_fElpaseTime)
{
	if( !m_bStart )
		return;
	if( m_pFixedRollingObjectManagerList )
		m_pFixedRollingObjectManagerList->Update(e_fElpaseTime);
	if( m_pJPIn->IsAnimationDone() )
	{
		if( m_pJPAddMoney->IsAnimationDone() )
		{
			m_bStart = false;
		}
		else
			m_pJPAddMoney->Update(e_fElpaseTime);
	}
	else
	{
		m_pJPIn->Update(e_fElpaseTime);
	}
	
}

void	cJPEffect::Render()
{
	if( !m_bStart )
		return;
	if( m_pJPIn->IsAnimationDone() )
	{
		m_pJPAddMoney->Render();
	}
	else
		m_pJPIn->Render();
	if( m_pJPIn->GetCurrentProgress()>0.5f )
	{
		if( m_pNumerialImage )
			m_pNumerialImage->DrawOnCenter(m_iMoney,(int)m_vNumerialPos.x,(int)m_vNumerialPos.y);
	}
	if( m_pFixedRollingObjectManagerList )
		m_pFixedRollingObjectManagerList->Render();
}

bool	cJPEffect::IsDone()
{
	return !m_bStart;
}

void	cJPEffect::SetNumerial(int e_iValue)
{
	m_iMoney = e_iValue;
}