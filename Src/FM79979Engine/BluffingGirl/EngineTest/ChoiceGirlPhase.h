#ifndef _CHOICE_GIRL_PHASE_H_
#define _CHOICE_GIRL_PHASE_H_

class	cChoiceGirlPhase:public cSimplePhase,public ISAXCallback
{
	//
	cResourceStamp			m_ResourceStamp;
	//
	std::wstring			m_strSelectedGirlName;
	//
	cPuzzleImage*			m_pChoiceGirlPI;
	cScroller*				m_pScroller;
	//
	std::string				m_strFileName;
	void					ProcessChoiceGirlPhaseData(TiXmlElement*e_pTiXmlElement);
	void					ProcessGirlData(TiXmlElement*e_pTiXmlElement);
	virtual	void			HandleElementData(TiXmlElement*e_pTiXmlElement);
public:
	cChoiceGirlPhase(const char*e_strFileName);
	~cChoiceGirlPhase();
	virtual	void			Destroy();
	virtual	void			Update(float e_fElpaseTime);
	virtual	void			Init();							//it would be called if the stage is changed.
	virtual	void			Render();
	virtual	void			DebugRender();
	virtual	void			MouseDown(int e_iX,int e_iY);
	virtual	void			MouseUp(int e_iX,int e_iY);
	virtual	void			MouseMove(int e_iX,int e_iY);
	virtual	const	WCHAR*	GetNextPhaseName();
	//get selected girl name
	virtual	void*			GetData();
};

#endif