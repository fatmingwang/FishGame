#include "stdafx.h"
#include "SlotTimeStamp.h"
#include "SlotGamePhase.h"
#include "GameNetwork.h"
#include "WorldTripSlotApp.h"

const wchar_t*     sTimeStampWithScore::TypeID( L"sTimeStampWithScore" );
const wchar_t*     sTS_MoneyChange::TypeID( L"sTS_MoneyChange" );
const wchar_t*     sTS_Jackpot::TypeID( L"sTS_Jackpot" );
const wchar_t*     sTS_FreeGame::TypeID( L"sTS_FreeGame" );


sTimeStampWithScore::sTimeStampWithScore(eTimeStampData e_eTimeStampData,float e_fElpaseTime,int e_iAddScore,int e_iMaxScore)
{
	TimeStampData = e_eTimeStampData;
	TimeStamp.SetTargetTime(e_fElpaseTime);
	iAddScore = e_iAddScore;
	iCurrentScore = 0;
	iMaxScore = e_iMaxScore;
}

//void	sTimeStampWithScore::DoConditionOk()
//{
//
//}

void	sTimeStampWithScore::Update(int e_iBalanceMoney,float e_fElpaseTime)
{
	bConditionOk = false;
	if(IsConditionOk(e_iBalanceMoney))
	{
		TimeStamp.Update(e_fElpaseTime);
		if( TimeStamp.bTragetTimrReached )
		{
			bConditionOk = true;
			iCurrentScore += iAddScore;
			if( abs(iCurrentScore) > abs(iMaxScore) )
				iCurrentScore = iMaxScore;
			TimeStamp.Start();
			DoConditionOk();
		}
	}
	else
	{
		TimeStamp.Start();
		iCurrentScore = 0;
	}
}

sTS_MoneyChange::sTS_MoneyChange(eTimeStampData e_eTimeStampData,float e_fElpaseTime,int e_iAddScore,int e_iMaxScore,int e_iMoney)
:sTimeStampWithScore(e_eTimeStampData,e_fElpaseTime,e_iAddScore,e_iMaxScore)
{
	iChangedBalance = 0;
	iMoney = e_iMoney;
}

bool	sTS_MoneyChange::IsConditionOk(int e_iBalanceMoney)
{
	iChangedBalance += e_iBalanceMoney;
	if( iMoney > 0 )
	{
		return iMoney <= iChangedBalance?true:false;
	}
	return iMoney>=e_iBalanceMoney?true:false;
}

void	sTS_MoneyChange::DoConditionOk()
{
	iChangedBalance = 0;
}

sTS_Jackpot::sTS_Jackpot(eTimeStampData e_eTimeStampData,float e_fElpaseTime,int e_iAddScore,int e_iMaxScore,int e_iTotalBalance,float	e_fProbability,eJPType e_eJPType)
:sTimeStampWithScore(e_eTimeStampData,e_fElpaseTime,e_iAddScore,e_iMaxScore)
{
	m_eJPType = e_eJPType;
	m_piJPOpened = cWorldTripSlotApp::m_spPlayerData->GetJPOpenCount(e_eJPType);
	m_fProbability = e_fProbability;
	m_iTotalBalance = e_iTotalBalance;	
}

void	sTS_Jackpot::DoConditionOk()
{

}

bool	sTS_Jackpot::IsConditionOk(int e_iBalanceMoney)
{
	//it should never happen
	if( m_piJPOpened )
	{
		if(*m_piJPOpened < 0 )
			*m_piJPOpened = 0;
		if( g_pSlotGamePhase->m_pBetBehaviorResult->GetRevenue() >= m_iTotalBalance*(1+*m_piJPOpened)  )
		{
			float	l_fValue = frand(0,1);
			if( m_fProbability>l_fValue )
			{
				if( m_eJPType == eJPT_NETWORK )
				{
					return false;
				}
				return true;
			}
			return false;
		}
	}
	return false;
}

sTS_FreeGame::sTS_FreeGame(eTimeStampData e_eTimeStampData,float e_fElpaseTime,int e_iAddScore,int e_iMaxScore)
:sTimeStampWithScore(e_eTimeStampData,e_fElpaseTime,e_iAddScore,e_iMaxScore)
{
	
}

void	sTS_FreeGame::DoConditionOk()
{

}

bool	sTS_FreeGame::IsConditionOk(int e_iBalanceMoney)
{
	return g_pSlotGamePhase->m_bFreeToPlay?false:true;
}

//bool	sTS_Combo::IsConditionOk()
//{
//	return true;
//}


sSlotTimeStamp::sSlotTimeStamp()
{
	ResetTimeStamp();
	m_pPlayerData = cWorldTripSlotApp::m_spPlayerData;
	m_uiHugeMoneyBase = 500;
}

sSlotTimeStamp::~sSlotTimeStamp()
{
	DELETE_VECTOR(m_TimeStampWithScore,sTimeStampWithScore*);
}

