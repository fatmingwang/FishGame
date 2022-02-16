#include "stdafx.h"
#include "BulletManager.h"
#include "Bullet.h"
#include "../ControlPanel/ControlPanel.h"
#include "../ProbabilityFish/FishProbability.h"
#include "../GameApp/GameApp.h"
#include "../Monster/MonsterManager.h"
#include "../MiniGame/MiniGameBase.h"
//
bool	cBulletManager::m_sbCollideSkip = false;
//







cBulletManager::cBulletManager()
{
	m_iNumWorking = 0;
	m_BulletCollisionData.SetFromResource(true);;
	m_WebCollisionData.SetFromResource(true);
	m_ppUpdateBulletData = 0;
}








cBulletManager::~cBulletManager()
{
	Destroy();
}








void		cBulletManager::Destroy()
{
	m_2DImageCollisionData.Destroy();
	m_BulletCollisionData.Destroy();
	m_WebCollisionData.Destroy();
	SAFE_DELETE(m_ppUpdateBulletData);
}








void		cBulletManager::Collide(cMonsterManager*e_pMonsterManager)
{
	m_sbCollideSkip = false;
	e_pMonsterManager->m_iNumMonsterHitted = 0;
	for(int j=0;j<this->m_iNumWorking;++j)
	{
		cBullet*l_pBullet = this->m_ppUpdateBulletData[j];
		//mini game skip
		if( !l_pBullet->m_pBulletCollision )
			continue;
		eBulletStatus	l_eBulletStatus = l_pBullet->m_eBulletStatus;
		bool	l_bHitted = false;
		for( int i=0;i<e_pMonsterManager->m_iNumMonsterAlive;++i )
		{
			cMonster*l_pMonster = e_pMonsterManager->m_ppWorkingMonster[i];
			//skip monster has been controlled by mini game.
			if( l_pMonster->IsStatusAllowToDied() )
			{
				if( l_eBulletStatus == eBS_MOVING && l_pMonster->IsCollide(l_pBullet->m_pBulletCollision) )
				{
					l_pMonster->AddBiteMoney(l_pBullet->m_iPayRate);
					l_pMonster->SetMonsterStatus(eMS_HITTED);
					l_bHitted = true;
					//
					sHittedFishData*l_pHittedFishData = &e_pMonsterManager->m_pHittedFish[e_pMonsterManager->m_iNumMonsterHitted];
					++l_pBullet->m_iNumMonsterShoot;
					l_pHittedFishData->m_pBullet = l_pBullet;
					l_pHittedFishData->m_pMonster = l_pMonster;
					l_pHittedFishData->m_pPlayer = l_pBullet->m_pPlayer;
					++e_pMonsterManager->m_iNumMonsterHitted;
					break;
				}
				//else
				//if( l_eBulletStatus == eBS_EXPLOSION_DONE && l_pMonster->IsCollide(l_pBullet->m_pWebCllision) )
				//{
					////l_pMonster->SetMonsterStatus(eMS_HITTED);
					//sHittedFishData*l_pHittedFishData = &e_pMonsterManager->m_pHittedFish[e_pMonsterManager->m_iNumMonsterHitted];
					//++l_pBullet->m_iNumMonsterShoot;
					//l_pHittedFishData->m_pBullet = l_pBullet;
					//l_pHittedFishData->m_pMonster = l_pMonster;
					//l_pHittedFishData->m_pPlayer = l_pBullet->m_pPlayer;
					//++e_pMonsterManager->m_iNumMonsterHitted;
					////immediately to dead
					////l_pMonster->SetMonsterStatus(eMS_WAITING_FOR_CLEAN);
					//l_bHitted = true;
				//}
			}
		}
		if( l_bHitted || l_eBulletStatus == eBS_EXPLOSION_DONE )
		{
			if( l_eBulletStatus == eBS_MOVING )
			{
				l_pBullet->m_eBulletStatus = eBS_EXPLOSION;
				l_pBullet->CastNetFishing();
				l_pBullet->m_iNumMonsterShoot = 0;

				if ( !cGameApp::m_spSoundParser->GetObject( L"4" )->IsPlay() )
					cGameApp::SoundPlay(L"4",true);
			}
			else
			if( l_eBulletStatus == eBS_EXPLOSION_DONE )
			{
				//prepare to reuse
				l_pBullet->Init();
				--m_iNumWorking;
				m_ppUpdateBulletData[j] = m_ppUpdateBulletData[m_iNumWorking];
			}
		}
	}
}








