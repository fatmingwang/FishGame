#include "stdafx.h"
#include "MiniGameActiverAllFishesFreeze.h"
#include "../GameApp/GameApp.h"
const wchar_t*         cMiniGameActiverAllFishesFreeze::TypeID( L"cMiniGameActiverAllFishesFreeze" );
cMiniGameActiverAllFishesFreeze::cMiniGameActiverAllFishesFreeze(TiXmlElement*e_pXMLElement):cMiniGameActiverBase(e_pXMLElement)
{
	TiXmlElement*l_pDataXMLElement = GetXmlElementByNameFromElement(L"Data",e_pXMLElement);
	m_FreezeTC.SetTargetTime(10.f);
	PARSE_ELEMENT_START(l_pDataXMLElement)
		COMPARE_NAME("Duration")
		{
			m_FreezeTC.SetTargetTime(VALUE_TO_FLOAT);
		}
	PARSE_NAME_VALUE_END
}

cMiniGameActiverAllFishesFreeze::cMiniGameActiverAllFishesFreeze(cMiniGameActiverAllFishesFreeze*e_pMiniGameActiverAllFishesFreeze)
:cMiniGameActiverBase(e_pMiniGameActiverAllFishesFreeze)
{
	m_FreezeTC.SetTargetTime(e_pMiniGameActiverAllFishesFreeze->m_FreezeTC.fTargetTime);
}

cMiniGameActiverAllFishesFreeze::~cMiniGameActiverAllFishesFreeze()
{

}

void	cMiniGameActiverAllFishesFreeze::Init()
{
	cMiniGameActiverBase::Init();
	m_FreezeTC.Start();
	cFishApp::m_sfMonsterUpdateSpeed = 0.f;
}

void	cMiniGameActiverAllFishesFreeze::Update(float e_fElpaseTime)
{
	cMiniGameActiverBase::Update(e_fElpaseTime);
	this->m_bSatisfiedCondition = false;
	m_FreezeTC.Update(e_fElpaseTime);
	if( m_FreezeTC.bTragetTimrReached )
	{
		if( cFishApp::m_sfMonsterUpdateSpeed <= 0.00001f )
		{
			cFishApp::m_sfMonsterUpdateSpeed = 1.f;
		}
		this->m_bSatisfiedCondition = true;
	}
}