#include "stdafx.h"
#include "CoinStack.h"

CoinStack::CoinStack()
{
	m_Money               = 0;
	m_FishID			  = 0;
	m_PosRot			  = 0.f;
	m_pFishCard			  = 0;	
	m_ShowFrame.SetLocalTransform(cMatrix44::Identity);
	m_MoveRemainTm		  = 0.f;
	m_pStackScoreNumerial = 0;

	m_bSetData       = false;
	m_bUseGrayColor  = false;
	m_bCalculatorPos = false;

	m_TargetPos    = Vector3( 0.f, 0.f, 0.f );
	m_DrawMoneyPos = Vector3( 0.f, 0.f, 0.f );
}

void CoinStack::Update( float ElapseTime )
{
	if ( m_pFishCard )
	{
		if ( m_bCalculatorPos && m_MoveRemainTm > 0.f )
		{
			Vector3 Pos = m_pFishCard->GetPos();

			if ( Pos.x != m_TargetPos.x )
			{
				float Min, Max, Result = Pos.x + ( ( m_TargetPos.x - Pos.x ) * ( ElapseTime / m_MoveRemainTm ) );

				if ( Pos.x <= m_TargetPos.x )
				{
					Min = Pos.x;
					Max = m_TargetPos.x; 
				}
				else
				{
					Min = m_TargetPos.x;
					Max = Pos.x; 
				}

				Pos.x = Result < Min ? Min : Result < Max ? Result : Max;
				m_MoveRemainTm -= ElapseTime;

				m_pFishCard->SetPos( Pos );
			}
		}

		m_pFishCard->GetObject(0)->SetColor( m_bUseGrayColor ? Vector4( 0.5f, 0.5f, 0.5f, 0.5f ) : Vector4( 1.f, 1.f, 1.f, 1.f ) );

		m_pFishCard->Update( m_bCalculatorPos ? ElapseTime : 0.00000001f );
	}
}

void CoinStack::Render()
{
	if ( m_pFishCard )
	{
		m_pFishCard->Render();

		if ( m_pStackScoreNumerial )
		{
			Vector4 *pTempColor = m_pStackScoreNumerial->GetColor();

			pTempColor->a = m_pFishCard->GetObject(0)->GetCurrentPointData()->vColor.a;

			m_pStackScoreNumerial->SetColor( *pTempColor );

			Vector3 NumTexPos = m_pFishCard->GetPos();

			if ( m_PosRot == 90.f )
			{
				float Temp = NumTexPos.x;

				NumTexPos.x = NumTexPos.y;
				NumTexPos.y = Temp;
			}
			else if ( m_PosRot == 180.f )
			{
				NumTexPos.x *= -1;
				NumTexPos.y = 0;
			}
			else if ( m_PosRot == 270.f )
			{
				NumTexPos.x *= -1;

				float Temp = NumTexPos.x;

				NumTexPos.x = NumTexPos.y;
				NumTexPos.y = Temp;
			}
			else
				NumTexPos.y = 0;

			cMatrix44 TempMatrix = cMatrix44::TranslationMatrix( NumTexPos );

			Vector3 DrawNumPos = TempMatrix.TransformCoordinate( m_DrawMoneyPos );

			m_pStackScoreNumerial->DrawOnCenter( m_Money, (int) DrawNumPos.x, (int) DrawNumPos.y );
		}
	}
}

void CoinStack::SetCoinStackData( cMPDI* pFishCardImage, cNumeralImage* pNumerialImage, float MoveRemainTime )
{
	 if ( !m_pFishCard && pFishCardImage && pNumerialImage )
	 {
		 m_pFishCard = pFishCardImage;

		 m_pFishCard->SetParent(&m_ShowFrame,false);
		 //for ( int i = 0; i < m_pFishCard->Count(); i++ )
		//	  m_pFishCard->GetObject(i)->SetExtractTransform( &m_ShowFrame );

		 m_pFishCard->Init();

		 m_pStackScoreNumerial = pNumerialImage;

		 m_pStackScoreNumerial->SetAngle( m_PosRot );

		 m_bSetData     = true;
		 m_MoveRemainTm = MoveRemainTime;
	 }
}

