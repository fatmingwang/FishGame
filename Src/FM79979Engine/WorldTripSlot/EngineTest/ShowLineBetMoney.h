#ifndef _SHOW_LINE_BET_MONEY_H_
#define _SHOW_LINE_BET_MONEY_H_

	class	cShowLineBetMoney
	{
		cPuzzleImage*			m_pPI;
		std::vector<Vector3>	m_PathForShowPerLineBetMoney;
		//
		int						m_iPerLineBetMoney;
		cBaseImage*				m_pImage;
	public:
		cShowLineBetMoney();
		~cShowLineBetMoney();
		void	Init();
		void	Update(float e_fElpaseTime);
		void	Render();
	};

#endif
