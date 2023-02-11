#pragma once

#include "BulletPlayer.h"

class	cPlayerBehaviorBase;
class	cMonster;
class	cMonsterManager;








class cBulletManager:public cNamedTypedObjectVector<cBulletPlayer>,public cNodeISAX
{
private:
	friend class cBullet;
	c2DImageCollisionData		m_2DImageCollisionData;
	void						ProcessCollisionFile(TiXmlElement*e_pRoot);
	void						ProcessPlayerBullet(TiXmlElement*e_pRoot);
	void						ProcessWeb(TiXmlElement*e_pRoot);
	virtual	bool				MyParse(TiXmlElement*e_pRoot);
	int							m_iNumWorking;
	//for update only
	cBullet**					m_ppUpdateBulletData;
	//each play has its limit bullet count
	int							m_iEachPlayerLimitCount;
	//
public:
	static bool					m_sbCollideSkip;
	cBulletManager();
	virtual ~cBulletManager();
	//
	cNamedTypedObjectVector<cbtConcaveShapeList>	m_BulletCollisionData;
	cNamedTypedObjectVector<cbtConcaveShapeList>	m_WebCollisionData;
	int							GetEachPlayerLimitCount(){return  m_iEachPlayerLimitCount;}
	//
	//each play has its own bullet limit.
	cBullet*					Require( int e_iPlayerIndex, int e_iLevel, int e_iPayRate );
	void						Init();
	void						Update(float e_fElpaseTime);
	void						Render();
	void						DebugRender();
	void						Destroy();
	void						Collide( cMonsterManager* e_pMonsterManager );
	//

	//================ Missile Added By Terry Begin =====================================================
	int							GetWorkingNum(){ return m_iNumWorking;}

	cBullet*					GetBulletData( int Idx ) { return m_ppUpdateBulletData[ Idx ];}
	// =============== Missile Added By Terry End =======================================================
};