#ifndef _GAME_MESSAGE_H_
#define _GAME_MESSAGE_H_

enum	eGameMessage
{
	eGM_IDLE = 0,
	eGM_FREE_TIME,
	eGM_SPIN,
	eGM_BIG_WIN,
	eGM_INSERT_COIN,
	eGM_MAX
};

class	cGameMessage
{
	cFMWorkingObjectChanger<cMPDI>	m_Status;
	eGameMessage	m_eGameMessage;
public:
	cGameMessage();
	~cGameMessage();
	eGameMessage	GetGameMessage(){return m_eGameMessage;}
	void		SetIdle();
	void		SetFreeTime(int e_iTime);
	void		SetSpin();
	void		SetBigWin();
	void		SetInsertCoin();
	void		Update(float e_fElpaseTime);
	void		Render();
};

#endif