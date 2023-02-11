#ifndef _QUICK_INGO_H_
#define _QUICK_INGO_H_

class	cQuickInfo
{
	cClickMouseBehavior			m_ShowInfoKey;
	bool			m_bShowInfo;
	Vector3			m_vStartDrawPos;
public:
	cQuickInfo(Vector3 e_vStartDrawPos,unsigned char e_WakeUpKey);
	virtual ~cQuickInfo();
	void	Update();
	void	Render();
	void	DisableShowInfo(){ m_bShowInfo = false; }
};

#endif