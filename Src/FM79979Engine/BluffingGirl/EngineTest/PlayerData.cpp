#include "stdafx.h"
#include "PlayerData.h"


cPlayerData::cPlayerData(char*e_strPlayerDataFileName,char*e_strPlayerSaveFileName)
{
	m_iNumGlasses = 0;
	m_iNumHearts = 0;
	m_LastTimeGetHeart.iYear;
	if(m_ItemFile.Openfile(e_strFileName))
	{
		ParsePlayerSaveFile();
	}
}

cPlayerData::~cPlayerData()
{

}

void	cPlayerData::ParsePlayerSaveFile()
{
	void*l_pData = m_ItemFile.GetDataFile(0);
	if( l_pData )
	{
	
	}
}
//date,UINT
//num glasses int
//num heart int
void	cPlayerData::Write()
{

}