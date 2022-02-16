#include "stdafx.h"
#include "MonsterManager.h"
#include "FishGroup.h"
#include "FishNormal.h"
#include "FishMultiCollision.h"
#include "FishDiedEffect.h"
#include "../GameApp/GameApp.h"
#include "../GameApp/FileNameDefine.h"
#include "../Bullet/Bullet.h"
#include "../PlayerBehavior/PlayerData.h"
#include "../ProbabilityFish/FishProbability.h"
#include "../ControlPanel/ControlPanel.h"
//
cMonsterManager::cMonsterManager()
{
	m_pFishDiedEffect = 0;
	m_iNumMonsterVisible = 0; 
	m_iNumMonsterHitted = 0;
	m_pHittedFish = 0;//amount is m_iMaxMonsterAlive
	m_iMaxMonsterAlive = 60;
	m_iNumMonsterAlive = 0;
	m_spFAIBehaviorParser = 0;
	m_ppWorkingMonster = 0;
	m_pAllFishGroupFile = 0;
	m_pMultiFishGroup = 0;
	m_iTotalAliveMonsterPrize = 0;
	memset(m_iCurrentBodyTypeCount,0,eBT_MAX*sizeof(int));
	memset( m_pBigWinEmitter, 0, sizeof( m_pBigWinEmitter ) );
}

cMonsterManager::~cMonsterManager()
{
	Destroy();
}

cMonster*	cMonsterManager::ProcessMonster( TiXmlElement *e_pTiXmlElement, const WCHAR *e_strTypeID,const WCHAR*e_strFishName,int e_iFishID)
{
	cMonster*l_pMonster = 0;
	if( !wcscmp( e_strTypeID, cNormalFish::TypeID ) )
	{
		l_pMonster = new cNormalFish( e_pTiXmlElement ,e_strFishName,e_iFishID );
	}
	else
	if( !wcscmp( e_strTypeID, cFishMultiCollisionData::TypeID ) )
	{
		l_pMonster = new cFishMultiCollisionData( e_pTiXmlElement ,e_strFishName,e_iFishID);
	}
	return l_pMonster;
}








