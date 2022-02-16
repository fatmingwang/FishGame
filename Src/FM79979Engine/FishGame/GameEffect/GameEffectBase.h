#ifndef _FISH_PRIZE_EFFECT_H_
#define _FISH_PRIZE_EFFECT_H_

////
////the start condition for inherit class to implement
//virtual	void				InternalInit() = 0;
////if m_bAnimationDone is false and start condition is actived,it will be actived.
//virtual	void				InternalUpdate(float e_fElpaseTime) = 0;
////if m_bAnimationDone is false and start condition is actived,it will be actived.
//virtual	void				InternalRender() = 0;

class cGameEffectBase: public cFMAnimationRule
{
	float						m_fPlayingTime;
public:
	cGameEffectBase();
	virtual ~cGameEffectBase();
	virtual	void				RearrangeTime(float e_fPercenttage){}
	virtual	void				RearrangeTimeByPercent(float e_fPercenttage){}
	virtual	void				RenderByGlobalTime(float e_fTime){}
	virtual	void				InvertOrder(){}
	virtual	float				GetEndTime(){ return this->m_fStartTime+m_fPlayingTime; }
	virtual	void				SetData(void*e_pPlayerBehaviorBase) = 0;
	virtual	NamedTypedObject*	Clone() = 0;
};

struct	sGameEffectInputData
{
	int			iPlayerID;
	cMatrix44	ShowMatrix;
	cMatrix44	DestinationMatrix;
	int			iValue;
	WCHAR*		strMonstrName;
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

#endif