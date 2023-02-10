#include "stdafx.h"
#include "PlayerData.h"
#include "WinMoneyEffectBase.h"

cWinMoneyEffectBase::cWinMoneyEffectBase()
{
	m_pNumMPDI  = 0;
	m_pCoinMPDI = 0;

	CommonInitData();
}

cWinMoneyEffectBase::~cWinMoneyEffectBase()
{
	SAFE_DELETE( m_pNumMPDI );
	SAFE_DELETE( m_pCoinMPDI );
}

void cWinMoneyEffectBase::Init()
{
	CommonInitData();
	InternalInit();
}

void cWinMoneyEffectBase::CommonInitData()
{
	m_Frame.SetLocalTransform(cMatrix44::Identity);
	m_iMoney  = 0;
	m_pPlayer = 0;

	m_bIsDone  = true;
	m_bIsUsing = false;
}

void cWinMoneyEffectBase::SetData( cMatrix44 AniMatrix, int Money, cPlayerData* pPlayer )
{
	m_Frame.SetLocalTransform( AniMatrix );
	m_iMoney  = Money;
	m_pPlayer = pPlayer;

	m_bIsDone  = false;
	m_bIsUsing = true;

	ModifyNumMPDIData( m_pPlayer->GetPlayerID() );
}

void cWinMoneyEffectBase::SetNumMPDIPos( Vector3 NewPos )
{
	if ( m_pNumMPDI )
		m_pNumMPDI->SetPos( NewPos );
}