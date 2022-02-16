#ifndef	_SLOT_GAME_PHASE_H_
#define	_SLOT_GAME_PHASE_H_

class	cSlotObjectManager;
class	cSlotRuleData;
class	cSlotObjectManagerList;
class	cStageData;
class	cSlotBetBehavior;
class	cSlotUI;
class	cSlotBetResult;
class	cSlotGamePhase:public cPhaseManager
{
	void										PhaseSetup();
	void										LoadResolutionChanger(char*e_strFileName);
	void										LoadSound(char*e_strFileName);
	void										LoadGameSetup();
	void										SetupSpinData(const WCHAR*e_strStageName);
	//
	void										ReleaseGame();
	void										LoadGame();
public:
	cSlotBetBehavior*							m_pBetBehavior;
	cSlotBetResult*								m_pBetBehaviorResult;
	cSlotObjectManagerList*						m_pSlotLineObjectManagerList;
	cSlotRuleData*								m_pSlotRuleData;
	cStageData*									m_pStageData;
	cSlotUI*									m_pSlotUI;
	//for global to multiply win money,cSpinPhase is using it now u could simply change to win more money
	//m_pExtraBonusMoneyRate,m_pRestFreeGameTimes is a pair
	std::vector<int>*							m_pExtraBonusMoneyRate;
	std::vector<int>*							m_pRestFreeGameTimes;
	//debug test
	bool*										m_pbForceIntoFreeGame;
	std::wstring*								m_pstrFreeGameTest;
	//free to play without and money for bet
	int											m_iFreeGame;
	bool										m_bFreeToPlay;
	bool										m_bOpenNetWork;
	int											m_iNumLineBetable;
	int											m_iExchangeRate;

	cSlotGamePhase();
	~cSlotGamePhase();
	virtual	void	Init();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render();
	virtual	void	DebugRender();
	//virtual	void	MouseDown(int e_iX,int e_iY);
	//virtual	void	MouseUp(int e_iX,int e_iY);
	//virtual	void	MouseMove(int e_iX,int e_iY);
	//virtual	void	KeyUp(char e_cKey);
	//virtual	void	KeyDown(char e_cKey);
	//virtual	void	KeyPress(char e_cKey);
	virtual	const	WCHAR*	GetNextPhaseName();
	virtual	void	FetchData(const WCHAR*e_strPhaseName,void*e_pData);
};

extern cSlotGamePhase*g_pSlotGamePhase;

#endif