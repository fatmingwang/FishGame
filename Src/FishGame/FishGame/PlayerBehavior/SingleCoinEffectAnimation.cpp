#include "stdafx.h"
#include "SingleCoinEffectAnimation.h"

SingleCoinEffectAnimation::SingleCoinEffectAnimation()
{
	m_AniDelayTime    = 0.f;
	m_MoveRemainTIme  = 0.f;
	m_bPlayAnimation  = false;
	m_pCoinEffectMPDI = 0;

	m_CoinEffectFrame.SetLocalTransform(cMatrix44::Identity);

	m_AniOriPos = cMatrix44::Identity;
	m_AniEndPos = cMatrix44::Identity;
}

SingleCoinEffectAnimation::~SingleCoinEffectAnimation()
{
	 SAFE_DELETE( m_pCoinEffectMPDI );
}

void SingleCoinEffectAnimation::CalculatorAnimationPos( float ElapseTime )
{
	Vector3 Pos       = m_CoinEffectFrame.GetWorldPosition();
	Vector3 TargetPos = m_AniEndPos.GetTranslation();

	if ( m_MoveRemainTIme > 0.f )
	{
		float NowPosX, NowPosY, Min, Max, Result;

		NowPosX = Pos.x;

		if ( NowPosX != TargetPos.x )
		{
			Result = NowPosX + ( ( TargetPos.x - NowPosX ) * ( ElapseTime / m_MoveRemainTIme ) );

			if ( NowPosX <= TargetPos.x )
			{
				Min = NowPosX;
				Max = TargetPos.x; 
			}
			else
			{
				Min = TargetPos.x;
				Max = NowPosX; 
			}

			Pos.x = Result < Min ? Min : Result < Max ? Result : Max;
		}

		NowPosY = Pos.y;

		if ( NowPosY != TargetPos.y )
		{
			Result = NowPosY + ( ( TargetPos.y - NowPosY ) * ( ElapseTime / m_MoveRemainTIme ) );

			if ( NowPosY <= TargetPos.y )
			{
				Min = NowPosY;
				Max = TargetPos.y; 
			}
			else
			{
				Min = TargetPos.y;
				Max = NowPosY; 
			}

			Pos.y = Result < Min ? Min : Result < Max ? Result : Max;
		}

		m_CoinEffectFrame.SetLocalPosition( Pos );

		m_MoveRemainTIme -= ElapseTime;
	}
	else
	{
		m_AniDelayTime   = 0.f;
		m_bPlayAnimation = false;
	}
}

void SingleCoinEffectAnimation::Update( float ElapseTime )
{
	if ( m_bPlayAnimation )
	{
		if ( m_AniDelayTime > 0.f )
			m_AniDelayTime -= ElapseTime;
		else
		{
			if ( m_pCoinEffectMPDI->IsAnimationDone() )
				CalculatorAnimationPos( ElapseTime );

			m_pCoinEffectMPDI->Update( ElapseTime );
		}
	}
}

void SingleCoinEffectAnimation::Render()
{
	if ( m_bPlayAnimation )
	{
		if ( m_AniDelayTime <= 0.f )
			m_pCoinEffectMPDI->Render();
	}
}

void SingleCoinEffectAnimation::LoadCoinEffectAnimationData( const WCHAR* MPDIListName, const WCHAR* MPDIName )
{
	if ( !m_pCoinEffectMPDI )
	{
		 cMPDIList *pMPDIList = cGameApp::GetMPDIListByFileName( MPDIListName );

		 m_pCoinEffectMPDI = new cMPDI( pMPDIList->GetObject( MPDIName ) );

		 if ( m_pCoinEffectMPDI )
		 {
			 m_pCoinEffectMPDI->SetParent(&m_CoinEffectFrame,false);
			 //for ( int i = 0; i < m_pCoinEffectMPDI->Count(); i++ )
			//	  m_pCoinEffectMPDI->GetObject(i)->SetExtractTransform( &m_CoinEffectFrame );
		 }
	}
}

void SingleCoinEffectAnimation::SetCoinEffectMPDIData( cMatrix44& AniOriPos, cMatrix44& AniEndPos, float AniDelayTime )
{
	if ( m_pCoinEffectMPDI )
	{
		Vector3 LengthVector = AniOriPos.GetTranslation() - AniEndPos.GetTranslation();

		float Length = LengthVector.Length();

		m_AniDelayTime   = AniDelayTime;
		m_bPlayAnimation = true;
		m_MoveRemainTIme = 1.5f * ( Length / cGameApp::m_svGameResolution.x );

		m_AniOriPos        = AniOriPos;
		m_AniEndPos        = AniEndPos;
		m_CoinEffectFrame.SetLocalTransform( AniOriPos) ;

	 	m_pCoinEffectMPDI->Init();
	}
}