bool	cMonsterManager::MyParse(TiXmlElement*e_pRoot)
{
	m_iMaxMonsterAliveWithPlayerCount.clear();
	PARSE_ELEMENT_START(e_pRoot)
		POINT	l_Table = VALUE_TO_POINT;
		this->m_iMaxMonsterAliveWithPlayerCount[l_Table.x] = l_Table.y;
	TO_NEXT_VALUE
	m_iMaxMonsterAlive = m_iMaxMonsterAliveWithPlayerCount[cFishApp::m_spControlPanel->m_iPlayerCount];
	if( m_iMaxMonsterAlive == 0 )
	{
		UT::ErrorMsg(L"m_iMaxMonsterAlive",L"can not be 0!");
	}
	if( m_iMaxMonsterAlive <= 0 || m_iMaxMonsterAlive >79979 )
		m_iMaxMonsterAlive = 100;
	SAFE_DELETE(m_pHittedFish);
	//generate plaers count multiply each player's bullet
	//int	l_iTotalHittedFish = cFishApp::m_spPlayerManager->Count()*cFishApp::m_pBulletManager->GetEachPlayerLimitCount();
	//give a big amount....FUCK
	m_pHittedFish = new sHittedFishData[3000];
	e_pRoot = e_pRoot->FirstChildElement();
	while(e_pRoot)
	{
		std::wstring	l_strMonsterName;
		int	l_iID = -1;
		if( !wcscmp(e_pRoot->Value(),L"FishBodyTypeAmount"))
		{
			int	l_iCount = 0;
			std::vector<int>l_DataCount;
			eBodyType	l_eBodyType = eBT_MAX;
			PARSE_ELEMENT_START(e_pRoot)
				COMPARE_NAME("Small")
				{
					l_eBodyType = eBT_SMALLE;
					l_iCount = VALUE_TO_INT;
				}
				else
				COMPARE_NAME("Medium")
				{
					l_eBodyType = eBT_MIDIUM;
					l_iCount = VALUE_TO_INT;
				}
				else
				COMPARE_NAME("Big")
				{
					l_eBodyType = eBT_BIG;
					l_iCount = VALUE_TO_INT;
				}
				else
				COMPARE_NAME("OccupiedCount")
				{
					 l_DataCount = GetIntegerListByCommaDivide(l_strValue,eBT_MAX);
				}
			PARSE_NAME_VALUE_END
			if( l_DataCount.size() )
			{
				assert(l_DataCount.size() == eBT_MAX);
				sOccupiedCount	l_sOccupiedCount;
				memcpy(l_sOccupiedCount.iOccupiedCount,&l_DataCount[0],sizeof(int)*eBT_MAX);
				this->m_BT_OccupiedCount[l_eBodyType] = l_sOccupiedCount;
			}
			this->m_iMaxBodyTypeCount[l_eBodyType] = l_iCount;
		}
		else
		if( !wcscmp(e_pRoot->Value(),L"FishGroupFile"))
		{
			const WCHAR*l_strFileName = e_pRoot->Attribute(L"FileName");
			if( l_strFileName )
			{
				SAFE_DELETE(m_pMultiFishGroup);
				//m_pMultiFishGroup = new cFishGroup(UT::WcharToChar(l_strFileName));
				m_pMultiFishGroup = new cFishGroup();
				m_pMultiFishGroup->ProcessData( UT::WcharToChar(l_strFileName).c_str() );
			}
		}
		else
		if( !wcscmp(e_pRoot->Value(),L"Fish"))
		{
			cFishApp::m_spMonsterManager->m_FishShowProbabilityList.ProcessFishData(e_pRoot);
		}
		if( !wcscmp(e_pRoot->Value(),L"Monster"))
		{
			cMonster*l_pMonster = 0;
			eBodyType	l_eBodyType = eBT_SMALLE;
			int	l_iID = this->Count();
			cFishGroup*l_pFishGroup = new cFishGroup;
			PARSE_ELEMENT_START(e_pRoot)
				COMPARE_NAME("Name")
				{
					l_strMonsterName = l_strValue;
					l_pFishGroup->SetName(l_strValue);
				}
				else
				COMPARE_NAME("Type")
				{
					l_pMonster = ProcessMonster(e_pRoot,l_strValue,l_strMonsterName.c_str(),l_iID);
					if( l_pMonster )
					{
						l_pMonster->SetName(l_strMonsterName.c_str());
						l_pMonster->m_iID = l_iID;
						if(!this->AddObject(l_pMonster))
						{
							UT::ErrorMsg(l_pMonster->GetName(),L"exists");
							delete l_pMonster;
						}
					}
					else
					{
						int iii=0;
					}

					if(!m_pAllFishGroupFile->AddObject(l_pFishGroup))
					{
						UT::ErrorMsg(l_strValue,L"this group exists!!");
					}
				}
				else
				COMPARE_NAME("ID")
				{
					//l_iID = VALUE_TO_INT;
				}
				else
				COMPARE_NAME("MonsterReserve")
				{
					const WCHAR*l_strFishCount = e_pRoot->Attribute(L"MonsterReserve");
					if( l_strFishCount )
					{
						m_iEachMonsterReserve[l_iID] = _wtoi(l_strFishCount);
					}
					else
					{
						m_iEachMonsterReserve[l_iID] = 10;
					}
				}
				else
				COMPARE_NAME("FishGroupFile")
				{
					l_pFishGroup->ProcessData(UT::WcharToChar(l_strValue).c_str());
				}
				else
				COMPARE_NAME("BodyType")
				{
					l_eBodyType = GetBodyType(l_strValue);
					l_pMonster->SetBodyType(l_eBodyType);
				}
				else
				COMPARE_NAME("Scale")
				{
					l_pMonster->m_fScale = VALUE_TO_FLOAT;
				}
				else
				COMPARE_NAME("GroupMovingDirectionOffsetX")
				{
					if( l_pFishGroup )
						l_pFishGroup->m_fDirectionOffsetX = VALUE_TO_VECTOR2;
				}
				else
				COMPARE_NAME("GroupMovingDirectionOffsetY")
				{
					if( l_pFishGroup )
						l_pFishGroup->m_fDirectionOffsetY = VALUE_TO_VECTOR2;
				}
				else
				COMPARE_NAME("StartOffsetTime")
				{
					if( l_pFishGroup )
						l_pFishGroup->m_fGroupShowOffsetTime = VALUE_TO_FLOAT;
				}
				else
				COMPARE_NAME("CanMissileLocked")
				{
					//l_pMonster->m_nCanMissileLocked = VALUE_TO_INT;
				}
				else
				COMPARE_NAME("MissileCradImage")
				{
					//CardImageData TempMissileCardImageData;

					//TempMissileCardImageData.FishID    = l_iID;
					//TempMissileCardImageData.ImageName = l_strValue;

					//m_MissileCardImageData.push_back( TempMissileCardImageData );
				}
				else
				COMPARE_NAME("CoinStackCardImage")
				{
					//CardImageData TempCoinStackCardImageData;
					//TempCoinStackCardImageData.FishID    = l_iID;
					//TempCoinStackCardImageData.ImageName = l_strValue;

					//m_CoinStackCardImageData.push_back( TempCoinStackCardImageData );
				}
			PARSE_NAME_VALUE_END
		}
		e_pRoot = e_pRoot->NextSiblingElement();
	}
	return true;
}








