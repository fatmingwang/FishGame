#ifndef _DOWN_SCREEN_H_
#define _DOWN_SCREEN_H_
#include "GameMessage.h"

	class	cBigWin;
	class	cShowLineBetMoney;
	class	cDownScreenEffect
	{
		cShowLineBetMoney*		m_pShowLineBetMoney;
		cGameMessage*			m_pGameMessage;
		cBigWin*				m_pBigWinMoneyMPDI;
		cPrtEmitter*			m_pCoinFountain;
		cBasicSound*			m_pCoinSound;

	public:
		cDownScreenEffect();
		~cDownScreenEffect();
		cGameMessage*			GetGameMessage(){return m_pGameMessage;}
		void	ChangeStatusByMoney(int e_iMoney);
		void	Update(float e_fElpaseTime);
		void	Render();
		bool	IsBigWinDone();
		void	DrawPerLineBetMoney();
	};

#endif