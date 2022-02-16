#include "stdafx.h"
#include "DownScreen.h"
#include "BigWin.h"
#include "ShowLineBetMoney.h"
cDownScreenEffect::cDownScreenEffect()
{
	m_pGameMessage = new cGameMessage();
	m_pBigWinMoneyMPDI = 0;
	m_pCoinFountain = 0;
	if(cGameApp::m_spPaticleManager->Parse(COIN_FOUNTAIN))
		m_pCoinFountain = cGameApp::m_spPaticleManager->GetObject(L"Coin");
	cMPDIList*l_pMPDIList = cGameApp::GetMPDIListByFileName(BIG_WIN_MPDILIST_FILE_NAME);
	m_pBigWinMoneyMPDI = new cBigWin(l_pMPDIList);
	m_pShowLineBetMoney = new cShowLineBetMoney();
	m_pShowLineBetMoney->Init();
}
cDownScreenEffect::~cDownScreenEffect()
{
	SAFE_DELETE(m_pShowLineBetMoney);
	SAFE_DELETE(m_pBigWinMoneyMPDI);
	SAFE_DELETE(m_pGameMessage);
}

void	cDownScreenEffect::ChangeStatusByMoney(int e_iMoney)
{
	if(e_iMoney>=2000)
	{
		if( m_pCoinFountain )
		{
			m_pCoinFountain->Emit(Vector3(cGameApp::m_svGameResolution.x/2.f,1225.f,0.f),false);
			cGameApp::m_spSoundParser->GetObject(COIN_FOUNTAIN_SOUND_NAME)->Play(true);
		}
		if( m_pBigWinMoneyMPDI )
			m_pBigWinMoneyMPDI->Init();
		this->m_pGameMessage->SetBigWin();
	}
}

void	cDownScreenEffect::Update(float e_fElpaseTime)
{
	if( m_pGameMessage )
		m_pGameMessage->Update(e_fElpaseTime);
	if( m_pCoinFountain )
		m_pCoinFountain->Update(e_fElpaseTime);
	if( m_pBigWinMoneyMPDI )
		m_pBigWinMoneyMPDI->Update(e_fElpaseTime);
	if( m_pShowLineBetMoney )
		m_pShowLineBetMoney->Update(e_fElpaseTime);
}
void	cDownScreenEffect::Render()
{
	if( m_pBigWinMoneyMPDI )
		m_pBigWinMoneyMPDI->Render();
	if( m_pCoinFountain )
		m_pCoinFountain->Render();
	if( m_pGameMessage )
		m_pGameMessage->Render();
}

bool	cDownScreenEffect::IsBigWinDone()
{
	if( m_pBigWinMoneyMPDI->IsStart() )
	{
		return m_pBigWinMoneyMPDI->IsAnimationDone();
	}
	return true;
}

void	cDownScreenEffect::DrawPerLineBetMoney()
{
	if( m_pShowLineBetMoney )
		m_pShowLineBetMoney->Render();
}