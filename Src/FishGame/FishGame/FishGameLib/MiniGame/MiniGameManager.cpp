#include "stdafx.h"
#include "MiniGameManager.h"
#include "MiniGameActiverAllFishesFreeze.h"
#include "MiniGameActiverAreaBomb.h"
#include "MiniGameActiverFullScreenBomb.h"
#include "MiniGameActiverPowerUp.h"
#include "../GameApp/GameApp.h"
#include "../GameApp/SceneChange.h"
#include "../GameApp/FileNameDefine.h"
#include "../Monster/MonsterManager.h"
#include "../PlayerBehavior/PlayerData.h"
#include "../ProbabilityFish/FishProbability.h"

cMiniGameManager::cMiniGameManager():ISAXCallback(true)
{
	m_iMaximumShowOnScreen = 1;
	m_iMaximumPayrate = 350;
	m_fMiniGameDuration = Vector2(5,10);
	m_MiniGameMonsterTime.SetTargetTime(m_fMiniGameDuration.Rand());
}


cMiniGameManager::~cMiniGameManager()
{

}
//<MiniGame MiniGameFishID="1" MiniGameDuration="5,10" MaximumPayrate="350" MaximumShowOnScreen="1" >
//	<Trigger>
//		<Game Name="cMiniGameActiverAllFishesFreeze" Type="cMiniGameTriggerBase" ShowProbability="0.4"  >
//			<BaseData MPDIListName="Fish/Image/MiniGame/FullScreenFreeze.mpdi" HintMPDI="Plate" ActiverMiniGameName="cMiniGameActiverAllFishesFreeze" />
//		</Game>
//		<Game Name="cMiniGameActiverAreaBomb"  Type="cMiniGameTriggerBase" ShowProbability="0.4"  >
//			<BaseData MPDIListName="Fish/Image/MiniGame/AreaBomb.mpdi" HintMPDI="Plate" ActiverMiniGameName="cMiniGameActiverAreaBomb" />
//		</Game>
//		<Game Name="cMiniGameActiverFullScreenBomb" Type="cMiniGameTriggerBase" ShowProbability="0.4" >
//			<BaseData MPDIListName="Fish/Image/MiniGame/FullScreenBomb.mpdi" HintMPDI="Plate" ActiverMiniGameName="cMiniGameActiverFullScreenBomb"  />
//		</Game>
//		<Game Name="cMiniGameActiverPowerUp" Type="cMiniGameTriggerBase" ShowProbability="0.4"  >
//			<BaseData MPDIListName="Fish/Image/MiniGame/PowerUp/PowerUp.mpdi" HintMPDI="Plate" ActiverMiniGameName="cMiniGameActiverPowerUp"  />
//		</Game>
//	</Trigger>
//	<Activer>
//		<Game Name="cMiniGameActiverAllFishesFreeze" Type="cMiniGameActiverAllFishesFreeze" >
//			<BaseData MPDIListName="Fish/Image/MiniGame/FullScreenFreeze.mpdi" MiniGameNameMPDI="Message" EffectShowMPDI="WeaponEffect" Sound="6" />
//			<Data Duration="5" />
//		</Game>
//		<Game Name="cMiniGameActiverAreaBomb" Type="cMiniGameActiverAreaBomb">
//			<BaseData MPDIListName="Fish/Image/MiniGame/AreaBomb/AreaBomb.mpdi" MiniGameNameMPDI="Message" EffectShowMPDI="WeaponEffect" Sound="6"/>
//			<PrizeWithProbability Prize="40,75,100,150,200" Probability="1,1,1,1,1" />
//			<Data ExplosionRadius="300" />
//		</Game>
//		<Game Name="cMiniGameActiverFullScreenBomb" Type="cMiniGameActiverFullScreenBomb">
//			<BaseData MPDIListName="Fish/Image/MiniGame/FullScreenBomb/FullScreenBomb.mpdi" MiniGameNameMPDI="Message" EffectShowMPDI="WeaponEffect" Sound="6"/>
//			<PrizeWithProbability Prize="40,75,100,150,200" Probability="1,1,1,1,1" />
//		</Game>
//		<Game Name="cMiniGameActiverPowerUp" Type="cMiniGameActiverPowerUp">
//			<BaseData MPDIListName="Fish/Image/MiniGame/PowerUp.mpdi" MiniGameNameMPDI="Message" Sound="6"/>
//			<Data Duration="6" />
//		</Game>
//	</Activer>
//</MiniGame>
void	cMiniGameManager::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	if(!e_pTiXmlElement->m_bDone)
	{
		const WCHAR*l_strValue = e_pTiXmlElement->Value();
		COMPARE_VALUE("MiniGame")
		{
			ProcessMiniGame(e_pTiXmlElement);
		}
		else
		COMPARE_VALUE("Trigger")
		{
			ProcessTrigger(e_pTiXmlElement);
		}
		else
		COMPARE_VALUE("Activer")
		{
			ProcessActiver(e_pTiXmlElement);
		}
	}
}
//<MiniGame MiniGameFishID="1" MiniGameDuration="5,10" MaximumPayrate="350" MaximumShowOnScreen="1" >
void	cMiniGameManager::ProcessMiniGame(TiXmlElement*e_pTiXmlElement)
{
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("MiniGameFishID")
		{
			m_MiniGameFishIDList.clear();
			m_MiniGameFishIDList = GetValueListByCommaDivide<int>(l_strValue);
		}
		else
		COMPARE_NAME("MiniGameDuration")
		{
			m_fMiniGameDuration = VALUE_TO_VECTOR2;
		}
		else
		COMPARE_NAME("MaximumPayrate")
		{
			m_iMaximumPayrate = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("MaximumShowOnScreen")
		{
			m_iMaximumShowOnScreen = VALUE_TO_INT;
		}
	PARSE_NAME_VALUE_END
}
//	<Trigger>
//		<Game Name="" Type="cMiniGameTriggerBase" Probability="0.2">
//			<BaseData MPDIListName="" HintMPDI="" ActiverMiniGameName=""  />
//			<Data Time="" Multiplier="2" />
//		</Game>
//		<Game Name="" MPDIListName="" HintMPDI="" />
//	</Trigger>
void	cMiniGameManager::ProcessTrigger(TiXmlElement*e_pTiXmlElement)
{
	e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
	std::vector<float>	l_fProbabilityVector;
	while( e_pTiXmlElement )
	{
		if( !wcscmp(e_pTiXmlElement->Value(),L"Game") )
		{
			std::wstring	l_strMiniGameName;
			cMiniGameTriggerBase*l_pMiniGameTriggerBase = 0;
			PARSE_ELEMENT_START(e_pTiXmlElement)
				COMPARE_NAME("Name")
				{
					l_strMiniGameName = l_strValue;
				}
				else
				COMPARE_NAME("Type")
				{
					if( !wcscmp(l_strValue,cMiniGameTriggerBase::TypeID) )
						l_pMiniGameTriggerBase = new cMiniGameTriggerBase(e_pTiXmlElement);
				}
				else
				COMPARE_NAME("ShowProbability")
				{
					l_fProbabilityVector.push_back(VALUE_TO_FLOAT);
				}
				else
				COMPARE_NAME("HotKey")
				{
					if( cGameApp::m_sbDebugFunctionWorking && l_pMiniGameTriggerBase )
					{
						cClickMouseBehavior	*l_pClickMouseBehavior = new cClickMouseBehavior();
						l_pClickMouseBehavior->SetName(l_strMiniGameName.c_str());
						l_pClickMouseBehavior->SetKeyData((unsigned char)l_strValue[0]);
						m_MiniGameTriggerHotKeyList.AddObjectNeglectExist(l_pClickMouseBehavior);
					}
				}
			PARSE_NAME_VALUE_END
			if( l_pMiniGameTriggerBase )
			{
				l_pMiniGameTriggerBase->SetName(l_strMiniGameName.c_str());
				cNamedTypedObjectVector<cMiniGameTriggerBase>*l_pMiniGameTriggerBaseVector = new cNamedTypedObjectVector<cMiniGameTriggerBase>;
				l_pMiniGameTriggerBaseVector->AddObjectNeglectExist(l_pMiniGameTriggerBase);
				l_pMiniGameTriggerBaseVector->SetName(l_pMiniGameTriggerBase->GetName());
				if( m_iMaximumShowOnScreen > 1 )
				{
					for( int i=0;i<m_iMaximumShowOnScreen-1;++i )
					{
						cMiniGameTriggerBase*l_pClone = dynamic_cast<cMiniGameTriggerBase*>(l_pMiniGameTriggerBase->Clone());
						l_pMiniGameTriggerBaseVector->AddObjectNeglectExist(l_pClone);
					}
				}
				this->m_MiniGameTriggerList.AddObjectNeglectExist(l_pMiniGameTriggerBaseVector);
			}
			else
			{
				if(l_fProbabilityVector.size())
					l_fProbabilityVector.erase(l_fProbabilityVector.begin()+l_fProbabilityVector.size()-1);
				UT::ErrorMsg(L"no this type",l_strMiniGameName.c_str());
			}
		}
		e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
	}
	m_MiniGameShowProbability.SetupProbabilityData(l_fProbabilityVector);
}
//	<Activer>
//		<Game Name="" Type="">
//			<BaseData MPDIListName="" HintMPDI="" EffectMPDI="" />
//			<PrizeWithProbability Prize="40,75,100,150,200" Probability="1,1,1,1,1" />
//			<Data ExplosionRadius="125" />
//		</Game>
//	</Activer>
void	cMiniGameManager::ProcessActiver(TiXmlElement*e_pTiXmlElement)
{
	e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while( e_pTiXmlElement )
	{
		if( !wcscmp(e_pTiXmlElement->Value(),L"Game") )
		{
			std::wstring	l_strMiniGameName;
			cMiniGameActiverBase*l_pMiniGameActiverBase = 0;
			PARSE_ELEMENT_START(e_pTiXmlElement)
				COMPARE_NAME("Name")
				{
					l_strMiniGameName = l_strValue;
				}
				else
				COMPARE_NAME("Type")
				{
					if( !wcscmp(l_strValue,cMiniGameActiverAllFishesFreeze::TypeID) )
					{
						l_pMiniGameActiverBase = new cMiniGameActiverAllFishesFreeze(e_pTiXmlElement);
					}
					else
					if( !wcscmp(l_strValue,cMiniGameActiverAreaBomb::TypeID) )
					{
						l_pMiniGameActiverBase = new cMiniGameActiverAreaBomb(e_pTiXmlElement);
					}
					else
					if( !wcscmp(l_strValue,cMiniGameActiverFullScreenBomb::TypeID) )
					{
						l_pMiniGameActiverBase = new cMiniGameActiverFullScreenBomb(e_pTiXmlElement);
					}
					else
					if( !wcscmp(l_strValue,cMiniGameActiverPowerUp::TypeID) )
					{
						l_pMiniGameActiverBase = new cMiniGameActiverPowerUp(e_pTiXmlElement);
					}
					else
					if( !wcscmp(l_strValue,cMiniGameTriggerBase::TypeID) )
					{
						l_pMiniGameActiverBase = new cMiniGameActiverBase(e_pTiXmlElement);
					}
					else
					{
						UT::ErrorMsg(L"unsupport activer type",l_strValue);
					}
				}
				else
				COMPARE_NAME("HotKey")
				{
					if( cGameApp::m_sbDebugFunctionWorking && l_pMiniGameActiverBase )
					{
						cClickMouseBehavior	*l_pClickMouseBehavior = new cClickMouseBehavior();
						l_pClickMouseBehavior->SetName(l_strMiniGameName.c_str());
						l_pClickMouseBehavior->SetKeyData((unsigned char)l_strValue[0]);
						m_MiniGameActiverHotKeyList.AddObjectNeglectExist(l_pClickMouseBehavior);
					}
				}
			PARSE_NAME_VALUE_END
			if( l_pMiniGameActiverBase )
			{
				l_pMiniGameActiverBase->SetName(l_strMiniGameName.c_str());
				cNamedTypedObjectVector<cMiniGameActiverBase>*l_pMiniGameActiverBaseVector = new cNamedTypedObjectVector<cMiniGameActiverBase>;
				l_pMiniGameActiverBaseVector->SetName(l_pMiniGameActiverBase->GetName());
				l_pMiniGameActiverBaseVector->AddObjectNeglectExist(l_pMiniGameActiverBase);
				if( m_iMaximumShowOnScreen > 1 )
				{
					for( int i=0;i<m_iMaximumShowOnScreen-1;++i )
					{
						cMiniGameActiverBase*l_pClone = dynamic_cast<cMiniGameActiverBase*>(l_pMiniGameActiverBase->Clone());
						l_pClone->SetName(l_strMiniGameName.c_str());
						l_pMiniGameActiverBaseVector->AddObjectNeglectExist(l_pClone);
					}
				}
				this->m_MiniGameActiverList.AddObjectNeglectExist(l_pMiniGameActiverBaseVector);
			}
			else
			{
				UT::ErrorMsg(L"no this type",l_strMiniGameName.c_str());
			}
		}
		e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
	}
}

void cMiniGameManager::Init()
{
	m_MiniGameTriggerTempContainer.Clear();
	m_MiniGameActiverTempContainer.Clear();
	m_MiniGameTriggerList.Destroy();
	m_MiniGameActiverList.Destroy();
	m_MiniGameTriggerHotKeyList.Destroy();
	m_MiniGameActiverHotKeyList.Destroy();
	m_MiniGameShowProbability.Clear();
	if(!this->Parse("Fish/MiniGame/MiniGameSetup.xml"))
	{
		UT::ErrorMsg(L"mini game manager parse init failed!",L"error!!");
	}
	m_MiniGameTriggerTempContainer.SetCount(m_iMaximumShowOnScreen);
	m_MiniGameActiverTempContainer.SetCount(m_iMaximumShowOnScreen);
	m_MiniGameMonsterTime.SetTargetTime(m_fMiniGameDuration.Rand());
}

void	cMiniGameManager::HoteyUpdate(float e_fElpaseTime)
{
	if ( cGameApp::m_sbDebugFunctionWorking )
	{
		int	l_iCount = m_MiniGameTriggerHotKeyList.Count();
		for( int i=0;i<l_iCount;++i )
		{
			cClickMouseBehavior*l_pClickMouseBehavior = m_MiniGameTriggerHotKeyList[i];
			l_pClickMouseBehavior->Update(e_fElpaseTime);
			l_pClickMouseBehavior->SingnalProcess();
			if(l_pClickMouseBehavior->IsSatisfiedCondition())
			{
				l_pClickMouseBehavior->Init();
				int	l_iFishIndex = rand()%m_MiniGameFishIDList.size();
				l_iFishIndex = m_MiniGameFishIDList[l_iFishIndex];
				int	l_iMiniGameIndex = m_MiniGameTriggerList.GetObjectIndexByName(l_pClickMouseBehavior->GetName());
				MiniGameTriggerGo(l_iMiniGameIndex,l_iFishIndex);
			}
		}
		l_iCount = m_MiniGameActiverHotKeyList.Count();
		for( int i=0;i<l_iCount;++i )
		{
			cClickMouseBehavior*l_pClickMouseBehavior = m_MiniGameActiverHotKeyList[i];
			l_pClickMouseBehavior->Update(e_fElpaseTime);
			l_pClickMouseBehavior->SingnalProcess();
			if(l_pClickMouseBehavior->IsSatisfiedCondition())
			{
				l_pClickMouseBehavior->Init();
				int	l_iMiniGameIndex = m_MiniGameTriggerList.GetObjectIndexByName(l_pClickMouseBehavior->GetName());
				int	l_iPayRate = rand()%350;
				int	l_iMoney = rand()%350000%10;
				RequireMiniGameActiverBase(l_iMiniGameIndex,l_iPayRate,l_iMoney,cFishApp::m_spPlayerManager->GetObject(rand()%cFishApp::m_spPlayerManager->Count()),0)?true:false;
			}
		}
	}
}

bool	cMiniGameManager::MiniGameTriggerGo(int e_iMiniGameIndex,int e_iFishID)
{
	if( !IsMiniGameUsing() )
	{
		cMonster*l_pMonster = cFishApp::m_spMonsterManager->m_FishShowProbabilityList.MonsterGo( e_iFishID );
		if( l_pMonster )
		{
			if(this->RequireMiniGameTrigger(e_iMiniGameIndex,l_pMonster))
			{
				return true;
			}
		}
	}
	return false;
}

void	cMiniGameManager::MiniGameMonsterGenerate(float e_fElpaseTime)	
{
	m_MiniGameMonsterTime.Update(e_fElpaseTime);
	if( m_MiniGameMonsterTime.bTragetTimrReached )
	{
		m_MiniGameMonsterTime.SetTargetTime(m_fMiniGameDuration.Rand());
		//check scene time.
		if( cFishApp::m_spSceneChange && !cFishApp::m_spSceneChange->IsTimeReachSceneChange(30.f) )
		{
			int	l_iSize = (int)m_MiniGameFishIDList.size();
			if( l_iSize )
			{
				int	l_iFishIndex = rand()%l_iSize;
				l_iFishIndex = m_MiniGameFishIDList[l_iFishIndex];
				int	l_iMiniGameIndex = m_MiniGameShowProbability.GetIndexByProbability();
				MiniGameTriggerGo(l_iMiniGameIndex,l_iFishIndex);
			}
		}
	}
}

void	cMiniGameManager::Update( float e_fElpaseTime )
{
	HoteyUpdate(e_fElpaseTime);
	MiniGameMonsterGenerate(e_fElpaseTime);
	for(int i=0;i<m_MiniGameTriggerTempContainer.m_iNumWorking;++i)
	{
		m_MiniGameTriggerTempContainer.m_ppObjects[i]->Update(e_fElpaseTime);
		if(m_MiniGameTriggerTempContainer.m_ppObjects[i]->IsSatisfiedCondition())
		{
			m_MiniGameTriggerTempContainer.m_ppObjects[i]->m_bInProgress = false;
			m_MiniGameTriggerTempContainer.Remove(i);
		}
	}
	for(int i=0;i<m_MiniGameActiverTempContainer.m_iNumWorking;++i)
	{
		m_MiniGameActiverTempContainer.m_ppObjects[i]->Update(e_fElpaseTime);
		if(m_MiniGameActiverTempContainer.m_ppObjects[i]->IsSatisfiedCondition())
		{
			m_MiniGameActiverTempContainer.m_ppObjects[i]->m_bInProgress = false;
			m_MiniGameActiverTempContainer.Remove(i);
		}
	}

}

bool cMiniGameManager::IsMiniGameUsing()
{
	return m_MiniGameTriggerTempContainer.m_iNumWorking+m_MiniGameActiverTempContainer.m_iNumWorking > 0?true:false;
}

int	cMiniGameManager::GetMiniGamePayRate(const WCHAR*e_strValue)
{
	int	l_iIndex = m_MiniGameActiverList.GetObjectIndexByName(e_strValue);
	if( l_iIndex != -1 )
	{
		cMiniGameActiverBase*l_pMiniGameActiverBase = m_MiniGameActiverList.GetObject(l_iIndex)->GetObject(0);
		if( l_pMiniGameActiverBase )
		{
			return l_pMiniGameActiverBase->ProcessMiniGameMonsterDataWithNewPrize();
		}
	}
	return 0;
}

void cMiniGameManager::RenderActiver()
{
	for(int i=0;i<m_MiniGameActiverTempContainer.m_iNumWorking;++i)
	{
		m_MiniGameActiverTempContainer.m_ppObjects[i]->Render();
	}
}

void cMiniGameManager::RenderTrigger()
{
	for(int i=0;i<m_MiniGameTriggerTempContainer.m_iNumWorking;++i)
	{
		m_MiniGameTriggerTempContainer.m_ppObjects[i]->Render();
	}
}

cMiniGameTriggerBase* cMiniGameManager::RequireMiniGameTrigger( const WCHAR* e_strName, cMonster* e_pMonster )
{
	if( m_iMaximumShowOnScreen > m_MiniGameActiverTempContainer.m_iNumWorking+m_MiniGameTriggerTempContainer.m_iNumWorking )
	{
		int	l_iIndex = m_MiniGameTriggerList.GetObjectIndexByName( e_strName );

		if ( l_iIndex != -1 )
			return RequireMiniGameTrigger( l_iIndex, e_pMonster );
	}

	return 0;
}

cMiniGameTriggerBase* cMiniGameManager::RequireMiniGameTrigger( int e_iIndex, cMonster* e_pMonster )
{
	assert( m_iMaximumShowOnScreen >= m_MiniGameActiverTempContainer.m_iNumWorking+m_MiniGameTriggerTempContainer.m_iNumWorking );
	cNamedTypedObjectVector< cMiniGameTriggerBase >*l_pMiniGameTriggerList = m_MiniGameTriggerList[ e_iIndex ];
	int	l_iCount = l_pMiniGameTriggerList->Count();
	for ( int i = 0; i < l_iCount; i++ )
	{
		 cMiniGameTriggerBase* l_pMiniGameTriggerBase = l_pMiniGameTriggerList->GetObject(i);
		 if ( !l_pMiniGameTriggerBase->m_bInProgress )
		 {
			 e_pMonster->SetMiniGameMonster(true,l_pMiniGameTriggerBase->m_strActiverMiniGameName.c_str());
			 l_pMiniGameTriggerBase->SetupShowData(e_pMonster );
			 m_MiniGameTriggerTempContainer.m_ppObjects[m_MiniGameTriggerTempContainer.m_iNumWorking] = l_pMiniGameTriggerBase;
			 ++m_MiniGameTriggerTempContainer.m_iNumWorking;
			 return l_pMiniGameTriggerBase;
		 }
	}
	return 0;
}

bool	cMiniGameManager::RemoveMiniGameWorkingTrigger(cMonster* e_pMonster)
{
	for(int i=0;i<m_MiniGameTriggerTempContainer.m_iNumWorking;++i)
	{
		if(m_MiniGameTriggerTempContainer.m_ppObjects[i]->m_pMonster == e_pMonster )
		{
			m_MiniGameTriggerTempContainer.m_ppObjects[i] = m_MiniGameTriggerTempContainer.m_ppObjects[m_MiniGameTriggerTempContainer.m_iNumWorking-1];
			--m_MiniGameTriggerTempContainer.m_iNumWorking;
			return true;
		}
	}
	return false;
}

cMiniGameActiverBase*	cMiniGameManager::RequireMiniGameActiverBase( const WCHAR* e_strName,int e_iPayRate,int e_iMoney,cPlayerBehaviorBase*e_pPlayer,cMonster*e_pMonster)
{
	//,std::vector<cMonster*>*e_pHittedMonsterVector
	int	l_iIndex = m_MiniGameActiverList.GetObjectIndexByName(e_strName);
	if( l_iIndex != -1 )
		return RequireMiniGameActiverBase(l_iIndex,e_iPayRate,e_iMoney,e_pPlayer,e_pMonster);
	return 0;
}

cMiniGameActiverBase*	cMiniGameManager::RequireMiniGameActiverBase(int e_iIndex,int e_iPayRate,int e_iMoney,cPlayerBehaviorBase*e_pPlayer,cMonster*e_pMonster)
{
	cNamedTypedObjectVector< cMiniGameActiverBase >*l_pMiniGameTriggerList = m_MiniGameActiverList[ e_iIndex ];
	int	l_iCount = l_pMiniGameTriggerList->Count();
	for ( int i = 0; i < l_iCount; i++ )
	{
		 cMiniGameActiverBase* l_pMiniGameActiverBase = l_pMiniGameTriggerList->GetObject(i);
		 if ( !l_pMiniGameActiverBase->m_bInProgress )
		 {
			 l_pMiniGameActiverBase->SetupShowData(e_iPayRate,e_iMoney,e_pPlayer,e_pMonster);
			 m_MiniGameActiverTempContainer.m_ppObjects[m_MiniGameActiverTempContainer.m_iNumWorking] = l_pMiniGameActiverBase;
			 ++m_MiniGameActiverTempContainer.m_iNumWorking;
			 return l_pMiniGameActiverBase;
		 }
	}
	return 0;
}