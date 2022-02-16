#include "../stdafx.h"
#include "NetWork.h"
#include "../GameplayUT/BinaryFile.h"
#include "../GameplayUT/GameApp.h"
#include "SDLnetsys.h"

#ifndef WIN32
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif

namespace FATMING_CORE
{
	//if data is completed received return NULL
	//or if return data is not null find out partial part and rebind

	sReceivedPacket*	ReceiveData(_TCPsocket*e_pTCPsocket)
	{
		sReceivedPacket*l_pPacket = new sReceivedPacket();
		l_pPacket->pReceivedSocket = e_pTCPsocket;
		if( !l_pPacket )
		{
			//out of memory
			return NULL;
		}
		//first received size
		int	l_iLength = SDLNet_TCP_Recv(e_pTCPsocket,&l_pPacket->iSize,sizeof(size_t));
		size_t	l_iRestLength = l_pPacket->iSize;
		if( l_iLength < sizeof(size_t) )
		{
			//connect failed,received wrong data
			delete l_pPacket;
			return NULL;
		}	
		/* check if anything is strange, like a zero length buffer */
		//l_pPacket->iSize = SDL_SwapBE32(l_pPacket->iSize);
		if(!l_pPacket->iSize)
			return NULL;
		l_pPacket->pData = new char[l_pPacket->iSize];
		memset(l_pPacket->pData,0,sizeof(char)*l_pPacket->iSize);
		char*	l_pBuffer = l_pPacket->pData;
		while( l_iRestLength )
		{
			l_iLength = SDLNet_TCP_Recv(e_pTCPsocket,l_pBuffer,l_iRestLength);
			l_iRestLength -= l_iLength;
			if( l_iLength )
			{
				l_pBuffer += l_iLength;
			}
			else
			{//data length error
				delete l_pPacket;
				return NULL;
			}
		}
		return l_pPacket;
	}

	void	NetworkListenThread(size_t _workParameter, size_t _pUri)
	{
		cNetwork*l_pNetwork = (cNetwork*)_workParameter;
		TCPsocket l_pNewClient = 0;
		while( l_pNetwork->m_pSocket )
		{
			int l_iNumready = 0;
			if(!l_pNetwork->CreateSocksetToListenData())
			{
				goto FAILED;
			}
			printf("listen\n");
			l_iNumready = SDLNet_CheckSockets(l_pNetwork->m_pAllSocketToListenClientMessage, (UINT)-1);
			if( l_iNumready == -1 )
			{
				goto FAILED;
				//printf("SDLNet_CheckSockets: %s\n",SDL_GetError());
			}
			if(!l_iNumready)
				continue;
			//new client?
			if(l_pNetwork->m_pSocket&&l_pNetwork->m_pSocket->iServerFlag && SDLNet_SocketReady(l_pNetwork->m_pSocket))
			{
				if( l_pNetwork->m_pSocket->iServerFlag )
				{
					l_pNewClient = SDLNet_TCP_Accept(l_pNetwork->m_pSocket);
					if( l_pNewClient )
					{
						l_pNetwork->AddClient(l_pNewClient);
						printf("new client\n");
						//continue;
					}
				}
			}
			if( l_pNetwork->m_pSocket )
			{
				if(l_pNetwork->m_pSocket->iServerFlag)
				{
					cFUSynchronizedHold hold(&l_pNetwork->m_ClientSocketVector);
					int	l_iNumClients = l_pNetwork->m_ClientSocketVector.size();
					for(int i=0; l_iNumready && i<l_iNumClients; i++)
					{
						TCPsocket l_pClient = l_pNetwork->m_ClientSocketVector[i];
						if(SDLNet_SocketReady(l_pClient))
						{
							sReceivedPacket*l_pPacket = ReceiveData(l_pClient);
							if( l_pPacket )
							{
								printf("recevied message\n");
								cFUSynchronizedHold hold(&l_pNetwork->m_ReceivedData);
								l_pNetwork->m_ReceivedData.push_back(l_pPacket);
							}
							else

							{
								printf("client connection failed\n");
								if(!l_pNetwork->RemoveClient(l_pClient))
								{
									assert(0&&"no this client");
								}
								--l_iNumClients;
							}
						}
					}
				}
				else
				{
					sReceivedPacket*l_pPacket = ReceiveData(l_pNetwork->m_pSocket);
					if( l_pPacket )
					{
						cFUSynchronizedHold hold(&l_pNetwork->m_ReceivedData);
						l_pNetwork->m_ReceivedData.push_back(l_pPacket);
					}
					else
					{
						goto FAILED;
						//printf("SDLNet_CheckSockets: %s\n",SDL_GetError());
					}
				}
			}
		}
		return;
	FAILED:
		l_pNetwork->m_bConnectionFailed = true;
		//cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine("disconnect!",true);
	}

