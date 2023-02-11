#ifndef _TEST_LOG_SETUP
#define _TEST_LOG_SETUP

//<Root>
//	<ScreenSkip On="1" 	Count="10" />
//
//	<Log	On="1" 		WriteTime="10" />
//</Root>

class	cTestLogFile:public ISAXCallback
{
	void				ProcessScreenSkipData(TiXmlElement*e_pTiXmlElement);
	void				ProcessLogData(TiXmlElement*e_pTiXmlElement);
	virtual	void		HandleElementData(TiXmlElement*e_pTiXmlElement);
	//
	int					m_iSkipFrame;
	bool				m_bSkipFrame;
	int					m_iSkipFrameCount;
	//
	UT::sTimeCounter	m_WriteLogTC;
	bool				m_bWriteLog;
public:
	cTestLogFile();
	virtual ~cTestLogFile();
	void				Init();
	bool				IsSkipFrame();
	bool				IsWriteFile();
	void				Update(float e_fElpaseTime);
	int					GetSkipFrame(){ return m_iSkipFrame; }
	void				SkipClear();
	bool				IsWriteLog(){return m_bWriteLog;}
};

#endif