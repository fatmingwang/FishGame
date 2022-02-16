#include "stdafx.h"
#include "PrizeEffect.h"

void PrizeEffect::LoadWinMoneyEffectMPDIData( WinMoneyEffectMPDIData& MPDIData )
{
	if ( !m_pCoinMPDI )
	{
		cMPDIList *pMPDIList = cGameApp::GetMPDIListByFileName( MPDIData.MPDIListName );	

		if ( pMPDIList )
		{
			m_pCoinMPDI = new cMPDI( pMPDIList->GetObject( MPDIData.EffectMPDIName ) );

			if ( m_pCoinMPDI )
			{
				m_pCoinMPDI->SetParent(&m_Frame,false);
				//for ( int i = 0; i < m_pCoinMPDI->Count(); i++ )
				//	 m_pCoinMPDI->GetObject(i)->SetExtractTransform( &m_mat );

				m_pCoinMPDI->Init();
			}
		}
	}

	if ( !m_pNumMPDI )
	{
		cMPDIList *pMPDIList = cGameApp::GetMPDIListByFileName( MPDIData.NumerialMPDIListName );	

		if ( pMPDIList )
		{
			m_pNumMPDI = new cMPDI( pMPDIList->GetObject( MPDIData.NumerialMPDIName ) );

			if ( m_pNumMPDI )
			{
				m_pNumMPDI->SetParent(&m_Frame,false);
				//for ( int i = 0; i < m_pNumMPDI->Count(); i++ )
				//	 m_pNumMPDI->GetObject(i)->SetExtractTransform( &m_mat );

				m_pNumMPDI->Init();
			}
		}
	}
}

void PrizeEffect::ModifyNumMPDIData( int PlayerID )
{
	if ( m_pCoinMPDI )
		m_pCoinMPDI->Init();

	if ( m_pNumMPDI )
	{
		m_pNumMPDI->Init();

		const int NumSize = 10;

		int NumAmount(0);
		int TempMoney = m_iMoney;
		int Number[ NumSize ] = {0};

		while ( TempMoney )
		{
			Number[ NumAmount++ ] = TempMoney % 10;
			TempMoney /= 10;
		}

		for ( int i = 0; i < NumSize; i++ )
		{
			 WCHAR NumMPDINmae[250] = {0};

			 wsprintf( NumMPDINmae, L"Player%dCoinScore%d", PlayerID + 1, Number[i] );

			 WCHAR PosNmae[250] = {0};

			 wsprintf( PosNmae, L"Player10CoinScore%03d", i );

			 int NumIdx = m_pNumMPDI->GetPuzzleImage( L"CoinScore" )->GetObjectIndexByName( NumMPDINmae );

			 m_pNumMPDI->GetObject( PosNmae )->ChangeAllImageByImageIndex( NumIdx );
			 m_pNumMPDI->GetObject( PosNmae )->GetCurrentPointData()->iImageIndex = NumIdx;

			 if ( i > 0 && i >= NumAmount )
				 m_pNumMPDI->GetObject( PosNmae )->SetCurrentTime( 999999.f );
		}
	}
}

void PrizeEffect::Update( float ElapseTime )
{
	if ( m_pCoinMPDI )
	{
		m_pCoinMPDI->Update( ElapseTime );

		if ( m_pCoinMPDI->IsAnimationDone() )
		{
			m_bIsDone  = true;
			m_bIsUsing = false;
		}
	}

	if ( m_pNumMPDI && m_pNumMPDI->GetCurrentProgress() <= 0.7f )
		m_pNumMPDI->Update( ElapseTime );
}

void PrizeEffect::Render()
{
	if ( m_pCoinMPDI )
		m_pCoinMPDI->Render();

	if ( m_pNumMPDI )
		m_pNumMPDI->Render();
}