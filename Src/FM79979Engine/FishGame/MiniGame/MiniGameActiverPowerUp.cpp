#include "stdafx.h"
#include "MiniGameActiverPowerUp.h"
#include "../PlayerBehavior/PlayerData.h"
const wchar_t*         cMiniGameActiverPowerUp::TypeID( L"cMiniGameActiverPowerUp" );
cMiniGameActiverPowerUp::cMiniGameActiverPowerUp(TiXmlElement*e_pXMLElement):cMiniGameActiverBase(e_pXMLElement)
{
	TiXmlElement*l_pDataXMLElement = GetXmlElementByNameFromElement(L"Data",e_pXMLElement);
	m_iMultiplier = 2;
	m_PowerUpTC.SetTargetTime(10.f);
	PARSE_ELEMENT_START(l_pDataXMLElement)
		COMPARE_NAME("Miltiplier")
		{
			m_iMultiplier = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("Duration")
		{
			m_PowerUpTC.SetTargetTime(VALUE_TO_FLOAT);
		}
	PARSE_NAME_VALUE_END
}

cMiniGameActiverPowerUp::cMiniGameActiverPowerUp(cMiniGameActiverPowerUp*e_pMiniGameActiverPowerUp)
:cMiniGameActiverBase(e_pMiniGameActiverPowerUp)
{
	m_iMultiplier = e_pMiniGameActiverPowerUp->m_iMultiplier;
	m_PowerUpTC.SetTargetTime(e_pMiniGameActiverPowerUp->m_PowerUpTC.fTargetTime);
}

cMiniGameActiverPowerUp::~cMiniGameActiverPowerUp()
{

}

void	cMiniGameActiverPowerUp::Init()
{
	cMiniGameActiverBase::Init();
	cPlayerData* pPlayer = ( cPlayerData* ) m_pPlayer;
	if ( pPlayer )
	{
		if( pPlayer->IsEnergyPower() )
		{//not allow this
			this->m_bSatisfiedCondition = true;	
		}
		else
		{
			m_PowerUpTC.Start();
			pPlayer->SetEnergyPower( true ,m_iMultiplier);
			//cGameApp::SoundPlay( L"18", true );
		}
	}
}

void	cMiniGameActiverPowerUp::Update(float e_fElpaseTime)
{
	cMiniGameActiverBase::Update(e_fElpaseTime);
	m_PowerUpTC.Update(e_fElpaseTime);
	if( m_PowerUpTC.bTragetTimrReached )
	{
		cPlayerData* pPlayer = ( cPlayerData* ) m_pPlayer;
		if ( pPlayer )
			pPlayer->SetEnergyPower( false ,1 );
		this->m_bSatisfiedCondition = true;
	}
	else
	{
		this->m_bSatisfiedCondition = false;
	}
}

void	cMiniGameActiverPowerUp::Render()
{
	cMiniGameActiverBase::Render();
}