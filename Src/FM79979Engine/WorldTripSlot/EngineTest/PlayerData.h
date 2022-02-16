#ifndef _PLAYER_DATA_H_
#define _PLAYER_DATA_H_

//================================
//
//================================

enum	eJPType
{
	eJPT_NETWORK = 0,
	eJPT_LOCAL,
	eJPT_MAX,
};

eJPType	GetJPType(const WCHAR*e_strName);

class	cPlayerData:public NamedTypedObject
{
	//struct	sMoney
	//{
	//	iPrevoiusMoney;
	//	iCurrentMoney;
	//};
	UT::sTimeCounter	m_TimeToSaveCurrentMoney;
	int					m_iPervoisMoney;
	int					m_iPervoisNetWorkJPMoney;
	cBinaryFile*		m_pMoneyFile;
	cBinaryFile*		m_pJPFile;
	//for net work,eJPT_NETWORK
	int				m_iNetWorkJPMoney;
	//for 10 combo,eJPT_LOCAL
	int				m_iLocalJPMoney;
	int				m_iMaxJPMoney;
public:
	cPlayerData();
	~cPlayerData();
	void			OpenJP();
	void			OpenPlayerMoney();
	void			ClosePlayerMoney();
	int				m_i64Money;
	void			Update(float e_fEplaseTime);
	void			WritePlayerMoney();
	void			WriteJP();
	int*			GetJP(eJPType e_eJPType);
	void			SetJP(eJPType e_eJPType,int e_iValue);
	int*			GetJPOpenCount(eJPType);
};

#endif