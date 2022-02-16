#include "../stdafx.h"
#include "ComportFishData.h"
#include "../FishProbability.h"
#include "../../Bullet/Bullet.h"
#include "../../PlayerBehavior/PlayerData.h"
#include "../../PlayerBehavior/PlayerData.h"
#include "../../GameApp/GameApp.h"
#include "../../Monster/MonsterManager.h"
//







sSendDataWithMonster::sSendDataWithMonster()
{

}








sSendDataWithMonster::~sSendDataWithMonster()
{
	
	DELETE_VECTOR( m_CompareDataList, sCompareData_MasterLee* );
}
//
void	sSendDataWithMonster::Clear( int e_iCount1 )
{
	//DELETE_VECTOR( m_CompareDataList, sCompareData_MasterLee* );
	int	l_i = (int)m_CompareDataList.size()-e_iCount1;
	for( int ii=0; ii<e_iCount1; ii++ )
	{
		sCompareData_MasterLee *l_pCompareData_MasterLee = m_CompareDataList[0];
		delete l_pCompareData_MasterLee;
		m_CompareDataList.erase(m_CompareDataList.begin());
	}
	//memset( &m_SendData, 0, sizeof(sSendFishData) );
	m_SendData.cHeader = 0x55;
	m_SendData.iNumFishHitted = l_i;
	for( int ii=0; ii<l_i; ii++ )
	{
		m_SendData.HittedFish[ii] = m_SendData.HittedFish[ii+e_iCount1];
	}
}

void	sSendDataWithMonster::Copy( sSendDataWithMonster *e_pSendDataWithMonster_MasterLee, int e_iCount1 )
{
	Clear( m_CompareDataList.size() );
	//
	m_SendData = e_pSendDataWithMonster_MasterLee->m_SendData;
	if( m_SendData.iNumFishHitted > g_iNumFishesCouldBeSend )
	{
		m_SendData.iNumFishHitted = g_iNumFishesCouldBeSend;
	}
	//for( size_t i=0;i<e_pSendDataWithMonster_MasterLee->ComparePlayer.size();++i )
	for( int i=0; i<e_iCount1; ++i )
	{
		sCompareData_MasterLee*l_pCompareData = new sCompareData_MasterLee( e_pSendDataWithMonster_MasterLee->m_CompareDataList[i] );
		m_CompareDataList.push_back( l_pCompareData );
	}
}





//============================================
//
//============================================
sCompareData_MasterLee::sCompareData_MasterLee(cMonster*e_pMonster,cPlayerBehaviorBase*e_pPlayerBehaviorBase,cBullet*e_pBullet,const WCHAR*e_strMinigameName)
{
	m_pMonster = e_pMonster;
	m_pPlayerBehaviorBase = e_pPlayerBehaviorBase;
	m_pBullet = e_pBullet;
	m_pstrMiniGameName = 0;
	if( e_strMinigameName )
	{
		m_pstrMiniGameName = new std::wstring;
		*m_pstrMiniGameName = e_strMinigameName;
	}
}








sCompareData_MasterLee::sCompareData_MasterLee( sCompareData_MasterLee *e_pCompareData )
{
	m_pstrMiniGameName = 0;
	if( e_pCompareData->m_pstrMiniGameName )
	{
		m_pstrMiniGameName = new std::wstring;
		*m_pstrMiniGameName = *e_pCompareData->m_pstrMiniGameName;
	}
	m_pBullet = e_pCompareData->m_pBullet;
	m_pMonster = e_pCompareData->m_pMonster;
	m_pPlayerBehaviorBase = e_pCompareData->m_pPlayerBehaviorBase;
	for( size_t i=0; i<e_pCompareData->m_MiniGameRelatedMonsterVector.size(); ++i )
	{
		m_MiniGameRelatedMonsterVector.push_back( e_pCompareData->m_MiniGameRelatedMonsterVector[i] );
	}
}








sCompareData_MasterLee::~sCompareData_MasterLee()
{
	SAFE_DELETE(m_pstrMiniGameName);
}