void	cMonsterManager::Init()
{
	this->Destroy();
	m_iTotalAliveMonsterPrize = 0;
	m_pFishDiedEffect = new cFishDiedEffect();
	m_pAllFishGroupFile = new cNamedTypedObjectVector<cFishGroup>;
	m_spFAIBehaviorParser = new cFAIBehaviorParser;
	bool	l_b = this->ParseWithMyParse(MONSTER_DATA);
	if( !l_b )
	{
		UT::ErrorMsg(MONSTER_DATA,"parse failed");
	}
	int	l_iCount = this->Count();
	for( int i=0;i<l_iCount;++i )
	{
		cMonster*l_pCloneMonster = dynamic_cast<cMonster*>(this->GetObject(i));
		cNamedTypedObjectVector<cMonster>*l_pFishList = new cNamedTypedObjectVector<cMonster>;
		l_pFishList->SetName(l_pCloneMonster->GetName());
		m_AllFish.AddObjectNeglectExist(l_pFishList);
		l_pCloneMonster->CollisionDataCheck();
		int	l_iReserveCount = this->m_iEachMonsterReserve[i];
		std::wstring	l_str;
		for( int j=0;j<l_iReserveCount;++j )
		{
			cMonster*l_pMonster = dynamic_cast<cMonster*>(l_pCloneMonster->Clone());
//			l_str = UT::ComposeMsgByFormat(L"%s%d",l_pMonster->GetName(),j);
//			l_pMonster->SetName(l_str.c_str());
			l_pFishList->AddObjectNeglectExist(l_pMonster);
		}
	}
	m_iSpecialScenceMaxMonsterAlive = 2000;
	m_ppWorkingMonster = new cMonster*[m_iSpecialScenceMaxMonsterAlive];
	for( int i=0;i<m_iSpecialScenceMaxMonsterAlive;++i )
	{
		m_ppWorkingMonster[i] = 0;
	}

// 	this->BossOut();

	if ( !cGameApp::m_spPaticleManager->GetObject( L"BigPrize" ) )
	{
		cGameApp::m_spPaticleManager->Parse( BIGWIN_PARTICLE );
	}

	for ( int i = 0; i < sizeof( m_pBigWinEmitter ) / sizeof( m_pBigWinEmitter[0] ); i++ )
	{
		 m_pBigWinEmitter[i] = new cPrtEmitter( cGameApp::m_spPaticleManager->GetObject( L"BigPrize" ) );
	}

	memset( m_BigWinEmitterDisplay, 0, sizeof( m_BigWinEmitterDisplay ) );

}








