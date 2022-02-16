#include "stdafx.h"
#include "DeviceRefundCoin.h"
//
#include "GameApp.h"
#include "../PlayerBehavior/PlayerData.h"
//
cRefoundCoinDeviceManager::cRefoundCoinDeviceManager()
{
}

cRefoundCoinDeviceManager::~cRefoundCoinDeviceManager()
{

}


void cRefoundCoinDeviceManager::Init(const char *filename)
{
	if(!cGameApp::m_sbDeviceExist)
	{
	}
	SAFE_REFUND_COIN_COUNT = g_pIOSMDriverInterface->GetSafeRefundCoinCount();
	if( !g_pIOSMDriverInterface||!cFishApp::m_spPlayerManager )
		return;
	this->Destroy();
	//rate of coin to score
	cEventVariable*l_pRefundCoinOrTicket = cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"RefundCoinOrTicket");
	bool bCoinDevice = l_pRefundCoinOrTicket->IsSame(L"Ticket")?false:true;


	int*	l_pCoinToScore = cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject( L"CoinToScore" )->GetInt();
	int*	l_pTicketToScore = cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject( L"TicketToScore" )->GetInt();



	std::wstring	l_str;
	for( int i=0;i<cFishApp::m_spPlayerManager->Count();++i )
	{
		cPlayerBehaviorBase*l_pPlayerBehaviorBase = cFishApp::m_spPlayerManager->GetObject(i);

		l_str = UT::ComposeMsgByFormat(L"InsertCoinCount_%d",i);
		cIOSMIOPackData*l_InsertCoinIOSMIOPackData = (cIOSMIOPackData*)g_pIOSMDriverInterface->GetObject(l_str.c_str());
		//
		l_str = UT::ComposeMsgByFormat(L"CoinRefund_%d",i);
		cIOSMIOPackData*l_pHopperValueIOSMIOPackData = (cIOSMIOPackData*)g_pIOSMDriverInterface->GetObject(l_str.c_str());
		//
		l_str = UT::ComposeMsgByFormat(L"RestCoinToRefund_%d",i);
		cIOSMIOPackData*l_pReHopperValueIOSMIOPackData = (cIOSMIOPackData*)g_pIOSMDriverInterface->GetObject(l_str.c_str());
		//
		l_str = UT::ComposeMsgByFormat(L"TicketRefund_%d",i);
		cIOSMIOPackData*l_pTicketValueIOSMIOPackData = (cIOSMIOPackData*)g_pIOSMDriverInterface->GetObject(l_str.c_str());
		//
		l_str = UT::ComposeMsgByFormat(L"RestTicketToRefund_%d",i);
		cIOSMIOPackData*l_pReTicketValueIOSMIOPackData = (cIOSMIOPackData*)g_pIOSMDriverInterface->GetObject(l_str.c_str());
		//
		l_str = UT::ComposeMsgByFormat(L"ReFundButton_%d",i);
		cIOSMIOData*l_pReturnKey = g_pIOSMDriverInterface->GetObject(l_str.c_str());
		//
		l_str = UT::ComposeMsgByFormat(L"EcchangeIn_%d",i);
		cIOSMIOData*l_pOpenKey = g_pIOSMDriverInterface->GetObject(l_str.c_str());
		//
		l_str = UT::ComposeMsgByFormat(L"EcchangeOut_%d",i);
		cIOSMIOData*l_pWashKey = g_pIOSMDriverInterface->GetObject(l_str.c_str());
		//
		l_str = UT::ComposeMsgByFormat(L"RefundSequence_%d",i);
		cIOSMIOPackData*l_pExchangeInSequenceIOSMIOPackData = (cIOSMIOPackData*)g_pIOSMDriverInterface->GetObject(l_str.c_str());

		l_str = UT::ComposeMsgByFormat(L"CoinReset_%d",i);
		cIOSMIOData*l_pCoinResetIOSMIOData = g_pIOSMDriverInterface->GetObject(l_str.c_str());
		l_str = UT::ComposeMsgByFormat(L"TicketReset_%d",i);
		cIOSMIOData*l_pTicketResetIOSMIOData = g_pIOSMDriverInterface->GetObject(l_str.c_str());


		assert(l_pPlayerBehaviorBase);
		assert(l_InsertCoinIOSMIOPackData);
		assert(l_pHopperValueIOSMIOPackData);
		assert(l_pReHopperValueIOSMIOPackData);
		assert(l_pTicketValueIOSMIOPackData);
		assert(l_pReTicketValueIOSMIOPackData);
		assert(l_pExchangeInSequenceIOSMIOPackData);
		assert(l_pCoinResetIOSMIOData);
		assert(l_pTicketResetIOSMIOData);

		cRefoundCoinDevice*l_pPlayer = new cRefoundCoinDevice(  l_pPlayerBehaviorBase,//player ID
																l_InsertCoinIOSMIOPackData,//Insert Coin Value
																l_pHopperValueIOSMIOPackData,//Hopper value
																l_pReHopperValueIOSMIOPackData,//ReHopper value
																l_pTicketValueIOSMIOPackData,//Ticket value
																l_pReTicketValueIOSMIOPackData,//ReTicket value
																l_pExchangeInSequenceIOSMIOPackData,
																l_pCoinResetIOSMIOData,
																l_pTicketResetIOSMIOData,
																l_pCoinToScore,//rate of coin to score
																l_pTicketToScore,
																*l_pReturnKey->GetKeyData(),//return key
																*l_pOpenKey->GetKeyData(),//Open key
																*l_pWashKey->GetKeyData()//Wash key
																);//coin Insert value
		this->AddObjectNeglectExist(l_pPlayer);
	}
}








void	cRefoundCoinDeviceManager::Update(float e_fElpaseTime)
{
		//std::string	l_str = UT::ComposeMsgByFormat("Have device");
		//cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(l_str.c_str());	
		//g_pRefoundCoinDevice->Update(e_fElpaseTime);
		int	l_iCount = this->Count();		
		for( int i=0;i<l_iCount;++i )
		//for(int i=0;i<cFishApp::m_spPlayerManager->Count();++i)
		//for(int i=0;i<8;++i)
		{
			//std::string	l_str = UT::ComposeMsgByFormat("Refund-Index:%d  OK",i);
			//cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(l_str.c_str());
			this->m_ObjectList[i]->Update(e_fElpaseTime);
		}				
}








void	cRefoundCoinDeviceManager::Render()
{
	int	l_iCount = this->Count();	
	for( int i=0;i<l_iCount;++i )
		this->m_ObjectList[i]->Render();
	//for(int i=0;i<cFishApp::m_spPlayerManager->Count();++i)
	//for(int i=0;i<8;++i)
		
}








void	cRefoundCoinDeviceManager::Destroy()
{

}








