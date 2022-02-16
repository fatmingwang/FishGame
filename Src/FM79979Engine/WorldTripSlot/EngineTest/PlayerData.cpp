#include "stdafx.h"
#include "PlayerData.h"
#include "GameNetwork.h"
#include "SlotGamePhase.h"


eJPType	GetJPType(const WCHAR*e_strName)
{
	if( !wcscmp(e_strName,L"NetWork") )
	{
		return 	eJPT_NETWORK;
	}
	else
	if( !wcscmp(e_strName,L"Local") )
	{
		return 	eJPT_LOCAL;
	}
	return eJPT_MAX;
}

cPlayerData::cPlayerData()
{
	m_iMaxJPMoney = 99999;
	m_iNetWorkJPMoney = m_iPervoisNetWorkJPMoney = 0;
	m_iLocalJPMoney = 0;
	m_iNetWorkJPMoney = 9999;
	m_pJPFile = 0;
	m_pMoneyFile = 0;
	OpenPlayerMoney();
}

void	cPlayerData::ClosePlayerMoney()
{
	SAFE_DELETE(m_pMoneyFile);
}

void	cPlayerData::OpenPlayerMoney()
{
	if(!m_pMoneyFile)
		m_pMoneyFile = new cBinaryFile;
	char*l_strMoneyFileName = "PlayerMoney.txt";
	int	l_iMoney = 0;
	if(UT::IsFileExists(l_strMoneyFileName))
	{
		m_pMoneyFile->Openfile(l_strMoneyFileName);
		int	l_iFileSize = m_pMoneyFile->GetDataFileSize();
		if( l_iFileSize >8 || l_iFileSize < 4 )
		{
			int a=0;
		}
		else
		if( l_iFileSize>0 )
		{
			l_iMoney  = *(int*)m_pMoneyFile->GetDataFile(0);
			//it's not allow
			if( l_iMoney<0 )
				l_iMoney = 0;
		}
		m_pMoneyFile->CloseFile();
	}
	m_pMoneyFile->Writefile(l_strMoneyFileName,true,false);
	m_pMoneyFile->WriteToFile(l_iMoney);
	NvFile*l_pFile = m_pMoneyFile->GetFile();
	m_pMoneyFile->WriteToFile(m_i64Money);
	NvFFlush(l_pFile);

	m_i64Money = l_iMoney;
	m_iPervoisMoney = m_i64Money;
	m_TimeToSaveCurrentMoney.SetTargetTime(0.1f);
	m_TimeToSaveCurrentMoney.Start();
}

void	cPlayerData::OpenJP()
{
	char*l_strJPFileName = "JP.txt";
	int	l_iMoney  = 0;
	if( !m_pJPFile )
	{
		m_pJPFile = new cBinaryFile();
		UT::SystemErrorCheck();
		if(UT::IsFileExists(l_strJPFileName))
		{
			m_pJPFile->Openfile(l_strJPFileName);
			int	l_iFileSize = m_pJPFile->GetDataFileSize();
			l_iMoney  = *(int*)m_pJPFile->GetDataFile(0);
			//it's not allow
			if( l_iMoney>999999999 )
				l_iMoney = 0;
			m_pJPFile->CloseFile();
		}
	}
	m_pJPFile->Writefile(l_strJPFileName,true,false);
	m_pJPFile->WriteToFile(l_iMoney);
	NvFile*l_pFile = m_pJPFile->GetFile();
	m_pJPFile->WriteToFile(m_i64Money);
	NvFFlush(l_pFile);
	m_iNetWorkJPMoney = l_iMoney;
	m_iPervoisNetWorkJPMoney = m_iNetWorkJPMoney;
}

cPlayerData::~cPlayerData()
{
	SAFE_DELETE(m_pMoneyFile);
	SAFE_DELETE(m_pJPFile);
}

void	cPlayerData::Update(float e_fEplaseTime)
{
	if( m_i64Money != m_iPervoisMoney )
	{
		m_TimeToSaveCurrentMoney.Update(e_fEplaseTime);
		if(m_TimeToSaveCurrentMoney.bTragetTimrReached )
		{
			WritePlayerMoney();
		}
	}
}

void	cPlayerData::WritePlayerMoney()
{
	if( m_i64Money != m_iPervoisMoney )
	{
		if( m_pMoneyFile )
		{
			NvFile*l_pFile = m_pMoneyFile->GetFile();
			NvFSeek(l_pFile,0,SEEK_SET);
			m_pMoneyFile->WriteToFile(m_i64Money);
			NvFFlush(l_pFile);
			cGameApp::OutputDebugInfoString(UT::ComposeMsgByFormat("Current Money:%d\n",m_i64Money));
		}
		m_iPervoisMoney = m_i64Money;
	}
	m_TimeToSaveCurrentMoney.Start();
}

void	cPlayerData::WriteJP()
{
	if( m_iNetWorkJPMoney != m_iPervoisNetWorkJPMoney )
	{
		if( m_pJPFile )
		{
			NvFile*l_pFile = m_pJPFile->GetFile();
			NvFSeek(l_pFile,0,SEEK_SET);
			m_pJPFile->WriteToFile(m_iNetWorkJPMoney);
			NvFFlush(l_pFile);
			cGameApp::OutputDebugInfoString(UT::ComposeMsgByFormat("Current JP:%d\n",m_iNetWorkJPMoney));
		}
		m_iPervoisNetWorkJPMoney = m_iNetWorkJPMoney;
	}
}

int*	cPlayerData::GetJP(eJPType e_eJPType)
{
	switch(e_eJPType)
	{
		case eJPT_NETWORK:
			return &m_iNetWorkJPMoney;
			break;
		case eJPT_LOCAL:
			return &m_iLocalJPMoney;
			break;
	}
	return 0;
}

void	cPlayerData::SetJP(eJPType e_eJPType,int e_iValue)
{
	switch(e_eJPType)
	{
		case eJPT_NETWORK:
			m_iNetWorkJPMoney = e_iValue;
			if( m_iNetWorkJPMoney>m_iMaxJPMoney )
				m_iNetWorkJPMoney = m_iMaxJPMoney;
			break;
		case eJPT_LOCAL:
			m_iLocalJPMoney = e_iValue;
			break;
	}
}

int*	cPlayerData::GetJPOpenCount(eJPType e_eJPType)
{
	WCHAR*l_str = 0;
	if(e_eJPType == eJPT_LOCAL  )
	{
		l_str = LOCAL_JP_OPEN_COUNT_VARIABLE_NAME;
	}
	else
	if(e_eJPType == eJPT_NETWORK  )
	{
		l_str = NETWORK_JP_OPEN_COUNT_VARIABLE_NAME;
	}

	//cEventVariable*l_pJP = g_pSlotGamePhase->m_pControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(l_str);
	//if( l_pJP )
		//return l_pJP->GetInt();
	return 0;
}