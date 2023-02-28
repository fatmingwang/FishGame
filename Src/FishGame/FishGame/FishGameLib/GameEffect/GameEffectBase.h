#pragma once

////
////the start condition for inherit class to implement
//virtual	void				InternalInit() = 0;
////if m_bAnimationDone is false and start condition is actived,it will be actived.
//virtual	void				InternalUpdate(float e_fElpaseTime) = 0;
////if m_bAnimationDone is false and start condition is actived,it will be actived.
//virtual	void				InternalRender() = 0;

class cGameEffectBase: public cFMTimeLineAnimationRule
{
	float						m_fPlayingTime;
public:
	cGameEffectBase();
	virtual ~cGameEffectBase();
	virtual	void				UpdateByGlobalTime(float e_fGlobalTime)override{}
	virtual	void				RearrangeTime(float e_fPercenttage)override {}
	virtual	void				RearrangeTimeByPercent(float e_fPercenttage)override {}
	virtual	void				RenderByGlobalTime(float e_fTime)override {}
	virtual	void				InvertOrder()override {}
	virtual	float				GetEndTime()override { return this->m_fStartTime+m_fPlayingTime; }
	virtual	void				SetData(void*e_pPlayerBehaviorBase) = 0;
	virtual	NamedTypedObject*	Clone()override = 0;
};

struct	sGameEffectInputData
{
	int				iPlayerID;
	cMatrix44		ShowMatrix;
	cMatrix44		DestinationMatrix;
	int				iValue;
	const wchar_t*	strMonstrName;
};

typedef cGameEffectBase*    (*GameEffectParseFunction)(TiXmlElement*e_pElement);

class cGameEffectManager : public cNamedTypedObjectVector< cGameEffectBase >,public ISAXCallback
{
	GameEffectParseFunction	m_pGameEffectParseFunction;
	//
	virtual	void		HandleElementData(TiXmlElement*e_pTiXmlElement);
	void				ProvessGameEffect(TiXmlElement*e_pTiXmlElement);
	cGameEffectBase*	ProvessGameEffectCardMarqueeManager(TiXmlElement*e_pTiXmlElement);
							
	cTempContainer<cGameEffectBase>											m_WorkingGameEffectBase;
	cNamedTypedObjectVector< cNamedTypedObjectVector< cGameEffectBase > >	m_AllGameEffectBaseList;
	//not wait for calling,it's a always running game effect
	cNamedTypedObjectVector<cGameEffectBase>								m_AlwaysRunGameEffectBaseList;
public:

	cGameEffectManager();
	virtual ~cGameEffectManager();
	//after call this please call cGameEffectBase::SetData
	cGameEffectBase* RequirePrizeEffect( const WCHAR* e_strEffectTypeName,void*e_pData, bool e_bForceRequire = false );

	void Init();
	void Render();
	void Update( float ElapseTime );
};