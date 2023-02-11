#include "stdafx.h"
#include "Weapon.h"
#include "../../Core/FMEvent/AllEventInclude.h"
#include "../ControlPanel/ControlPanel.h"
#include "../GameApp/GameApp.h"
#include "../Bullet/BulletManager.h"
#include "../Bullet/Bullet.h"
#include "PlayerData.h"


cBulletFireStatus::cBulletFireStatus(TiXmlElement* e_pTiXmlElement,Frame*e_pPlayerFrame)
{
	m_pmatPlayerFrame = e_pPlayerFrame;
	m_pPlayerLvUIMPDI = 0;
	m_vLevelPos = Vector3::Zero;
	m_pLevelNumberImage = 0;	
	m_iLastCurrentLevel = -1;
	m_iLevelRangeIndex = 0;
	e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while( e_pTiXmlElement )
	{
		sLevelRange	l_LevelRange;
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME("Min")
			{
				l_LevelRange.iStart = VALUE_TO_INT;
			}
			else
			COMPARE_NAME("Max")
			{
				l_LevelRange.iEnd = VALUE_TO_INT;
			}
			else
			COMPARE_NAME("Index")
			{

			}
			else
			COMPARE_NAME("AddValueRate")
			{
				//l_LevelRange.fAddValueRate = VALUE_TO_FLOAT
			}
			else
			COMPARE_NAME("AddValue")
			{
				//l_LevelRange.iAddValueRate = VALUE_TO_INT;
			}
		PARSE_NAME_VALUE_END
		l_LevelRange.iAddValueRate = cFishApp::m_spControlPanel->m_iBulletPayRateAddPoint;
		m_LevelRangeList.push_back(l_LevelRange);
		e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
	}
	cPuzzleImage*l_pNumber = cGameApp::GetPuzzleImageByFileName((WCHAR*)PLAYER_NUMRIAL);
	if(l_pNumber)
	{
		m_pLevelNumberImage = l_pNumber->GetNumerialImageByName(L"TubeLVNumerial0",L"TubeLVNumerial9");
	}
	else
	{
		UT::ErrorMsg(L"PlayerUI.pi",L"not exit");
	}

	m_iCurrentLevel = cFishApp::m_spControlPanel->m_iBulletPayRateMinPoint;
	cMPDIList*l_pPlayerMPDIList = cGameApp::GetMPDIListByFileName(PLAYERUI_MPDI);
	if( l_pPlayerMPDIList )
	{
		cMultiPathDynamicImage*l_pPlayerUIPosInfo = l_pPlayerMPDIList->GetObject(L"PlayerUIPosInfo");
		if( l_pPlayerUIPosInfo )
		{
			m_vLevelPos = l_pPlayerUIPosInfo->GetObject(L"TubeLVNumerial1")->GetOriginalPointList()[0];
			m_vLevelPos = e_pPlayerFrame->GetWorldTransform().TransformCoordinate(m_vLevelPos);
		}
	}
	
	cMPDI*l_pPlayerLvUI = l_pPlayerMPDIList->GetObject( L"PlayerLvUI" );
	assert(l_pPlayerLvUI);

	m_pPlayerLvUIMPDI = new cMultiPathDynamicImage( l_pPlayerLvUI );
	m_pPlayerLvUIMPDI->Init();
	m_pPlayerLvUIMPDI->SetParent(e_pPlayerFrame,false);
	//for( int i=0;i<m_pPlayerLvUIMPDI->Count();++i )
	{
	//	m_pPlayerLvUIMPDI->GetObject(i)->SetExtractTransform(e_pmatPlayerTransform);
	}

	LevelCheck();
}

cBulletFireStatus::~cBulletFireStatus()
{
	SAFE_DELETE(m_pLevelNumberImage);
	SAFE_DELETE(m_pPlayerLvUIMPDI);
}

