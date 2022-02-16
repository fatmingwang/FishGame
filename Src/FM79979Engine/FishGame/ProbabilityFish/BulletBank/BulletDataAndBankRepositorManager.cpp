#include "../stdafx.h"
#include "BulletDataAndBankRepositorManager.h"
#include "BankFormat.h"
#include "BankRunner.h"
#include "../../Bullet/Bullet.h"
#include "../../Monster/MonsterManager.h"
#include "../../GameApp/GameApp.h"
#include "../../GameApp/FileNameDefine.h"
#include "../../PlayerBehavior/PlayerData.h"
#include "../FishProbability.h"
#include "../../MiniGame/MiniGameManager.h"

int		g_iEasyDiedCount = 1;
#ifdef DEBUG
bool	g_bEasyMonsterDied = true;
#else
bool	g_bEasyMonsterDied = false;
#endif

cBulletDataAndBankRepositorManager::cBulletDataAndBankRepositorManager()
{
	m_pSubRepositoryBankManager = new cNamedTypedObjectVector<cBankRunnerManager>;
	for( int i=0;i<cFishApp::m_spControlPanel->m_iPlayerCount;++i )
	{
		cBankRunnerManager*l_pBankRunnerManager = new cBankRunnerManager();
		m_pSubRepositoryBankManager->AddObjectNeglectExist( l_pBankRunnerManager );
	}
	//m_BulletFireAverageVector;
	m_i64TotalBulletValue = 0;
	m_iBulletCount = 0;
	m_i64Aeverage = 0;
	m_iSequenceEmptyShot = 0;
}








cBulletDataAndBankRepositorManager::~cBulletDataAndBankRepositorManager()
{
	SAFE_DELETE(m_pSubRepositoryBankManager);
}








void	cBulletDataAndBankRepositorManager::Init()
{
	int	l_iCount = m_pSubRepositoryBankManager->Count();
	for( int i=0;i<l_iCount;++i )
	{
		(*m_pSubRepositoryBankManager)[i]->Destroy();
		if(!(*m_pSubRepositoryBankManager)[i]->ParseWithMyParse(BANK_FORMAT_PROBABILITY_FILE))
		{
			UT::ErrorMsg(BANK_FORMAT_PROBABILITY_FILE,"parse failed");
		}
	}
	m_iSequenceEmptyShot = 0;
}




void	cBulletDataAndBankRepositorManager::Update_HitMonsterForBankFormat()
{
	//
	for( int i=0; i<cFishApp::m_spMonsterManager->m_iNumMonsterHitted; ++i )
	{
		sHittedFishData*l_pHittedFishData = &cFishApp::m_spMonsterManager->m_pHittedFish[i];
		if( !l_pHittedFishData->m_pMonster->IsStatusAllowToDied() )
		{
			continue;
		} 
		bool	l_bFishDied = false;
		if( m_iSequenceEmptyShot > 12*cFishApp::m_spControlPanel->m_iPlayerCount )
		{
			if( cFishApp::m_spMonsterManager->m_iNumMonsterAlive > 0 )
			{
				if( !l_pHittedFishData->m_pMonster->IsMiniGameMonster() )
				{
					int	l_iOutMoney = l_pHittedFishData->m_pMonster->GetPrize()*l_pHittedFishData->m_pBullet->GetPayRate()*l_pHittedFishData->m_pPlayer->GetPayRateMultier();
					int64	l_iExtraRevenue = GetExtractRevenueValue( l_iOutMoney,l_pHittedFishData->m_pBullet->GetPayRate() );
					if( l_iExtraRevenue >= l_iOutMoney )
					{
						bool l_bResult = cFishApp::m_spProbabilityFish->MonsterToDied( l_pHittedFishData, l_iOutMoney, false, false );
						if( l_bResult )
						{
							m_iSequenceEmptyShot = 0;
						} 
						continue;
					} 
				} 
			} 
		} 

		if( cFishApp::m_spControlPanel->m_iPlayerCount != 1 )
		{
			if( !IsPayRateLegalToWinBigMoney( l_pHittedFishData->m_pMonster->GetPrize()*l_pHittedFishData->m_pPlayer->GetPayRateMultier(), l_pHittedFishData->m_pBullet->GetPayRate()) )
			{
				++m_iSequenceEmptyShot;
				continue;
			} 
		} 

		int		l_iOutMoney = 0;
		float	l_fAddProbability = 0.f;
		l_bFishDied = DoProbailityWithTotalRevenueForBankFormat( l_pHittedFishData, &l_fAddProbability, &l_iOutMoney );
		if( g_bEasyMonsterDied && rand() % g_iEasyDiedCount == 0 )
			l_bFishDied = true;
		if( l_bFishDied )
		{
			bool l_bResult = cFishApp::m_spProbabilityFish->MonsterToDied( l_pHittedFishData, l_iOutMoney, false, false );
			if( l_bResult )
			{
				m_iSequenceEmptyShot = 0;
			} 
		}
		else
		{
			cFishApp::m_spProbabilityFish->MonsterToKeepGoing( l_pHittedFishData, l_fAddProbability );
			++m_iSequenceEmptyShot;
		} 
	}
	//
	cFishApp::m_spMonsterManager->m_iNumMonsterHitted = 0;
}