	void	NetworkListenThreadDone(size_t _workParameter, size_t _pUri)
	{
		//cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine("NetworkListenThreadDone thread done",true);
		printf("NetworkListenThreadDone thread done");
	}

	//void	NetworkSendData(size_t _workParameter, size_t _pUri)
	//{
	//
	//}
	//
	//void	NetworkSendDataDone(size_t _workParameter, size_t _pUri)
	//{
	//
	//}

	cNetwork::cNetwork()
	{
		m_pDisconnectCallback = 0;
		m_pNewClientConnectedCallBack = 0;
		m_bConnectionFailed = false;
		m_bDisconnectCall = false;
		m_pThreadPool = new cFUThreadPool();
		//one for received thread,another one for miscellaneous
		m_pThreadPool->Spawn(2);
		SDLNet_Init();
		m_pAllSocketToListenClientMessage = 0;
		m_iPort = -1;
		m_pSocket = 0;
	}

	cNetwork::~cNetwork()
	{
		//cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine("delete cNetwork",true);
		this->Destroy();
		SAFE_DELETE(m_pThreadPool);
		SDLNet_Quit();
	}

	/* create a socket set that has the server socket and all the client sockets */
	bool	cNetwork::CreateSocksetToListenData()
	{
		printf("create socket\n");
		if( m_pAllSocketToListenClientMessage )
			SDLNet_FreeSocketSet(m_pAllSocketToListenClientMessage);
		size_t	l_iNumClient = 0;
		//is it server?
		cFUSynchronizedHold hold(&m_ClientSocketVector);
		if( m_pSocket->iServerFlag )
			l_iNumClient = m_ClientSocketVector.size();
		//+1 fo server
		m_pAllSocketToListenClientMessage = SDLNet_AllocSocketSet(l_iNumClient+1);
		if(!m_pAllSocketToListenClientMessage)
		{
			//printf("SDLNet_AllocSocketSet: %s\n", SDL_GetError());
			//exit(1); /*most of the time this is a major error, but do what you want. */
			return false;
		}

		SDLNet_TCP_AddSocket(m_pAllSocketToListenClientMessage,m_pSocket);
		for(size_t i=0;i<l_iNumClient;i++)
			SDLNet_TCP_AddSocket(m_pAllSocketToListenClientMessage,m_ClientSocketVector[i]);
		return true;
	}


	void	cNetwork::AddClient(_TCPsocket*e__pTCPsocket)
	{
		cFUSynchronizedHold hold(&m_ClientSocketVector);
		int	l_iSize = (int)m_ClientSocketVector.size();
		for( int i=0;i<l_iSize;++i )
		{
			if(m_ClientSocketVector[i]->channel == e__pTCPsocket->channel )
			{
				UT::ErrorMsg(L"Client was exist",L"add new client error");
				return;
			}
		}
		m_ClientSocketVector.push_back(e__pTCPsocket);
		if(m_pNewClientConnectedCallBack)
		{
			m_pNewClientConnectedCallBack(e__pTCPsocket);
		}
	}

	void	cNetwork::RemoveAllClient()
	{
		cFUSynchronizedHold hold(&m_ClientSocketVector);
		while( m_ClientSocketVector.size() )
		{
			if( m_ClientSocketVector[0] )
			{
				if(m_pDisconnectCallback)
				{
					m_pDisconnectCallback(m_ClientSocketVector[0]);
				}
				SDLNet_TCP_Close(m_ClientSocketVector[0]);
			}
			m_ClientSocketVector.erase(m_ClientSocketVector.begin());
		}
	}


