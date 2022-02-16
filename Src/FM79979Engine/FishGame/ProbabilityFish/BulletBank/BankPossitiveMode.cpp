#include "../stdafx.h"
#include "BankPossitiveMode.h"
#include "../FishProbability.h"
#include "../../GameApp/GameApp.h"
#include "BankRunner.h"

cPossitiveBankMode::cPossitiveBankMode(float e_pTargetProbability)
:cBankMode(e_pTargetProbability)
{
	int	l_iCoinToScore = cFishApp::m_spControlPanel->GetCoinToScore();
	m_FakeRevenue.SetupData(Vector2(10.f,1000.f),Vector2(100.f*l_iCoinToScore,10000.f*l_iCoinToScore));
}








cPossitiveBankMode::cPossitiveBankMode(TiXmlElement*e_pTiXmlElement):cBankMode(e_pTiXmlElement)
{
	int	l_iCoinToScore = cFishApp::m_spControlPanel->GetCoinToScore();
	m_FakeRevenue = sFakeRevenue(e_pTiXmlElement->FirstChildElement());
}








cPossitiveBankMode::~cPossitiveBankMode()
{

}








void	cPossitiveBankMode::MinusRevenue(int64 e_i64Value)
{
	m_i64CurrentOut += e_i64Value;
	m_i64Revenue -= e_i64Value;
	this->m_FakeRevenue.FakeMoneyMinus((int)e_i64Value);
}








bool	cPossitiveBankMode::IsModeSatisfied()
{
	//if( this->m_i64Revenue > 0 && this->m_FakeRevenue.iFakeRevenue <= 0 )
	if( this->m_FakeRevenue.iFakeRevenue <= 0 || m_FakeRevenue.TC.bTragetTimrReached )
		return true;
	return false;
}








void	cPossitiveBankMode::ResetDataInit()
{
	m_FakeRevenue.Init();
	cEventVariable*l_pCoinToScore = cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"CoinToScore");
	int			l_iCoinToScore = *l_pCoinToScore->GetInt();
	int64	l_iMultiplier = 1;
	float	l_fProbability = cFishApp::m_spProbabilityFish->GetOffsetProbability();
	float	l_fOffset = l_fProbability;//-cFishApp::m_spProbabilityFish->m_fSafeToGiveProbabilityOffset;
	cBankRunner*l_pBankRunner = (cBankRunner*)this->GetOwner();
	if(  l_fOffset > 0.f  )
	{
		float	l_f = (float)l_pBankRunner->m_i64LeastMoney/cFishApp::m_spControlPanel->m_iBulletPayRateLimit;
		if( frand(0,1) <= l_f/2.f )
		{
			if( l_fProbability >=0.06 )
				l_iMultiplier = (int)(l_fProbability*5000);
			else
			if( l_fProbability >=0.04 )
				l_iMultiplier = (int)(l_fProbability*4000);
			else
			if( l_fProbability >=0.02 )
				l_iMultiplier = (int)(l_fProbability*3000);
			else
			if( l_fProbability >=0.015 )
				l_iMultiplier = (int)(l_fProbability*2000);
			else
			if( l_fProbability >=0.01 )
				l_iMultiplier = (int)(l_fProbability*1500);
			else
			if( l_fProbability >=0.009 )
				l_iMultiplier = (int)(l_fProbability*1500);
			else
			if( l_fProbability >=0.008 )
				l_iMultiplier = (int)(l_fProbability*1500);
			else
			if( l_fProbability >=0.007 )
				l_iMultiplier = (int)(l_fProbability*1500);
			else
			if( l_fProbability >=0.006 )
				l_iMultiplier = (int)(l_fProbability*1500);
			else
			if( l_fProbability >=0.005 )
				l_iMultiplier = (int)(l_fProbability*1300);
			else
			if( l_fProbability >=0.004 )
				l_iMultiplier = (int)(l_fProbability*1300);
			else
			if( l_fProbability >=0.003 )
				l_iMultiplier = (int)(l_fProbability*1300);
			else
			if( l_fProbability >=0.002 )
				l_iMultiplier = (int)(l_fProbability*1300);
			else
				l_iMultiplier = (int)(l_fProbability*1300);
		
		
		}
	}
	m_FakeRevenue.iFakeRevenue *= (int)l_iMultiplier;
	int	l_iMaxGiveMoney = (int)l_pBankRunner->m_i64LeastMoney*300;
	if( m_FakeRevenue.iFakeRevenue > l_iMaxGiveMoney  )
		m_FakeRevenue.iFakeRevenue = l_iMaxGiveMoney;
}








bool	cPossitiveBankMode::IsMoneyOutOk(int e_iMoney,int e_iPayRate)
{
	if( this->m_FakeRevenue.iFakeRevenue > 0 )
	{
		float	l_fRandValue = frand(0,1);
		if( l_fRandValue <= this->m_FakeRevenue.fFakeRevenueFetchProbability.Rand() )
		{
			int	l_iPrize = e_iMoney/e_iPayRate;
			m_FakeRevenue.bCurrentRoundFakeRevenueGet = true;
			float	l_fProbability = (float)((double)(m_i64CurrentOut+e_iMoney)/(m_i64CurrentIn+this->m_FakeRevenue.GetchFakeRevenue()));
			if( m_FakeRevenue.iFakeRevenue > e_iMoney )
			//if( l_fProbability > 0.f && l_fProbability <= m_TargetProbability)
				return true;
			//to avoid stack at fake revenue status.
			this->m_FakeRevenue.iFakeRevenue = 0;
			return false;
		}
		//return false;
		if( this->m_iNumRevenueAddAndNoRevenueMinus > 5 )
			return cBankMode::IsMoneyOutOk(e_iMoney,e_iPayRate);
	}
	else
	{
		this->m_FakeRevenue.bCurrentRoundFakeRevenueGet = false;
		return cBankMode::IsMoneyOutOk(e_iMoney,e_iPayRate);
	}
	return false;
}








void	cPossitiveBankMode::Update(float e_fElpaseTime)
{
	cBankMode::Update(e_fElpaseTime);
	this->m_FakeRevenue.Update(e_fElpaseTime);
}








bool	cPossitiveBankMode::IsFakeMoneyEnoughToPay(int e_iMoney)
{
	if(m_FakeRevenue.iFakeRevenue >= e_iMoney)
	{
		return true;
	}
	return false;
}








void	cPossitiveBankMode::DebugRender( int e_iPosX,int e_iPosY )
{
	std::wstring	l_str = 
		UT::ComposeMsgByFormat( L"PosR:%d, In:%d, Out:%d",
			(int)this->m_i64Revenue,
			(int)this->m_i64CurrentIn,
			this->m_i64CurrentOut );
	cGameApp::m_spGlyphFontRender->RenderFont(e_iPosX,e_iPosY,l_str.c_str());
}









