#include "stdafx.h"
#include "MonsterManager.h"
#include "FishNormal.h"
#include "FishMultiCollision.h"
#include "FishGroup.h"
#include "../GameApp/GameApp.h"
#include "../ProbabilityFish/FishProbability.h"
//
cFAIBehaviorParser *cMonsterManager::m_spFAIBehaviorParser = 0;
const float	g_fMonsterHittedHintTime = 1.f;

eBodyType	GetBodyType(const WCHAR*e_str)
{
	if( !wcscmp( e_str, L"Small" ) )
	{
		return eBT_SMALLE;
	}
	else
	if( !wcscmp( e_str, L"Medium" ) )
	{
		return eBT_MIDIUM;
	}
	else
	if( !wcscmp( e_str, L"Big" ) )
	{
		return eBT_BIG;
	}
	UT::ErrorMsg( e_str, L"ErrorBodyType" );
	return eBT_MAX;
}

int	NewPrizeWithRule(int e_iGap,int e_iStartScore,int e_iScore)
{
	if( e_iScore >= e_iStartScore )
	{
		if ( e_iScore % e_iGap  )
			e_iScore -= e_iScore % e_iGap;
	}
	else
	{
		if ( e_iScore % 10  )
			e_iScore -= e_iScore % 10;
	}
	return e_iScore;
}

cMonster::cMonster()
{
	m_fScale = 1.f;
	m_fAliveTime = 0.f;
	m_iCurrentFrameBiteMoney = 0;
	m_iTotalBiteMoney = 0;
	m_vHittedHintColor = Vector4::Zero;
	m_eBodyType = eBT_MAX;
	m_bMiniGameMonster = false;
	m_pHittedPlayer = 0;
	m_pBullet = 0;
	m_fRadius = 100.f;
	m_eMonsterStatus = eMS_NONE;
	m_eMonsterType = eMT_MAX;

	m_piPrize = 0;
	m_pFloatingPrize = 0;

	m_fProbabilityToDied = 0.5f;
	m_fRestProbabilityValueToDied = 1.f;
	m_HittedHintTC.SetTargetTime(g_fMonsterHittedHintTime);
	m_HittedHintTC.bTragetTimrReached = true;
	m_iID = -1;
}





cMonster::cMonster( cMonster *e_pMonster )
{
	m_strDiedPrizeAnimationName = e_pMonster->m_strDiedPrizeAnimationName;
	m_strPrizeAnimationName = e_pMonster->m_strPrizeAnimationName;
	m_strPrizeParticleName = e_pMonster->m_strPrizeParticleName;
	m_fScale = e_pMonster->m_fScale;
	m_fAliveTime = 0.f;
	m_iCurrentFrameBiteMoney = 0;
	m_iTotalBiteMoney = 0;
	m_vHittedHintColor = Vector4::Zero;
	m_eBodyType = e_pMonster->m_eBodyType;
	m_bMiniGameMonster = false;
	SetName(e_pMonster->GetName());
	m_pBullet = 0;
	m_pHittedPlayer = 0;
	m_iID = e_pMonster->m_iID;
	m_eMonsterStatus = eMS_NONE;
	m_fRadius = e_pMonster->m_fRadius;
	m_eMonsterType = e_pMonster->m_eMonsterType;
	m_piPrize = 0;
	m_pFloatingPrize = 0;
	if( e_pMonster->m_piPrize )
	{
		m_piPrize = new int;
		*m_piPrize = *e_pMonster->m_piPrize;
	}
	else
	{
		m_pFloatingPrize = new sMinMaxData<int>;
		*m_pFloatingPrize = *e_pMonster->m_pFloatingPrize;
	}

	m_fProbabilityToDied = e_pMonster->m_fProbabilityToDied;
	m_fRestProbabilityValueToDied = 1.f;
	m_HittedHintTC.SetTargetTime(e_pMonster->m_HittedHintTC.fTargetTime);
	m_HittedHintTC.bTragetTimrReached = true;
}








cMonster::~cMonster()
{
	SAFE_DELETE(m_piPrize);
	SAFE_DELETE(m_pFloatingPrize);
}








void	cMonster::Init()
{
	m_bMiniGameMonster = false;
	m_fAliveTime = 0.f;
	m_pHittedPlayer = 0;
	m_iTotalBiteMoney = 0;
	m_iCurrentFrameBiteMoney = 0;
	m_eMonsterStatus = eMS_ALIVE;
	m_fRestProbabilityValueToDied = 1.f;
	m_pBullet = 0;
	m_vHittedHintColor = Vector4::One;
	InternalInit();
}

unsigned int	g_iCurrentHittedColorIndex = 0;
Vector4	g_vHittedHintColor[10] = {Vector4(0.5f,0.5f,0.5f,1.0f),
								 Vector4(1.0f,1.0f,1.0f,1.0f),
								 Vector4(1.0f,1.0f,1.0f,1.0f),
								 Vector4(1.0f,1.0f,1.0f,1.0f),
								 Vector4(1.0f,1.0f,1.0f,1.0f),
								 Vector4(1.0f,1.0f,1.0f,1.0f),
								 Vector4(1.0f,1.0f,1.0f,1.0f),
								 Vector4(1.0f,1.0f,1.0f,1.0f),
								 Vector4(1.0f,1.0f,1.0f,1.0f),
								 Vector4(1.0f,1.0f,1.0f,1.0f)
								};








