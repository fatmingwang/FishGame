#pragma once
//
enum	eBulletStatus
{
	eBS_MOVING = 0,
	eBS_HITTED,
	eBS_EXPLOSION,
	eBS_EXPLOSION_DONE,
	eBS_IDLE,
	eBS_DONE,
	eBS_TRACING_MONSTER,
	eBS_MAX,
};
//
class cPlayerBehaviorBase;
class cMiniGameBase;
class cMonster;
//
class	cBullet:public NamedTypedObject
{
private:
	Frame					m_BulletFrame;
	float					m_fAngle;
	Vector3					m_vPos;

	friend class			cBulletPlayer;
	friend class			cBulletManager;
	friend class			cWeb;
	//web
	bool					m_bDoCollided;
	cMPDI*					m_pWebMPDI;
	cbtShapeCollision*		m_pWebCllision;
	void					CastNetFishing();
	void					WebUpdate(float e_fElpaseTime);
	//bullet
	eBulletStatus			m_eBulletStatus;
	cbtShapeCollision*		m_pBulletCollision;
	cPlayerBehaviorBase		*m_pPlayer;
	cMPDI*					m_pBulletMPDI;
	Vector3					m_vDirection;
	float					m_fSpeed;
	void					SetDirection( Vector3 e_vDirection );
	void					SetDirection( float e_fAngle );
	void					BulletUpdate( float e_fElpaseTime );
	void					SetTransform();
	//bullet level,useless just for future function
	int						m_iPowerLevel;
	//how many will be paid
	int						m_iPayRate;
	int						m_iNumMonsterShoot;
	//
public:
	//
	cBullet( cMPDI* e_pBulletMPDI, cbtShapeCollision* e_pBulletCollision, float e_fSpeed, cMPDI* e_pWeb, cbtShapeCollision* e_pWebCollision );
	virtual ~cBullet();
	//
	virtual	void	Init();
	virtual	void	Update( float e_fElpaseTime );
	virtual	void	Render();
	virtual	void	DebugRender();
	virtual	void	Destroy();
	//
	void			Fire(cPlayerBehaviorBase*e_pPlayer,Vector3 e_vStartPos,Vector3 e_vDirection);
	void			Fire(cPlayerBehaviorBase*e_pPlayer,Vector3 e_vStartPos,float e_fAngle);
	virtual	bool	Collide(cbtShapeCollision*e_pbtShapeCollision);
	int				GetPayRate(){return m_iPayRate;}
	int				GetNumMonsterShoot(){ return m_iNumMonsterShoot; };
};