cBullet*	cBulletManager::Require( int e_iPlayerIndex, int e_iLevel, int e_iPayRate )
{
	cBulletPlayer*l_pPlayerBullet = this->GetObject(e_iPlayerIndex);
	if( l_pPlayerBullet )
	{
		cBullet*l_pBullet = l_pPlayerBullet->Require( e_iLevel );
		if( l_pBullet )
		{
			l_pBullet->m_iPayRate = e_iPayRate;
			l_pBullet->m_iPowerLevel = e_iLevel;
			//
			//change to player to call this cFishApp::m_spProbabilityFish->RevenueAdd(l_pBullet->m_iPayRate);
			this->m_ppUpdateBulletData[this->m_iNumWorking] = l_pBullet;
			l_pBullet->Init();
			++m_iNumWorking;
			cGameApp::SoundPlay(L"3",true);
		}
		return l_pBullet;
	}
	return 0;
}








void	cBulletManager::Init()
{
	m_iNumWorking = 0;
	this->Destroy();
	if( this->Count() == 0 )
	{
		if(!this->ParseWithMyParse(BULLET_DATA))
		{
			UT::ErrorMsg(BULLET_DATA,"parse failed!");
		}
	}
}








void	cBulletManager::Update(float e_fElpaseTime)
{
	for( int i=0;i<m_iNumWorking;++i )
	{
		m_ppUpdateBulletData[i]->Update(e_fElpaseTime);
		if( m_ppUpdateBulletData[i]->m_eBulletStatus == eBS_DONE )
		{
			--m_iNumWorking;
			m_ppUpdateBulletData[i]->m_eBulletStatus = eBS_IDLE;
			m_ppUpdateBulletData[i] = m_ppUpdateBulletData[m_iNumWorking];
			
		}
	}
}








void	cBulletManager::Render()
{
	for( int i=0;i<m_iNumWorking;++i )
	{
		m_ppUpdateBulletData[i]->Render();
	}
}








void	cBulletManager::DebugRender()
{
	for( int i=0;i<m_iNumWorking;++i )
	{
		m_ppUpdateBulletData[i]->DebugRender();
	}
}

//<Root>
//	<CollisionFile FileName="" />
//	<Bullet MPDIList="00.mpdi" MovingMPDI="Moving" >
//		<LV1 Name=""/>
//	</Bullet>
//	<Web MPDIList="00.mpdi" ExlposionMPDI="Explosion">
//		<LV1 Name=""/>
//	</Web>
//</Root>








void	cBulletManager::ProcessCollisionFile(TiXmlElement*e_pRoot)
{
	const WCHAR*l_strFileName = e_pRoot->Attribute(L"FileName");
	if( l_strFileName )
	{
		if(!m_2DImageCollisionData.Parse(UT::WcharToChar(l_strFileName)))
		{
			UT::ErrorMsg(l_strFileName,L"parse failed");
		}
	}
}
//	<PlayerBullet MPDIList="00.mpdi" MovingMPDI="Moving" >
//		<LV1 Name=""/>
//	</PlayerBullet>








