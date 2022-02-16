#include "stdafx.h"
#include "SlotTemperatureScore.h"

//<ScroeMultiplier BalanceMoneyStartBase="1000" NextLevelMultipler="10" ExtraPercentageBase="0.05" MaxExtractPencentage="0.5" MaxMultiplier="2" >
sScroeMultiplier::sScroeMultiplier(TiXmlElement*e_pTiXmlElement)
{
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("BalanceMoneyStartBase")
		{
			this->iBalanceMoneyStartBase = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("NextLevelMultipler")
		{
			this->iNextLevelMultipler = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("ExtraPercentageBase")
		{
			this->fExtraPercentageBase = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("MaxExtractPencentage")
		{
			this->fMaxExtractPencentage = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("MaxMultiplier")
		{
			this->fMaxMultiplier = VALUE_TO_FLOAT;
		}
	PARSE_NAME_VALUE_END
}

float	sScroeMultiplier::GetMultiplier(int64 e_i64Income,int64 e_i64Out)
{
	int64		l_iBalance = e_i64Income-e_i64Out-iBalanceMoneyStartBase;
	float	l_fExtraMultiplier = l_iBalance/iNextLevelMultipler*fExtraPercentageBase;
	if( l_fExtraMultiplier>fMaxExtractPencentage )
		l_fExtraMultiplier = fMaxExtractPencentage;
	else
	if( l_fExtraMultiplier<0.f )
		l_fExtraMultiplier = 0.f;
	float	l_Multiplier = (float)((double)e_i64Income/(e_i64Out+1))+l_fExtraMultiplier;
	if( l_Multiplier >this->fMaxMultiplier ) 
		l_Multiplier = this->fMaxMultiplier;
	if( l_Multiplier<0.1f )
		l_Multiplier = 0.1f;
	return l_Multiplier;
}

//<ConditionScoreData MoneyChange="100" Score="-100" Description="贏一百塊的時候扣一百分">
//<ConditionScoreData MoneyChange="-100" Score="100" Description="輸一百塊的時候加一百分">
sConditionScoreDataList::sConditionScoreDataList(TiXmlElement*e_pTiXmlElement)
{
	e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while( e_pTiXmlElement )
	{
		sConditionScoreData	l_ConditionScoreData;
		PARSE_ELEMENT_START(e_pTiXmlElement)
				COMPARE_NAME("MoneyChange")
				{
					l_ConditionScoreData.iMoney = VALUE_TO_INT;
				}
				else
				COMPARE_NAME("Score")
				{
					l_ConditionScoreData.iScore = VALUE_TO_INT;
				}
			TO_NEXT_VALUE
				this->ConditionScoreDataList.push_back(l_ConditionScoreData);
		e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
	}
}

int	sConditionScoreDataList::GetScore(int e_iCurrentBetBalance)
{
	int	l_iFocusIndex = -1;
	int	l_iSize = (int)ConditionScoreDataList.size();
	int	l_iCloseScore = INT_MAX;
	for(int i=0;i<l_iSize;++i)
	{
		int	l_iMoneyOffset = e_iCurrentBetBalance-ConditionScoreDataList[i].iMoney;
		if( abs(l_iCloseScore)>abs(l_iMoneyOffset) )
		{
			l_iCloseScore = l_iMoneyOffset;
			l_iFocusIndex = i;
		}
	}
	if( l_iFocusIndex != -1 )
		return ConditionScoreDataList[l_iFocusIndex].iScore;
	return 0;
}

cSlotTemperatureScore::cSlotTemperatureScore()
{
	m_pScroeMultiplier = 0;
	m_pConditionScoreDataList = 0;
	m_iCurrentScore = 0;
}

cSlotTemperatureScore::~cSlotTemperatureScore()
{
	SAFE_DELETE(m_pConditionScoreDataList);
	SAFE_DELETE(m_pScroeMultiplier);
}

void	cSlotTemperatureScore::Update(cSlotBetResult*e_pBetBehaviorResult)
{
	m_SlotTimeStamp.Update(e_pBetBehaviorResult);
	m_iCurrentScore += m_SlotTimeStamp.GetScore();
	if( m_pConditionScoreDataList )
			m_iCurrentScore += this->m_pConditionScoreDataList->GetScore(-e_pBetBehaviorResult->GetCurrentRoundBetBalance());
}

void	cSlotTemperatureScore::Render()
{
	std::wstring	l_str = UT::ComposeMsgByFormat(L"Temperature:%d",m_iCurrentScore);
	//cGameApp::m_spGlyphFontRender->SetScale(2.f);
	cGameApp::m_spGlyphFontRender->RenderFont(0,400,l_str.c_str());
	cGameApp::m_spGlyphFontRender->SetScale(1.f);
}