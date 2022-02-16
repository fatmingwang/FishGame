#ifndef _GAME_NET_WORK_
#define	_GAME_NET_WORK_
//
////struct sockaddr_in sa;
////// store this IP address in sa:
////inet_pton(AF_INET, "192.0.2.33", &(sa.sin_addr));
////
////// now get it back and print it
////inet_ntop(AF_INET, &(sa.sin_addr), str, INET_ADDRSTRLEN);
//enum	eGameMessageID
//{
//	//server
//	eGMI_TO_SERVER_START_ID = 79979,
//	eGMI_TO_SERVER_JP_SCORE,
//	eGMI_TO_SERVER_I_WIN_JP,
//	eGMI_TO_SERVER_FLUSH_JP,
//	//client
//	eGMI_TO_CLIENT_START_ID = 839657,
//	eGMI_TO_CLIENT_JP_SCORE,
//	eGMI_TO_CLIENT_JP_IS_GET_BY_YOU,	//if eGMI_TO_CLIENT_JP_IS_GET_BY_ORTHERS was received, client cannt get jp at a small time.
//	eGMI_TO_CLIENT_JP_IS_GET_BY_ORTHERS,//avoid JP was open at same time.
//	eGMI_MAX,
//};
//
//struct	sGamePacket
//{
//	int		iMessageID;
//	int		iJPScore;
//};
//
////struct	sAddress
////{
////	int	iAdressp[4];
////};
//
//class	cGameNetwork:public cNetwork
//{
//	friend void			ReConnectedThread(size_t _workParameter, size_t _pUri);
//	friend void			ReConnectedThreadDone(size_t _workParameter, size_t _pUri);
//
//	//if trying to connect true,or false
//	//if true the thread is trying to connected
//	bool				m_bConnectingTest;
//	bool				m_bServer;
//	std::string			m_strServerIP;
//	//
//	UT::sTimeCounter	m_ReconnectTimer;
//	//std::vector<sClientAddress>	m_Allo
//	int		m_iJPScore;
//	//-1
//	int		m_iJPWinner;
//	void	MessageProcess(sReceivedPacket*e_pPacket);
//	//it always be false,but if server or client send data to server to ask I want WIN JP,and compare JP money is allow to win money it will be true
//	//but after set it as true set it to fase immediatlly
//	GET_SET_DEC(bool,m_bICouldWINJP,IsICouldWINJP,SetICouldWINJP);
//	//if JP is fetch,true and received JP flush message false
//	//while true any new message for JP fetch is disable,and if client is disconnect while fetch is sent the stam will be always be false until next time to restart server
//	bool	m_bJPFetchStamp;
//public:
//	//if JP info is sent this will be true and next frame will be false again
//	bool	m_bJPInfoGet;
//	cGameNetwork();
//	~cGameNetwork();
//	//open file and setup server and client data.
//	void	ParseFile();
//	//please call this at right time
//	void	Connect();
//	//if failed reconnect
//	void	Update(float e_fElpaseTime);
//	void	Render();
//	using	cNetwork::SendData;
//	bool	SendData(sGamePacket*e_pGamePacket);
//	bool	IsServer(){ return m_bServer; }
//	bool	IsJPRequestAccept();
//};
//

#endif