#ifndef	_DICE_TABLE_H_
#define	_DICE_TABLE_H_

struct	sDiceRule
{
	int		iMin;
	int		iMax;
	float	fPercentForSpecialNumeral1;
	int		iNumDice;
	int		iAllDiceCount;
	//<DiceRule Min="1" Max="6" OneDiceRule="0" 
	sDiceRule(TiXmlElement*e_pelement);
	~sDiceRule(){}
};

//game diffculy dice rand reference(more dice 1 more hard) (?%)
//the AI call Dice1 probability (?%0
//level to to call next dice?(1-n),ex: Dice 2 has 5 next to call Dice n has 5+n
class	cBluffingDiceAI;
class	cBluffingDiceGame;
class	cDiceTable
{
	//
	friend class								cBluffingDiceAI;
	friend class								cBluffingDiceGame;
	//
	cPuzzleImageUnit*							m_pSelectedNumeralImage;
	cPuzzleImageUnit*							m_pSelectedDiceImage;
	//
	sDiceRule*									m_pDiceRule;
	int											m_iCurrentSelectDiceCount;
	int											m_iCurrentSelectDicePoint;
	//
	void										ProcessImageData(TiXmlElement*e_pElement);
	void										ProcessPosData(TiXmlElement*e_pElement);
	//
	//
	cClickMouseBehaviorList<cImageButton>		m_DiceImageButtonContainer;
	cClickMouseBehaviorList<cImageButton>		m_NumeralImageButton;
	//
	Vector3										m_vMyDiceImagePos;
	float										m_fMyDiceImageGap;
	std::wstring								m_strMyDiceImagePrefixName;
	//
	std::vector<int>							m_MyDiceContent;
	cBehaviorObjectList<cBaseImage>				m_MyDiceContentImage;
	void										GenerateDiceContentImages();
	//
	cPuzzleImageUnit*							m_pBlockImage;
	cPuzzleImage*								m_pPI;
	//if true render will be m_DiceImageVector or m_pBlockImage
	bool										m_bReveal;
	//
	bool										IsCallConditionOk(int e_iCurrentDicePoint,int e_iCurrentDiceCount,int e_iNewDicePoint,int e_iNewDiceCount,int e_iTotalPlayer);
public:
	//int e_iNumDice,float e_fPrecent
	cDiceTable(TiXmlElement*e_pElement,sDiceRule*e_pDiceRule);
	~cDiceTable();
	void										Init();
	void										Update(float e_fElpaseTime,int e_iCurrentDicePoint,int e_iCurrentDiceCount,int e_iTotalPlayer);
	void										Render();
	void										DebugRender();
	void										MouseDown(int e_iX,int e_iY);
	void										MouseUp(int e_iX,int e_iY);
	void										MouseMove(int e_iX,int e_iY);
	int											GetCurrentSelectNumeral();
	int											GetCurrentSelectDice();
	void										SetReveal(bool e_bReveal){m_bReveal = e_bReveal;}
	bool										IsReveal(){return m_bReveal;}
	bool										IsConditionOk();
	void										SetDiceContent(std::vector<int> e_DiceContent);
};

#endif