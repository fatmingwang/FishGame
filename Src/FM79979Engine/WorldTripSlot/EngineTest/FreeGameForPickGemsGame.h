#ifndef _FREE_GAME_FOR_PICK_GEMS_GAME_H_
#define _FREE_GAME_FOR_PICK_GEMS_GAME_H_

#include "BaseExtraGame.h"

struct	sGemsBonusData
{
	//if we choice this object spin fre game for so many time for foree
	int					iFreeGameTime;
	//each round could has a chance to win more money
	std::vector<float>	BonusRateList;
};


struct	sFreeGameData
{
	sFreeGameData();
	~sFreeGameData();
	int						iTimeForPickupGems;
	//while free game merge below 3 MPDI to whoe result
	cMPDI*					pPrizeMPDI;
	cMPDI*					pPrizeAwardMPDI;
	cMPDI*					pPrizeAward_NumerialMPDI;
	void					RestIntoFreeGameTimes();
	//ignore this
	void					SetupRestFreeSpinTimes(int e_iPrize);
	void					Update(float e_fElpaseTime);
	void					Render();
};

class	cFreeGameForPickGemsGame;
//==================================
//fuck.....I am using a extra bonus rate to  win more money at SpinPhase
//==================================
class	cPickupObjectForFreeGameAndExtraBonusRate:public cImageButton
{
	friend class							cFreeGameForPickGemsGame;
	int										m_iMoney;
	cSubMPDI*								m_pPickUpGemMPDI;
	virtual bool							InternalCollide(int e_iPosX,int e_iPosY);
	sGemsBonusData		m_GemsBonusData;
public:
	cPickupObjectForFreeGameAndExtraBonusRate(cSubMPDI*e_pClickButton,cSubMPDI*e_pPickupGemMPDI,int e_iMoney,sGemsBonusData e_sGemsBonusData);
	~cPickupObjectForFreeGameAndExtraBonusRate();
	bool			IsButtonSatisfiedCondition(){ return cImageButton::IsSatisfiedCondition(); }
	virtual bool    IsSatisfiedCondition();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render();
	//if true ortther could't grab any more
	bool	IsPrizeGrabed();
	void	DoPickupGem();
};

class	cFreeGameForPickGemsGame:public cBaseExtraGame
{
	virtual	bool	InternalConditionCheck();
	//
	UT::sTimeCounter						m_TimeForNoActionToLeave;
	cMPDI*									m_pEnteringMPDI;
	cMPDI*									m_pLeavingMPDI;
	cMPDI*									m_pAwardListMPDI;
	//here for the spinobject show MPDI
	cMPDI*									m_pFreeGameSpinObjectShow;
	//
	cMPDIList*								m_pMPDIList;
	//
	bool									m_bPickRockToWinMoneyDone;
	int										m_iRockShowRowIndex;
	//
	cDoAllMouseBehaviorList<cPickupObjectForFreeGameAndExtraBonusRate>	m_ImageButtonList;
	std::wstring							m_strImageName;
	//set result for play
	void									SetupResult();
	//
	//virtual	bool	InternalConditionCheck();
	std::vector<sGemsBonusData>				m_GemsBonusDataList;
	//
	bool									IsObjectPickUpDone();
	//
	cMPDI*									m_pConstantUI;
	cMPDI*									m_pUpperScreenUI;
	//
	void									DoSelected();

	cPickupObjectForFreeGameAndExtraBonusRate*m_pSelectedObject;
public:
	DEFINE_TYPE_INFO();
	sFreeGameData							m_FreeGameData;
	cFreeGameForPickGemsGame(const WCHAR*e_strConditionName,int e_iConditionCount,TiXmlElement*e_pGemsBonusData,cMPDI*e_pConstantUI);
	~cFreeGameForPickGemsGame();
	virtual	void	LoadResource();
	virtual	void	Destroy();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render();
    virtual void	MouseMove(int e_iPosX,int e_iPosY);
    virtual void	MouseDown(int e_iPosX,int e_iPosY);
    virtual void    MouseUp(int e_iPosX,int e_iPosY);
};

#endif