#ifndef _IO8051_SETUP_H_
#define _IO8051_SETUP_H_

#define	USING_GH8051

#define	NEW_8051_PROTOCOL

#define	REFRESH_8051_IO_TIME_OFFSET				0.016f

#define	MAX_COM_PORT_COUNT						1

#define	COM_PORT_SUPPORT_PLAYER_COUNT			10

#ifdef	NEW_8051_PROTOCOL
//receve
	#define	IO_8051_BYTE_COUNT						66
#else
	#define	IO_8051_BYTE_COUNT						56
#endif
//common send for receving
#define	COMMON_SEND_FOR_RECEVE_BYTE_COUNT		15
//send,coin
#define	REFUND_SINGNAL_BYTE_COUNT				25
//coin not enough reset byte count.
#define	RESET_REFUND_SINGNAL_BYTE_COUNT			15


#define	ALL_COM_PORT_BYTE_COUNT					MAX_COM_PORT_COUNT*IO_8051_BYTE_COUNT
#define	BIT_COUNT								8


#define	CHECK_SUM_COUNT							3
#define	RESET_CHECK_SUM							RESET_REFUND_SINGNAL_BYTE_COUNT-CHECK_SUM_COUNT
class	cFUThreadPool;
class cIO8051Setup
{
	cFUThreadPool		*m_pThread;

	UT::sTimeCounter	m_NoIOSingnalTimeout;
	//debug info
	WCHAR			m_strError[MAX_PATH];
	WCHAR			m_strReceiveDebug[MAX_PATH];
	//
	HANDLE			m_pFileHandler[MAX_COM_PORT_COUNT];
	//
	int				m_iNumComport;
	int				m_iIOstate[MAX_COM_PORT_COUNT];//state machine to control IO situation
	int				m_iBoardSelect;
	//for send reset singnal() or refund singnal
	int				m_iRefundSingnalSendDataSize;
	//
	WCHAR			m_strExchangeIndex[MAX_PATH];//Index SWAP
	float			m_fSeconds;
	//
	HANDLE			OpenComPort(const WCHAR*e_strComPortName,DCB*e_pDCB);
	bool			IsCommStateOk(DCB*e_pDCB,HANDLE e_Handle);
	void			InitXML(const char *filename);
	//data for receive
	BOOL 			m_bReadBit[MAX_COM_PORT_COUNT][ALL_COM_PORT_BYTE_COUNT*BIT_COUNT];
	BYTE			m_cDataBytesReceive[MAX_COM_PORT_COUNT][IO_8051_BYTE_COUNT];
	//data for send
	BYTE			m_cRefundDeviceSingnalData[REFUND_SINGNAL_BYTE_COUNT];
	//vector<int>		m_ExchangeIndexVector;
	//
	void			ErrorProcess(HANDLE e_Handle);
	//
	void			SingnalProcess(float e_fElpaseTime);
	bool			RefundSingnalSending();
	void			WorkingBordChange();
	bool			m_bIOError;
	DCB				m_DCB[MAX_COM_PORT_COUNT];
public:
	cIO8051Setup();
	virtual ~cIO8051Setup();
	//
	void				Init(bool e_bResetCoin);
	void				DoUpdate();
	bool				ReadInputData(int e_iAddress,bool*e_pbData);
	bool				ReadIOChar(int e_iAddress,char*e_pcValue);
	bool				WriteIOChar(int e_iAddress,char e_cValue);
	bool				WriteIOBit(int e_iAddress,bool e_bValue);
	virtual void		Update(float e_fElpaseTime);
	virtual void		Render();
	bool				IsIOError(){return m_bIOError;}
	
};
#endif