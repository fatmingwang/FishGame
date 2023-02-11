#include "stdafx.h"
#include "PlayerBehaviorBase.h"
#include "PlayerData.h"
#include "../GameApp/GameApp.h"
#include "../ControlPanel/ControlPanel.h"

cPlayerBehaviorBase::cPlayerBehaviorBase()
{
	m_iPayRateMultier = 1;
	m_pScoreLimit = 0;
	m_iMinScoreBet = 0;
	m_iMaxLevelNumber = 0;
	m_pWeapon = 0;
	m_i64Score = 0;
	m_iPlayerID = -1;
	m_vScorePos = Vector3::Zero;
	m_pScoreNumberImage = 0;
	m_bRefundCoin = false;;
	m_bCoinNotEnough = false;
	m_pMessageMoneyReachLimit = 0;
	m_pMessageInsertCoin = 0;
	m_pMessageCoinRefundNotEnough = 0;
	m_pMessageIOError = 0;
	m_pMessagePlayerLock = 0;
	m_NoMoney_InsertCoinHintTC.SetTargetTime(5.f);

}

cPlayerBehaviorBase::~cPlayerBehaviorBase()
{
	SAFE_DELETE(m_pScoreNumberImage);	
	SAFE_DELETE(m_pMessageMoneyReachLimit);
	SAFE_DELETE(m_pMessageInsertCoin);
	SAFE_DELETE(m_pMessageCoinRefundNotEnough);
	SAFE_DELETE(m_pMessageIOError);
	SAFE_DELETE(m_pMessagePlayerLock);

}

void	cPlayerBehaviorBase::Init()
{
	if( !m_pMessageInsertCoin )
	{
		m_pScoreLimit = &cFishApp::m_spControlPanel->m_i64MoneyLimit;
		m_iMinScoreBet = cFishApp::m_spControlPanel->m_iBulletPayRateMinPoint;
		m_iMaxLevelNumber = cFishApp::m_spControlPanel->m_iBulletPayRateLimit;		
		cMPDIList* l_pMessageMPDIList=cGameApp::GetMPDIListByFileName( MESSAGE_MPDILIST );		
		assert(l_pMessageMPDIList ->GetObject(L"MoneyLimit_D"));
		assert(l_pMessageMPDIList ->GetObject(L"MoneyNotEnough_D"));
		assert(l_pMessageMPDIList ->GetObject(L"MoneyLocked_D"));
		assert(l_pMessageMPDIList ->GetObject(L"PleaseInsertCoin_D"));
		assert(l_pMessageMPDIList ->GetObject(L"IOError"));
		m_pMessageMoneyReachLimit = new cMultiPathDynamicImage( l_pMessageMPDIList ->GetObject(L"MoneyLimit_D") );
		m_pMessageMoneyReachLimit->Init();
		m_pMessageMoneyReachLimit->SetParent(&m_PlayerFrame,false);
		m_pMessageIOError = new cMultiPathDynamicImage( l_pMessageMPDIList ->GetObject(L"IOError") );
		m_pMessageIOError->Init();
		m_pMessageIOError->SetParent(&m_PlayerFrame,false);
		m_pMessageCoinRefundNotEnough = new cMultiPathDynamicImage( l_pMessageMPDIList ->GetObject(L"MoneyNotEnough_D") );
		m_pMessageCoinRefundNotEnough->Init();
		m_pMessageCoinRefundNotEnough->SetParent(&m_PlayerFrame,false);
		m_pMessagePlayerLock = new cMultiPathDynamicImage( l_pMessageMPDIList ->GetObject(L"MoneyLocked_D") );
		m_pMessagePlayerLock->Init();
		m_pMessagePlayerLock->SetParent(&m_PlayerFrame,false);
		m_pMessageInsertCoin = new cMultiPathDynamicImage( l_pMessageMPDIList ->GetObject(L"PleaseInsertCoin_D") );
		m_pMessageInsertCoin->Init();
		m_pMessageInsertCoin->SetParent(&m_PlayerFrame,false);
		cPuzzleImage*l_pNumber = cGameApp::GetPuzzleImageByFileName((WCHAR*)PLAYER_NUMRIAL);
		m_pScoreNumberImage = l_pNumber->GetNumerialImageByName(L"PlayerScoreNumerial0",L"PlayerScoreNumerial9");
		m_PlayerMatrix = cMatrix44::TranslationMatrix(m_vPlayerPos)*cMatrix44::RotationMatrix(m_vPlayerRot);
	}
	InternalInit();
}

void	cPlayerBehaviorBase::Update(float e_fElpaseTime)
{
	InternalUpdate(e_fElpaseTime);
	if( m_i64Score < m_iMinScoreBet )
	{
		m_NoMoney_InsertCoinHintTC.Update(e_fElpaseTime);
		if( m_NoMoney_InsertCoinHintTC.bTragetTimrReached )
		{
			m_pMessageInsertCoin->Update(e_fElpaseTime);
		}
	}
	else
	{
		m_NoMoney_InsertCoinHintTC.Start();
	}

	if( m_i64Score >= *m_pScoreLimit && m_pMessageMoneyReachLimit )
	{
		m_pMessageMoneyReachLimit->Update(e_fElpaseTime);
	}

	if( g_pIOSMDriverInterface->IsIOError() && m_pMessageIOError )
	{
		m_pMessageIOError->Update(e_fElpaseTime);
	}

	if(m_bCoinNotEnough)
	{
		m_pMessageCoinRefundNotEnough->Update(e_fElpaseTime);
	}
	if(m_i64Score>=9999999)
	{
		m_pMessagePlayerLock->Update(e_fElpaseTime);
	}

	if( m_pWeapon )
		m_pWeapon->Update(e_fElpaseTime);
}