float	cBulletDataAndBankRepositorManager::GetBulletPayRateLimitOffsetProbability(int e_iPayrate)
{
	float	l_fValue = (float)e_iPayrate/cFishApp::m_spControlPanel->m_iBulletPayRateLimit;
	if( l_fValue < 0.1f )
	{
		return 0.1f;
	} 
	return l_fValue;
}








int64 cBulletDataAndBankRepositorManager::GetExtractRevenueValue(int e_iOutMoney,int e_iPayRate)
{
	if( e_iPayRate == 0 )
	{
		return 0;
	} 
	int	l_iPrize = e_iOutMoney/e_iPayRate;
	int	l_iOffsetPrize = l_iPrize += 5;	
	const	int	l_iMaxPrize = 100;
	if( l_iOffsetPrize > l_iMaxPrize )
	{
		l_iOffsetPrize = l_iMaxPrize;
	} 
	if( frand(0,1) > 1.f/l_iOffsetPrize )
	{
		return 0;
	} 
	float	l_fTargetProbability = cFishApp::m_spProbabilityFish->GetTargetProbability();
	float	l_fProbability = cFishApp::m_spProbabilityFish->GetCurrentRealProbability();
	float	l_fOffsetProbability = cFishApp::m_spProbabilityFish->GetOffsetProbability();
	if( l_fProbability > 0.f && l_fOffsetProbability > cFishApp::m_spProbabilityFish->m_fSafeToGiveProbabilityOffset )
	{
		const	int	l_iProbabilityToFetch = (int)(l_fOffsetProbability*100);
		float	l_fProbabilityByPayRate = GetBulletPayRateLimitOffsetProbability( e_iPayRate );
		l_fProbabilityByPayRate /= 2.f;
		const	float	l_fMiniProbabilityToFetch = 0.03f;
		const	float	l_fMaxiProbabilityToFetch = 0.2f;
		if( l_fProbabilityByPayRate < l_fMiniProbabilityToFetch )	l_fProbabilityByPayRate = l_fMiniProbabilityToFetch;
		if( l_fMaxiProbabilityToFetch > l_fMaxiProbabilityToFetch )	l_fProbabilityByPayRate = l_fMaxiProbabilityToFetch;
		if( frand(0,1) < l_fProbabilityByPayRate )
		{
			return e_iOutMoney;
		} 
	} 
	int64	l_i64Revenue = *(cFishApp::m_spProbabilityFish->m_pi64LatestRevenue);
	//1 second 3 shot,60second for 1minute,8 people,assume 8hours for open time,14days to report money.
	int64	l_iMaxRevenueForNormalShoot = (int64)(cFishApp::m_spControlPanel->m_iBulletPayRateLimit*3*60*8*8*14*(double)(1-l_fTargetProbability));
	if( l_i64Revenue > l_iMaxRevenueForNormalShoot )
	{
		return e_iOutMoney;
	} 
	return e_iOutMoney;
}








