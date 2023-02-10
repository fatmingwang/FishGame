#ifndef _SERIAL_CTRL_H
#define _SERIAL_CTRL_H
#ifdef _WIN32
#include "../../Core/NetWork/Network.h"
/**************************************************************************/
/* Title: CSerialIO: a useful and simple serial control                   */
/* Author: wufengkai                                                      */
/* Email:  tojine@hotmail.com                                             */

class cSerialCtrl
{
	std::wstring	m_strDebugString;
private:
	BOOL m_portStatus;                    
	HANDLE m_portHandle;                  
	DCB m_portConfig;   
public:
	cSerialCtrl();
	virtual ~cSerialCtrl();             
	void SetPortStatus(BOOL bOnOff);
	BOOL GetPortStatus();
	HANDLE GetPortHandle();
	BOOL OpenPort(DCB dcb, const WCHAR * portName=L"COM1");
	BOOL OpenPort(const WCHAR * baudRate=L"115200", const WCHAR * portName=L"COM1");
	BOOL Read(char * inputData, const unsigned int & sizeBuffer, unsigned long & length);
	BOOL Write(const char *outputData, const unsigned int & sizeBuffer, unsigned long & length);
	BOOL ClosePort();
	void	DebugRender();
};

class CSerialIO;

class cSerialThread:public cFUThreadPool
{
	//safe time to write
	float		m_fTimeToWrite;
	float		m_fTargetTimeToWrite;
	bool		m_bSafeToDoSingnal;
public:
	//
	cSerialThread ();               // Constructor.
	virtual ~cSerialThread();       // Destructor.
	virtual	void Clear();
	void ClosePort();
	virtual BOOL InitInstance();   // virtual function that derive from base-class.
	virtual int Run();             // virtual function that derive from base-class.  
	void setOwner(CSerialIO* serialIO)
	{
		m_serialIO = serialIO;
	}   

	static cSerialCtrl& serialCtrl() // Implement as the singleton funciton.
	{
		static cSerialCtrl serial;
		return serial;
	}
	bool		IsSafeToDoSingnal(){return  m_bSafeToDoSingnal;}
	void		SetSafeToDoSingnal(bool e_b){m_bSafeToDoSingnal = e_b;}
private:
	CSerialIO*  m_serialIO;         // The pointer that pointer to object user data, such dialog,window

};

#define MAX_SEND_BUFFER 1024

class CSerialIO
{

public:
	CSerialIO();
	virtual ~CSerialIO();

    void			OpenPort(WCHAR*strPortName,WCHAR*strBaudRate);	//open serial port with parameter name and baud rate
	virtual void	OnEventOpen(BOOL bSuccess);						//handle the event whether the port is successfully opened 
	void			ClosePort();									//close serial port
	virtual void	OnEventClose(BOOL bSuccess);					//handle the event whether the port is successfully closed
	virtual void	OnEventRead(char *inPacket,int inLength);		// handle the received data from serial
	virtual void	OnEventWrite(int nWritten);						//return how many bytes has been written
	void			Write(char *outPacket,int outLength);			// write data directly 

	BOOL			GetSendActivateValue(); 
	void			SetSendActivate(BOOL bSendActivate);
	BOOL			GetPortActivateValue();
	void			SetPortActivate(BOOL bPortActivate);
	BOOL			GetProcessActivateValue();
	void			SetProcessActivate(BOOL bProcessActivate);
	DCB				m_DCB;
	char			m_sendBuffer[MAX_SEND_BUFFER];
	unsigned int	m_sendSize;
	std::wstring	m_strPortName;
	BOOL			m_bClosePort;
	bool			IsSafeToDoSingnal(){return  m_serialProcess->IsSafeToDoSingnal();}
	cSerialThread* m_serialProcess;
private:
	BOOL Init();
	void UnInit();
	BOOL m_bProccessActivate;
	BOOL m_bPortActivate;
	BOOL m_bSendActivate;
	std::wstring	m_strBaudRate;
};
#endif//end win32
#endif//end _SERIAL_CTRL_H