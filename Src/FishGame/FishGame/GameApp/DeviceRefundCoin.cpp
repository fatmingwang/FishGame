#include "stdafx.h"
#include "DeviceRefundCoin.h"
//
#include "GameApp.h"
#include "../PlayerBehavior/PlayerData.h"
#include "../../IOSimulationMachine/8051/GH8051.h"
//
#include "../ProbabilityFish/FishProbability.h"

bool	LONG_PRESSED_BUTTON_TIME_FOR_REFUND_CHECK	=   false;
float	LONG_PRESSED_BUTTON_TIME_FOR_REFUND			=	3.f;
#ifdef DEBUG
float	LONG_PRESSED_BUTTON_TIME_FOR_EXCHANGE_IN	=	0.2f;
#else
float	LONG_PRESSED_BUTTON_TIME_FOR_EXCHANGE_IN	=	2.f;
#endif
float	LONG_PRESSED_BUTTON_TIME_FOR_EXCHANGE_OUT	=	2.f;

int		SAFE_REFUND_COIN_COUNT		=			50;
#define	SAFE_SCORE_RANGE_TO_PLAY_GAME			10


UT::sTimeAndFPS	g_RefundCoinGap;
cRefoundCoinDevice::cRefoundCoinDevice(
									   cPlayerBehaviorBase*e_pPlayer,//player ID
									   cIOSMIOPackData*e_pInsertCoinCount,//insert coin count
									   cIOSMIOPackData*e_pHopperValue,//refund coin device
									   cIOSMIOPackData*e_pResetCoinToRefund,//
									   cIOSMIOPackData*e_pTicketValue,//
									   cIOSMIOPackData*e_pRestTicketToRefund,//
									   cIOSMIOPackData*e_pExchangeInSequence,//new stand for card reader machine
									   cIOSMIOData*	e_pResetWantRefundCountCoinDevice,
									   cIOSMIOData*	e_pResetWantRefundCountTicketDevice,
									   int*e_piCoinToScore,//
									   int*e_piRefundCoinOrTicketValue,//
									   unsigned char e_ucReturnKey,//
									   unsigned char e_ucExchangeInKey,
									   unsigned char e_ucExchangeOutKey
									   )
{
	m_iExchangeDebugValueTotal = 0;
	m_iExchangeDebugValue = 0;
	m_bRefundCoinStart = false;
	m_i64RestCoinToRefund = 0;
	m_bRefundCoin = false;
	m_piExchangeInScore = cFishApp::m_spControlPanel->m_pEventManager->m_pEventVariableManager->GetObject(L"Exchange_In")->GetInt();
	m_pExchangeInSequence = e_pExchangeInSequence;
	m_pPlayer = e_pPlayer;
	m_pInsertCoinCount = e_pInsertCoinCount;
	bool	l_bCoinDevice = true;
	if( cFishApp::m_spControlPanel )
	{
		cEventVariable*l_pRefundCoinOrTicket = cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"RefundCoinOrTicket");
		l_bCoinDevice = l_pRefundCoinOrTicket->IsSame(L"Ticket")?false:true;
	}
	if( l_bCoinDevice )
	{
		m_pWantRefundCountDevice = e_pHopperValue;
		m_pCurrentRefundCountDevice = e_pResetCoinToRefund;
		m_pResetWantRefundCountDevice = e_pResetWantRefundCountCoinDevice;
		m_piRefundCoinOrTicketValue = e_piCoinToScore;
	}
	else
	{
		m_pWantRefundCountDevice = e_pTicketValue;
		m_pCurrentRefundCountDevice = e_pRestTicketToRefund;	
		m_pResetWantRefundCountDevice = e_pResetWantRefundCountTicketDevice;
		m_piRefundCoinOrTicketValue = e_piRefundCoinOrTicketValue;
	}
	assert(m_pWantRefundCountDevice);
	assert(m_pCurrentRefundCountDevice);
	assert(m_pResetWantRefundCountDevice);
	m_piCoinToScore = e_piCoinToScore;
	m_RefundKey.SetKeyData(e_ucReturnKey);
	m_ExchangeInKey.SetKeyData(e_ucExchangeInKey);
	m_ExchangeOutKey.SetKeyData(e_ucExchangeOutKey);
	m_bWorking = false;
	m_pBinaryFile = 0;
	m_RefundToCheckCoinTC.SetTargetTime(6.f);
	g_RefundCoinGap.Update();
	m_RefundCoinKeyPressTimeTC.SetTargetTime(LONG_PRESSED_BUTTON_TIME_FOR_REFUND);
	m_ExchangeInKeyPressTimeTC.SetTargetTime(LONG_PRESSED_BUTTON_TIME_FOR_EXCHANGE_IN);
	//m_Wait8051SingnalToActive.SetTargetTime(REFRESH_8051_IO_TIME_OFFSET);
}