bool cBulletDataAndBankRepositorManager::IsPayRateLegalToWinBigMoney( int e_iPrize, int e_iPayRate )
{
	//if it is small payrate it should give a extra revenue benfit while time is reached
	static	bool	l_sbGiveForMinShot = false;
	static	float	l_sElpaseTime = 0.f;
	const float	l_Time = 1800.f;
	l_sElpaseTime += cGameApp::m_sTimeAndFPS.fElpaseTime;
	//30 minutes
	if( l_sElpaseTime >= l_Time )
	{
		l_sElpaseTime = 0.f;
		l_sbGiveForMinShot = true;
	} 
	//avoid small bullet shoot big prize
	const int	l_iSmallDivide = 20;
	int	l_iMaxScore = cFishApp::m_spControlPanel->m_iBulletPayRateLimit;
	l_iMaxScore = l_iMaxScore/l_iSmallDivide;
	const int	l_iBigPrize = 100;
	if( e_iPrize >= l_iBigPrize && e_iPayRate < l_iMaxScore )
	{
		if( !l_sbGiveForMinShot )
		{
			return false;
		} 
		l_sbGiveForMinShot = false;
	} 
	return true;
}








bool	cBulletDataAndBankRepositorManager::DoProbailityWithTotalRevenueForBankFormat( sHittedFishData *e_pHittedFishData, float *e_pfAddProbability, int *e_piOutMoney )
{
	if( e_pHittedFishData->m_pMonster->IsMiniGameMonster() )
	{
		int	l_iPayRate = e_pHittedFishData->m_pBullet->GetPayRate();
		int	l_iLeastOutPrize = 10;
		int	l_iMiniGamePayrate = cFishApp::m_spMiniGameManager->GetMiniGamePayRate(e_pHittedFishData->m_pMonster->m_strMiniGameName.c_str());
		int	l_iOutMoney = l_iMiniGamePayrate*l_iPayRate;
		int64	l_i64ExtraMoney = GetExtractRevenueValue( l_iOutMoney,l_iPayRate);
		if( l_i64ExtraMoney < l_iOutMoney  )
			return false;
		//if( l_iMiniGameIndex == 3 )
		{//this mini game ask least prize is 50% of screen's monster
			//l_iLeastOutPrize = cFishApp::m_spMonsterManager->m_iTotalAliveMonsterPrize/2;
		}
		float	l_fProbability = GetBulletPayRateLimitOffsetProbability( l_iPayRate );
		if( frand(0,1) <= l_fProbability )
		{
			return true;
		}  
		return false;
	}
	else
	{
		sHittedFishData *l_pHittedFishData = e_pHittedFishData;
		int	l_iOutMoney = l_pHittedFishData->m_pMonster->GetPrize() * l_pHittedFishData->m_pBullet->GetPayRate() * l_pHittedFishData->m_pPlayer->GetPayRateMultier();
		int	l_iPayRate = l_pHittedFishData->m_pBullet->GetPayRate();
		*e_piOutMoney = l_iOutMoney;
		
		//check bite money and out money.
		cBankRunner *l_pSubRepositoryBank = GetSubRepositoryBankByPayRate( l_iPayRate, e_pHittedFishData->m_pPlayer->GetPlayerID() );
		if( l_pHittedFishData->m_pMonster->GetTotalBiteMoney() >= l_iOutMoney)
		{
			//if bite money is enough and check happy revenue,then give a random chance to give money
			if( l_pSubRepositoryBank->IsFakeMoneyEnoughToPay(l_iOutMoney) )
			{
				return true;
			} 
		} 
		
		if( l_pSubRepositoryBank->IsMoneyOutOk( l_iOutMoney, l_iPayRate ) )
		{
			return true;
		} 
	}
	return false;
}