void	cPlayerBehaviorBase::Render()
{
	InternalRender();
	if( m_pScoreNumberImage )
		m_pScoreNumberImage->Draw(this->m_i64Score,(int)m_vScorePos.x,(int)m_vScorePos.y);

	if( m_pWeapon )
		m_pWeapon->Render();

	if( m_i64Score < m_iMinScoreBet && m_pMessageInsertCoin )
	{
		if( m_NoMoney_InsertCoinHintTC.bTragetTimrReached )
		{
			m_pMessageInsertCoin->Render();
		}
	}
	if( m_i64Score >= *m_pScoreLimit && m_pMessageMoneyReachLimit )
	{
		m_pMessageMoneyReachLimit->Render();
	}

	if( g_pIOSMDriverInterface->IsIOError() && m_pMessageIOError )
	{
		m_pMessageIOError->Render();
	}

	if(m_bCoinNotEnough)
	{
		m_pMessageCoinRefundNotEnough->Render();
	}

	if(m_i64Score>=9999999)
	{
		m_pMessagePlayerLock->Render();
	}

}

//int	cPlayerBehaviorBase::GetCurrentLevel()
//{
//	return m_pWeapon->GetPower();
//}
//================================================================
//
//
//
//================================================================
cPlayerManager::cPlayerManager()
{
	cClickMouseBehavior::m_sfRepeatTime = 0.1f;
	m_iPlayerCount = 0;
	//m_bSwitchPlayerIOInPut = false;
	m_AutoPlay.SetKeyData(106);
}


cPlayerManager::~cPlayerManager()
{
	this->Destroy();
}

void	cPlayerManager::Init()
{	
	Destroy();
	m_iPlayerCount = cFishApp::m_spControlPanel->m_iPlayerCount;
	if(this ->Count()==0)
	{
		bool l_b;
		cNodeISAX	l_NodeISAX;
		std::string	l_strFileName = UT::ComposeMsgByFormat("%s%d.xml",PLAYER_DATA,m_iPlayerCount);
		l_b = l_NodeISAX.ParseDataIntoXMLNode(l_strFileName.c_str());
		if(l_b)
		{
			TiXmlElement*l_pElement = l_NodeISAX.GetRootElement();
			l_pElement = l_pElement->FirstChildElement();
			const WCHAR*l_strName = l_pElement->Value();
			while(l_pElement)
			{
				l_strName = l_pElement->Value();
				COMPARE_NAME("Player")
				{
					cPlayerData*playerData = new  cPlayerData(l_pElement);
					PARSE_ELEMENT_START(l_pElement)
					COMPARE_NAME("ID")
					{
						playerData->m_iPlayerID = VALUE_TO_INT;
					}
					PARSE_NAME_VALUE_END
					this->AddObjectNeglectExist(playerData);
				}
				l_pElement = l_pElement->NextSiblingElement();
			}
		}
		else
		{
			UT::ErrorMsg(L"PlayerData.xml",L"not exit");
		}

	}
	
	
	for(int i=0;i<this->Count();++i)
	{
		this->GetObject(i)->Init();
	}
	
}

void	cPlayerManager::Update( float e_fElpaseTime )
{
	if( cGameApp::m_sbDebugFunctionWorking )
	{
		m_AutoPlay.SingnalProcess();
		m_AutoPlay.Update(e_fElpaseTime);
		if( m_AutoPlay.IsSatisfiedCondition() )
		{
			m_AutoPlay.Init();
			for(int i=0;i<this->Count();++i)
			{
				cPlayerData*l_pPlayerData = (cPlayerData*)this->GetObject(i);
				l_pPlayerData->m_bAutoPlay = !l_pPlayerData->m_bAutoPlay;
			}
		}
	}
	for(int i=0;i<this->Count();++i)
	{
		this->GetObject(i)->Update(e_fElpaseTime);
	}
}

void	cPlayerManager::Render()
{
	for(int i=0;i<this->Count();++i)
	{
		this->GetObject(i)->Render();
	}
}

void	cPlayerManager::Destroy()
{
	for(int i=0;i<this->Count();++i)
	{
		//this->GetObject(i)->Destroy();
	}
}
//================================================================
//
//
//
//================================================================
cPlayerWeapon::cPlayerWeapon()
{
	m_eMovingDirection = eD_MAX;
	m_bFire = false;
}

cPlayerWeapon::~cPlayerWeapon()
{

}

void	cPlayerWeapon::StatusChange(eDirection e_eDirection)
{
	this->m_eMovingDirection = e_eDirection;
}