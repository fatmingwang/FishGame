#ifndef _JP_EFFECT_H_
#define _JP_EFFECT_H_



class	cFixedRollingObjectManagerList;

class	cJPEffect
{
	cMPDI*m_pJPIn;
	cMPDI*m_pJPAddMoney;
	cNumerialImage*m_pNumerialImage;
	int				m_iMoney;
	bool			m_bStart;
	Vector2			m_vNumerialPos;
	cFixedRollingObjectManagerList*	m_pFixedRollingObjectManagerList;
public:
	cJPEffect(int e_iJPIndex);
	~cJPEffect();
	void	Init(int e_iMoney);
	void	Update(float e_fElpaseTime);
	void	Render();
	float	GetCurrentProgress();
	bool	IsDone();
	//while render
	void	SetNumerial(int e_iValue);
};


#endif