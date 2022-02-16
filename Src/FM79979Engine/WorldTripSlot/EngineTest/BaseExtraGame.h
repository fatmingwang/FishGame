#ifndef _BASE_EXTRA_GAME_H_
#define _BASE_EXTRA_GAME_H_


enum	eFreeGameList
{
	eFGL_EXPLOSION = 0,
	eFGL_PICK_ROCK_BONUS = 1,
	eFGL_MINING = 2,
	eFGL_PICKAXE = 3,
	eFGL_WHOLE_TARGET_SAME = 4,
	eFGL_MAX = 5,
};

class	cBaseExtraGame:public cClickMouseBehavior
{
protected:
	eFreeGameList	m_eFreeGameList;
	//
	std::wstring	m_strConditionName;
	int				m_iConditionCount;
	bool	m_bStart;
	bool	m_bDone;
	//
	virtual	bool	InternalConditionCheck();
public:
	DEFINE_TYPE_INFO();
	cBaseExtraGame(const WCHAR*e_strConditionName,int e_iConditionCount);
	~cBaseExtraGame();
	bool			IsDone();
	void			SetDone(bool e_bDone){ m_bDone = e_bDone; }
	void			Start();
	//check condition is ok or not
	bool	IsConditionOk();
	//just init for basic parameter not real into this ExtraGame
	virtual	void	Init();
	//now condition is ok for into this game,parse resource
	virtual	void	LoadResource() = 0;
	virtual	void	Destroy() = 0;
	virtual	void	Update(float e_fElpaseTime) = 0;
	virtual	void	Render() = 0;

};

#endif