	bool	cNetwork::RemoveClient(_TCPsocket*e__pTCPsocket)
	{
		/* close the old socket, even if it's dead... */
		int	l_iSize = (int)m_ClientSocketVector.size();
		for( int i=0;i<l_iSize;++i )
		{
			if( m_ClientSocketVector[i] == e__pTCPsocket )
			{
				_TCPsocket*l_pTCPsocket = m_ClientSocketVector[i];
				if( e__pTCPsocket )
				{
					if(m_pDisconnectCallback)
					{
						m_pDisconnectCallback(e__pTCPsocket);
					}
					SDLNet_TCP_Close(e__pTCPsocket);
				}
				m_ClientSocketVector.erase(m_ClientSocketVector.begin()+i);
				return true;
			}
		}
		return false;
	}

	bool	cNetwork::IsConnectionFailed()
	{
		return m_bConnectionFailed;
	}

	bool	cNetwork::IsConnected()
	{
		return m_pSocket?true:false;
	}

	bool	cNetwork::SendData(_TCPsocket*e_pTCPsocket,sSendPacket*e_pPacket)
	{
		if( e_pTCPsocket )
		{
			printf("send data\n");
			char*l_pData = new char[sizeof(size_t)+e_pPacket->iSize];
			memcpy(l_pData,&e_pPacket->iSize,sizeof(size_t));
			memcpy(l_pData+sizeof(size_t),e_pPacket->pData,e_pPacket->iSize);
			bool	l_bSent = SDLNet_TCP_Send(e_pTCPsocket,l_pData,e_pPacket->iSize+sizeof(size_t)) == 0?false:true;
			delete l_pData;
			return l_bSent;
		}
		return false;
	}

	bool	cNetwork::SendDataToServer(sSendPacket*e_pPacket)
	{
		if( m_pSocket )
			return SendData(this->m_pSocket,e_pPacket);
		return false;
	}

	bool	cNetwork::SendDataToAllClient(sSendPacket*e_pPacket)
	{
		if(!e_pPacket)
			return false;
		cFUSynchronizedHold hold(&m_ClientSocketVector);
		int	l_iSize = m_ClientSocketVector.size();
		for( int i=0;i<l_iSize;++i )
		{
			/* putMsg is in tcputil.h, it sends a buffer over a socket */
			/* with error checking */
			if(!SendData(m_ClientSocketVector[i],e_pPacket))
			{
				//this->RemoveClient(m_ClientSocketVector[i]);
				//--i;
			}
		}
		return true;
	}

	bool	cNetwork::CreateAsServer(int e_iPort)
	{
		return OpenSocket(e_iPort,0);
	}

	bool	cNetwork::CreateAsClient(int e_iPort,const char*e_strIP)
	{
		//return OpenSocket(e_iPort,e_strIP);
		OpenSocket(e_iPort,e_strIP);
		//sSendPacket	l_Packet;
		//l_Packet.iSize = 17;
		//l_Packet.pData  = new char[17];
		//sprintf(l_Packet.pData,"%s","fcuk1234567\0");
		//SendDataToServer(&l_Packet);
		return true;
	}


	bool	cNetwork::GetIP()
	{
		char l_strName[MAX_PATH];
		if (gethostname(l_strName, sizeof(l_strName)) == SOCKET_ERROR) 
		{
			return false;
		}
		hostent *l_pPhe = gethostbyname(l_strName);
		if (l_pPhe == 0) 
		{
			return false;
		}
		//
		//for (int i = 0; l_pPhe->h_addr_list[i] != 0; ++i) 
		//{
		//	struct in_addr addr;
		//	memcpy(&addr, l_pPhe->h_addr_list[i], sizeof(struct in_addr));
		//	//server machine might has more than one network adapter,just get first one.
		//	if( i == 0 )
		//		memcpy(m_MyIP,&addr.S_un.S_un_b,sizeof(char)*4);
		//}
		
        m_strIP = inet_ntoa (*(struct in_addr *)*l_pPhe->h_addr_list);
		sprintf(l_strName,"%s",m_strIP.c_str());


		char*	l_str = strtok(l_strName,".");
		int	l_iIndex = 0;
		while(l_str)
		{
			m_MyIP[l_iIndex] = atoi(l_str);
			l_str = strtok(0,".");
			++l_iIndex;
		}
		printf("IP:%s\n",l_str);
		return true;
	}

