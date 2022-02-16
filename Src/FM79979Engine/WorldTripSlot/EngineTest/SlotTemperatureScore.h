#ifndef _SLOT_TEMPRATURE_SCORE_H_
#define _SLOT_TEMPRATURE_SCORE_H_

#include "SlotTimeStamp.h"



//ex:balance is 1000	1000/1000= 1		1/10(NextLevelMultipler) extrace percentage is 0.05(ExtraPercentageBase)*0.1 = 0.005
//ex:balance is 10000	10000/1000= 10		10/10(NextLevelMultipler) extrace percentage is 0.05(ExtraPercentageBase)*1 = 0.05
//ex:balance is 100000	100000/1000= 100	100/10(NextLevelMultipler) extrace percentage is 0.05(ExtraPercentageBase)*10 = 0.5
//ex:balance is 100000	1000000/1000= 1000	1000/10(NextLevelMultipler) extrace percentage is 0.05(ExtraPercentageBase)*100 = 5  but 5 is bigger than 0.5 so maximum is 0.5


//so the last score is Balance to ScroeMultiplier+(income/(out+1))*CurrentScore = FinalMultiplier
//if FinalMultiplier is bigger than 2 and it's 2.
//
//ExtraPercentageBase肂κだゑ = Μや/BalanceMoneyStartBase(Μや秨﹍膀计)/NextLevelMultipler(单ゑ禯膀计)*ExtraPercentageBase(肂κだゑ膀计)
//ex:balance is 100000	100000/1000= 100	100/10(NextLevelMultipler) extrace percentage is 0.05(ExtraPercentageBase)*10 = 0.5
//
//程だ计瞯(MaxMultiplier) = 肂κだゑ+(Μ/(や+1))
//ex:0.5+(10000/(5000+1)) = 2.49
//2.49MaxMultiplier ┮程だ计瞯2

//Μ6000や5000 肂κだゑ(6000-5000)/1000/10*0.05 = 0.005
//
//ex:0.005+(6000/(5000+1)) = 1.2047
//┮程だ计瞯1.2047
//狦程だ计瞯2玥程だ计瞯2

//程だ计瞯x瞷だ计 碞琌程だ计夹非


//<!-- BalanceMoneyStartBase肂κだゑ窥膀计   NextLevelMultipler计膀非窥 ExtraPercentage璶膀非κだゑ MaxExtractPencentage程肂κだゑ MaxMultiplier程だ计计-->
//<ScroeMultiplier BalanceMoneyStartBase="1000" NextLevelMultipler="10" ExtraPercentageBase="0.05" MaxExtractPencentage="0.5" MaxMultiplier="2" >


struct	sScroeMultiplier
{
	int		iBalanceMoneyStartBase;
	int		iNextLevelMultipler;
	float	fExtraPercentageBase;
	float	fMaxExtractPencentage;
	float	fMaxMultiplier;
	sScroeMultiplier(TiXmlElement*e_pTiXmlElement);
	float	GetMultiplier(int64 e_iIncome,int64 e_iOut);

//<ScroeMultiplier BalanceMoneyStartBase="1000" NextLevelMultipler="10" ExtraPercentageBase="0.05" MaxExtractPencentage="0.5" MaxMultiplier="2" >
};

//<!-- change score while money is change -->
//<ConditionScoreData MoneyChange="100" Score="-100" Description="win 100 dollars minus 100 score">

struct	sConditionScoreDataList
{
	//find out proper one and set score
	struct	sConditionScoreData
	{
		int	iMoney;
		int	iScore;
	};
	sConditionScoreDataList(TiXmlElement*e_pTiXmlElement);
	std::vector<sConditionScoreData>	ConditionScoreDataList;
	int	GetScore(int e_iCurrentBetBalance);
};


class	cSlotGoodTables;
class	cSlotProbability;
class	cSlotTemperatureScore
{
	friend class	cSlotGoodTables;
	friend class	cSlotProbability;
	int	m_iCurrentScore;
	sSlotTimeStamp				m_SlotTimeStamp;
	sScroeMultiplier*			m_pScroeMultiplier;
	sConditionScoreDataList*	m_pConditionScoreDataList;
public:
	cSlotTemperatureScore();
	~cSlotTemperatureScore();
	void	Update(cSlotBetResult*e_pBetBehaviorResult);
	void	Render();
};


#endif