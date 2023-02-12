#pragma once

class	cMonster;
class	cPlayerBehaviorBase;
class	cMiniGameManager;

class	cMiniGameTriggerBase:public cSimplePhase
{
	friend class			cMiniGameManager;
	cMPDI*					m_pHintMPDI;
	cMonster*				m_pMonster;
	std::wstring			m_strActiverMiniGameName;
	bool					m_bInProgress;
	void*					m_pExtraData;
public:
	DEFINE_TYPE_INFO();
	//cMiniGameTriggerBase(const WCHAR*e_strMPDILIstFileName,const WCHAR*e_strHintMPDIName,const WCHAR*e_strActiverMiniGameName);
	cMiniGameTriggerBase(TiXmlElement*e_pXMLElement);
	cMiniGameTriggerBase(cMiniGameTriggerBase*e_pMiniGameTriggerBase);
	CLONE_MYSELF(cMiniGameTriggerBase);
	virtual ~cMiniGameTriggerBase();
	virtual	void			Init();
	virtual	void			Update(float e_fElpaseTime);
	virtual	void			Render();
	virtual	const   WCHAR*	GetNextPhaseName(){ return 0; }
	virtual	void			SetupShowData(cMonster*e_pMonster);
	void*					GetExtraData(){return m_pExtraData;}
	void*					CloneExtraData(){ return 0; }
};



class	cMiniGameActiverBase:public cSimplePhase
{
	friend class						cMiniGameManager;
protected:
	cBasicSound*						m_pSound;
	int									m_iMoney;
	int									m_iPayRate;
	cPlayerBehaviorBase*				m_pPlayer;
	std::vector<cMonster*>				m_HittedMonsterVector;
	cMPDI*								m_pMiniGameNameMPDI;
	cMPDI*								m_pEffectShowMPDI;
	bool								m_bInProgress;
	void*								m_pExtraData;
	sProbabilityWithValue<int,int>		m_PrizeWithProbabilityVector;
	cMonster*							m_pMonster;
public:
	DEFINE_TYPE_INFO();
	cMiniGameActiverBase(TiXmlElement*e_pXMLElement);
	cMiniGameActiverBase(cMiniGameActiverBase*e_pMiniGameActiverBase);
	CLONE_MYSELF(cMiniGameActiverBase);
	//cMiniGameActiverBase(const WCHAR*e_strMPDILIstFileName,const WCHAR*e_strMiniGameNameMPDI,const WCHAR*e_strEffectShowMPDI);
	virtual ~cMiniGameActiverBase();
	virtual	void			Init();
	virtual	void			Update(float e_fElpaseTime);
	virtual	void			Render();
	virtual	const   WCHAR*	GetNextPhaseName(){ return 0; }
	virtual	void			SetupShowData(int e_iPayRate,int e_iMoney,cPlayerBehaviorBase*e_pPlayer,cMonster*e_pMonster);
	int						GetMoney(){return m_iMoney;}
	void					SetMoney(int e_iMoney){m_iMoney = e_iMoney;}
	void*					GetExtraData(){return m_pExtraData;}
	int						ProcessMiniGameMonsterDataWithNewPrize();
	void*					CloneExtraData(){ return 0; }	
};