cRefoundCoinDevice::~cRefoundCoinDevice() 
{
	 
}

void	cRefoundCoinDevice::DoExchangeIn(int e_iMultiplier)
{
	int	l_iSocre = e_iMultiplier*(*m_piExchangeInScore);

	cPlayerData*l_pPlayerData = (cPlayerData*)m_pPlayer;
	if( m_pPlayer->m_i64Score < cFishApp::m_spControlPanel->m_i64MoneyLimit && m_pPlayer->m_i64Score+l_iSocre >= cFishApp::m_spControlPanel->m_i64MoneyLimit )
	{
		cGameApp::SoundPlay(L"BaouGi",true);
	}

	m_pPlayer->m_i64Score += l_iSocre;
	m_ExchangeInKey.Init();
	cFishApp::m_spProbabilityFish->ExchangeIn( l_iSocre );
	cGameApp::SoundPlay( L"Button_Bet", true );
}

void	cRefoundCoinDevice::Update_ExchangeBehaviod( float e_fElpaseTime )
{
	m_ExchangeInKey.SingnalProcess();
	m_ExchangeInKey.Update( e_fElpaseTime );
	if( !cFishApp::m_spControlPanel->m_bShowReportTipImage )
	{
		//if ( m_ExchangeInKey.IsSatisfiedCondition() )
		//{	
		//	const int	l_iLongPresseExchangeIn;
		//	m_pPlayer->m_i64Score += *m_piExchangeInScore;
		//	m_ExchangeInKey.Init();
		//	//
		//	cFishApp::m_spProbabilityFish->ExchangeIn( *m_piExchangeInScore );
		//	//
		//	cGameApp::SoundPlay( L"Button_Bet", true );
		//}
		if( !m_ExchangeInKeyPressTimeTC.bTragetTimrReached )
		{
			if( cGameApp::m_sucKeyData[m_ExchangeInKey.GetKeyData()] )
			{
				m_ExchangeInKeyPressTimeTC.Update(e_fElpaseTime);
				if( m_ExchangeInKeyPressTimeTC.bTragetTimrReached )
				{
					const int	l_iLongPresseExchangeIn = 10;
					DoExchangeIn(l_iLongPresseExchangeIn);
				}
			}
		}
		else
		{
			if( !cGameApp::m_sucKeyData[m_ExchangeInKey.GetKeyData()] )
			{
				m_ExchangeInKeyPressTimeTC.Start();
			}
		}
		m_iExchangeDebugValue = this->m_pExchangeInSequence->GetPackDataByByte();
		m_iExchangeDebugValueTotal	+= m_iExchangeDebugValue;
		if( cGameApp::m_sucKeyData[m_ExchangeInKey.GetKeyData()] == false )
		{
			if( m_pExchangeInSequence && this->m_pExchangeInSequence->GetPackDataByByte() > 0 )
			{
				int	l_iExChangeInSequenct = this->m_pExchangeInSequence->GetPackDataByByte();
				DoExchangeIn(l_iExChangeInSequenct);
				void*l_pData = m_pExchangeInSequence->GetData();
				*(unsigned char*)l_pData = 0;
			}
		}
	}

	m_ExchangeOutKey.SingnalProcess();
	m_ExchangeOutKey.Update(e_fElpaseTime);
	float	l_fMouseDownElpaseTime = m_ExchangeOutKey.GetMouseMoveData().GetDownElpaseTime();
	if(m_ExchangeOutKey.IsSatisfiedCondition() || l_fMouseDownElpaseTime > LONG_PRESSED_BUTTON_TIME_FOR_EXCHANGE_OUT )
	{
		int64	l_i64ExchangeOutMoney = *m_piExchangeInScore;
		if( l_fMouseDownElpaseTime > LONG_PRESSED_BUTTON_TIME_FOR_EXCHANGE_OUT )
		{
			l_i64ExchangeOutMoney = m_pPlayer->m_i64Score; 
			m_pPlayer->m_i64Score = 0;
		}
		else
		{
			if( *m_piExchangeInScore > m_pPlayer->m_i64Score )
			{
				l_i64ExchangeOutMoney = m_pPlayer->m_i64Score;
				m_pPlayer->m_i64Score = 0;
			}
			else
				m_pPlayer->m_i64Score -= *m_piExchangeInScore;
		}
		cFishApp::m_spProbabilityFish->ExchangeOut( l_i64ExchangeOutMoney );
		//m_pPlayer->m_i64Score = 0;
		m_ExchangeOutKey.Init();
		this->m_pPlayer->m_bCoinNotEnough = false;
	}
}

