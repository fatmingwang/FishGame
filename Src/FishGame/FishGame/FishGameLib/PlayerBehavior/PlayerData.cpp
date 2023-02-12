#include "stdafx.h"
#include "PlayerData.h"
#include <direct.h>
#include "../GameBullet/Bullet.h"
#include "../GameApp/GameApp.h"
#include "../GameBullet/BulletManager.h"
#include "../MiniGame/MiniGameBase.h"
#include "../Monster/MonsterManager.h"
#include "../ProbabilityFish/FishProbability.h"
#include "../ControlPanel/ControlPanel.h"
#include "CoinStackEffect.h"
#include "WinMoneyEffect.h"
#include "Weapon.h"
#include "../GameEffect/GameEffectBase.h"
#include "../FileNameDefine.h"

cPlayerData::cPlayerData( TiXmlElement*e_pElement )
{
	m_bAutoPlay = false;
	m_fIsLevelKeyDownTC =0.0f;
	m_fLevelClickRepeatTime =0.f;
	m_fChangeLevelClickRepeatTime = 0.f;
	m_pPlayerUIMPDI = 0;
	m_bIsEnergyPower =false;
	
	m_fAutoFireTime = 0.0f;
	m_fWeaponIdleTimeMax =10.f;

	ProcessPlayerCoin();


	//CoinStack
	//m_pCoinStackEffect = 0;
	//CoinStack

	m_iCoinRangeIndex = 0;
	m_PlayerBodyMPDIWorkingList.SetCurrentWorkingObject(0);

	ProcessPlayer(e_pElement);
	ProcessPlayerCoin();
}








cPlayerData::~cPlayerData()
{
	SAFE_DELETE(m_pWeapon);
	SAFE_DELETE(m_pPlayerUIMPDI);

	//CoinStack
	//SAFE_DELETE(m_pCoinStackEffect);
	//CoinStack
}








void cPlayerData::AddScore( cMonster* e_pFish, int Money )
{	
	m_i64Score += Money;

	if ( !e_pFish)
		return;

	//CoinStack
	//if ( m_pCoinStackEffect )
	//{
	//	CoinStackPushData TempCoinStackData;

	//	TempCoinStackData.Money      = Money;
	//	TempCoinStackData.e_pFishID     = e_pFish->GetID();
	//	TempCoinStackData.PosRot	 = GetPlayerRotation().z;
	//	TempCoinStackData.ShowMatrix = cMatrix44::TranslationMatrix( GetPlayerPosition() ) * cMatrix44::RotationMatrix( GetPlayerRotation() );

	//	m_pCoinStackEffect->PushCoinStackData( TempCoinStackData );
	//}
	//CoinStack

	m_iCoinRangeIndex = 0;
	for( size_t i=0;i<m_CoinRangeList.size();++i )
	{
		if(Money>=m_CoinRangeList[i].iStart&&Money<=m_CoinRangeList[i].iEnd)
		{
			m_iCoinRangeIndex = (int)i;
			break;
		}
	}

	const wchar_t* l_strCoinEffectName = nullptr;

	eWinMoneyEffectCoinCount l_eWinMoneyEffectCoinCount;

	if( m_iCoinRangeIndex == 0 )
	{
		l_strCoinEffectName = L"SilverCoin";
		l_eWinMoneyEffectCoinCount = eWinMoneyEffectCoinCount_Triple;
	}
	else if(m_iCoinRangeIndex == 1)
	{
		l_strCoinEffectName = L"SilverCoin";
		l_eWinMoneyEffectCoinCount = eWinMoneyEffectCoinCount_Penta;
	}
	else if(m_iCoinRangeIndex == 2)
	{
		l_strCoinEffectName = L"GoldCoin";
		l_eWinMoneyEffectCoinCount = eWinMoneyEffectCoinCount_Triple;
	}
	else if(m_iCoinRangeIndex == 3)
	{
		l_strCoinEffectName = L"GoldCoin";
		l_eWinMoneyEffectCoinCount = eWinMoneyEffectCoinCount_Penta;
	}
	auto l_Mat = cMatrix44::TranslationMatrix(e_pFish->GetCenterPos())* cMatrix44::RotationMatrix(m_vPlayerRot);

	cFishApp::m_spWinMoneyEffectManager->RequireWinMoneyEffectBase(l_strCoinEffectName, l_Mat,Money, this, l_eWinMoneyEffectCoinCount );

	sGameEffectInputData	l_GameEffectInputData;
	l_GameEffectInputData.iPlayerID = this->m_iPlayerID;
	l_GameEffectInputData.strMonstrName = e_pFish->GetName();
	l_GameEffectInputData.ShowMatrix = e_pFish->GetTransform();
	l_GameEffectInputData.iValue = Money;

	cFishApp::m_spGameEffectManager->RequirePrizeEffect(L"cGameEffectCardMarqueeManager",(void*)&l_GameEffectInputData,false);
}








