#ifndef _PLAYER_BEHAVIOR_BASE_H_
#define _PLAYER_BEHAVIOR_BASE_H_

class	cMonster;
class	cPlayerWeapon;
class	cRefoundCoinDevice;
class	cProbabilityFish;
class	cFileSaver;
class	cPlayerBehaviorBase : public NamedTypedObject
{
	friend class				cFileSaver;
	friend class				cPlayerManager;
	friend class				cRefoundCoinDevice;
	friend class				cProbabilityFish;
	cMatrix44					m_PlayerMatrix;
protected:
	cMPDI*						m_pPlayerLvUIMPDI;
	UT::sTimeCounter			m_NoMoney_InsertCoinHintTC;
	Frame						m_PlayerFrame;
	//Message
	cMultiPathDynamicImage*		m_pMessageMoneyReachLimit;		//m_pScoreLimit
	cMultiPathDynamicImage*		m_pMessageInsertCoin;			//m_i64Score <= 0
	cMultiPathDynamicImage*		m_pMessageCoinRefundNotEnough;	//
	cMultiPathDynamicImage*		m_pMessageIOError;
	cMultiPathDynamicImage*		m_pMessagePlayerLock;			//money over too much this player is not allow to play anymore
	

	cNumerialImage*				m_pScoreNumberImage;
	Vector3						m_vScorePos;
	Vector3						m_vPlayerPos;
	Vector3						m_vPlayerRot;


	cPlayerWeapon*				m_pWeapon;
	int							m_iPayRateMultier;
	int							m_iPlayerID;
	int64						m_i64Score;
	//
	int64*						m_pScoreLimit;
	int							m_iMinScoreBet;
	int							m_iMaxLevelNumber;
	bool						m_bCoinNotEnough;
	bool						m_bRefundCoin;
	//

	virtual	void				InternalRender() = 0;
	virtual	void				InternalInit() = 0;
	virtual	void				InternalUpdate(float e_fElpaseTime) = 0;
	//
public :

	cPlayerBehaviorBase();
	virtual ~cPlayerBehaviorBase();
	//
	virtual	void				AddScore( cMonster* Fish, int Money ) = 0;
	int							GetPlayerID(){return m_iPlayerID;}
	int64						GetScore(){return m_i64Score;}
	virtual	void				Init();
	virtual	void				Update( float e_fElpaseTime );
	virtual	void				Render();
	cPlayerWeapon*				GetWeapon(){return m_pWeapon;}
	//int						GetCurrentLevel();
	Vector3						GetPlayerPosition(){return m_vPlayerPos;}
	Vector3						GetPlayerRotation(){return m_vPlayerRot;}
	int							GetPayRateMultier(){return m_iPayRateMultier;}
	void						SetPayRateMultier(int e_iPayRateMultier){m_iPayRateMultier = e_iPayRateMultier;}
	cMatrix44					GetPlayerMatrix(){ return m_PlayerMatrix; }
	//
};
//================================================================
//
//
//
//================================================================
class	cPlayerManager : public cNamedTypedObjectVector<cPlayerBehaviorBase>
{
private:
	cClickMouseBehavior		m_AutoPlay;
	int						m_iPlayerCount;
public:
	cPlayerManager();
	virtual ~cPlayerManager();
	//
	void	Init();
	void	Update( float e_fElpaseTime );
	void	Render();
	void	Destroy();
};
//================================================================
//
//
//
//================================================================
class	cPlayerWeapon:public NamedTypedObject
{
protected:
	eDirection		m_eMovingDirection;
	bool			m_bFire;
public:
	cPlayerWeapon();
	virtual ~cPlayerWeapon();
	virtual	void	Update(float e_fElpaseTime) = 0;
	void			StatusChange(eDirection e_eDirection);
	virtual	bool	Fire(cPlayerBehaviorBase*e_pPlayerBehaviorBase) = 0;
	virtual	bool	IsFire() = 0;
	virtual	int		GetPower() = 0;
	virtual	void	SetPower(int e_iCurrentLevel) = 0;
	virtual void	SetEnergyTube(bool e_bIsSetEnergyTube) = 0;
	virtual	void	LevelUp(bool e_bLevelChange) = 0;

	virtual	void	Destroy(){}
};


#endif