void	cRefoundCoinDevice::CoinRefund()
{
	m_pPlayer->m_bCoinNotEnough = false;

	m_RefundKey.Init();

	//cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(L"Refund key start");
	if(cGameApp::m_sbDeviceExist)
	{
		if(this->m_pPlayer->m_i64Score >= *m_piRefundCoinOrTicketValue)//money is enough to refund.
		{
			m_RefundToCheckCoinTC.Start();
			m_bRefundCoin = true;
			m_i64RestCoinToRefund = this->m_pPlayer->m_i64Score/(*m_piRefundCoinOrTicketValue);

			if(m_i64RestCoinToRefund>SAFE_REFUND_COIN_COUNT)
			{
				m_i64RestCoinToRefund = SAFE_REFUND_COIN_COUNT;
			}
			this->m_pWantRefundCountDevice->SetPackDataByByte((unsigned char) m_i64RestCoinToRefund);
			//OutputDebugString(L"refund start\n");
			m_bRefundCoinStart = false;
			//void*	l_pData = this->m_pCurrentRefundCountDevice->GetData();
			//*(unsigned char*)l_pData = (unsigned char)m_i64RestCoinToRefund;
		}
		else//not enough money.
		{
			m_bRefundCoin = false;
		}
	}
	else
	{
		m_bRefundCoin = false;
		//
		cFishApp::m_spProbabilityFish->RefundCoin( m_pPlayer->m_i64Score );
		//
		m_pPlayer->m_i64Score = 0;
	}
}