//<TimeStamp>
	//<ToastCoin	 ElpaseTime="300"		Score="20"					MaxScore="100"		/>
	//<FreeGame		 ElpaseTime="6000"		Score="200"					MaxScore="600"		/>
	//<MoneyChange	 ElpaseTime="300"		RoundBalance="-125"			Score="100"			MaxScore="100"		/>
	//<MoneyChange	 ElpaseTime="300"		RoundBalance="125"			Score="-100"		MaxScore="-100"		/>
	//JP win cindition,36000 second is equal 10 hours.this one for 10 combo and net JP.
	//<Jackpot		 ElpaseTime="36000"		TotalBalanceBiggerThanJP="99999"		PayRate="1.1"	/>
//</TimeStamp>
void	sSlotTimeStamp::ProcessTimeStamp(TiXmlElement*e_pTiXmlElement)
{
	DELETE_VECTOR(m_TimeStampWithScore,sTimeStampWithScore*);
	TiXmlElement*l_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while( l_pTiXmlElement )
	{
		const WCHAR*l_strName = l_pTiXmlElement->Value();
		const WCHAR*l_strElpaseTime = l_pTiXmlElement->Attribute(L"ElpaseTime");
		const WCHAR*l_strScore = l_pTiXmlElement->Attribute(L"Score");
		const WCHAR*l_strMaxScore = l_pTiXmlElement->Attribute(L"MaxScore");
		float	l_fElpaseTime	= (float)_wtof(l_strElpaseTime);
		int		l_iScore		= _wtoi(l_strScore);
		int		l_iMaxScore		= _wtoi(l_strMaxScore);
		sTimeStampWithScore*l_pTimeStampWithScore = 0;
		if(	!wcscmp(l_strName,L"FreeGame")	)
		{
			l_pTimeStampWithScore = new sTS_FreeGame(eTSD_FREE_GAME,l_fElpaseTime,l_iScore,l_iMaxScore);
		}
		else
		if(	!wcscmp(l_strName,L"MoneyChange")	)
		{
			const WCHAR*l_strMoney = l_pTiXmlElement->Attribute(L"Money");

			int	l_iMoney = _wtoi(l_strMoney);
			l_pTimeStampWithScore = new sTS_MoneyChange(eTSD_MONEY_CHANGE,l_fElpaseTime,l_iScore,l_iMaxScore,l_iMoney);
		}
		else
		if(	!wcscmp(l_strName,L"Jackpot")	)
		{
			const WCHAR*l_strTotalBalanceBiggerThanJP = l_pTiXmlElement->Attribute(L"TotalBalanceBiggerThanJP");
			const WCHAR*l_strProbability = l_pTiXmlElement->Attribute(L"Probability");
			const WCHAR*l_strType = l_pTiXmlElement->Attribute(L"Type");
			eJPType	l_eJPType = GetJPType(l_strType);
			int		l_iTotalBalanceBiggerThanJP = _wtoi(l_strTotalBalanceBiggerThanJP);
			float	l_fProbability = (float)_wtof(l_strProbability);
			l_pTimeStampWithScore = new sTS_Jackpot(eTSD_JP,l_fElpaseTime,l_iScore,l_iMaxScore,l_iTotalBalanceBiggerThanJP,l_fProbability,l_eJPType);
		}
		if( l_pTimeStampWithScore )
			m_TimeStampWithScore.push_back(l_pTimeStampWithScore);
		l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
	}
}

void	sSlotTimeStamp::Update(cSlotBetResult*e_pBetBehaviorResult)
{
	this->m_TimeStamp.Update();
	size_t	l_uiSize = m_TimeStampWithScore.size();
	int	l_iBalance = g_pSlotGamePhase->m_pBetBehaviorResult->GetCurrentRoundBetBalance();
	l_iBalance += rand(-5,5);
	for( size_t i = 0;i<l_uiSize;++i )
	{
		m_TimeStampWithScore[i]->Update(l_iBalance,this->m_TimeStamp.fElpaseTime);
	}
}

void	sSlotTimeStamp::ResetTimeStamp()
{
		m_TimeStamp.Update();
}

float	sSlotTimeStamp::GetTimeStamp(eTimeStampData e_eTimeStampData)
{
	return m_TimeStampWithScore[e_eTimeStampData]->TimeStamp.fTargetTime-m_TimeStampWithScore[e_eTimeStampData]->TimeStamp.fRestTime;
}

int		sSlotTimeStamp::GetScore()
{
	int	l_iScore = 0;
	size_t	l_uiSize = m_TimeStampWithScore.size();
	for( size_t i = 0;i<l_uiSize;++i )
	{
		l_iScore += m_TimeStampWithScore[i]->iCurrentScore;
	}
	return l_iScore;
}

bool	sSlotTimeStamp::IsJPOpen(eJPType e_eJPType)
{
	size_t	l_uiSize = m_TimeStampWithScore.size();
	for(size_t i=0;i<l_uiSize;++i  )
	{
		if(m_TimeStampWithScore[i]->Type() == sTS_Jackpot::TypeID)
		{
			sTS_Jackpot*l_pTS_Jackpot = (sTS_Jackpot*)m_TimeStampWithScore[i];
			if(l_pTS_Jackpot->m_eJPType == e_eJPType)
			{
				if(l_pTS_Jackpot->bConditionOk)
					return true;
				return false;
			}
		}
	}
	return false;
}