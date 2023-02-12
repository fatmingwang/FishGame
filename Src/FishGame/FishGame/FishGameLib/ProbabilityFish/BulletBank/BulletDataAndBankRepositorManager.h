#pragma once
//
//
class	cBankRunnerManager;
class	cBankRunner;
struct	sHittedFishData;
class	cBulletDataAndBankRepositorManager
{
private:
	int										m_iSequenceEmptyShot;
	//
	bool									IsPayRateLegalToWinBigMoney( int e_iPrize, int e_iPayRate );
	bool									DoProbailityWithTotalRevenueForBankFormat( sHittedFishData*e_pHittedFishData, float*e_pfAddProbability, int*e_piOutMoney );
	float									GetBulletPayRateLimitOffsetProbability( int e_iPayrate );
	void									RevenueMinus(int e_iPayRate,int e_iMoney,int e_iPlayerID,bool e_bMiniGame);
	void									Update_HitMonsterForBankFormat();
	//
	cNamedTypedObjectVector<cBankRunnerManager>*	m_pSubRepositoryBankManager;
	int64									m_i64TotalBulletValue;
	int64									m_i64Aeverage;
	int										m_iBulletCount;
	int										GetBulletAeverageFormatIndex( int e_iValue, int e_iPlayerID );

	//
public:
	cBulletDataAndBankRepositorManager();
	virtual ~cBulletDataAndBankRepositorManager();
	//
	float									GetBulletAeverageOffsetPercentage( int e_iValue );
	//
	void									Init();
	void									Update( float e_fElpaseTime );
	cBankRunner								*GetSubRepositoryBankByPayRate( int e_iPayRate, int e_iPlayerID );
	void									DebugRender();
	//
	void									RevenueAdd( int e_iPayRate, int e_iPlayerID, int e_iRealPayRate );
	void									RevenueMinus( int e_iOutMoney, int e_iPayRate, int e_iPlayerID, int e_iRealPayRate );
	bool									IsMoneyOutOk( int e_iOutMoney, int e_iPayRate, int e_iPlayerID );
	int64									GetExtractRevenueValue( int e_iOutMoney, int e_iPayRate );
	//
};
