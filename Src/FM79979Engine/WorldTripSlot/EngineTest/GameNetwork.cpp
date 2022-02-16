#include "stdafx.h"
#include "GameNetwork.h"
#include "SlotGamePhase.h"
#include "PlayerData.h"

//cGameNetwork*g_pGameNetwork = 0;
////while new client is join call back function will call immediately
//void	NewClientMessage(void*e_pData)
//{
//	_TCPsocket*l__pTCPsocket = (_TCPsocket*)e_pData;
//	sGamePacket l_GamePacket;
//	sSendPacket	l_Packet;
//	l_Packet.iSize = sizeof(sGamePacket);
//	l_Packet.pData = (char*)&l_GamePacket;
//	cPlayerData*l_pPlayerData = cWorldTripSlotApp::m_spPlayerData;
//	l_GamePacket.iMessageID = eGMI_TO_CLIENT_JP_SCORE;
//	l_GamePacket.iJPScore = *l_pPlayerData->GetJP(eJPT_NETWORK);
//	g_pGameNetwork->SendData(l__pTCPsocket,&l_Packet);
//	l_Packet.pData = 0;
//}
//
//
//void	ReConnectedThread(size_t _workParameter, size_t _pUri)
//{
//	cGameApp::OutputDebugInfoString("tring to connect start\n");
//	cGameNetwork*l_pGameNetwork = (cGameNetwork*)_workParameter;
//	l_pGameNetwork->m_bConnectingTest = true;
//	l_pGameNetwork->Connect();
//	if(l_pGameNetwork->IsConnected())
//	{
//		cGameApp::OutputDebugInfoString("connected\n");
//	}
//	else
//	{
//		cGameApp::OutputDebugInfoString("connect failed \n");
//	}
//}
//
//void	ReConnectedThreadDone(size_t _workParameter, size_t _pUri)
//{
//	cGameNetwork*l_pGameNetwork = (cGameNetwork*)_workParameter;
//	l_pGameNetwork->m_bConnectingTest = false;
//	cGameApp::OutputDebugInfoString("tring to connect end\n");
//}
//
//cGameNetwork::cGameNetwork()
//{
//	m_bJPFetchStamp = false;
//	m_bICouldWINJP = false;
//	g_pGameNetwork = this;
//	m_bServer = false;
//	m_ReconnectTimer.SetTargetTime(5.f);
//	this->m_pNewClientConnectedCallBack = NewClientMessage;
//	m_bConnectingTest = false;
//}
//
//cGameNetwork::~cGameNetwork()
//{
//
//}
//
//void	cGameNetwork::Connect()
//{
//	cGameApp::OutputDebugInfoString("connection test\n");
//	if(!this->m_pSocket)
//	{
//		if(m_bServer)
//		{
//			cGameApp::OutputDebugInfoString("I am server\n");
//			if(!this->CreateAsServer(m_iPort))
//			{
//				cGameApp::OutputDebugInfoString("server create failed\n");
//			}
//		}
//		else
//		{
//			cGameApp::OutputDebugInfoString("I am client\n");
//			if(!this->CreateAsClient(m_iPort,m_strServerIP.c_str()))
//			{
//				cGameApp::OutputDebugInfoString("connect to server failed\n");
//			}
//		}
//	}
//}
//
//void	cGameNetwork::ParseFile()
//{
//	if(GetIP())
//	{
//		cNodeISAX	l_NodeISAX;
//		if(l_NodeISAX.ParseDataIntoXMLNode("WorldTripSlot/NetworkData.xml"))
//		{
//			TiXmlElement*l_pTiXmlElement = l_NodeISAX.GetRootElement();
//			const WCHAR*l_strPort = l_pTiXmlElement->Attribute(L"Port");
//			const WCHAR*l_strReconnectTime = l_pTiXmlElement->Attribute(L"ReconnectTime");
//			float	l_fTime = (float)_wtof(l_strReconnectTime);
//			m_ReconnectTimer.SetTargetTime(l_fTime);
//			m_ReconnectTimer.Start();
//			this->m_iPort = _wtoi(l_strPort);
//			l_pTiXmlElement = l_NodeISAX.GetRootElement()->FirstChildElement();
//			while( l_pTiXmlElement )
//			{
//				const WCHAR*l_strValue = l_pTiXmlElement->Value();
//				const WCHAR*l_strIP = l_pTiXmlElement->Attribute(L"IP");
//				if( !wcscmp(l_strValue,L"Server") )
//				{
//					m_strServerIP = UT::WcharToChar(l_strIP);
//					if(!wcscmp(UT::CharToWchar(this->m_strIP.c_str()).c_str(),l_strIP))
//					{
//						cGameApp::OutputDebugInfoString("I am server\n");
//						m_bServer = true;
//					}
//					else
//					{
//						cGameApp::OutputDebugInfoString("I am client\n");
//						m_bServer = false;
//					}
//					cGameApp::OutputDebugInfoString("server IP %s\n",m_strServerIP.c_str());
//				}
//				else
//				if( !wcscmp(l_strValue,L"Client") )
//				{
//				
//				}
//				l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
//			}
//		}
//		if(this->m_bServer)
//		{
//			cGameApp::OutputDebugInfoString("open JP data %s\n",m_strServerIP.c_str());
//			cWorldTripSlotApp::m_spPlayerData->OpenJP();
//		}
//	}
//}
//
//void	cGameNetwork::MessageProcess(sReceivedPacket*e_pPacket)
//{
//	sGamePacket*l_pGamePacket = (sGamePacket*)e_pPacket->pData;
//	sGamePacket l_GamePacket;
//	cPlayerData*l_pPlayerData = cWorldTripSlotApp::m_spPlayerData;
//	sSendPacket	l_Packet;
//	l_Packet.iSize = sizeof(sGamePacket);
//	l_Packet.pData = (char*)&l_GamePacket;
//	if(this->m_bServer)
//	{
//		if(  l_pGamePacket->iMessageID == eGMI_TO_SERVER_JP_SCORE )
//		{
//			cGameApp::OutputDebugInfoString("eGMI_TO_SERVER_JP_SCORE\n");
//			int	l_iValue = *l_pPlayerData->GetJP(eJPT_NETWORK)+l_pGamePacket->iJPScore;
//			l_pPlayerData->SetJP(eJPT_NETWORK,l_iValue);
//			l_GamePacket.iJPScore = *l_pPlayerData->GetJP(eJPT_NETWORK);
//			l_GamePacket.iMessageID = eGMI_TO_CLIENT_JP_SCORE;
//			this->SendDataToAllClient(&l_Packet);
//		}
//		else
//		if(  l_pGamePacket->iMessageID == eGMI_TO_SERVER_I_WIN_JP )
//		{
//			cGameApp::OutputDebugInfoString("eGMI_TO_SERVER_I_WIN_JP\n");
//			if( IsJPRequestAccept() )
//			{
//				m_bJPFetchStamp = true;
//				l_GamePacket.iMessageID = eGMI_TO_CLIENT_JP_IS_GET_BY_YOU;
//				this->SendDataToAllClient(&l_Packet);
//			}
//			else
//			{
//				l_GamePacket.iMessageID = eGMI_TO_CLIENT_JP_IS_GET_BY_ORTHERS;
//				this->SendData(e_pPacket->pReceivedSocket,&l_Packet);
//			}
//		}
//		else
//		if( l_pGamePacket->iMessageID == eGMI_TO_SERVER_FLUSH_JP )
//		{
//			cGameApp::OutputDebugInfoString("eGMI_TO_SERVER_FLUSH_JP\n");
//			m_bJPFetchStamp = false;
//			l_pPlayerData->SetJP(eJPT_NETWORK,0);
//			l_GamePacket.iJPScore = *l_pPlayerData->GetJP(eJPT_NETWORK);
//			l_GamePacket.iMessageID = eGMI_TO_CLIENT_JP_SCORE;
//			this->SendDataToAllClient(&l_Packet);
//		}
//	}
//	else
//	{
//		if(  l_pGamePacket->iMessageID == eGMI_TO_CLIENT_JP_SCORE )
//		{
//			cGameApp::OutputDebugInfoString("eGMI_TO_CLIENT_JP_SCORE\n");
//			l_pPlayerData->SetJP(eJPT_NETWORK,l_pGamePacket->iJPScore);
//		}
//		else
//		if(  l_pGamePacket->iMessageID == eGMI_TO_CLIENT_JP_IS_GET_BY_YOU )
//		{
//			cGameApp::OutputDebugInfoString("eGMI_TO_CLIENT_JP_IS_GET_BY_YOU\n");
//			m_bJPInfoGet = true;
//			this->m_bICouldWINJP = true;
//		}
//		else
//		if(  l_pGamePacket->iMessageID == eGMI_TO_CLIENT_JP_IS_GET_BY_ORTHERS )
//		{
//			cGameApp::OutputDebugInfoString("eGMI_TO_CLIENT_JP_IS_GET_BY_ORTHERS\n");
//			m_bJPInfoGet = true;
//			this->m_bICouldWINJP = false;
//		}
//	}
//	l_Packet.pData = 0;
//}
//
//void	cGameNetwork::Update(float e_fElpaseTime)
//{
//	if(!g_pSlotGamePhase->m_bOpenNetWork)
//		return;
//	m_bJPInfoGet = false;
//	cNetwork::Update(e_fElpaseTime);
//	if(!this->IsConnected())
//	{
//		if(!m_bConnectingTest)
//		{
//			m_ReconnectTimer.Update(e_fElpaseTime);
//			if( m_ReconnectTimer.bTragetTimrReached )
//			{
//				FUStaticFunctor2<size_t, size_t, void>* workFunctor = new FUStaticFunctor2<size_t, size_t, void>(&ReConnectedThread);
//				FUStaticFunctor2<size_t, size_t, void>* doneFunctor = new FUStaticFunctor2<size_t, size_t, void>(&ReConnectedThreadDone);
//				this->m_pThreadPool->ExecuteWork(workFunctor,doneFunctor,(size_t)this,0);
//				m_ReconnectTimer.Start();
//			}
//		}
//	}
//	else
//	{
//		cFUSynchronizedHold hold(&m_ReceivedData);
//		while(m_ReceivedData.size())
//		{//new data is coming!
//			sReceivedPacket*l_pPacket = m_ReceivedData[0];
//			MessageProcess(l_pPacket);
//			delete l_pPacket;
//			m_ReceivedData.erase(m_ReceivedData.begin());
//		}
//	}
//}
//
//void	cGameNetwork::Render()
//{
//	
//}
//
//bool	cGameNetwork::IsJPRequestAccept()
//{
//	if( m_bJPFetchStamp )
//		return false;
//	if( *cWorldTripSlotApp::m_spPlayerData->GetJP(eJPT_NETWORK) > 50000 )
//		return true;
//	return false;
//}
//
//bool	cGameNetwork::SendData(sGamePacket*e_pGamePacket)
//{
//	sSendPacket l_Packet;
//	l_Packet.iSize = sizeof(sGamePacket);
//	l_Packet.pData = (char*)e_pGamePacket;
//	if(e_pGamePacket->iMessageID == eGMI_TO_SERVER_FLUSH_JP )
//	{
//		if(IsServer())
//		{
//			e_pGamePacket->iMessageID = eGMI_TO_CLIENT_JP_SCORE;
//			e_pGamePacket->iJPScore = 0;
//			SendDataToAllClient(&l_Packet);
//		}
//		else
//		{
//			e_pGamePacket->iMessageID = eGMI_TO_SERVER_FLUSH_JP;
//			SendDataToServer(&l_Packet);
//		}
//	}
//	else
//	if(e_pGamePacket->iMessageID == eGMI_TO_SERVER_JP_SCORE )
//	{
//		if(IsServer())
//		{
//			int	l_iValue = *cWorldTripSlotApp::m_spPlayerData->GetJP(eJPT_NETWORK)+ e_pGamePacket->iJPScore;
//			cWorldTripSlotApp::m_spPlayerData->SetJP(eJPT_NETWORK,l_iValue);
//			e_pGamePacket->iJPScore = l_iValue;
//			e_pGamePacket->iMessageID = eGMI_TO_CLIENT_JP_SCORE;
//			SendDataToAllClient(&l_Packet);
//		}
//		else
//		{
//			e_pGamePacket->iMessageID = eGMI_TO_SERVER_JP_SCORE;
//			SendDataToServer(&l_Packet);
//		}
//	}
//	//
//	l_Packet.pData = 0;
//	return true;
//}