void	cBulletFireStatus::Update(float e_fElpaseTime)
{
	m_iLastCurrentLevel = m_iLevelRangeIndex;

	if(m_pPlayerLvUIMPDI)
		m_pPlayerLvUIMPDI->Update(e_fElpaseTime);
}

void	cBulletFireStatus::Render()
{
	if(m_pPlayerLvUIMPDI)
		m_pPlayerLvUIMPDI->Render();

	if( m_pLevelNumberImage )
	{
		m_pLevelNumberImage->DrawOnCenter(this->m_iCurrentLevel,(int)m_vLevelPos.x,(int)m_vLevelPos.y,cMatrix44::StripPosFromMatrix(m_pmatPlayerFrame->GetWorldTransform()));
	}
}

void	cBulletFireStatus::LevelCheck()
{
	m_iLevelRangeIndex = 0;
	if( m_iCurrentLevel < cFishApp::m_spControlPanel->m_iBulletPayRateMinPoint ||
		m_iCurrentLevel > cFishApp::m_spControlPanel->m_iBulletPayRateLimit)
	{
		m_iCurrentLevel = cFishApp::m_spControlPanel->m_iBulletPayRateMinPoint;
		m_iLevelRangeIndex = 0;
	}
	for( size_t i=0;i<m_LevelRangeList.size();++i )
	{
		if(m_iCurrentLevel>=m_LevelRangeList[i].iStart&&m_iCurrentLevel<=m_LevelRangeList[i].iEnd)
		{
			m_iLevelRangeIndex = i;
			return;
		}
	}
}

bool	cBulletFireStatus::IsLevelChange()
{
	return m_iLastCurrentLevel == m_iLevelRangeIndex?false:true;
}

void	cBulletFireStatus::LevelChange(bool e_bLevelChange)
{
	sLevelRange	l_sLevelRange = m_LevelRangeList[this->m_iLevelRangeIndex];	
	if( e_bLevelChange )
	{
		m_iCurrentLevel += l_sLevelRange.iAddValueRate;
		if( this->m_iCurrentLevel > l_sLevelRange.iEnd )
		{
			m_iLevelRangeIndex += 1;
			if( m_iLevelRangeIndex >= (int)m_LevelRangeList.size() )
			{
				m_iLevelRangeIndex = 0;
				l_sLevelRange = m_LevelRangeList[this->m_iLevelRangeIndex];
				m_iCurrentLevel = l_sLevelRange.iStart;
			}
			else
			{
			
			}
		}
		else
		{
		
		}
	}
	else
	{
		m_iCurrentLevel -= l_sLevelRange.iAddValueRate;
		if( this->m_iCurrentLevel < l_sLevelRange.iStart )
		{
			m_iLevelRangeIndex -= 1;
			if( m_iLevelRangeIndex < 0 )
			{
				m_iLevelRangeIndex = (int)m_LevelRangeList.size()-1;
				l_sLevelRange = m_LevelRangeList[this->m_iLevelRangeIndex];
				m_iCurrentLevel = l_sLevelRange.iStart;
			}
			else
			{
			
			}
		}
		else
		{
		
		}	
	}
	LevelCheck();
}
//====================================================
//
//
//
//====================================================
cFishPlayerWeapon::cFishPlayerWeapon(TiXmlElement* e_pTiXmlElement,Frame*e_pPlayerFrame)
{
	m_pPlayerFrame = e_pPlayerFrame;
	Vector3	l_vScale,l_vRotation,l_vTranslation;
	float	l_fInvert;
	m_pPlayerFrame->GetWorldTransform().Decompose(l_vScale,l_vRotation,l_vTranslation,l_fInvert);
	m_fOffsetAngle = D3DXToDegree(l_vRotation.z);
	m_fCurrentAnimationTime = 0.5f;
	m_fAngle = 0.f;
	m_fRotateSpeed = 0.5f;
	m_vBulletDirection = Vector3::XAxis;
	m_vTubeOffsetPos = Vector3::Zero;
	const WCHAR*l_strRotationSpeed = e_pTiXmlElement->Attribute(L"RotateSpeed");
	if( l_strRotationSpeed )
	{
		this->m_fRotateSpeed = GetFloat(l_strRotationSpeed);
	}
	//PlayerUI
	e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while( e_pTiXmlElement )
	{
		const WCHAR*l_strValue = e_pTiXmlElement->Value();
		COMPARE_VALUE("WeaponLevelData")
		{
			ProcessWeaponData(e_pTiXmlElement);
		}
		else
		COMPARE_VALUE("BulletFireStatus")
		{
			m_pBulletFireStatus = new cBulletFireStatus(e_pTiXmlElement,e_pPlayerFrame);
		}
		e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
	}
	AngleUpdate();
	m_bFire = false;
	m_vOffsetPos = Vector3::Zero;
	m_fTubeHeight = 0.f;
	m_IdleWeaponMPDIWorkingList.SetCurrentWorkingObject(0);
	m_ShotWeaponMPDIWorkingList.SetCurrentWorkingObject(0);
	CheckCurrentWeaponOffsetPos();
	
	m_bIsSetEnergyTube =false;
	m_iLevelIndex =0;
}

