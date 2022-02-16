#ifndef _SLOT_TIME_STAMP_H_
#define _SLOT_TIME_STAMP_H_

#include "PlayerData.h"
#include "SpinObjectPrizeData.h"
//let player feel game is interactiver,not only const game mode.
enum	eTimeStampData
{
	eTSD_TOAST_COIN = 0,	//while credit is 0 and coin has no toast it's idle
	//eTSD_BONUS,				//combo
	eTSD_FREE_GAME,			//free game
	eTSD_JP,				//jackpot
	eTSD_MONEY_CHANGE,		//this one is special for round times not time elpased.
	eTSD_MAX,				//
};

//time for elpase time
//Score for current status add or minus
//LimitCount for how many added

//Balance here will compare previous player money and current money
//<TimeStamp>
	//<ToastCoin	 ElpaseTime="300"		Score="20"					MaxScore="100"		/>
	//<FreeGame		 ElpaseTime="6000"		Score="200"					MaxScore="600"		/>
	//<MoneyChange	 ElpaseTime="300"		RoundBalance="-125"			Score="100"			MaxScore="100"		/>
	//<MoneyChange	 ElpaseTime="300"		RoundBalance="125"			Score="-100"		MaxScore="-100"		/>
	//JP win cindition,36000 second is equal 10 hours.this one for 10 combo and net JP.
	//<Jackpot		 ElpaseTime="36000"		TotalBalanceBiggerThanJP="99999"		PayRate="1.1"	/>
//</TimeStamp>

//every time stamp score will be minus while player is win money
//
//time stamp will calculate all event is not reached condition and 
//ex: toast how long is is hasn't toatst money,if it is long enougn the acore will be rised
//ex:MoneyChange Stamp how long is it has't win or lost spesific money,socre will be changed.
//
struct	sTimeStampWithScore:public NamedTypedObject
{
	bool				bConditionOk;
	//debug data
	eTimeStampData		TimeStampData;
	//
	virtual	bool		IsConditionOk(int e_iBalanceMoney) = 0;
	//time stamp
	UT::sTimeCounter	TimeStamp;
	//oculd be nagtive
	int					iAddScore;
	//the score will change at update,please do not set this directly
	int					iCurrentScore;
	//if limit is reached do not add score until condition is unlock.
	int					iMaxScore;
	//
	void				Update(int e_iBalanceMoney,float e_fElpaseTime);
	sTimeStampWithScore(eTimeStampData e_eTimeStampData,float e_fElpaseTime,int e_iAddScore,int e_iMaxScore);
	virtual	void		DoConditionOk() = 0;
	DEFINE_TYPE_INFO();
};

struct	sTS_MoneyChange:public sTimeStampWithScore
{
	//each update will change this value and while time is passed
	int	iChangedBalance;
	virtual	bool	IsConditionOk(int e_iBalanceMoney);
	int	iMoney;
	sTS_MoneyChange(eTimeStampData e_eTimeStampData,float e_fElpaseTime,int e_iAddScore,int e_iMaxScore,int e_iMoney);
	virtual	void		DoConditionOk();
	DEFINE_TYPE_INFO();
};

//================
//if time was long enough balance is bigger then m_iTotalBalance and payrate is bigger then m_fPayRate condition is ok.
//================
//Type="Local"
//Type="NetWork"
struct	sTS_Jackpot:public sTimeStampWithScore
{
	eJPType				m_eJPType;
	//probability show percentage
	float				m_fProbability;
	//how many time JP opened
	int*				m_piJPOpened;
	sTS_Jackpot(eTimeStampData e_eTimeStampData,float e_fElpaseTime,int e_iAddScore,int e_iMaxScore,int e_iTotalBalance,float	e_fProbability,eJPType e_eJPType);
	int					m_iTotalBalance;
	bool				IsConditionOk(int e_iBalanceMoney);
	virtual	void		DoConditionOk();
	DEFINE_TYPE_INFO();
};
//================
//if time was long enough  and there is no free game is entering condition is true
//================
struct	sTS_FreeGame:public sTimeStampWithScore
{
	sTS_FreeGame(eTimeStampData e_eTimeStampData,float e_fElpaseTime,int e_iAddScore,int e_iMaxScore);
	virtual	bool	IsConditionOk(int e_iBalanceMoney);
	virtual	void		DoConditionOk();
	DEFINE_TYPE_INFO();
};

//=======================
//if min combo is not reached condition will be true
//=======================
//struct	sTS_Combo:public sTimeStampWithScore
//{
//	int				m_iMinComboCount;
//	sTS_Combo(int e_iMinComboCount)
//	{
//	
//	}
//	virtual	bool	IsConditionOk();
//};


//========================
//
//========================
class	cSlotGoodTables;
class	sSlotTimeStamp
{
	friend class	cSlotGoodTables;
	std::vector<sTimeStampWithScore*>	m_TimeStampWithScore;
	//call time stamp at bet phase,and get a time offset.
	UT::sTimeAndFPS	m_TimeStamp;
	cPlayerData*	m_pPlayerData;
	UINT			m_uiHugeMoneyBase;		//the value to detect how much won is equal huge money.
	//parse data
	void	ProcessTimeStamp(TiXmlElement*e_pTiXmlElement);
	bool	IsJPOpen(eJPType e_eJPType);
public:
	sSlotTimeStamp();
	~sSlotTimeStamp();
	//
	//call it at bet phase,befor generate next spin data
	void	Update(cSlotBetResult*e_pBetBehaviorResult);
	void	ResetTimeStamp();
	float	GetTimeStamp(eTimeStampData e_eTimeStampData);
	int		GetScore();
};


#endif