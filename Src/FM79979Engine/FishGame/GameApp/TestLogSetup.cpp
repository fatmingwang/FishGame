#include "stdafx.h"
#include "TestLogSetup.h"

cTestLogFile::cTestLogFile()
{
	m_iSkipFrame = 0;
	m_iSkipFrameCount = 0;
	m_bSkipFrame = false;
	m_WriteLogTC.SetTargetTime(60);
	m_WriteLogTC.SetLoop(false);
	m_bWriteLog = false;
}

cTestLogFile::~cTestLogFile()
{

}
//<Root>
//	<ScreenSkip On="1" 	Count="10" />
//
//	<Log	On="1" 		WriteTime="10" />
//</Root>
void	cTestLogFile::ProcessScreenSkipData(TiXmlElement*e_pTiXmlElement)
{
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("On")
		{
			m_bSkipFrame = VALUE_TO_BOOLEAN;
		}
		else
		COMPARE_NAME("Count")
		{
			m_iSkipFrame = VALUE_TO_INT;
		}
	PARSE_NAME_VALUE_END
}

void	cTestLogFile::ProcessLogData(TiXmlElement*e_pTiXmlElement)
{
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("On")
		{
			m_bWriteLog = VALUE_TO_BOOLEAN;
		}
		else
		COMPARE_NAME("WriteTime")
		{
			m_WriteLogTC.SetLoop(true);
			m_WriteLogTC.SetTargetTime(VALUE_TO_FLOAT);
		}
	PARSE_NAME_VALUE_END
}

void	cTestLogFile::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	COMPARE_VALUE("ScreenSkip")
	{
		ProcessScreenSkipData(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("Log")
	{
		ProcessLogData(e_pTiXmlElement);
	}
//<Root>
//	<ScreenSkip On="1" 	Count="10" />
//
//	<Log	On="1" 		WriteTime="10" />
//</Root>
}

void	cTestLogFile::Init()
{	
	this->Parse(TEST_LOG_FILE);
}

bool	cTestLogFile::IsSkipFrame()
{
	return m_bSkipFrame;
	if( !m_bSkipFrame )
		return false;

	if(this->m_iSkipFrameCount == 0)
		return false;
	return false;
}

bool	cTestLogFile::IsWriteFile()
{
	if( !m_bWriteLog )
		return false;
	return m_WriteLogTC.bTragetTimrReached;
}

void	cTestLogFile::Update(float e_fElpaseTime)
{
	//if(this->m_bSkipFrame)
	//{
	//	++m_iSkipFrameCount;
	//	if( this->m_iSkipFrameCount >= this->m_iSkipFrame )
	//	{
	//		m_iSkipFrameCount = 0;
	//	}
	//}
	if( this->m_bWriteLog )
	{
		m_WriteLogTC.Update(e_fElpaseTime);
	}
}

void	cTestLogFile::SkipClear()
{
	m_iSkipFrameCount = m_iSkipFrame-1;
}