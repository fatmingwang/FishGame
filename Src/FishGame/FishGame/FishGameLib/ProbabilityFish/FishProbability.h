#ifndef _FISH_PROBABILITY_H_
#define _FISH_PROBABILITY_H_
//
class	cProbabilityData;
class	cProbability_Fatming;
class	cProbability_MasterLee;
class	cMonster;
class	cBullet;
class	cPlayerBehaviorBase;
struct	sHittedFishData;
class	cFileSaver;
class	cBulletDataAndBankRepositorManager;
class	cProbabilityFish: public cNodeISAX
{
	friend class						cBulletDataAndBankRepositorManager;
	virtual	bool						MyParse(TiXmlElement*e_pRoot);
private:
	//
	float								m_fMachinRunTime;
	int64*								m_pi64LatestInMoney;
	int64*								m_pi64LatestOutMoney;
	int64*								m_pi64LatestRevenue;
	int64*								m_pi64LatestExchangeIn;
	int64*								m_pi64LatestExchangeOut;
	int64*								m_pi64LatestCoinIn;
	int64*								m_pi64LatestCoinOut;
	float*								m_pfTargetProbability;
	cFileSaver*							m_pFileSaver;
	cBulletDataAndBankRepositorManager*	m_pBulletData;
	//
	bool								m_bIsShow_RestartMachineMessage;
	cMultiPathDynamicImage				*m_pMPDI_MasterLeeIOErrorMessage;
	int									m_iMaxScoreInOneFire;
public:
	bool								MonsterToDied( sHittedFishData *e_pHittedFishData, int e_iOutMoney, bool e_bForceToDied, bool e_bMasterLi );
	void								MonsterToKeepGoing( sHittedFishData *e_pHittedFishData, float e_fAddProbability );

	cProbability_Fatming				*m_pProbability_Fatming;
	cProbability_MasterLee				*m_pProbability_MasterLee;
	//
	float								m_fSafeToGiveProbabilityOffset;
	//
	cProbabilityFish();
	virtual ~cProbabilityFish();
	void								Init();
	void								Update( float e_fElpaseTime );
	void								Render();
	void								DebugRender();
	void								RevenueAdd( int e_iMoney, int e_iPlayerID );
	void								RevenueMinus( int e_iMoney, int e_iPlayerID);
	float								GetCurrentRealProbability();
	float								GetCurrentProbability();;
	float								GetTargetProbability();
	float								GetOffsetProbability();
	int64								GetLeatestInMoney();
	float								GetNewProbailityByAddMoney(int e_iOutMoney);
	bool								IsProbailitySafeToOut(float e_fProbaility);
	//
	Vector3								GetRandomPos( cMonster *e_pMonster );
	Vector3								GetRandomPos( float e_fRadius );
	//
	//for control panel
	void								ClearLatestRecord(bool e_bClearPlayerMoney);
	//
	//for player exchange and coin behavior.
	void								ExchangeIn( int64 e_i64Value );
	void								ExchangeOut( int64 e_i64Value );
	void								ToastCoin( int64 e_i64Value );
	void								RefundCoin( int64 e_i64Value );
	//
	void								SetShow_MasterLeeIOErrorMessage( bool e_bIsShowRestartMachineMessage ){ m_bIsShow_RestartMachineMessage = e_bIsShowRestartMachineMessage; };
	bool								IsMasterLeeIOError(){return m_bIsShow_RestartMachineMessage;};
	void								WrtiePlayerMoney(float e_fElpaseTime);

	cBulletDataAndBankRepositorManager*	GetBulletData(){return  m_pBulletData;}
};

#endif