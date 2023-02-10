#include "stdafx.h"
#include "MiniGameBase.h"
#include "../Monster/Monster.h"
#include "../PlayerBehavior/PlayerData.h"
const wchar_t*         cMiniGameTriggerBase::TypeID( L"cMiniGameTriggerBase" );
const wchar_t*         cMiniGameActiverBase::TypeID( L"cMiniGameActiverBase" );
cMiniGameTriggerBase::cMiniGameTriggerBase(TiXmlElement*e_pXMLElement)
{
	assert(e_pXMLElement);
	TiXmlElement*l_pBaseDataXMLElement = GetXmlElementByNameFromElement(L"BaseData",e_pXMLElement);
	m_bInProgress = false;
	m_pHintMPDI = 0;
	m_pMonster = 0;
	m_pExtraData = 0;
	cMPDIList*l_pMPDIList = 0;
	PARSE_ELEMENT_START(l_pBaseDataXMLElement)
		COMPARE_NAME("MPDIListName")
		{
			l_pMPDIList = cGameApp::GetMPDIListByFileName(l_strValue,true);
			if( !l_pMPDIList )
			{
				UT::ErrorMsg(l_strValue,L"mpdi file is not exists!");
			}
		}
		else
		COMPARE_NAME("HintMPDI")
		{
			if( l_pMPDIList )
			{
				m_pHintMPDI = dynamic_cast<cMPDI*>(l_pMPDIList->GetObject(l_strValue)->Clone());
			}
		}
		else
		COMPARE_NAME("ActiverMiniGameName")
		{
			m_strActiverMiniGameName = l_strValue;
		}
	PARSE_NAME_VALUE_END
	if( !m_pHintMPDI )
	{
		UT::ErrorMsg(L"mini game Trigger HintMPDI is null",L"Error");
	}
}

cMiniGameTriggerBase::cMiniGameTriggerBase(cMiniGameTriggerBase*e_pMiniGameTriggerBase)
{
	m_bInProgress = false;
	m_pHintMPDI = 0;
	m_pMonster = 0;
	m_pExtraData = 0;
	if( e_pMiniGameTriggerBase->m_pHintMPDI )
		m_pHintMPDI = dynamic_cast<cMPDI*>(e_pMiniGameTriggerBase->m_pHintMPDI->Clone());
}

//cMiniGameTriggerBase::cMiniGameTriggerBase(const WCHAR*e_strMPDILIstFileName,const WCHAR*e_strHintMPDIName,const WCHAR*e_strActiverMiniGameName)
//{
//	m_bInProgress = false;
//	m_pHintMPDI = 0;
//	m_pMonster = 0;
//	cMPDIList*l_pMPSILIdt = cGameApp::GetMPDIListByFileName(e_strMPDILIstFileName,true);
//	m_pHintMPDI = dynamic_cast<cMPDI*>(l_pMPSILIdt->GetObject(e_strHintMPDIName)->Clone());
//	this->m_strActiverMiniGameName = e_strActiverMiniGameName;
//}

cMiniGameTriggerBase::~cMiniGameTriggerBase()
{
	SAFE_DELETE(m_pHintMPDI);
	SAFE_DELETE(m_pExtraData);
}
void	cMiniGameTriggerBase::Init()
{
	if( m_pHintMPDI )
		m_pHintMPDI->Init();
	m_bInProgress = true;
	m_bSatisfiedCondition = false;
}

void	cMiniGameTriggerBase::Update(float e_fElpaseTime)
{
	if( m_pMonster && m_pHintMPDI )
	{
		m_pHintMPDI->SetWorldTransform( m_pMonster->GetTransform() );
		m_pHintMPDI->Update(e_fElpaseTime);
		if( !m_pMonster->IsStatusAllowToDied() )
		{
			this->m_bInProgress = false;
			this->m_bSatisfiedCondition = true;
		}
	}
}

void	cMiniGameTriggerBase::Render()
{
	if( m_pHintMPDI )
		m_pHintMPDI->Render();
}

void	cMiniGameTriggerBase::SetupShowData(cMonster*e_pMonster)
{
	m_pMonster = e_pMonster;
	Init();
}
//			<PrizeWithProbability Prize="40,75,100,150,200" Probability="1,1,1,1,1" />
cMiniGameActiverBase::cMiniGameActiverBase(TiXmlElement*e_pXMLElement)
{
	assert(e_pXMLElement);
	m_pExtraData = 0;
	m_bInProgress = false;
	m_pPlayer = 0;
	m_pMiniGameNameMPDI = 0;
	m_pEffectShowMPDI = 0;
	m_pSound = 0;
	TiXmlElement*l_pPrizeWithProbabilityXMLElement = GetXmlElementByNameFromElement(L"PrizeWithProbability",e_pXMLElement);
	if( l_pPrizeWithProbabilityXMLElement )
	{
		PARSE_ELEMENT_START(l_pPrizeWithProbabilityXMLElement)
			COMPARE_NAME("Prize")
			{
				m_PrizeWithProbabilityVector.ValueVector = GetValueListByCommaDivide<int>(l_strValue);
			}
			else
			COMPARE_NAME("Probability")
			{
				m_PrizeWithProbabilityVector.ProbabilityVector = GetValueListByCommaDivide<int>(l_strValue);
			}
		PARSE_NAME_VALUE_END
	}
	TiXmlElement*l_pBaseDataXMLElement = GetXmlElementByNameFromElement(L"BaseData",e_pXMLElement);
	cMPDIList*l_pMPDIList = 0;
	PARSE_ELEMENT_START(l_pBaseDataXMLElement)
		COMPARE_NAME("Name")
		{
			this->SetName(l_strValue);
		}
		else
		COMPARE_NAME("MPDIListName")
		{
			l_pMPDIList = cGameApp::GetMPDIListByFileName(l_strValue,true);
			if( !l_pMPDIList )
			{
				UT::ErrorMsg(l_strValue,L"mpdi file is not exists!");
			}
		}
		else
		COMPARE_NAME("MiniGameNameMPDI")
		{
			if( l_pMPDIList )
			{
				m_pMiniGameNameMPDI = dynamic_cast<cMPDI*>(l_pMPDIList->GetObject(l_strValue)->Clone());
			}
		}
		else
		COMPARE_NAME("EffectShowMPDI")
		{
			if( l_pMPDIList )
			{
				m_pEffectShowMPDI = dynamic_cast<cMPDI*>(l_pMPDIList->GetObject(l_strValue)->Clone());
			}
		}
		else
		COMPARE_NAME("Sound")
		{
			m_pSound = cGameApp::m_spSoundParser->GetObject(l_strValue);
		}
	PARSE_NAME_VALUE_END
}

