#ifndef	_BLUFFING_DICE_GAME_PHASE_
#define	_BLUFFING_DICE_GAME_PHASE_

//<>
//<DiceTableData FileName="">
//<AIData WinCallPercentage="0.6" SkipStep="1,2" VisibleDice="5,6" />
//

class	cDiceTable;
class	cBluffingDiceAI;
class	cAIDiceTableCallResult;
class	cSpecialDiceRule;
struct	sDiceRule;

class	cBluffingDiceGame:public cSimplePhase,public ISAXCallback
{
	enum	eCatchType
	{
		eCT_AI_CATCH = 0,
		eCT_PLAYER_CATCH,
		eCT_NO_CATCH
	};
	struct	sCallBehavior
	{
		std::wstring						m_strImagePrefixName;
		cBaseImage*							pImage;
		Vector3								vImagePos;
		cSubMPDI*							pSubMPDI;
		sCallBehavior(){ Init(); }
		~sCallBehavior(){}
		void	Init(){pImage = 0;pSubMPDI = 0;}
	};
	cResourceStamp							m_ResourceStamp;
	std::string								m_strFileName;
	//
	cSpecialDiceRule*						m_pSpecialDiceRule;
	int										m_iCallDicePoint;
	int										m_iCallDiceCountIndex;
	sCallBehavior							m_CallDicePointBehavior;
	sCallBehavior							m_CallDiceCountBehavior;

	cSubMPDI*								m_pPlayerCallSubMPDI;
	cSubMPDI*								m_pAICallSubMPDI;
	cSubMPDI*								m_pAICatchSubMPDI;
	cSubMPDI*								m_pPlayerCatchSubMPDI;
	cMPDI*									m_pPlayerWin;
	cMPDI*									m_pAIWin;

	cPuzzleImage*							m_pDiceImagePI;
	//
	cDiceTable*								m_pPlayerDiceTable;
	cImageButton*							m_pCallImageButton;
	cImageButton*							m_pCatchImageButton;
	cBasicSound*							m_pCallClickLegalSound;
	cBasicSound*							m_pCallClickInlegalSound;
	cClickMouseBehaviorList<cImageButton>*	m_pFailedMenuButtonList;
	//m_AIDiceTable m_pBluffingDiceAI conbine together,here AI not create as many many as m_AIDiceTable's size,
	//its because all AI should be same data or it's easy to fail by one of them.
	std::vector<cAIDiceTableCallResult*>	m_AIDiceTableCallResultVector;
	cBluffingDiceAI*						m_pBluffingDiceAI;
	//
	sDiceRule*								m_pDiceRule;
	//
	virtual	void							HandleElementData(TiXmlElement*e_pTiXmlElement);
	void									ProcessDiceRule(TiXmlElement*e_pTiXmlElement);
	void									ProcessDiceTableData(TiXmlElement*e_pTiXmlElement);
	void									ProcessDiceAIData(TiXmlElement*e_pTiXmlElement);
	void									ProcessImageData(TiXmlElement*e_pTiXmlElement);
	void									ProcessSoundData(TiXmlElement*e_pTiXmlElement);
	//
	int										m_iTurnToCallIndex;//0 for player,1-n for AI,if reach m_AIDiceTable.size back to 0
	//
	cPuzzleImage*							m_pGameResultImagePI;
	eCatchType								m_eCatchType;
	int										m_iWinIndex;//0 for player
	bool									IsCatchWin();
	//
	void									CallDiceImageUpdate(int e_iCount,int e_iPoint);
	void									DiceCallUpdate(float e_fElpaseTime);
	void									DiceCatchUpdate(float e_fElpaseTime);
	//
public:
	cBluffingDiceGame(char*e_strGameSetupFileName);
	~cBluffingDiceGame();
	virtual	void	Destroy();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Init();							//it would be called if the stage is changed.
	virtual	void	Render();
	virtual	void	DebugRender();
	virtual	void	MouseDown(int e_iX,int e_iY);
	virtual	void	MouseUp(int e_iX,int e_iY);
	virtual	void	MouseMove(int e_iX,int e_iY);
	virtual	const   WCHAR*	GetNextPhaseName();
};

#endif