void	cMonsterManager::Update(float e_fElpaseTime)
{
	memset(m_iCurrentBodyTypeCount,0,eBT_MAX*sizeof(int));
	m_iTotalAliveMonsterPrize = 0;
	m_FishShowProbabilityList.Update(e_fElpaseTime);
	m_iNumMonsterVisible = 0;
	for( int i=0; i<m_iNumMonsterAlive; ++i )
	{
		cMonster *l_pMonster = m_ppWorkingMonster[i];

		if(!l_pMonster)
		{
			break;
		}
		l_pMonster->Update( e_fElpaseTime );

		if(l_pMonster ->IsVisible())
		{
			m_iNumMonsterVisible ++;
		}

		if( l_pMonster->m_eMonsterStatus == eMS_WAITING_FOR_CLEAN )
		{
			--m_iNumMonsterAlive;
			m_ppWorkingMonster[i] = m_ppWorkingMonster[m_iNumMonsterAlive];
			--i;
			l_pMonster->SetMonsterStatus( eMS_NONE );
		}

		sOccupiedCount	l_OccupiedCount = m_BT_OccupiedCount[l_pMonster->GetBodyType()];
		for( int i=0;i<eBT_MAX;++i )
		{
			m_iCurrentBodyTypeCount[i] += l_OccupiedCount.iOccupiedCount[i];
		}
		m_iTotalAliveMonsterPrize += l_pMonster->GetPrize();
	}
	//
	for ( int i = 0; i < sizeof( m_BigWinEmitterDisplay ) / sizeof( m_BigWinEmitterDisplay[0] ); i++ )
	{
		 if ( m_BigWinEmitterDisplay[i] != 0 && m_pBigWinEmitter[i] )
		 {
			 m_pBigWinEmitter[i]->Update( e_fElpaseTime );
		 }
	}
	if( m_pFishDiedEffect )
		m_pFishDiedEffect->Update(e_fElpaseTime);
}

void	cMonsterManager::LastRender()
{
	bool bSkipRenderMissileDirTex(0);

	for( int i=0;i<m_iNumMonsterAlive;++i )
	{
		if( m_ppWorkingMonster[i]->GetMonsterStatus() == eMS_DIED_SHOW )
		{
			m_ppWorkingMonster[i]->Render();
		} 
	}

	for ( int i = 0; i < sizeof( m_BigWinEmitterDisplay ) / sizeof( m_BigWinEmitterDisplay[0] ); i++ )
	{
		 if ( m_BigWinEmitterDisplay[i] != 0 && m_pBigWinEmitter[i] )
		 {
			 m_pBigWinEmitter[i]->Render();

			 if ( m_pBigWinEmitter[i]->GetCurrentWorkingParticles() == 0 )
				 m_BigWinEmitterDisplay[i] = 0;
		 }
	}
	if( m_pFishDiedEffect )
		m_pFishDiedEffect->Render();
}








void	cMonsterManager::Render()
{
	int	l_iIDCount = this->Count();
	for( int j=0;j<l_iIDCount;++j )
	{
		for( int i=0;i<m_iNumMonsterAlive;++i )
		{
			if( m_ppWorkingMonster[i]->GetMonsterStatus() != eMS_DIED_SHOW )
			{
				if( j== m_ppWorkingMonster[i]->GetID() )
				{
					m_ppWorkingMonster[i]->Render();
				}
			}
		}
	}
	//
}








void	cMonsterManager::DebugRender()
{
	for( int i=0;i<m_iNumMonsterAlive;++i )
	{
		m_ppWorkingMonster[i]->DebugRender();
	}
	std::wstring	l_str = UT::ComposeMsgByFormat(L"Fish:%d",m_iNumMonsterAlive);
	cGameApp::m_spGlyphFontRender->RenderFont(0,200,l_str.c_str());
}








