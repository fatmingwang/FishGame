#ifndef _BIG_WIN_H
#define	_BIG_WIN_H


	class	cBigWin
	{
		cMPDI*	m_pBumpOut;
		cMPDI*	m_pBumpLoop;
	public:
		cBigWin(cMPDIList*e_pMPDIList);
		~cBigWin();
		void			Init();
		bool			IsStart();
		void			Update(float e_fElpaseTime);
		void			Render();
		bool			IsAnimationDone();
	};

#endif