cFishPlayerWeapon::~cFishPlayerWeapon()
{
	SAFE_DELETE(m_pBulletFireStatus);
}

void	cFishPlayerWeapon::ProcessWeaponData(TiXmlElement* e_pElement)
{
	e_pElement = e_pElement->FirstChildElement();
	while( e_pElement )
	{
		cMPDIList*l_pMPDIList = 0;
		PARSE_ELEMENT_START(e_pElement)
			COMPARE_NAME("MPDIList")
			{
				l_pMPDIList = cGameApp::GetMPDIListByFileName(l_strValue);
				assert(l_pMPDIList);
			}
			else
			COMPARE_NAME("IdleMPDI")
			{
				if( l_pMPDIList  )
				{
					cMPDI*l_pCloneMPDI = l_pMPDIList->GetObject(l_strValue);
					if( l_pCloneMPDI )
					{
						cMPDI*l_pMPDI = dynamic_cast<cMPDI*>(l_pCloneMPDI->Clone());
						//l_pMPDI->SetExtraTransform(m_pmatPlayerTransform);
						l_pMPDI->SetParent(m_pPlayerFrame,false);
						m_IdleWeaponMPDIWorkingList.AddObjectNeglectExist(l_pMPDI);
					}
					else
					{
						UT::ErrorMsg(l_strValue,L"not find!");
					}
				}
			}
			else
			COMPARE_NAME("ShotMPDI")
			{
				if( l_pMPDIList  )
				{
					cMPDI*l_pCloneMPDI = l_pMPDIList->GetObject(l_strValue);
					if( l_pCloneMPDI )
					{
						cMPDI*l_pMPDI = dynamic_cast<cMPDI*>(l_pCloneMPDI->Clone());
						l_pMPDI->SetParent(&m_WeaponFrame,false);
						//l_pMPDI->SetExtraTransform(&m_matWeaponTransform);
						m_ShotWeaponMPDIWorkingList.AddObjectNeglectExist(l_pMPDI);
					}
					else
					{
						UT::ErrorMsg(l_strValue,L"not find!");
					}
				}
			}
		PARSE_NAME_VALUE_END
		e_pElement = e_pElement->NextSiblingElement();
	}
	assert(m_ShotWeaponMPDIWorkingList.Count() == m_IdleWeaponMPDIWorkingList.Count());

}

