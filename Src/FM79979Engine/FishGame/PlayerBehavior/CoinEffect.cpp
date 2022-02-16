#include "stdafx.h"
#include "PlayerData.h"
#include "CoinEffect.h"

void cCoinEffect::LoadWinMoneyEffectMPDIData( WinMoneyEffectMPDIData& MPDIData )
{
	for ( int i = 0; i < MAX_COINEFFECT_COUNT; i++ )
	{
		 m_CoinEffect[i].Init();
		 m_CoinEffect[i].LoadCoinEffectAnimationData( MPDIData.MPDIListName, MPDIData.EffectMPDIName );
	}

	if ( !m_pNumMPDI )
	{
		cMPDIList *pMPDIList = cGameApp::GetMPDIListByFileName( MPDIData.NumerialMPDIListName );	

		if ( pMPDIList )
		{
			m_pNumMPDI = new cMPDI( pMPDIList->GetObject( MPDIData.NumerialMPDIName ) );

			if ( m_pNumMPDI )
			{
				m_pNumMPDI->SetParent(&this->m_Frame,false);
				//for ( int i = 0; i < m_pNumMPDI->Count(); i++ )
				//	 m_pNumMPDI->GetObject(i)->SetExtractTransform( &m_mat );

				m_pNumMPDI->Init();
			}
		}
	}
}

void cCoinEffect::ModifyNumMPDIData( int PlayerID )
{
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

void cCoinEffect::Update( float ElapseTime )
{
	int PlayCoinEffectAniCount(0);

	for ( int i = 0; i < MAX_COINEFFECT_COUNT; i++ )
	{
		 if ( !m_CoinEffect[i].GetAnimationIsDone() )
		 {
			 PlayCoinEffectAniCount++;
			 m_CoinEffect[i].Update( ElapseTime );
		 }
	}

	if ( m_pNumMPDI && !m_pNumMPDI->IsAnimationDone() )
		m_pNumMPDI->Update( ElapseTime );

	if ( PlayCoinEffectAniCount == 0 )
	{
		m_bIsDone  = true;
		m_bIsUsing = false;
	}
}

void cCoinEffect::Render()
{
	for ( int i = 0; i < MAX_COINEFFECT_COUNT; i++ )
		 if ( !m_CoinEffect[i].GetAnimationIsDone() )
			 m_CoinEffect[i].Render();

	if ( m_pNumMPDI && !m_pNumMPDI->IsAnimationDone() )
		m_pNumMPDI->Render();
}

void cCoinEffect::SetMoneyMPDICoinCount( int MoneyMPDICoinCount )
{
	float RotAngleZ = m_pPlayer->GetPlayerRotation().z;

	for ( int i = 0; i < MAX_COINEFFECT_COUNT; i++ )
	{
		 if ( i < MoneyMPDICoinCount )
		 {
			 cMatrix44 TempMatrix = this->m_Frame.GetWorldTransform();

			 if ( RotAngleZ == 0.f )
				 TempMatrix.m[3][0] += ( i * 20.f );
			 else if ( RotAngleZ == 90.f )
				 TempMatrix.m[3][1] -= ( i * 20.f );
			 else if ( RotAngleZ == 180.f )
				 TempMatrix.m[3][0] -= ( i * 20.f );
			 else if ( RotAngleZ == 270.f )
				 TempMatrix.m[3][1] += ( i * 20.f );

			 cMatrix44 TempTargetMatrix = cMatrix44::TranslationMatrix( m_pPlayer->GetPlayerPosition() ) * cMatrix44::RotationMatrix( m_pPlayer->GetPlayerRotation() );

			 if ( RotAngleZ == 0.f )
				 TempTargetMatrix.m[3][0] += ( i * 10.f );
			 else if ( RotAngleZ == 90.f )
				 TempTargetMatrix.m[3][1] -= ( i * 10.f );
			 else if ( RotAngleZ == 180.f )
				 TempTargetMatrix.m[3][0] -= ( i * 10.f );
			 else if ( RotAngleZ == 270.f )
				 TempTargetMatrix.m[3][1] += ( i * 10.f );

			 m_CoinEffect[i].SetCoinEffectMPDIData( TempMatrix, TempTargetMatrix, i * 0.05f );
		 }
	}
}