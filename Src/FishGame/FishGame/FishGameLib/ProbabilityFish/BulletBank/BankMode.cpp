#include "stdafx.h"
#include "BankMode.h"
#include "../FishProbability.h"
#include "../../GameApp/GameApp.h"
#include "BankRunner.h"
#include "BulletDataAndBankRepositorManager.h"
#include "../../ControlPanel/ControlPanel.h"


cBankMode::cBankMode(float e_pTargetProbability)
{
	m_TargetProbability = e_pTargetProbability;
	m_i64CurrentIn = 0;
	m_i64CurrentOut = 0;
	m_i64Revenue = 0;
	m_i64TotalBulletValue = 0;
	m_i64Aeverage = 0;
	m_iBulletCount = 0;
	m_iNumRevenueAddAndNoRevenueMinus = 0;
}








cBankMode::cBankMode( TiXmlElement *e_pTiXmlElement )
{
	m_iNumRevenueAddAndNoRevenueMinus = 0;
	m_i64TotalBulletValue = 0;
	m_i64Aeverage = 0;
	m_iBulletCount = 0;

	m_TargetProbability = cFishApp::m_spControlPanel->m_RealProbility/100.f;
	m_i64CurrentIn = 0;
	m_i64CurrentOut = 0;
	m_i64Revenue = 0;
}

cBankMode::~cBankMode()
{

}

float	cBankMode::GetNewProbability(int e_iMoney)
{
	return (float)((double)(m_i64CurrentOut+e_iMoney)/(m_i64CurrentIn+1)); 
}

float	cBankMode::GetProbability()
{
	return (float)(double)m_i64CurrentOut/(m_i64CurrentIn+1); 
}

bool	cBankMode::IsMoneyOutOk(int e_iMoney,int e_iPayRate)
{
	float	l_fCompareProbability = frand(0,1);
	const int	l_iNegativeModeMultlpier = 1;
	int	l_iPrize = e_iMoney/e_iPayRate;
	if( l_iPrize < 5 )
		l_iPrize = 5;
	l_iPrize *= l_iNegativeModeMultlpier;
	float	l_fProbabilityToGive2 = 1.f/l_iPrize;
	//if( l_fCompareProbability<=l_fProbabilityToGive || l_fCompareProbability<=l_fProbabilityToGive2 )
	//if( l_fCompareProbability<=l_fProbabilityToGive )
	if( l_fCompareProbability <= l_fProbabilityToGive2 )
	{
		float	l_fProbability = GetNewProbability(e_iMoney);
		if( l_fProbability > 0.f && l_fProbability <= m_TargetProbability)
			return true;
	}
	//if prize is bigger than 20 check extra revenue money to give
	const int	l_iBigPrize = 10;
	if( l_iPrize >= l_iBigPrize || e_iMoney > 1500000 )
	{//while fire empty shot reach a limit give extra revenue
		const float	l_fNumShotNoRevenueToEasyFetchExtraRevenue = 10.f;
		float	l_fMultiplier = m_iNumRevenueAddAndNoRevenueMinus/l_fNumShotNoRevenueToEasyFetchExtraRevenue;
		if( l_fCompareProbability < l_fMultiplier )
		{
			cBankRunner*l_pBankRunner = (cBankRunner*)this->GetOwner();
			int64	l_i64Value = cFishApp::m_spProbabilityFish->GetBulletData()->GetExtractRevenueValue(e_iMoney,e_iPayRate);
			if( l_i64Value > e_iMoney )
			{//fetch from real revenue so ignore next revenue minus.
				this->m_i64Revenue += e_iMoney;
				if( m_i64CurrentOut > e_iMoney )
					m_i64CurrentOut -= e_iMoney;
				return true;
			}
		}
	}
	return false;
}

void	cBankMode::ModeAssign(cBankMode*e_pBankMode)
{
	//m_i64CurrentIn = e_pBankMode->m_i64CurrentIn;
	//m_i64CurrentOut = e_pBankMode->m_i64CurrentOut;
	m_i64CurrentIn = 0;
	m_i64CurrentOut = 0;
	m_i64Revenue = e_pBankMode->m_i64Revenue;
	if( m_i64Revenue > 0 )
	{//store win money
		m_i64Revenue = (int64)(m_i64Revenue*m_TargetProbability);
	}
	float	l_fProbability = cFishApp::m_spProbabilityFish->GetOffsetProbability();
	//if( l_fProbability > cFishApp::m_spProbabilityFish->m_fSafeToGiveProbabilityOffset/2    )
	if( l_fProbability  )
	{
		if( frand(0,1) <= 0.3f )
			m_i64Revenue = 0;
	}

	ResetDataInit();
	//e_pBankMode
}

void	cBankMode::BulletDataUpdate(int64 e_i64Value)
{
	m_i64TotalBulletValue += e_i64Value;
	++m_iBulletCount;
	m_i64Aeverage = m_i64TotalBulletValue/m_iBulletCount;
}

void	cBankMode::Update(float e_fElpaseTime)
{

}

void	cBankMode::MinusRevenue(int64 e_i64Value)
{
	m_i64CurrentOut += e_i64Value;
	m_i64Revenue -= e_i64Value;
}

void	cBankMode::AddRevenue(int64 e_i64Value)
{
	BulletDataUpdate(e_i64Value);

	m_i64CurrentIn += e_i64Value;
	e_i64Value = (int64)(e_i64Value*m_TargetProbability);
	m_i64Revenue += e_i64Value;
}








