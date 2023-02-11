#ifndef _SERIAL_IO_BASE_H
#define _SERIAL_IO_BASE_H

/**************************************************************************/
/* Title: cSerialIOBase: a useful and simple serial control              */
/* Author: wufengkai                                                      */
/* Email:  tojine@hotmail.com                                             */

#ifdef _WIN32

class cSerialThread;
class cSerialIOBase
{
private:
	bool			m_bProccessActivate;
	bool			m_bPortActivate;
	bool			m_bSendActivate;
	std::wstring	m_strBaudRate;
	//
	bool Init();
	void UnInit();
	//
public:
	DCB				m_DCB;
	char			m_sendBuffer[MAX_SEND_BUFFER];
	unsigned int	m_sendSize;
	std::wstring	m_strPortName;
	bool			m_bClosePort;
	//
	cSerialIOBase();
	virtual ~cSerialIOBase();
	//
    void			OpenPort( WCHAR *strPortName, WCHAR *strBaudRate );	//open serial port with parameter name and baud rate
	virtual void	OnEventOpen( bool bSuccess );						//handle the event whether the port is successfully opened 
	void			ClosePort();										//close serial port
	virtual void	OnEventClose( bool bSuccess );						//handle the event whether the port is successfully closed
	virtual void	OnEventRead( char *inPacket, int inLength );		// handle the received data from serial
	virtual void	OnEventWrite( int nWritten );						//return how many bytes has been written
	void			Write( char *outPacket, int outLength );			// write data directly 

	bool			GetSendActivateValue(); 
	void			SetSendActivate( bool bSendActivate );
	bool			GetPortActivateValue( void );
	void			SetPortActivate( bool bPortActivate );
	bool			GetProcessActivateValue( void );
	void			SetProcessActivate( bool bProcessActivate );

	cSerialThread	*m_pSerialThread;
};

class cSerialCtrl
{
protected:
	std::wstring	m_strDebugString;
private:
	bool m_portStatus;                    
	HANDLE m_portHandle;                  
	DCB m_portConfig; 
	//
public:
	cSerialCtrl();
	virtual ~cSerialCtrl();             
	void	SetPortStatus(bool bOnOff);
	bool	GetPortStatus();
	HANDLE	GetPortHandle();
	bool	OpenPort(DCB dcb, const WCHAR * portName=L"COM1");
	bool	OpenPort(const WCHAR * baudRate=L"115200", const WCHAR * portName=L"COM1");
	bool	Read(char * inputData, const unsigned int & sizeBuffer, unsigned long & length);
	bool	Write(const char *outputData, const unsigned int & sizeBuffer, unsigned long & length);
	bool	ClosePort();
	void	DebugRender();
};

#endif//end win32
#endif//end _SERIAL_CTRL_H