	bool	cNetwork::OpenSocket(int e_iPort,const char*e_strIP)
	{
		if(this->IsConnected())
		{
			this->Destroy();
		}
		m_iPort = e_iPort;
		printf("port:%d\n",m_iPort);
		printf("fetch IP \n");
		/* Resolve the argument into an IPaddress type */
		if(SDLNet_ResolveHost(&m_IP,e_strIP,m_iPort)==-1)
		{
			printf("fetch IP failed\n");
			return false;
		}
		UINT l_uiAddr;
		l_uiAddr = SDL_SwapBE32(m_IP.host);
		printf("IP Address : %d.%d.%d.%d\n",
				l_uiAddr>>24,
				(l_uiAddr>>16)&0xff,
				(l_uiAddr>>8)&0xff,
				l_uiAddr&0xff);


		m_strHost = SDLNet_ResolveIP(&m_IP);
		m_pSocket = SDLNet_TCP_Open(&m_IP);
		printf("socket open\n");
		//if SDLNet_TCP_Open called server's SDLNet_TCP_Accept will be triggered
		if( !m_pSocket )
		{
			printf("socket open failed\n");
			return false;
		}
		this->m_bConnectionFailed = false;
		CreateThread();
		return true;
	}

	bool	cNetwork::IsReceivedData()
	{
		cFUSynchronizedHold hold(&m_ReceivedData);
		return m_ReceivedData.size()>0?true:false;
	}

	bool	cNetwork::CreateThread()
	{
		printf("create thread to listen\n");
		FUStaticFunctor2<size_t, size_t, void>* workFunctor = new FUStaticFunctor2<size_t, size_t, void>(&NetworkListenThread);
		FUStaticFunctor2<size_t, size_t, void>* doneFunctor = new FUStaticFunctor2<size_t, size_t, void>(&NetworkListenThreadDone);
		this->m_pThreadPool->ExecuteWork(workFunctor,doneFunctor,(size_t)this,0);
		return true;
	}

	void	cNetwork::Destroy()
	{
		//cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine("Destroy cNetwork",true);
		CloseSocket();
		m_pThreadPool->WaitForIdle();
		m_pThreadPool->Clear();
	}

	void		cNetwork::CloseSocket()
	{
		if( m_pSocket )
		{
			//cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine("Destroy socket",true);
			SDLNet_TCP_Close(m_pSocket);
			m_pSocket = 0;
			this->m_pThreadPool->WaitForIdle();
			RemoveAllClient();
			{
				cFUSynchronizedHold hold(&m_ReceivedData);
				m_ReceivedData.Clear();
			}
			{
				cFUSynchronizedHold hold2(&m_ClientSocketVector);
				m_ClientSocketVector.clear();
			}
			if( m_pAllSocketToListenClientMessage )
			{
				SDLNet_FreeSocketSet(m_pAllSocketToListenClientMessage);
				m_pAllSocketToListenClientMessage = 0;
			}
		}		
	}

	void	cNetwork::Update(float e_fElpaseTime)
	{
		if( this->m_bConnectionFailed )
		{
			this->CloseSocket();
			m_bConnectionFailed = false;
		}
	}

	bool	cNetwork::DownloadFile(WCHAR*e_strURL,WCHAR*e_strLocalFileName)
	{
#ifdef WIN32
			HRESULT hRez = URLDownloadToFile( NULL, e_strURL,e_strLocalFileName, 0, NULL );
			if( hRez == 0 )
			{
				return true;
			}
			else
			{
			 // download failed
			}
#elif defined(IOS)
			//method 1
			//https://developer.apple.com/library/mac/#documentation/Cocoa/Conceptual/URLLoadingSystem/Tasks/UsingNSURLDownload.html#//apple_ref/doc/uid/20001839
			//method 2
			//NSDATA *data = [NSDATA dataWithContentOfURL:@"www.yourserver.com/abc.txt"];
			////method 3
			//NSURLRequest* req = [NSURLRequest requestWithURL:[NSURL URLWithString:@"ftp://myftpserver/path/to/file.txt"]];
			//float aFloat;
			//NSScanner *theScanner = [NSScanner scannerWithString:aString];
			//while ([theScanner isAtEnd] == NO) {

			//	[theScanner scanFloat:&aFloat];
			//	// implementation continues...
			//}
#elif defined(LINUX)
//linux?
#elif defined(ANDROID)
		assert(0&&"not support this platform");
#endif
		return false;
	}
//end namespace
}