#ifndef _BULLET_PLAYER_H_
#define _BULLET_PLAYER_H_
#include "BulletData.h"
class	cBullet;

//implement PlayerBuller
class	cBulletPlayer:public NamedTypedObject
{
	friend class	cBulletManager;
public:
	struct	sBulletData
	{
		cbtShapeCollision*	m_pBulletCollision;
		cMPDI*				m_pBulletMPDI;
		float				m_fSpeed;
		cbtShapeCollision*	m_pbtWebCollision;
		cMPDI*				m_pWebMPDI;
	};
private:
	void									AddBulletData(sBulletData*e_pBulletData);
	std::vector<sBulletData*>				m_BulletDataList;
	//level,count.
	std::vector<std::vector<cBullet*>* >	m_AllBullet;
	cBullet*								Require(int e_iLevel);
	void									ProcessBullet(int e_iAmount);
public:
	cBulletPlayer();
	virtual ~cBulletPlayer();
	//void									Fire(cPlayer*e_pPlayer,Vector3 e_vStartPos,Vector3 e_vDirection,int e_iLevel);
	//virtual	void							Update(float e_fElpaseTime);
	//virtual	void							Render();
	//virtual	void							Init();
};

#endif