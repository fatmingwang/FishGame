#ifndef _SINGLE_COIN_EFFECT_ANIMATION_H_
#define _SINGLE_COIN_EFFECT_ANIMATION_H_

class SingleCoinEffectAnimation
{
public:

	bool	    m_bPlayAnimation;	
 
	float	    m_AniDelayTime;
	float		m_MoveRemainTIme;

	cMPDI*      m_pCoinEffectMPDI;

	Frame	   m_CoinEffectFrame;

	cMatrix44   m_AniOriPos;
	cMatrix44   m_AniEndPos;

	SingleCoinEffectAnimation();
	~SingleCoinEffectAnimation();

	void Init(){};
	void Render();
	void Update( float ElapseTime );

	bool GetAnimationIsDone() { return !m_bPlayAnimation; }

	void CalculatorAnimationPos( float ElapseTime );
	void LoadCoinEffectAnimationData( const WCHAR* MPDIListName, const WCHAR* MPDIName );
	void SetCoinEffectMPDIData( cMatrix44& AniOriPos, cMatrix44& AniEndPos, float AniDelayTime );
};

#endif