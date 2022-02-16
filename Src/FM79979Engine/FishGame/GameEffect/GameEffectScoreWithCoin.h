#ifndef _SCORE_WITH_COIN_H_
#define _SCORE_WITH_COIN_H_

class	cGameEffectScoreWithCoin:public cGameEffectBase
{
	struct	sCoinSpinAnimationData
	{
		sCoinSpinAnimationData();
		~sCoinSpinAnimationData();
		bool						bDone;
		float						fAngle;
		int							iPuzzleImageUnitIndex;
		Vector3						vMoveDirection;
		Vector3						vEndPos;
		Vector3						vCurrntPos;
		float						fSpeed;
		Vector3						vCurrentPos;
		float						fCurrentTime;
		int							iCurrentIndex;
		void						SetData(Vector3 e_vStartPos,Vector3 e_vEndPos,float e_fSpeed,float e_fAngle);
		void						Update(float e_fElpaseTime,std::vector<cImageIndexOfAnimation::sImageIndexAndTimeGap>*e_pAnimationData);
		void						Render(cPuzzleImage*ePI);
	};

	struct	sValueImagePopup
	{
		int							iValue;
		cCurveWithTime				CurveWithTime;
		sValueImagePopup();
		~sValueImagePopup();
		void						Update(float e_fElpaseTime);
		void						Render(cNumerialImage*e_pNumerialImage);
	};

	std::list<sCoinSpinAnimationData>						m_CoinSpinAnimationDataList;
	cPuzzleImage*				m_pPI;
	friend class				cGameEffectManager;
	//
	virtual	void				InternalInit();
	virtual	void				InternalUpdate(float e_fElpaseTime);
	virtual	void				InternalRender();
	//
	cNumerialImage*				m_pNumerialImage;
	int							m_iValue;
	cCurveWithTime				m_NumerialJumpCurve;
	float						m_fSpeedToDestination;
public:
	DEFINE_TYPE_INFO();
	cGameEffectScoreWithCoin(cNumerialImage*e_pNumerialImage,float e_fSpeedToDestination);
	virtual ~cGameEffectScoreWithCoin();
	//Fish Name,player ID,Matrix
	virtual	void				SetData(void*e_pPlayerBehaviorBase);
	virtual	NamedTypedObject*	Clone(){return  0;}
	virtual	void				Update(float e_fElpaseTime);
};

#endif