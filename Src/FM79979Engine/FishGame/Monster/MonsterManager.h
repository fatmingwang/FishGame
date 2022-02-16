#ifndef _cMonsterManager_H_
#define _cMonsterManager_H_
#include "Monster.h"
#include "FishDiedEffect.h"
#include "MonsterGenerate.h"


class	cBulletDataAndBankRepositorManager;
struct	sHittedFishData
{
	cPlayerBehaviorBase	*m_pPlayer;
	cBullet				*m_pBullet;
	cMonster			*m_pMonster;
};

class	cMonsterManager:public cNamedTypedObjectVector<cMonster>,public cNodeISAX
{
private:
	struct	sOccupiedCount
	{
		int	iOccupiedCount[eBT_MAX];
	};
	//each type'sfish on the screen
	int				m_iCurrentBodyTypeCount[eBT_MAX];
	std::map<eBodyType,sOccupiedCount>	m_BT_OccupiedCount;
	int				m_iMaxBodyTypeCount[eBT_MAX];

	friend class	cBulletManager;
	friend class	cProbabilityFish;
	friend class	cProbability_Fatming;
	friend class	cProbability_MasterLee;
	friend class	cFishGroup;
	friend class	cBulletDataAndBankRepositorManager;
	//
	virtual	bool					MyParse(TiXmlElement*e_pRoot);	
	//from data.
	cNamedTypedObjectVector< cNamedTypedObjectVector<cMonster> >	m_AllFish;
	cMonster*						ProcessMonster(TiXmlElement*e_pTiXmlElement,const WCHAR*e_strTypeID,const WCHAR*e_strFishName,int e_iFishID);
	int								m_iNumMonsterAlive;
	int								m_iMaxMonsterAlive;
	int								m_iSpecialScenceMaxMonsterAlive;
	int								m_iNumMonsterVisible;
	std::map<int,int>				m_iMaxMonsterAliveWithPlayerCount;
	//just a reference
	std::map<int,int>				m_iEachMonsterReserve;
	cMonster**						m_ppWorkingMonster;
	//for hit
	int								m_iNumMonsterHitted;
	sHittedFishData					*m_pHittedFish;//amount is m_iMaxMonsterAlive
	//
	//
	cNamedTypedObjectVector<cFishGroup>*	m_pAllFishGroupFile;
	cFishGroup*						m_pMultiFishGroup;
	//alive monster prize
	int								m_iTotalAliveMonsterPrize;
	//
	cFishDiedEffect*				m_pFishDiedEffect;
public:
	cMonsterManager();
	virtual ~cMonsterManager();
	//
	static			cFAIBehaviorParser* m_spFAIBehaviorParser;
	void			MonsterLeaveScene();
	void			Init();
	void			Update(float e_fElpaseTime);
	void			Render();
	//while monster died
	void			LastRender();
	void			DebugRender();
	void			Destroy();
	cMonster*		Require(const WCHAR*e_strFishName,bool e_bForceFetch = false);
	cMonster*		Require(int e_iFishIndex,bool e_bForceFetch = false);
	bool			RestMonsterToRequire(int e_iFishIndex,int e_iCount);
	bool			RestMonsterToRequire(const WCHAR*e_strName,int e_iCount);
	int				GetSmallestPrizeFishID(int*e_piPrize);
	int				GetSmallestPrizeFishIDFromAliveFish(int*e_piPrize);
	cMonster**		GetWorkingMonster(){ return m_ppWorkingMonster; }
	int				GetCountMonsterAlive(){ return m_iNumMonsterAlive; }
	int				GetCountMonsterVisible(){ return m_iNumMonsterVisible; }
	int				GetMaxMonsterAlive(){return  m_iMaxMonsterAlive;}

	//for row or column
	int								FishGroupGo(int e_iFishID,int e_iCount);
	//for single fish mpdi group
	int								FishGroupGo(WCHAR*e_strFishName,int e_iTargetCount);
	//for multi fish mpdi group
	int								FishGroupGo();


	int				m_BigWinEmitterDisplay[20];
	cPrtEmitter*	m_pBigWinEmitter[20];
	void			DisplayBigWinParticle( Vector3 DisplayPos );
	void			ClearAllMissileLockedRecordByPlayerID( int PlayerID );
	void			AddFishDiedPrizeAnimation(cMPDI*e_pMPDI,int e_iCount);
	void			ShowFishDiedPrizeAnimation(const WCHAR*e_strName,Vector3 e_vPos,float e_fAngle);
	//probability check
	sFishShowProbaility		m_FishShowProbabilityList;
};

#endif