void	cFishPlayerWeapon::CheckCurrentWeaponOffsetPos()
{
	if(m_IdleWeaponMPDIWorkingList.Count())
	{
		Vector3	l_vPos = m_IdleWeaponMPDIWorkingList.GetCurrentWorkingObject()->GetObject(0)->GetOriginalPointList()[0];
		sTexBehaviorDataWithImageIndexData*l_p = m_IdleWeaponMPDIWorkingList.GetCurrentWorkingObject()->GetObject(0)->GetPointData(0);
		if(l_p->pPI)
		{
			cPuzzleImageUnit*l_pPIUnit = l_p->pPI->GetObject(l_p->iImageIndex);
			POINT	l_Pos = *l_pPIUnit->GetOffsetPos();
			l_Pos.x	+= *l_pPIUnit->GetWidth();
			l_Pos.y += *l_pPIUnit->GetHeight();
			l_Pos.x /= 2;
			l_Pos.y /= 2;
			m_fTubeHeight = (float)l_Pos.y;
			l_vPos.x += l_Pos.x;
			l_vPos.y += l_Pos.y;
			m_vOffsetPos = -l_vPos;
			m_vOffsetPos = cMatrix44::ZAxisRotationMatrix(D3DXToRadian(m_fOffsetAngle)).TransformVector(m_vOffsetPos);
		}
	}
}

void	cFishPlayerWeapon::AngleUpdate()
{
	m_fAngle = 180 - ( m_fCurrentAnimationTime*180 + m_fOffsetAngle );

	m_vBulletDirection = AngleToDirection( 360 - m_fAngle );
	m_vTubeOffsetPos = m_vBulletDirection*m_fTubeHeight;
	m_vTubeOffsetPos = this->m_pPlayerFrame->GetWorldTransform().GetTranslation()-m_vOffsetPos+m_vTubeOffsetPos;
}

bool	cFishPlayerWeapon::Fire(cPlayerBehaviorBase*e_pPlayerBehaviorBase)
{
	if( m_ShotWeaponMPDIWorkingList.GetCurrentWorkingObject() )
	{
		//if( !m_ShotWeaponMPDIWorkingList.GetCurrentWorkingObject()->IsDone() )
		//	return false;
	}
	cBullet* l_pBullet = cFishApp::m_pBulletManager->Require( e_pPlayerBehaviorBase->GetPlayerID(), m_iLevelIndex, this->m_pBulletFireStatus->m_iCurrentLevel );
	if( l_pBullet )
	{
		m_vBulletDirection = AngleToDirection( 360 - m_fAngle );
		m_vTubeOffsetPos = m_vBulletDirection*m_fTubeHeight;
		m_vTubeOffsetPos = this->m_pPlayerFrame->GetWorldTransform().GetTranslation()-m_vOffsetPos+m_vTubeOffsetPos;

		l_pBullet->Fire(e_pPlayerBehaviorBase,m_vTubeOffsetPos,m_vBulletDirection);
		this->m_bFire = true;
		m_ShotWeaponMPDIWorkingList.SetCurrentWorkingObject(m_iLevelIndex);
	}
	return m_bFire;
}

void	cFishPlayerWeapon::DirectionChangeUpdate(float e_fElpaseTime)
{
	if( m_eMovingDirection != eD_MAX )
	{
		const float	l_fMinTime = 0.001f;
		const float	l_fMaxTime = 0.999f;
		if( m_eMovingDirection == eD_LEFT )
		{
			if(m_fCurrentAnimationTime > l_fMinTime)
			{
				m_fCurrentAnimationTime -= m_fRotateSpeed*e_fElpaseTime;
			}
			else
			{
				m_fCurrentAnimationTime=l_fMinTime;
			}	
		}
		else
		if( m_eMovingDirection == eD_RIGHT )
		{
			if( m_fCurrentAnimationTime < l_fMaxTime )
			{
				m_fCurrentAnimationTime += m_fRotateSpeed*e_fElpaseTime;
			}
			else
			{
				m_fCurrentAnimationTime = l_fMaxTime;
			}
		}
		if( m_fCurrentAnimationTime >= l_fMaxTime )
			m_fCurrentAnimationTime = l_fMaxTime;
		if( m_fCurrentAnimationTime <= l_fMinTime )
			m_fCurrentAnimationTime = l_fMinTime;
		AngleUpdate();
		this->m_eMovingDirection = eD_MAX;
	}
}

void	cFishPlayerWeapon::LevelUp(bool e_bLevelChange)
{
	this->m_pBulletFireStatus->LevelChange(e_bLevelChange);
}