void	cBulletDataAndBankRepositorManager::RevenueMinus( int e_iPayRate,int e_iMoney,int e_iPlayerID,bool e_bMiniGame )
{
	RevenueMinus( e_iMoney, e_iPayRate, e_iPlayerID, e_iPayRate );
}



int		cBulletDataAndBankRepositorManager::GetBulletAeverageFormatIndex(int e_iValue,int e_iPlayerID)
{
	float	l_fPercentage = (float)(e_iValue/(double)m_i64Aeverage);
	if( l_fPercentage> 1.f )
		l_fPercentage = 1.f;
	//it doesn't matter 0 or else ID because each player's data is same
	int		l_iIndex = (*this->m_pSubRepositoryBankManager)[e_iPlayerID]->GetIndexByPerenctage(l_fPercentage,e_iValue,e_iPlayerID);
	assert( l_iIndex != -1 && "this bank format index is not legal");
	return l_iIndex;
}








float	cBulletDataAndBankRepositorManager::GetBulletAeverageOffsetPercentage(int e_iValue)
{
	return 0.f;
}

//







void	cBulletDataAndBankRepositorManager::RevenueAdd(int e_iPayRate,int e_iPlayerID,int e_iRealPayRate)
{
	m_i64TotalBulletValue += e_iPayRate;
	++m_iBulletCount;
	m_i64Aeverage = m_i64TotalBulletValue/m_iBulletCount;
	int		l_iIndex = GetBulletAeverageFormatIndex(e_iRealPayRate,e_iPlayerID);
	if( l_iIndex != -1 )
	{
		cBankRunner*l_pSubRepositoryBank = (*m_pSubRepositoryBankManager)[e_iPlayerID]->GetObject(l_iIndex);
		l_pSubRepositoryBank->AddRevenue(e_iPayRate);
	}
}








void	cBulletDataAndBankRepositorManager::RevenueMinus(int e_iOutMoney,int e_iPayRate,int e_iPlayerID,int e_iRealPayRate)
{
	int		l_iIndex = GetBulletAeverageFormatIndex(e_iRealPayRate,e_iPlayerID);
	if( l_iIndex != -1 )
	{
		cBankRunner *l_pSubRepositoryBank = (*m_pSubRepositoryBankManager)[e_iPlayerID]->GetObject(l_iIndex);
		l_pSubRepositoryBank->MinusRevenue(e_iOutMoney);
	}
}








bool	cBulletDataAndBankRepositorManager::IsMoneyOutOk(int e_iOutMoney,int e_iPayRate,int e_iPlayerID)
{
	int		l_iIndex = GetBulletAeverageFormatIndex(e_iPayRate,e_iPlayerID);
	if( l_iIndex != -1 )
	{
		cBankRunner*l_pSubRepositoryBank = (*m_pSubRepositoryBankManager)[e_iPlayerID]->GetObject(l_iIndex);
		return l_pSubRepositoryBank->IsMoneyOutOk(e_iOutMoney,e_iPayRate);
	}
	return false;
}








void	cBulletDataAndBankRepositorManager::Update(float e_fElpaseTime)
{
	int	l_iCount = m_pSubRepositoryBankManager->Count();
	for( int i=0;i<l_iCount;++i )
		(*this->m_pSubRepositoryBankManager)[i]->Update(e_fElpaseTime);
	Update_HitMonsterForBankFormat();
}








cBankRunner*	cBulletDataAndBankRepositorManager::GetSubRepositoryBankByPayRate(int e_iPayRate,int e_iPlayerID)
{
	int		l_iIndex = GetBulletAeverageFormatIndex(e_iPayRate,e_iPlayerID);
	if( l_iIndex != -1 )
	{
		return (*m_pSubRepositoryBankManager)[e_iPlayerID]->GetObject(l_iIndex);
	}
	return 0;
}








void	cBulletDataAndBankRepositorManager::DebugRender()
{
	int	l_iCount = m_pSubRepositoryBankManager->Count();
	for( int i=0;i<l_iCount;++i )
		(*this->m_pSubRepositoryBankManager)[i]->DebugRender( 60, 420 + i*30 );
}