void	cRefoundCoinDevice::Update_CoinBehavior( float e_fElpaseTime )
{
	//toast coin
	if(cGameApp::m_sbDeviceExist)
	{
		int64 l_i64ToastCoin = *m_piCoinToScore * this->m_pInsertCoinCount->GetPackDataByByte();
		void*l_pData = this->m_pInsertCoinCount->GetData();
		*(unsigned char*)l_pData = 0;
		m_pPlayer->m_i64Score += l_i64ToastCoin;
		//
		cFishApp::m_spProbabilityFish->ToastCoin( l_i64ToastCoin );
	}
	else
	{//detect toast coin key
		if( m_pInsertCoinCount->GetKeyData()  )
		{
			//m_cToastCoin = *m_piCoinToScore;
		}
	}
	m_pPlayer->m_bRefundCoin = m_bRefundCoin;
	//refunding
	if( m_bRefundCoin )
	{
		cPlayerData*l_pPlayerData = (cPlayerData*)m_pPlayer;
		int	l_iRestCoinToRefund = (int)(l_pPlayerData->m_i64Score/(*m_piRefundCoinOrTicketValue));
		if( l_iRestCoinToRefund > SAFE_REFUND_COIN_COUNT + SAFE_SCORE_RANGE_TO_PLAY_GAME )
		{
			m_pPlayer->m_bRefundCoin = false;
		}
	}
	if( m_bRefundCoin )
	{
		//OutputDebugString(L"\n");
		int64	l_i64RestCoin = this->m_pCurrentRefundCountDevice->GetPackDataByByte();
//		OutputDebugString(ValueToStringW(l_i64RestCoin));
//		OutputDebugString(L"rest coin\n");
		if( !m_bRefundCoinStart )
		{
			//if( g_iRefundSingnalStep[this->m_pPlayer->m_iPlayerID] >= 2 )
			{
				m_bRefundCoinStart = true;
//				OutputDebugString(L"refund go\n");
			}
		}

		if( l_i64RestCoin != m_i64RestCoinToRefund && m_bRefundCoinStart )
		{
			int64 l_i64RefundCoin = *m_piRefundCoinOrTicketValue * (m_i64RestCoinToRefund-l_i64RestCoin);
			m_i64RestCoinToRefund = l_i64RestCoin;
			//
			cFishApp::m_spProbabilityFish->RefundCoin( l_i64RefundCoin );
			//
			m_pPlayer->m_i64Score -= l_i64RefundCoin;//
			m_RefundToCheckCoinTC.Start();
			//more coin to refund?
			if( m_i64RestCoinToRefund <= 0 )
			{
				CoinRefund();
			}
		}

		//motor spin too long,no coin
		if( m_RefundToCheckCoinTC.bTragetTimrReached )
		{
			m_bRefundCoin = false;
			this->m_pPlayer->m_bCoinNotEnough = true;
			m_pResetWantRefundCountDevice->WriteIO(true);
		}
	}
	else
	{//check refund key pressed
		if( !LONG_PRESSED_BUTTON_TIME_FOR_REFUND_CHECK )
		{
			m_RefundKey.SingnalProcess();
			m_RefundKey.Update(e_fElpaseTime);
			if( m_RefundKey.IsSatisfiedCondition() )
			{
				g_RefundCoinGap.Update();
				if( g_RefundCoinGap.fElpaseTime >= 0.3f )
				{
					m_bRefundCoinStart = false;
					//m_Wait8051SingnalToActive.
					CoinRefund();
					m_RefundKey.Init();
				}
				else
				{
					m_RefundKey.Init();
				}
			}
		}
		else
		{
			//long pressed
			if( !m_RefundCoinKeyPressTimeTC.bTragetTimrReached )
			{
				if(cGameApp::m_sucKeyData[m_RefundKey.GetKeyData()])
				{
					m_RefundCoinKeyPressTimeTC.Update(e_fElpaseTime);
					if( m_RefundCoinKeyPressTimeTC.bTragetTimrReached )
					{
						g_RefundCoinGap.Update();
						if( g_RefundCoinGap.fElpaseTime >= 0.3f )
						{
							m_bRefundCoinStart = false;
							//m_Wait8051SingnalToActive.
							CoinRefund();
						}
						else
						{
							m_RefundKey.Init();
						}
					}
				}
			}
			else
			{
				if( !cGameApp::m_sucKeyData[m_RefundKey.GetKeyData()] )
					m_RefundCoinKeyPressTimeTC.Start();
			}
		}
	}
}








void cRefoundCoinDevice::Update( float e_fElpaseTime )
{
	if( g_pIOSMDriverInterface->IsIOError() || cFishApp::m_spProbabilityFish->IsMasterLeeIOError())
	{
		return;
	}
	//
	Update_CoinBehavior( e_fElpaseTime );
	//
	Update_ExchangeBehaviod( e_fElpaseTime );
	//
	if( m_bRefundCoin )
	{
		m_RefundToCheckCoinTC.Update(e_fElpaseTime);
	}
}








void cRefoundCoinDevice::Render()
{
//	std::wstring	l_str = UT::ComposeMsgByFormat(L"DebugState:%d  %d",m_iDebugState,m_iDebugState1);
//	cGameApp::m_spGlyphFontRender->RenderFont(200 , 400+m_pPlayer->m_iPlayerID*50, l_str.c_str());
	//swprintf(m_strReceive, MAX_PATH, L"Refund State = %d",m_iDebugState);
	//cGameApp::m_spGlyphFontRender->RenderFont(800 , 800, m_strReceive);

	//std::wstring	l_str = UT::ComposeMsgByFormat(L"P%d:Exchange:%s,ExchangeValue%d,ExchangeTotalValue%d",m_pPlayer->m_iPlayerID,cGameApp::m_sucKeyData[m_ExchangeInKey.GetKeyData()]?L"1":L"0",m_iExchangeDebugValue,m_iExchangeDebugValueTotal);
	//cGameApp::m_spGlyphFontRender->RenderFont(350,m_pPlayer->m_iPlayerID*30+200,l_str.c_str());
}