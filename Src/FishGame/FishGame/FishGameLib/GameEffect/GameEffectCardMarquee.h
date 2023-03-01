#pragma once


enum	eQueueCardStatus
{
	eQCS_SHOW,
	eQCS_DISAPPEAR,
	eQCS_MAX
};

class	cGameEffectPlayerCardMarquee:public NamedTypedObject
{
	struct	sCardData
	{
		sCardData();
		~sCardData();
		cMatrix44*			pMat;
		void				SetData(Vector3	e_vPos,cPuzzleImageUnit*e_pPuzzleImageUnit,float e_fTimeToShow,float e_fTimeToDisappear,int e_iValue,cMatrix44*pMat);
		UT::sTimeCounter	TimeToShow;
		UT::sTimeCounter	TimeToDisappear;
		eQueueCardStatus	QueueCardStatus;
		cPuzzleImageUnit*	PuzzleImageUnit;
		Vector3				vPos;
		int					iValue;
		void				Update(float e_fElpaseTime,Vector3 e_vMovePos);
		void				Render(cPuzzleImage*e_pPI,cNumeralImage*e_pNumerialImage);
		float				fImageAngle;
	};
	cMatrix44							m_ShowMatrix;
	sMoveToDestTimeCounter<Vector3>		m_MoveTC;
	std::list<sCardData>				m_CardDataList;
	UT::sTimeCounter					m_AfterMoveHoldTime;
	int									m_iViewCount;
	//if working count is bigger than ViewableCount elpase time will speed up
	int									m_iMaximumCount;
	cPuzzleImage*						m_pCardPI;
	cNumeralImage*						m_pNumerialImage;
	//
	void								ResetCardPos();
public:
	cGameEffectPlayerCardMarquee(cPuzzleImage*e_pCardPI,int e_iMaximumCount,float e_fTimeToDestination,Vector3 e_vStartPos,Vector3 e_vDestination,float e_fHoldTime,int e_iViewCount,cMatrix44 e_matShowMatrix,cNumeralImage*e_pNumerialImage);
	virtual ~cGameEffectPlayerCardMarquee();
	void					Init();
	void					AddData(const wchar_t*e_strFishName,int e_iValueForShow);
	void					Update(float e_fElpaseTime);
	void					Render();
};

class	cGameEffectManager;
class	cGameEffectCardMarqueeManager:public cGameEffectBase
{
	friend class	cGameEffectManager;
	cNamedTypedObjectVector<cGameEffectPlayerCardMarquee>	m_GameEffectPlayerCardMarqueeList;
	virtual	void				InternalInit()override;
	virtual	void				InternalUpdate(float e_fElpaseTime)override;
	virtual	void				InternalRender()override;
	//
	cNumeralImage*				m_pNumerialImage;
public:
	DEFINE_TYPE_INFO();
	cGameEffectCardMarqueeManager(cNumeralImage*e_pNumerialImage);
	virtual ~cGameEffectCardMarqueeManager();
	//Fish Name,player ID,Matrix
	virtual	void				SetData(void*e_pPlayerBehaviorBase)override;
	virtual	NamedTypedObject*	Clone()override {return  0;}
	virtual	void				Update(float e_fElpaseTime)override;
};

