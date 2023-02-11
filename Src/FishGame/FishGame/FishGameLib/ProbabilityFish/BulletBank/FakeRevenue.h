





#ifndef _FAKE_REVENUE_H_
#define _FAKE_REVENUE_H_

//fake money bank







struct	sFakeRevenue
{
	int						iFakeRevenue;
	UT::sTimeCounter		TC;
	UT::sMinMaxData<int>	FakeRevenueMoneyRange;
	UT::sMinMaxData<float>	FakeRevenueMoneyCircle;
	bool					bCurrentRoundFakeRevenueGet;
	UT::sMinMaxData<float>	fFakeRevenueFetchProbability;
	int						iFakeRevenueGiveDivision;
	float					fHappyTimeScale;
	//
	void	SetupData(Vector2 e_vDuration,Vector2 e_vMoneyRange);
	//
	sFakeRevenue();
	sFakeRevenue(TiXmlElement*e_pXmlElement);
	//
	//call it to assign new data
	void	Init();
	void	Update(float e_fElpaseTime);
	void	FakeMoneyMinus(int e_iMoney);
	//while bCurrentRoundFakeRevenueGet is active call this to get fake revenue
	int		GetchFakeRevenue();
};

#endif