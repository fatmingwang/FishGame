#include "../stdafx.h"
#include "FakeRevenue.h"
#include "../../GameApp/GameApp.h"
//fake money bank

sFakeRevenue::sFakeRevenue()
{
	iFakeRevenue = 0;
	bCurrentRoundFakeRevenueGet = false;
	fFakeRevenueFetchProbability = Vector2(0.1f,0.1f);
	iFakeRevenueGiveDivision = 2;
	fHappyTimeScale = 1.f;
	SetupData(Vector2(10.f,1000.f),Vector2(100.f,10000.f));
}

sFakeRevenue::sFakeRevenue(TiXmlElement*e_pXmlElement)
{
	cEventVariable*l_pCoinToScore = cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"CoinToScore");
	int			l_iCoinToScore = *l_pCoinToScore->GetInt();
	iFakeRevenue = 0;
	bCurrentRoundFakeRevenueGet = false;
	fFakeRevenueFetchProbability = Vector2(0.1f,0.1f);
	iFakeRevenueGiveDivision = 2;
	SetupData(Vector2(10.f,1000.f),Vector2(100.f,10000.f));
	fHappyTimeScale = 1.f;
	PARSE_ELEMENT_START(e_pXmlElement)
		COMPARE_NAME("HappyRevenue")
		{
			FakeRevenueMoneyRange = VALUE_TO_VECTOR2*(float)(l_iCoinToScore);
		}
		else
		COMPARE_NAME("FakeRevenue")
		{
			iFakeRevenue = VALUE_TO_INT*(l_iCoinToScore);
		}
		else
		COMPARE_NAME("FakeRevenueCircle")
		{
			FakeRevenueMoneyCircle = VALUE_TO_VECTOR2;
			TC.SetTargetTime(FakeRevenueMoneyCircle.Rand());
			TC.Start();
		}
		else
		COMPARE_NAME("FakeRevenueFetchProbability")
		{
			fFakeRevenueFetchProbability = VALUE_TO_VECTOR2;
		}
		else
		COMPARE_NAME("FakeRevenueGiveDivision")
		{
			iFakeRevenueGiveDivision = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("HappyTimeScale")
		{
			fHappyTimeScale = VALUE_TO_FLOAT;
		}
	PARSE_NAME_VALUE_END
}

void	sFakeRevenue::SetupData(Vector2 e_vDuration,Vector2 e_vMoneyRange)
{
	FakeRevenueMoneyCircle = e_vDuration;
	FakeRevenueMoneyRange = e_vMoneyRange;
	TC.SetTargetTime(FakeRevenueMoneyCircle.Rand());
}

void	sFakeRevenue::Update(float e_fElpaseTime)
{
	TC.Update(e_fElpaseTime);
	if( TC.bTragetTimrReached )
	{
		iFakeRevenue = this->FakeRevenueMoneyRange.Rand();
		TC.SetTargetTime(this->FakeRevenueMoneyCircle.Rand());
	}
}

void	sFakeRevenue::FakeMoneyMinus(int e_iMoney)
{
	if(bCurrentRoundFakeRevenueGet)
	{
		iFakeRevenue -=	e_iMoney;
		if( iFakeRevenue<0 )
			iFakeRevenue = 0;
	}
}

int		sFakeRevenue::GetchFakeRevenue()
{
	int	l_FakeMoneyFetch = rand(iFakeRevenue/iFakeRevenueGiveDivision,iFakeRevenue);
	return l_FakeMoneyFetch;
}

void	sFakeRevenue::Init()
{
	cEventVariable*l_pCoinToScore = cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"CoinToScore");
	iFakeRevenue = FakeRevenueMoneyRange.Rand();
	TC.SetTargetTime(this->FakeRevenueMoneyCircle.Rand());
}