void cPlayerData::ProcessPlayerCoin()
{
	int	l_iAddPoint = *cFishApp::m_spControlPanel->m_pEventManager->m_pEventVariableManager->GetObject(L"BulletPayRateAddPoint")->GetInt();
	int	l_iMaxBulletPayrate = cFishApp::m_spControlPanel->m_iBulletPayRateLimit;
	l_iMaxBulletPayrate /= 3;
	int	l_iModulusValue = l_iMaxBulletPayrate % l_iAddPoint;
	if( l_iModulusValue )
	{
		l_iMaxBulletPayrate -= l_iModulusValue;
	}
	int	l_iStartIndex = 0;
	int	l_iMin = 1;
	int	l_iMax = l_iMaxBulletPayrate;
	for( int i=0;i<3;++i )
	{
		sCoinRange	l_CoinRange;
		l_CoinRange.iEnd = l_iMax;
		l_CoinRange.iStart = l_iMin;
		m_CoinRangeList.push_back(l_CoinRange);
		l_iMin = l_iMax+1;
		l_iMax += l_iMaxBulletPayrate;
	}
	sCoinRange	l_CoinRange;
	l_CoinRange.iEnd = 100001;
	l_CoinRange.iStart = 900000;
	m_CoinRangeList.push_back(l_CoinRange);
}








void cPlayerData::ProcessPlayer( TiXmlElement*e_pElement )
{

	TiXmlElement*l_pTiXmlElement = e_pElement->FirstChildElement();
	const WCHAR*l_strName = l_pTiXmlElement->Value();
	while(l_pTiXmlElement)
	{
		l_strName = l_pTiXmlElement->Value();
		COMPARE_NAME("PlayerData")
		{
			this->ProcessPlayerData(l_pTiXmlElement);
		}
		else
		COMPARE_NAME("IO")
		{
			this->ProcessPlayerIO(l_pTiXmlElement);
		}
		else
		COMPARE_NAME("Weapon")
		{
			this->ProcessWeapon(l_pTiXmlElement);
		}
		else
		COMPARE_NAME("PlayerBody")
		{
			this->ProcessPlayerBody(l_pTiXmlElement);
		}
		l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
	}
}
void cPlayerData::ProcessWeapon( TiXmlElement*e_pElement )
{
	this->m_pWeapon = new cFishPlayerWeapon(e_pElement,&this->m_PlayerFrame);
}

