#ifndef _CHOICE_GIRL_STAGE_DATA_H_
#define _CHOICE_GIRL_STAGE_DATA_H_
	//after select girl into this menu to select which stage is going to play
	class	cChoiceGirlStagePhase:public cSimplePhase,public ISAXCallback
	{
		class	cStageData:public NamedTypedObject
		{
		public:
			std::wstring	m_strStageMame;
			std::wstring	m_strImageMame;
			std::wstring	m_strGameBGMame;
			std::wstring	m_strVideoName;
			std::wstring	m_strGameSetupFileName;
			bool			m_bStageClear;//fetch file to detect file clear or not.
			cStageData(){ m_bStageClear = false; }
			~cStageData(){}
		};
		cNamedTypedObjectVector<cStageData>		m_StageDataVector;
		std::wstring			m_strCurrentSelectedGorlName;


		cResourceStamp			m_ResourceStamp;
		//
		cPuzzleImage*			m_pChoiceGirlStagePI;
		cScroller*				m_pScroller;
		//
		std::string				m_strFileName;
		void					ProcessChoiceGirlPhaseData(TiXmlElement*e_pTiXmlElement);
		void					ProcessStageData(TiXmlElement*e_pTiXmlElement);
		virtual	void			HandleElementData(TiXmlElement*e_pTiXmlElement);
	public:
		cChoiceGirlStagePhase();
		~cChoiceGirlStagePhase();

		virtual	void	Destroy();
		virtual	void	Update(float e_fElpaseTime);
		virtual	void	Init();							//it would be called if the stage is changed.
		virtual	void	Render();
		virtual	void	DebugRender();
		virtual	void	MouseDown(int e_iX,int e_iY);
		virtual	void	MouseUp(int e_iX,int e_iY);
		virtual	void	MouseMove(int e_iX,int e_iY);
		virtual	const   WCHAR*	GetNextPhaseName();
		//return cBluffingDiceGame data file name.
		virtual	void*	GetData();
		virtual	void	FetchData(const WCHAR*e_strPhaseName,void*e_pData);
	};

#endif