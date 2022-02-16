#ifndef _cLiSchoolMasterSerialIO_H_
#define _cLiSchoolMasterSerialIO_H_
#include "SerialIOBase.h"
struct sSendGameStartPacket;


class	cSerialIO_MasterLee : public cSerialIOBase
{
	bool					IsDataMatch();
private:
	friend class cSerialThread;
	//
	sSendGameStartPacket	*m_pSendPacket_GameStart;
	bool					m_bControlPaneStartDataSend;
	static cBinaryFile*		m_spErrorLogFile;
	//
	void					ProcessFishDied( char *e_szPacket, int e_nLength );
	void					ProcessFishLuckyRoulette( char *e_szPacket, int e_nLength );
	void					ProcessKey( char *e_szPacket, int e_nLength );
	//
public:
	//
	static char				m_sstrReSendPacket[ MAX_SEND_BUFFER ];
	static int				m_siReSendSize;
	//
	static bool				m_sbWriteLog;
	static bool				m_sbWriteErrorLog;
	static int				m_siCom;
	static float			m_sfTimeOut;
	static bool				m_sbReceiveStartSingnal;	//related to m_pSendPacket_GameStart
	static float			m_sfComportIOTimeOutError;
	//
	cSerialIO_MasterLee();
	virtual ~cSerialIO_MasterLee();
	//
	//send control panel data too.
	virtual void			OnEventOpen( bool e_bSuccess );
	virtual void			OnEventClose( bool e_bSuccess );
	virtual void			OnEventRead( char *e_szPacket, int e_nLength );
	virtual void			OnEventWrite( int e_nWritten );
	//
	bool					ReSendLastPacket();
	//
	static void				WriteLog( const char *e_szError, bool e_bWriteToErrorLogFile );
	static void				WriteComportDataLog( const char *e_pData, int e_iLength, char *e_strDescription,cBinaryFile*e_pcBinaryFile );
	
};
//

void	SendEmptyPacket( cSerialIO_MasterLee *e_pLiSchoolMasterSerialIO );

#endif