bool	cMonster::IsCollide( cMonster *e_pMonster )
{
	sSphere	l_sCircle1(e_pMonster->GetPos(),e_pMonster->GetRadius());
	sSphere	l_sCircle2(GetPos(),m_fRadius);
	if(UT::SphereCollideSphere(l_sCircle1,l_sCircle2))
		return true;
	return false;
}








void	cMonster::SetMiniGameMonster(bool e_b,const WCHAR*e_strName)
{
	m_bMiniGameMonster = e_b;
	m_strMiniGameName = e_strName;
}








bool	cMonster::IsMiniGameMonster()
{
	return m_bMiniGameMonster;
}








void	cMonster::Update(float e_fElpaseTime)
{
	if( m_eMonsterStatus == eMS_WAITING_FOR_CLEAN )
	{
		return;
	}
	m_fAliveTime += e_fElpaseTime;
	//if(m_eMonsterStatus == eMS_STRUGGLE)
	//	e_fElpaseTime /= 3.f;
	if( m_eMonsterStatus == eMS_HITTED || m_eMonsterStatus == eMS_STRUGGLE )
	{
		if( !m_HittedHintTC.bTragetTimrReached )
		{
			//give a chance to play struggle
			if( m_HittedHintTC.fRestTime == g_fMonsterHittedHintTime )
			{
				int	l_iChanceToBeStruggle = 20;
				if( this->GetPrize() > l_iChanceToBeStruggle )
				{
					l_iChanceToBeStruggle = this->GetPrize();
				}
				if( rand()%l_iChanceToBeStruggle == 0 )
				{
					m_eMonsterStatus = eMS_STRUGGLE;
				}
			}
			m_HittedHintTC.Update(e_fElpaseTime);
			++g_iCurrentHittedColorIndex;
			if( g_iCurrentHittedColorIndex>=10 )
				g_iCurrentHittedColorIndex = 0;
			m_vHittedHintColor = g_vHittedHintColor[g_iCurrentHittedColorIndex];
		}
		else
		{
			m_vHittedHintColor = Vector4::One;
			m_eMonsterStatus = eMS_ALIVE;
		}
	}
	else
	{
	}
	m_iTotalBiteMoney += m_iCurrentFrameBiteMoney;
	m_iCurrentFrameBiteMoney = 0;
	InternalUpdate(e_fElpaseTime);
}

void	cMonster::AddBiteMoney(int e_iMoney)
{
	m_iCurrentFrameBiteMoney += e_iMoney;
}








int		cMonster::GetPrize()
{
	if( m_piPrize )
	{
		return *m_piPrize;
	}
	int	l_iPrize = m_pFloatingPrize->Rand();
	return NewPrizeWithRule(50,100,l_iPrize);
}








bool	cMonster::IsMiniGameShotOkToDied()
{
	if( !IsStatusAllowToDied() || this->m_bMiniGameMonster || !this->IsVisible() )
	{
		return false;
	}
	//
	return true;
}








bool	cMonster::IsStatusAllowToDied()
{
	if( m_eMonsterStatus == eMS_WAITING_FOR_CLEAN || m_eMonsterStatus == eMS_DIED_SHOW
		|| m_eMonsterStatus == eMS_NONE || m_eMonsterStatus == eMS_MAX )
	{
		return false;
	}
	//
	return true;
}








void	cMonster::SetMonsterStatus( eMonsterStatus e_eMonsterStatus )
{
	if( m_eMonsterStatus == eMS_STRUGGLE && e_eMonsterStatus == eMS_HITTED )
	{
	
	}
	else
	{
		m_eMonsterStatus = e_eMonsterStatus;
	}

	if( m_eMonsterStatus == eMS_HITTED )
	{
		m_HittedHintTC.Start();
	}
	else 
	if ( m_eMonsterStatus == eMS_DIED_SHOW )
	{
		if( !wcscmp(m_strPrizeParticleName.c_str(),L"BigWinParticle") )
		{
			//if ( GetPrize() >= 20  || ( m_iID>=18 && m_iID<=24 ) )
			{
				cFishApp::m_spMonsterManager->DisplayBigWinParticle( GetCenterPos() );
			}
		}
		if ( !cGameApp::m_spSoundParser->GetObject( L"8" )->IsPlay() )
		{
			cGameApp::SoundPlay( L"8", true );
		}
	}
}








void	cMonster::ProcessMonsterData(TiXmlElement*e_pTiXmlElement)
{
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("Prize")
		{
			m_piPrize = new int;
			*m_piPrize = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("FloatingPrize")
		{
			m_pFloatingPrize = new sMinMaxData<int>;
			*m_pFloatingPrize = VALUE_TO_VECTOR2;
		}
		else
		COMPARE_NAME("Probability")
		{
			m_fProbabilityToDied = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("PrizeAnimationName")
		{
			m_strPrizeAnimationName = l_strValue;
		}
		else
		COMPARE_NAME("PrizeParticleName")
		{
			m_strPrizeParticleName = l_strValue;
		}
	PARSE_NAME_VALUE_END
}