void	cMonsterManager::Destroy()
{
	m_BT_OccupiedCount.clear();
	m_iNumMonsterAlive = 0;
	m_iNumMonsterHitted = 0;
	m_iMaxMonsterAliveWithPlayerCount.clear();
	m_iEachMonsterReserve.clear();
	m_AllFish.Destroy();
	cNamedTypedObjectVector<cMonster>::Destroy();
	if( m_ppWorkingMonster )
	{
		for( int i=0;i<m_iSpecialScenceMaxMonsterAlive;++i )
			m_ppWorkingMonster[i] = 0;
		SAFE_DELETE_ARRAY(m_ppWorkingMonster);
	}
	SAFE_DELETE(m_pHittedFish);
	SAFE_DELETE(m_spFAIBehaviorParser);
	SAFE_DELETE(m_pAllFishGroupFile);
	SAFE_DELETE(m_pFishDiedEffect);

	for ( int i = 0; i < sizeof( m_pBigWinEmitter ) / sizeof( m_pBigWinEmitter[0] ); i++ )
		 if ( m_pBigWinEmitter[i] )
			 SAFE_DELETE( m_pBigWinEmitter[i] );
}

bool	cMonsterManager::RestMonsterToRequire(int e_iFishIndex,int e_iCount)
{
	cMonster *l_pMonster = this->GetObject( e_iFishIndex );
	return RestMonsterToRequire(l_pMonster->GetName(),e_iCount);
}

bool	cMonsterManager::RestMonsterToRequire(const WCHAR*e_strName,int e_iCount)
{
	int	l_iRestMonsterToRequire = 0;
	if( m_iNumMonsterAlive < m_iMaxMonsterAlive )
	{
		//cMonster*l_pMonster = this->GetObject(e_strFishName);
		int	l_iIndex = this->GetObjectIndexByName(e_strName);
		if( l_iIndex != -1 )
		{
			cMonster *l_pMonster = 0;
			cNamedTypedObjectVector<cMonster> *l_pFishList = this->m_AllFish[l_iIndex];
			if( l_pFishList->Count() <1 )
				return false;
			eBodyType l_eBodyType = l_pFishList->GetObject(0)->GetBodyType();
			int	l_iBodyTypeCount = this->m_iCurrentBodyTypeCount[l_eBodyType];
			int	l_iCount = l_pFishList->Count();
			for( int i=0; i<l_iCount; ++i )
			{
				l_pMonster = l_pFishList->GetObject(i);
				eMonsterStatus	l_eMonsterStatus = l_pMonster->GetMonsterStatus();
				if( l_eMonsterStatus == eMS_NONE )
				{
					eBodyType l_eBodyType = l_pMonster->GetBodyType();
					if( l_iBodyTypeCount +1 <=this->m_iMaxBodyTypeCount[l_eBodyType] )
					{
						++l_iBodyTypeCount;
						++l_iRestMonsterToRequire;
						if( l_iRestMonsterToRequire >= e_iCount )
							return true;
					}
				}
				l_pMonster = 0;
			}
		}
	}
	return false;
}







cMonster*	cMonsterManager::Require( int e_iFishIndex, bool e_bForceFetch )
{
	cMonster *l_pMonster = this->GetObject( e_iFishIndex );
	if( l_pMonster )
	{
		return Require( l_pMonster->GetName(), e_bForceFetch );
	}
	return 0;
}








