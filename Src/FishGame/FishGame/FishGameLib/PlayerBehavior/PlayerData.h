#ifndef _cPlayerData_H_
#define _cPlayerData_H_
//
#include "PlayerBehaviorBase.h"

class	cPlayerManager;

class	cPlayerData : public cPlayerBehaviorBase
{
	friend class				cPlayerManager;
	//
	void						ProcessPlayerCoin();
	void						ProcessPlayer( TiXmlElement* e_pElement );
	void						ProcessPlayerData( TiXmlElement* e_pElement );
	void						ProcessWeapon( TiXmlElement* e_pElement );
	void						ProcessPlayerIO( TiXmlElement* e_pElement );
	void						ProcessPlayerBody( TiXmlElement* e_pElement );
private:
	cMPDI*						m_pPlayerUIMPDI;
	bool						m_bAutoPlay;
	bool						m_bIsEnergyPower;

	cFMWorkingObjectChanger<cMPDI>	m_PlayerBodyMPDIWorkingList;

	float						m_fLevelClickRepeatTime;
	float						m_fIsLevelKeyDownTC;
	float						m_fChangeLevelClickRepeatTime;

	UT::sTimeCounter			m_AutoFireTC;
	float						m_fAutoFireTime;
	float						m_fWeaponIdleTimeMax;	
	//
	void						KeyUpdate(float e_fElpaseTime);

	//

	struct	sCoinRange
	{
		int	iStart;
		int	iEnd;
	};
	std::vector<sCoinRange>		m_CoinRangeList;
	int							m_iCoinRangeIndex;

public:
	//
	cPlayerData( TiXmlElement* e_pElement );
	virtual ~cPlayerData();

	
	unsigned char				m_cInput_Right;
	unsigned char				m_cInput_Left;
	cClickMouseBehavior			m_FireClick;
	unsigned char				m_cInput_Level;
	unsigned char				m_cInput_Down;
	unsigned char				m_cInput_Up;

	cClickMouseBehavior			m_LevelClick;

	virtual	void		InternalRender();
	virtual	void		InternalInit();
	virtual	void		InternalUpdate(float e_fElpaseTime);
	virtual	void		Destroy();
	//
	virtual	void		AddScore( cMonster* Fish, int Money );

	void				SetEnergyPower(bool e_bIsEnergyPower,int e_iMultiplier);
	bool				IsEnergyPower(){return m_bIsEnergyPower;}
	void				AutoPlayUpdate(float e_fElpaseTime);

	cMatrix44			GetBulletPos();
	//while game start player's weapon level not we expect so adjust it,
	void				WeaponLVImageCheck();
};

#endif