void	cBulletManager::ProcessPlayerBullet(TiXmlElement*e_pRoot)
{
	const WCHAR*l_strMPDIList = e_pRoot->Attribute(L"MPDIList");
	const WCHAR*l_strIndex = e_pRoot->Attribute(L"Index");
	const WCHAR*l_strBulletColor = e_pRoot->Attribute(L"BulletColor");
	Vector4	l_vBulletColor = Vector4::One;
	if( l_strBulletColor )
	{
		l_vBulletColor = GetVector4(l_strBulletColor);
		l_vBulletColor.x += 0.4f;
		l_vBulletColor.y += 0.4f;
		l_vBulletColor.z += 0.4f;
	}
	int	l_iIndex = GetInt(l_strIndex);
	if( l_strIndex )
	{
		if(  _wtoi(l_strIndex) >= cFishApp::m_spControlPanel->m_iPlayerCount )
			return;
	}
	cMPDIList*l_pMPDIList = cGameApp::GetMPDIListByFileName(l_strMPDIList);
	const WCHAR*l_strMPDIList2 = e_pRoot->Attribute( L"NetMPDIList" );
	cMPDIList* l_pNetMPDIList = cGameApp::GetMPDIListByFileName( e_pRoot->Attribute( L"NetMPDIList" ) );
	cBulletPlayer*l_pPlayerBullet = new cBulletPlayer();
	e_pRoot = e_pRoot->FirstChildElement();
	while( e_pRoot )
	{
		cBulletPlayer::sBulletData*l_pBulletData = new cBulletPlayer::sBulletData();
		PARSE_ELEMENT_START(e_pRoot)
			COMPARE_NAME("BulletCollision")
			{
				cbtConcaveShapeList*l_pbtConcaveShapeList = m_2DImageCollisionData.GetObject(l_strValue);
				if( l_pbtConcaveShapeList )
				{
					l_pBulletData->m_pBulletCollision = l_pbtConcaveShapeList;
					this->m_WebCollisionData.AddObjectNeglectExist(l_pbtConcaveShapeList);
				}
				else
				{
					UT::ErrorMsg(l_strValue,L"can not find collision data");
				}
			}
			else
			COMPARE_NAME("BulletMPDI")
			{
				l_pBulletData->m_pBulletMPDI = l_pMPDIList->GetObject(l_strValue);
				l_pBulletData->m_pBulletMPDI->SetColorScale(l_vBulletColor);
			}
			else
			COMPARE_NAME("Speed")
			{
				l_pBulletData->m_fSpeed = VALUE_TO_FLOAT;
			}
			else
			COMPARE_NAME("WebMPDI")
			{
				l_pBulletData->m_pWebMPDI = l_pNetMPDIList->GetObject(l_strValue);;
				//l_pBulletData->m_pWebMPDI->SetColor(l_vBulletColor);
			}
			else
			COMPARE_NAME("WebCollsiion")
			{
				cbtConcaveShapeList*l_pbtConcaveShapeList = m_2DImageCollisionData.GetObject(l_strValue);
				if( l_pbtConcaveShapeList )
				{
					l_pBulletData->m_pbtWebCollision = l_pbtConcaveShapeList;
					this->m_WebCollisionData.AddObjectNeglectExist(l_pbtConcaveShapeList);
				}
				else
				{
					UT::ErrorMsg(l_strValue,L"can not find collision data");
				}
			}
			else
			COMPARE_NAME("WebScale")
			{
				if(l_pBulletData->m_pWebMPDI)
				{
					l_pBulletData->m_pWebMPDI->SetScale(VALUE_TO_FLOAT);
				}
			}
		PARSE_NAME_VALUE_END
		l_pPlayerBullet->AddBulletData(l_pBulletData);
		e_pRoot = e_pRoot->NextSiblingElement();
	}
	l_pPlayerBullet->ProcessBullet(m_iEachPlayerLimitCount);
	this->AddObjectNeglectExist(l_pPlayerBullet);
}









void	cBulletManager::ProcessWeb(TiXmlElement*e_pRoot)
{
	e_pRoot = e_pRoot->FirstChildElement();
	while( e_pRoot )
	{
		const WCHAR*l_strColisionName = e_pRoot->Attribute(L"Name");
		cbtConcaveShapeList*l_pbtConcaveShapeList = m_2DImageCollisionData.GetObject(l_strColisionName);
		if( l_pbtConcaveShapeList )
		{
			m_BulletCollisionData.AddObjectNeglectExist(l_pbtConcaveShapeList);
		}
		else
		{
			UT::ErrorMsg(l_strColisionName,L"can not find collision data");
		}
		e_pRoot = e_pRoot->NextSiblingElement();
	}
}









bool	cBulletManager::MyParse(TiXmlElement*e_pRoot)
{
	Destroy();
	const WCHAR*l_strEachPlayerLimitCount = e_pRoot->Attribute(L"EachPlayerLimitCount");
	m_iEachPlayerLimitCount = 50;
	if( l_strEachPlayerLimitCount )
		m_iEachPlayerLimitCount = _wtoi(l_strEachPlayerLimitCount);
	e_pRoot = e_pRoot->FirstChildElement();
	while( e_pRoot )
	{
		const WCHAR*l_strName = e_pRoot->Value();
		COMPARE_NAME("CollisionFile")
		{
			ProcessCollisionFile(e_pRoot);
		}
		else
		COMPARE_NAME("PlayerBullet")
		{
			ProcessPlayerBullet(e_pRoot);
		}
		else
		COMPARE_NAME("Web")
		{
			ProcessWeb(e_pRoot);
		}
		e_pRoot = e_pRoot->NextSiblingElement();
	}
	int	l_iCount = this->Count();
	if( l_iCount > 0 )
	{
		cBulletPlayer*l_pPlayerBullet = this->GetObject(0);
		int	l_iLevel = l_pPlayerBullet->m_BulletDataList.size();
		m_ppUpdateBulletData = new cBullet*[m_iEachPlayerLimitCount*l_iCount*l_iLevel];
	}
	return true;
}