cMonster*	cMonsterManager::Require( const WCHAR *e_strFishName, bool e_bForceFetch )
{
	if( m_iNumMonsterAlive < m_iMaxMonsterAlive || e_bForceFetch )
	{
		//cMonster*l_pMonster = this->GetObject(e_strFishName);
		int	l_iIndex = this->GetObjectIndexByName(e_strFishName);
		if( l_iIndex != -1 )
		{
			cMonster *l_pMonster = 0;
			cNamedTypedObjectVector<cMonster> *l_pFishList = this->m_AllFish[l_iIndex];
			int	l_iCount = l_pFishList->Count();
			for( int i=0; i<l_iCount; ++i )
			{
				l_pMonster = l_pFishList->GetObject(i);
				eMonsterStatus	l_eMonsterStatus = l_pMonster->GetMonsterStatus();
				if( l_eMonsterStatus == eMS_NONE )
				{
					eBodyType l_eBodyType = l_pMonster->GetBodyType();
					if(this->m_iCurrentBodyTypeCount[l_eBodyType] +1 <=this->m_iMaxBodyTypeCount[l_eBodyType] || e_bForceFetch)
					{
						this->m_iCurrentBodyTypeCount[l_eBodyType] += 1;
						//
						this->m_ppWorkingMonster[m_iNumMonsterAlive] = l_pMonster;
						//
						++m_iNumMonsterAlive;
						//
						Vector3	l_vPos = cFishApp::m_spProbabilityFish->GetRandomPos(l_pMonster);
						l_pMonster->SetPos(l_vPos);
						l_pMonster->Init();
						l_pMonster->Update(0.0f);
						l_pMonster->SetMonsterStatus(eMS_ALIVE);
						return l_pMonster;
					}
				}
				l_pMonster = 0;
			}
		}
	}
	//
	return 0;
}








int	cMonsterManager::FishGroupGo(WCHAR*e_strFishName,int e_iTargetCount)
{
	int	l_iIndex = m_pAllFishGroupFile->GetObjectIndexByName(e_strFishName);
	cFishGroup*l_pFishGroup = this->m_pAllFishGroupFile->GetObject(l_iIndex);
	if( l_pFishGroup )
	{
		int	l_iOutCount = l_pFishGroup->FishGroupGoFishesCount(l_iIndex);
		if( e_iTargetCount >= l_iOutCount )
		{
			int	l_iCount = l_pFishGroup->m_FishGroup.Count();
			if( l_iCount == 0 )
				return 0;
			int	l_iValue = rand()%l_iCount;
			if( this->RestMonsterToRequire(e_strFishName,e_iTargetCount) )
			{
				return l_pFishGroup->FishGroupGo(l_iValue);
			}
		}
	}
	return 0;
}

int		cMonsterManager::FishGroupGo()
{
	if(!m_pMultiFishGroup)
		return 0;
	int	l_iCount = m_pMultiFishGroup->m_FishGroup.Count();
	if( l_iCount == 0 )
		return 0;
	int	l_iValue = rand()%l_iCount;
	return m_pMultiFishGroup->FishGroupGo(l_iValue);

}







int	cMonsterManager::FishGroupGo(int e_iFishID,int e_iCount)
{
	if( !this->RestMonsterToRequire(e_iFishID,e_iCount) )
	{
		return 0;
	}
	cFishGroup*l_pFishGroup = m_pAllFishGroupFile->GetObject(e_iFishID);
	float	l_fTimeOffset = 0.5f;
	sMinMaxData<float>	l_fDirectionOffsetX = {-0.1f,0.1f};
	sMinMaxData<float>	l_fDirectionOffsetY = {-0.1f,0.1f};
	if( l_pFishGroup )
	{
		l_fTimeOffset = l_pFishGroup->m_fGroupShowOffsetTime;
		l_fDirectionOffsetX = l_pFishGroup->m_fDirectionOffsetX;
		l_fDirectionOffsetY = l_pFishGroup->m_fDirectionOffsetY;
	}
	int	l_iGoCount = 0;
	if(this->m_iMaxMonsterAlive-this->m_iNumMonsterAlive>e_iCount)
	{
		cMonster*l_pFirstMonster = this->Require(e_iFishID);
		if( !l_pFirstMonster )
			return 0;
		float	l_fRadius = l_pFirstMonster->m_fRadius;
		Vector3	l_vPos = cFishApp::m_spProbabilityFish->GetRandomPos(l_pFirstMonster);
		l_pFirstMonster->SetPos(l_vPos);
		l_pFirstMonster->SetPath();
		cCurveWithTime l_Path = *l_pFirstMonster->GetPath();
		e_iCount -= 1;
		for( int i=0;i<e_iCount;++i )
		{
			cCurveWithTime l_Path2 = l_Path.GetOffsetCurve(l_fTimeOffset*i+l_fTimeOffset,l_fDirectionOffsetX,l_fDirectionOffsetY,5);
			l_Path2.SetCalAngle(true);
			cMonster*l_pMonster = this->Require(e_iFishID);
			if( l_pMonster )
			{
				//fuck...performance is fucking slow.because path set twice
				l_pMonster->SetPath();
				l_pMonster->SetPos(l_vPos);
				//l_Path2.FixPoint(0,l_vPos,0.f);
				l_pMonster->SetPath(&l_Path2);
			}
			else
				return i;
		}
		return e_iCount;
	}
	return 0;
}








