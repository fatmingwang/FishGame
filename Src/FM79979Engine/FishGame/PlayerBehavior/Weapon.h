#ifndef _cFishPlayerWeapon_H_
#define	_cFishPlayerWeapon_H_

#include "PlayerData.h"

class	cBulletFireStatus
{
	friend class cFishPlayerWeapon;
	//
	cMPDI*						m_pPlayerLvUIMPDI;
	cNumerialImage*				m_pLevelNumberImage;
	struct	sLevelRange
	{
		int	iStart;
		int	iEnd;
		int	iAddValueRate;
	};

	int							m_iCurrentLevel;
	int							m_iLastCurrentLevel;
	int							m_iLevelRangeIndex;
	std::vector<sLevelRange>	m_LevelRangeList;
	Vector3						m_vLevelPos;
	Frame*						m_pmatPlayerFrame;
	void						LevelCheck();
public:
	cBulletFireStatus(TiXmlElement* e_pElement,Frame*e_pPlayerFrame);
	virtual ~cBulletFireStatus();

	void						LevelChange(bool e_bLevelChange);
	bool						IsLevelChange();
	void						Update(float e_fElpaseTime);
	void						Render();
};


class	cFishPlayerWeapon:public cPlayerWeapon
{
	cBulletFireStatus*				m_pBulletFireStatus;
	Frame*							m_pPlayerFrame;
	Frame							m_WeaponFrame;
	Vector3							m_vOffsetPos;
	float							m_fTubeHeight;
	int								m_iLevelIndex;
	//
	void							DirectionChangeUpdate(float e_fElpaseTime);
	float							m_fOffsetAngle;
	float							m_fAngle;
	float							m_fRotateSpeed;
	float							m_fCurrentAnimationTime;
	bool							m_bIsSetEnergyTube;

	Vector3							m_vBulletDirection;
	Vector3							m_vTubeOffsetPos;

	
	void							ProcessWeaponData(TiXmlElement* e_pElement);
	cFMWorkingObjectChanger<cMPDI>	m_IdleWeaponMPDIWorkingList;
	cFMWorkingObjectChanger<cMPDI>	m_ShotWeaponMPDIWorkingList;

	bool							m_bFire;
	void							CheckCurrentWeaponOffsetPos();
public:
	cFishPlayerWeapon(TiXmlElement* e_pElement,Frame*e_pPlayerFrame);
	virtual ~cFishPlayerWeapon();
	virtual	void					Init();
	void							Render();
	bool							IsFire();

	void							SetWeaponAnimationTime(float e_fCurrentAnimationTime){ m_fCurrentAnimationTime = e_fCurrentAnimationTime; };
	virtual void					SetEnergyTube(bool e_bIsSetEnergyTube);
	bool							IsSetEnergyTube(){return m_bIsSetEnergyTube;}
	void							AngleUpdate();
	virtual	bool					Fire(cPlayerBehaviorBase*e_pPlayerBehaviorBase);
	virtual	void					LevelUp(bool e_bLevelChange);
	virtual	int						GetPower();
	virtual	void					SetPower(int e_iCurrentLevel);
	Vector3							GetTubeOffsetPos(){return m_vTubeOffsetPos;};
	float							GetTubeAngle(){return m_fAngle;};
	//
	virtual	void					Update(float e_fElpaseTime);
	
};


#endif