#ifndef _MAP_VIEW_PHASE_H_
#define _MAP_VIEW_PHASE_H_

class cMapViewPhase:public cSimplePhase,public ISAXCallback
{
	std::wstring							m_strSelectedStageName;
	virtual	void							HandleElementData(TiXmlElement*e_pTiXmlElement);
	void									ProcessPhaseData(TiXmlElement*e_pTiXmlElement);
	void									ProcessSelection(TiXmlElement*e_pTiXmlElement);
	//
	int										m_iMPDIBGIndex;//0
	cMPDIList*								m_pMPFIList;
	cDoAllMouseBehaviorList<cImageButton>	m_SelectionButton;
	cMPDI*									m_pBGMPDI;
	cBasicSound*							m_pClickSound;
	std::string								m_strFileName;
	std::vector<DoButtonGoal_Callback*>		m_AllButtonFunction;
public:
	cMapViewPhase(const char*e_strMainMenuFileName);
	~cMapViewPhase();
	virtual	void	Destroy();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Init();							//it would be called if the stage is changed.
	virtual	void	Render();
	virtual	void	DebugRender();
	virtual	void	MouseDown(int e_iX,int e_iY);
	virtual	void	MouseUp(int e_iX,int e_iY);
	virtual	void	MouseMove(int e_iX,int e_iY);
	void			KeyDown(char e_char);
	virtual	const   WCHAR*	GetNextPhaseName();
	virtual	void*	GetData();
};

#endif