void cPlayerData::ProcessPlayerBody( TiXmlElement*e_pElement )
{
	e_pElement = e_pElement->FirstChildElement();
	Vector3	l_vPlayerBodyShowPos(Vector3::Zero);
	while(e_pElement)
	{
		cMPDIList*l_pMPDIList = 0;
		PARSE_ELEMENT_START(e_pElement)
			COMPARE_NAME("MPDIList")
			{
				l_pMPDIList = cGameApp::GetMPDIListByFileName(l_strValue);
			}
			else
			COMPARE_NAME("MPDI")
			{
				if( l_pMPDIList  )
				{
					cMPDI*l_pCloneMPDI = l_pMPDIList->GetObject(l_strValue);
					if( l_pCloneMPDI )
					{
						cMPDI*l_pMPDI = dynamic_cast<cMPDI*>(l_pCloneMPDI->Clone());
						l_pMPDI->SetParent(&this->m_PlayerFrame,false);
						m_PlayerBodyMPDIWorkingList.AddObjectNeglectExist(l_pMPDI);
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
	m_PlayerBodyMPDIWorkingList.SetCurrentWorkingObject(0);

}







void cPlayerData::ProcessPlayerData( TiXmlElement*e_pElement )
{
	//Vector3	l_vPos;
	//Vector3	l_vRotation;
	PARSE_ELEMENT_START(e_pElement)
		COMPARE_NAME("Position")
		{
			m_vPlayerPos = VALUE_TO_VECTOR3;
		}
		else
		COMPARE_NAME("Rotation")
		{
			m_vPlayerRot = VALUE_TO_VECTOR3;
		}
	PARSE_NAME_VALUE_END
	m_PlayerFrame.SetLocalTransform( cMatrix44::TranslationMatrix(m_vPlayerPos)*cMatrix44::RotationMatrix(m_vPlayerRot));
}	








void cPlayerData::ProcessPlayerIO( TiXmlElement*e_pElement )
{
	PARSE_ELEMENT_START(e_pElement)
		COMPARE_NAME("Right")
		{
			this->m_cInput_Right= (unsigned char)(l_strValue[0]);
		}
		else
		COMPARE_NAME("Left")
		{
			this->m_cInput_Left= (unsigned char)(l_strValue[0]);
		}
		else
		COMPARE_NAME("Up")
		{
			this->m_cInput_Up=	(int)VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("Down")
		{
			this->m_cInput_Down= cGameApp::m_sbDebugFunctionWorking ? 40 : (int)VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("Fire")
		{
			m_FireClick.SetKeyData((unsigned char)(l_strValue[0]));
			m_FireClick.SetPressedRepeatMouseUp(true);
			m_FireClick.SetRepeatTime(0.4f);
		}
		else
		COMPARE_NAME("Level")
		{
			this->m_cInput_Level = (unsigned char)(l_strValue[0]);
			m_LevelClick.SetKeyData(m_cInput_Level);
			m_LevelClick.SetPressedRepeatMouseUp(true);
		}
		else
		COMPARE_NAME("FireRepeat")
		{
			m_FireClick.SetRepeatTime(VALUE_TO_FLOAT);
			m_fAutoFireTime = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("LevelUpRepeat")
		{
			m_fLevelClickRepeatTime = VALUE_TO_FLOAT;
			m_fChangeLevelClickRepeatTime =m_fLevelClickRepeatTime;
			m_LevelClick.SetRepeatTime(m_fChangeLevelClickRepeatTime);
		}
	PARSE_NAME_VALUE_END
}

void cPlayerData::InternalInit()
{	
	m_FireClick.Init();
	m_LevelClick.Init();

	cMPDIList*l_pPlayerMPDIList = cGameApp::GetMPDIListByFileName(PLAYERUI_MPDI);
	assert(l_pPlayerMPDIList);
	cMultiPathDynamicImage *l_pAllPlayerPos = l_pPlayerMPDIList->GetObject(L"PlayerUIPosInfo");
	
	cMPDI*l_pPlayerUI = l_pPlayerMPDIList->GetObject( L"PlayerUI" );
	assert(l_pPlayerUI);

	m_pPlayerUIMPDI = new cMultiPathDynamicImage( l_pPlayerUI );
	m_pPlayerUIMPDI->Init();

	std::wstring	l_str = UT::ComposeMsgByFormat(L"PlayerUI%d",(this->m_iPlayerID+1));
	int l_iIndexPlayerUI = m_pPlayerUIMPDI->GetPuzzleImage(L"PlayerUI")->GetObjectIndexByName(l_str.c_str());
	m_pPlayerUIMPDI->GetObject( 0 )->ChangeAllImageByImageIndex(l_iIndexPlayerUI);;
	m_pPlayerUIMPDI->SetParent( &m_PlayerFrame,false );
	m_PlayerBodyMPDIWorkingList.SetCurrentWorkingObject(m_iPayRateMultier-1);

	m_vScorePos = l_pAllPlayerPos->GetObject(L"PlayerScoreNumerial1")->GetOriginalPointList()[0];
	m_vScorePos = this->m_PlayerFrame.GetWorldTransform().TransformCoordinate(m_vScorePos);
	
	Vector3	l_vScale,l_vRotation,l_vTranslation;
	float	l_fInvert;
	m_PlayerFrame.GetWorldTransform().Decompose(l_vScale,l_vRotation,l_vTranslation,l_fInvert);
	m_pScoreNumberImage->SetAngle(D3DXToDegree(l_vRotation.z));

	m_AutoFireTC.SetTargetTime(m_fAutoFireTime);
	m_AutoFireTC.Start();

	//CoinStack Added By Terry Begin
	//if ( !m_pCoinStackEffect )
	//{
	//	m_pCoinStackEffect = new CoinStackEffect();
	//	m_pCoinStackEffect->Init();
	//}
	//CoinStack Added By Terry End
}








void cPlayerData::InternalUpdate( float e_fElpaseTime )
{
	//if ( m_pCoinStackEffect )
	//	m_pCoinStackEffect->Update( e_fElpaseTime );
	
	//if(!g_pIOSMDriverInterface->IsIOError())
	if (1)
	{
		if(!cFishApp::m_spProbabilityFish->IsMasterLeeIOError() && m_i64Score <=9999999 && !m_bCoinNotEnough && !cFishApp::m_spControlPanel->m_bShowReportTipImage)
		{
			KeyUpdate(e_fElpaseTime);
		}
	}
	else
	{
		m_pMessageIOError->Update(e_fElpaseTime);	
	}

	m_pPlayerUIMPDI->Update(e_fElpaseTime);
	
	if( m_PlayerBodyMPDIWorkingList.GetCurrentWorkingObject() )
	{
		m_PlayerBodyMPDIWorkingList.GetCurrentWorkingObject()->Update(e_fElpaseTime);
	}


	if(m_bAutoPlay)
	{
		AutoPlayUpdate(e_fElpaseTime);
	}
}








void cPlayerData::InternalRender()
{
	//if ( m_pCoinStackEffect )
	//	m_pCoinStackEffect->Render();

	if(m_PlayerBodyMPDIWorkingList.GetCurrentWorkingObject())
			m_PlayerBodyMPDIWorkingList.GetCurrentWorkingObject()->Render();

	m_pPlayerUIMPDI->Render();
}

void cPlayerData::Destroy()
{

}

void	cPlayerData::KeyUpdate(float e_fElpaseTime)
{
	eDirection	l_eDirection = eD_MAX;

	if( cGameApp::m_sucKeyData[this->m_cInput_Left])
	{
		l_eDirection = eD_LEFT;
	}
	else if( cGameApp::m_sucKeyData[this->m_cInput_Right])
	{
		l_eDirection = eD_RIGHT;
	}
	else if ( cGameApp::m_sucKeyData[ m_cInput_Up ] )
	{
		l_eDirection = eD_UP;
	}
	else if ( cGameApp::m_sucKeyData[ m_cInput_Down ] )
	{
		l_eDirection = eD_DOWN;
	}

	m_FireClick.SingnalProcess();
	m_FireClick.Update(e_fElpaseTime);
	//if( m_FireClick.IsSatisfiedCondition() && !g_pIOSMDriverInterface->IsIOError() )
	if (m_FireClick.IsSatisfiedCondition())
	{
		m_FireClick.Init();
		if( m_i64Score >= m_iMinScoreBet )
		{
			int	l_iPayrate = this->m_pWeapon->GetPower();
			if(m_i64Score < l_iPayrate )
			{
				l_iPayrate = (int)m_i64Score;
			}

			if(cFishApp::m_spMonsterManager->GetCountMonsterVisible()>0 && m_pWeapon && !m_pWeapon->IsFire() )
			{
				m_pWeapon->Fire(this);
				this->m_i64Score -= l_iPayrate;
				cFishApp::m_spProbabilityFish->RevenueAdd( l_iPayrate, GetPlayerID());
				cGameApp::SoundPlay(L"3",true);
			}
		}
	}
	//Level Key
	if ( cGameApp::m_sucKeyData[ m_cInput_Level ] )
	{
		m_fIsLevelKeyDownTC+=e_fElpaseTime;
	}
	else
	{
		m_fIsLevelKeyDownTC =0.f;

	}
	m_LevelClick.SingnalProcess();
	m_LevelClick.Update(e_fElpaseTime);
	if(m_LevelClick.IsSatisfiedCondition())
	{
		float	l_fReduceTimne = 0.03f;
		if(m_fIsLevelKeyDownTC>=1.0f)
		{
			m_fChangeLevelClickRepeatTime -= l_fReduceTimne;
		}
		else
		{
			m_fChangeLevelClickRepeatTime =m_fLevelClickRepeatTime;
		}
		if(m_fChangeLevelClickRepeatTime <= 0.1f)
		{
			m_fChangeLevelClickRepeatTime = 0.1f;
		}


		m_LevelClick.SetRepeatTime(m_fChangeLevelClickRepeatTime);
		m_LevelClick.Init();;

		if( m_pWeapon )
			m_pWeapon->LevelUp(true);
	}
	//Level Key End

	if( m_pWeapon )
		m_pWeapon->StatusChange(l_eDirection);
}

void cPlayerData::AutoPlayUpdate(float e_fElpaseTime)
{
	m_AutoFireTC.Update(e_fElpaseTime);
	if(m_AutoFireTC.bTragetTimrReached)
	{
		int	l_iPayrate =0;
		if(m_i64Score>this->m_pWeapon->GetPower())
		{
			l_iPayrate = this->m_pWeapon->GetPower();
		}
		else
		{
			l_iPayrate = (int)m_i64Score;
		}
		if(cFishApp::m_spMonsterManager->GetCountMonsterVisible()>0 && m_pWeapon && !m_pWeapon->IsFire() && m_i64Score >= m_iMinScoreBet)
		{
			((cFishPlayerWeapon*)m_pWeapon)->SetWeaponAnimationTime( rand(0.0f,1.0f) );
			((cFishPlayerWeapon*)m_pWeapon)->AngleUpdate();

			m_pWeapon->Fire(this);
			this->m_i64Score -= l_iPayrate;
			cFishApp::m_spProbabilityFish->RevenueAdd( l_iPayrate, GetPlayerID());
			cGameApp::SoundPlay(L"3",true);
		}
		m_AutoFireTC.SetTargetTime(m_fAutoFireTime);
		m_AutoFireTC.Start();	
	}
}

void cPlayerData::SetEnergyPower(bool e_bIsEnergyPower,int e_iMultiplier)
{
	m_bIsEnergyPower = e_bIsEnergyPower;
	m_iPayRateMultier = e_iMultiplier;
	//m_PlayerBodyMPDIWorkingList.SetCurrentWorkingObject(m_iPayRateMultier-1);
	m_PlayerBodyMPDIWorkingList.SetCurrentWorkingObject(1);
	if( m_pWeapon )
		m_pWeapon->SetEnergyTube(e_bIsEnergyPower);
}



cMatrix44 cPlayerData::GetBulletPos()
{
	Vector3 l_vTubeOffsetPos,l_vTubeRotation;
	l_vTubeOffsetPos =( (cFishPlayerWeapon*)m_pWeapon )->GetTubeOffsetPos();
	l_vTubeRotation = Vector3( 0,0,-(  ( (cFishPlayerWeapon*)m_pWeapon )->GetTubeAngle()+270 ) );
	return cMatrix44::TranslationMatrix( l_vTubeOffsetPos ) * cMatrix44::RotationMatrix( l_vTubeRotation );
}

void	cPlayerData::WeaponLVImageCheck()
{
	//for(int i=0; i < m_iLevelCount ; ++i)
	//{
	//	if(this->m_iLevelNumber>=this->m_vLevelMin[i] && this->m_iLevelNumber<=this->m_vLevelMax[i])
	//	{
	//		this->m_iLevel = this->m_vLevelIndex[i];
	//	}
	//}
	//std::wstring	l_str = UT::ComposeMsgByFormat( L"TubePos%d", (this->m_iLevel+1) );
	//int l_iIndexPlayerUI = m_pPlayerImage->GetPuzzleImage( L"PlayerUI" )->GetObjectIndexByName( l_str.c_str() );
	//m_pPlayerImage->GetObject(L"TubePos")->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
	//m_pPlayerImage->GetObject(L"TubePos")->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;
	//m_pPlayerImage->UpdateByGlobalTime(m_fTubeTime+0.1f);
}