bool	cFishPlayerWeapon::IsFire()
{
	return m_bFire;
}

int		cFishPlayerWeapon::GetPower()
{
	return this->m_pBulletFireStatus->m_iCurrentLevel;
}

void	cFishPlayerWeapon::SetPower(int e_iCurrentLevel)
{
	this->m_pBulletFireStatus->m_iCurrentLevel = e_iCurrentLevel;
	this->m_pBulletFireStatus->LevelCheck();
}

void cFishPlayerWeapon::SetEnergyTube(bool e_bIsSetEnergyTube)
{
	m_bIsSetEnergyTube = e_bIsSetEnergyTube;
	if(e_bIsSetEnergyTube)
	{
		m_iLevelIndex = m_IdleWeaponMPDIWorkingList.Count()-1;
	}
	else
	{
		m_iLevelIndex = m_pBulletFireStatus->m_iLevelRangeIndex;
	}
	m_IdleWeaponMPDIWorkingList.SetCurrentWorkingObject(m_iLevelIndex);
	m_ShotWeaponMPDIWorkingList.SetCurrentWorkingObject(m_iLevelIndex);
}

void	cFishPlayerWeapon::Init()
{

}

void	cFishPlayerWeapon::Update(float e_fElpaseTime)
{
	DirectionChangeUpdate(e_fElpaseTime);

	if(!m_bIsSetEnergyTube)
		m_iLevelIndex = m_pBulletFireStatus->m_iLevelRangeIndex;

	if( this->m_pBulletFireStatus )
	{
		if( m_pBulletFireStatus->IsLevelChange() )
		{
			this->m_IdleWeaponMPDIWorkingList.SetCurrentWorkingObject(m_iLevelIndex);
			CheckCurrentWeaponOffsetPos();
		}
		m_pBulletFireStatus->Update(e_fElpaseTime);
	}


	if( m_bFire )
	{
		if( m_ShotWeaponMPDIWorkingList.GetCurrentWorkingObject() )
		{
			m_ShotWeaponMPDIWorkingList.GetCurrentWorkingObject()->Update(e_fElpaseTime);
			m_WeaponFrame.SetLocalTransform( cMatrix44::TranslationMatrix(this->m_pPlayerFrame->GetWorldTransform().GetTranslation()-this->m_vOffsetPos)*
											 cMatrix44::ZAxisRotationMatrix(D3DXToRadian(360-this->m_fAngle))
											 );
			//m_matWeaponTransform = cMatrix44::TranslationMatrix(this->m_pmatPlayerTransform->GetTranslation()-this->m_vOffsetPos)*cMatrix44::ZAxisRotationMatrix(D3DXToRadian(360-this->m_fAngle));
			if(m_ShotWeaponMPDIWorkingList.GetCurrentWorkingObject()->IsAnimationDone())
			{
				m_bFire = false;
				m_IdleWeaponMPDIWorkingList.GetCurrentWorkingObject()->UpdateByGlobalTime(m_fCurrentAnimationTime);
			}
		}
	}
	else
	{
		if( m_IdleWeaponMPDIWorkingList.GetCurrentWorkingObject() )
		{
			m_IdleWeaponMPDIWorkingList.GetCurrentWorkingObject()->UpdateByGlobalTime(m_fCurrentAnimationTime);
		}
	}
}

void	cFishPlayerWeapon::Render()
{
	if(this->m_bFire)
	{
		if(m_ShotWeaponMPDIWorkingList.GetCurrentWorkingObject())
			m_ShotWeaponMPDIWorkingList.GetCurrentWorkingObject()->Render();
	}
	else
	{
		if(m_IdleWeaponMPDIWorkingList.GetCurrentWorkingObject())
			m_IdleWeaponMPDIWorkingList.GetCurrentWorkingObject()->RenderByGlobalTime();
	}
	if( m_pBulletFireStatus )
		m_pBulletFireStatus->Render();
}