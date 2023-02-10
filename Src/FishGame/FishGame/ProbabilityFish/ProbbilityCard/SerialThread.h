#ifndef _SERIAL_THREAD_H_
#define _SERIAL_THREAD_H_

#ifdef _WIN32
#include "../../../Core/NetWork/Network.h"


class cSerialIOBase;
class cSerialCtrl;

class cSerialThread : public cFUThreadPool
{
	static cSerialCtrl	*m_spSerialCtrl;
private:
	cSerialIOBase		*m_pSerialIO_Base;         // The pointer that pointer to object user data, such dialog,window
	//safe time to write
	float				m_fCurrentTimeToWrite;
	//
public:
	static float		m_sfTargetTimeToWrite;
	//
	cSerialThread ();						// Constructor.
	virtual ~cSerialThread();				// Destructor.
	//
	virtual	void		Clear();
	void				ClosePort();
	virtual bool		InitInstance();		// virtual function that derive from base-class.
	virtual int			Run();				// virtual function that derive from base-class.  
	//
	void setOwner( cSerialIOBase *e_pSerialIO_Base )
	{
		m_pSerialIO_Base = e_pSerialIO_Base;
	}   
	//
	static cSerialCtrl *serialCtrl() // Implement as the singleton funciton.
	{
		return m_spSerialCtrl;
	}
};

#endif//end win32
#endif//end _SERIAL_CTRL_H