int		cMonsterManager::GetSmallestPrizeFishID(int*e_piPrize)
{
	int	l_iCount = this->m_AllFish.Count();
	int	l_iSmallestPrize = 799979;
	int	l_iSmallestIndex = -1;
	for( int i=0;i<l_iCount;++i )
	{
		if(l_iSmallestPrize > m_AllFish[i]->GetObject(0)->GetPrize() )
		{
			l_iSmallestPrize = m_AllFish[i]->GetObject(0)->GetPrize();
			l_iSmallestIndex = i;
			*e_piPrize = l_iSmallestPrize;
		}
	}
	return l_iSmallestIndex;
}

int		cMonsterManager::GetSmallestPrizeFishIDFromAliveFish(int*e_piPrize)
{
	int	l_iSmallestPrize = 799979;
	int	l_iSmallestIndex = -1;
	for( int i=0;i<this->m_iNumMonsterAlive;++i )
	{
		cMonster*l_pMonster = this->m_ppWorkingMonster[i];
		if( l_pMonster->IsStatusAllowToDied() )
		{
			if(l_iSmallestPrize > l_pMonster->GetPrize() )
			{
				l_iSmallestPrize = m_ppWorkingMonster[i]->GetPrize();
				l_iSmallestIndex = i;
				*e_piPrize = l_iSmallestPrize;
			}
		}
	}
	return l_iSmallestIndex;
}








void		cMonsterManager::MonsterLeaveScene()
{
	for( int i=0; i<this->m_iNumMonsterAlive; ++i )
	{
		cMonster *l_pMonster = this->m_ppWorkingMonster[i];
		if( l_pMonster->IsStatusAllowToDied() )
		{
			l_pMonster->MonsterLeaveScene();
		}
	}
}


void cMonsterManager::DisplayBigWinParticle( Vector3 DisplayPos )
{
	static int DisplayIdx(0);

	if ( m_pBigWinEmitter[ DisplayIdx ] )
	{
		m_pBigWinEmitter[ DisplayIdx ]->Emit( DisplayPos, false );

		m_BigWinEmitterDisplay[ DisplayIdx ] = 1;

		DisplayIdx++;

		if ( DisplayIdx >= 10 )
			DisplayIdx = 0;
		cGameApp::SoundPlay( L"12", false );
	}
}

void	cMonsterManager::AddFishDiedPrizeAnimation(cMPDI*e_pMPDI,int e_iCount)
{
	if(m_pFishDiedEffect)
	{
		m_pFishDiedEffect->AddMPDI(e_pMPDI,e_iCount);
	}
}

void	cMonsterManager::ShowFishDiedPrizeAnimation(const WCHAR*e_strName,Vector3 e_vPos,float e_fAngle)
{
	if(m_pFishDiedEffect)
	{
		m_pFishDiedEffect->Show(e_strName,e_vPos,e_fAngle);
	}
}