cMiniGameActiverBase::cMiniGameActiverBase(cMiniGameActiverBase*e_pMiniGameActiverBase)
{
	m_pMonster = 0;
	m_pExtraData = 0;
	m_bInProgress = false;
	m_pPlayer = 0;
	m_pMiniGameNameMPDI = 0;
	m_pEffectShowMPDI = 0;
	m_pMonster = 0;
	m_pSound = e_pMiniGameActiverBase->m_pSound;
	if(e_pMiniGameActiverBase->m_pMiniGameNameMPDI)
		m_pMiniGameNameMPDI = dynamic_cast<cMPDI*>(e_pMiniGameActiverBase->m_pMiniGameNameMPDI->Clone());
	if(e_pMiniGameActiverBase->m_pEffectShowMPDI)
		m_pEffectShowMPDI = dynamic_cast<cMPDI*>(e_pMiniGameActiverBase->m_pEffectShowMPDI->Clone());
}

//cMiniGameActiverBase::cMiniGameActiverBase(const WCHAR*e_strMPDILIstFileName,const WCHAR*e_strMiniGameNameMPDI,const WCHAR*e_strEffectShowMPDI)
//{
//	m_bInProgress = false;
//	m_pPlayer = 0;
//	m_pMiniGameNameMPDI = 0;
//	m_pEffectShowMPDI = 0;
//	cMPDIList*l_pMPSILIdt = cGameApp::GetMPDIListByFileName(e_strMPDILIstFileName,true);
//	m_pMiniGameNameMPDI = dynamic_cast<cMPDI*>(l_pMPSILIdt->GetObject(e_strMiniGameNameMPDI)->Clone());
//	m_pEffectShowMPDI = dynamic_cast<cMPDI*>(l_pMPSILIdt->GetObject(e_strEffectShowMPDI));
//}

cMiniGameActiverBase::~cMiniGameActiverBase()
{
	SAFE_DELETE(m_pMiniGameNameMPDI);
	SAFE_DELETE(m_pEffectShowMPDI);
	SAFE_DELETE(m_pExtraData);
}

void	cMiniGameActiverBase::Init()
{
	assert(m_pPlayer);
	if( m_pEffectShowMPDI )
		m_pEffectShowMPDI->Init();
	if( m_pMiniGameNameMPDI )
	{
		m_pMiniGameNameMPDI->Init();
		m_pMiniGameNameMPDI->SetWorldTransform(m_pPlayer->GetPlayerMatrix());
	}
	m_HittedMonsterVector.clear();
	m_bInProgress = true;
	m_bSatisfiedCondition = false;
	if( m_pSound )
		m_pSound->Play(true);
}
void	cMiniGameActiverBase::Update(float e_fElpaseTime)
{
	this->m_bSatisfiedCondition = true;
	if( m_pEffectShowMPDI )
	{
		m_pEffectShowMPDI->Update(e_fElpaseTime);
		if( !m_pEffectShowMPDI->IsAnimationDone() )
			m_bSatisfiedCondition = false;
		if( m_pEffectShowMPDI->GetCurrentProgress() >= 0.3f )
		{
			if( m_HittedMonsterVector.size() )
			{
				int	l_iSize = (int)m_HittedMonsterVector.size();
				for( int i=0;i<l_iSize;++i )
				{
					m_HittedMonsterVector[i]->SetMonsterStatus(eMS_DIED_SHOW);
				}
				m_HittedMonsterVector.clear();
			}
		}
	}
	if( m_pMiniGameNameMPDI )
	{
		m_pMiniGameNameMPDI->Update(e_fElpaseTime);
		if( !m_pMiniGameNameMPDI->IsAnimationDone() )
		{
			m_bSatisfiedCondition = false;
		}
	}
}

void	cMiniGameActiverBase::Render()
{
	if( m_pEffectShowMPDI )
	{
		m_pEffectShowMPDI->Render();;
	}
	if( m_pMiniGameNameMPDI )
	{
		m_pMiniGameNameMPDI->Render();
	}
}

void	cMiniGameActiverBase::SetupShowData(int e_iPayRate,int e_iMoney,cPlayerBehaviorBase*e_pPlayer,cMonster*e_pMonster)
{
	m_iMoney = e_iMoney;
	m_iPayRate = e_iPayRate;
	m_pPlayer = e_pPlayer;
	m_pMonster = e_pMonster;
	m_HittedMonsterVector.clear();
	Init();
}

int		cMiniGameActiverBase::ProcessMiniGameMonsterDataWithNewPrize()
{
	int*	l_iValue = m_PrizeWithProbabilityVector.GetValueByProbability();
	if( l_iValue )
		return *l_iValue;
	return 0;
}