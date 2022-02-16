#include "stdafx.h"
#include "BigWin.h"

cBigWin::cBigWin(cMPDIList*e_pMPDIList)
{
	m_pBumpOut = e_pMPDIList->GetObject(L"BigWinIN");
	m_pBumpLoop = e_pMPDIList->GetObject(L"BigWinIdle");
}

cBigWin::~cBigWin()
{

}

void	cBigWin::Init()
{
	if(m_pBumpOut)
		m_pBumpOut->Init();
	if(m_pBumpLoop)
		m_pBumpLoop->Init();
}

bool	cBigWin::IsStart()
{
	return m_pBumpOut->IsStart();
}

void			cBigWin::Update(float e_fElpaseTime)
{
	if( m_pBumpOut->IsAnimationDone() )
	{
		m_pBumpLoop->Update(e_fElpaseTime);
	}
	else
	{
		m_pBumpOut->Update(e_fElpaseTime);
	}
}

void	cBigWin::Render()
{
	if( m_pBumpOut->IsAnimationDone() )
	{
		m_pBumpLoop->Render();
	}
	else
	{
		m_pBumpOut->Render();
	}
}

bool	cBigWin::IsAnimationDone()
{
	return m_pBumpLoop->IsAnimationDone();
}