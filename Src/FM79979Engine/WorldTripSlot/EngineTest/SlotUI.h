#ifndef	_SLOT_UI_H_
#define	_SLOT_UI_H_

class	cWinmoneyLines;
class	cSlotUI
{
	enum	eInfoPosList
	{
		eIPL_TotalBetMoneyPos = 0,
		eIPL_WinMoneyPos,
		eIPL_NumLineBetPos,
		eIPL_PerLineBetMoneyPos,
		eIPL_PlayerMoneyPos,
		eIPL_JP1,
		eIPL_JP2,
		eIPL_MAX,
	};
	Vector3									m_vInfoFontPos[eIPL_MAX];
	int*									m_piInfoValue[eIPL_MAX];
	cNumerialImage*m_pNumerialImage;
	int										m_iNumLinesBet;
public:
	cSlotUI();
	~cSlotUI();

	void									SetBetLines(int e_iNumLines);
	void									Render();
	void									DebugRender();
	void									UpdateSelectedLines(float e_fElpaseTime);
	void									RenderBetLines();
	//how many lines we have bet,while betPhase this will show on the screen
	cWinmoneyLines*							m_pWinmoneyLines;
};

#endif