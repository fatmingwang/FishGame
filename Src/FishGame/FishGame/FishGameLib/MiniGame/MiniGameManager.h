#pragma once

#include "MiniGameBase.h"

class cMiniGameManager:public ISAXCallback
{
	//
	virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement);
	void			ProcessMiniGame(TiXmlElement*e_pTiXmlElement);
	void			ProcessTrigger(TiXmlElement*e_pTiXmlElement);
	void			ProcessActiver(TiXmlElement*e_pTiXmlElement);
	//
	cTempContainer<cMiniGameTriggerBase>								m_MiniGameTriggerTempContainer;
	cTempContainer<cMiniGameActiverBase>								m_MiniGameActiverTempContainer;
	cNamedTypedObjectVector<cNamedTypedObjectVector<cMiniGameTriggerBase> >			m_MiniGameTriggerList;
	cNamedTypedObjectVector<cNamedTypedObjectVector<cMiniGameActiverBase> >			m_MiniGameActiverList;
	cNamedTypedObjectVector<cClickMouseBehavior>								m_MiniGameTriggerHotKeyList;
	cNamedTypedObjectVector<cClickMouseBehavior>								m_MiniGameActiverHotKeyList;
	sProbabilityVector<float>											m_MiniGameShowProbability;
	//
	//alow to attach mini game
	std::vector<int>													m_MiniGameFishIDList;
	UT::sMinMaxData<float>												m_fMiniGameDuration;
	UT::sTimeCounter													m_MiniGameMonsterTime;
	int																	m_iMaximumPayrate;
	int																	m_iMaximumShowOnScreen;
	//
	void																HoteyUpdate(float e_fElpaseTime);
	bool																MiniGameTriggerGo(int e_iMiniGameIndex,int e_iFishID);
	void																MiniGameMonsterGenerate(float e_fElpaseTime);
	//
	cMiniGameTriggerBase*												RequireMiniGameTrigger( int e_iIndex, cMonster* e_pMonster);
	cMiniGameActiverBase*												RequireMiniGameActiverBase( int e_iIndex,int e_iPayRate,int e_iMoney,cPlayerBehaviorBase*e_pPlayer,cMonster*e_pMonster);
public:
	cMiniGameManager();
	virtual ~cMiniGameManager();

	void						Init();
	void						Update( float e_fElpaseTime );
	void						RenderActiver();
	void						RenderTrigger();
	bool						IsMiniGameUsing();
	//fetch payrate by mini game name
	int							GetMiniGamePayRate(const WCHAR*e_strValue);

	cMiniGameTriggerBase*		GetMiniGameTriggerBase( cMonster* e_pMonster );
	cMiniGameActiverBase*		GetMiniGameActiverBase( cPlayerBehaviorBase* e_pPlayerBehaviorBase );
	
	bool						RemoveMiniGameWorkingTrigger(cMonster* e_pMonster);
	cMiniGameTriggerBase*		RequireMiniGameTrigger( const WCHAR* e_strName, cMonster* e_pMonster );

	cMiniGameActiverBase*		RequireMiniGameActiverBase( const WCHAR* e_strName,int e_iPayRate,int e_iMoney,cPlayerBehaviorBase*e_pPlayer,cMonster*e_pMonster);
};