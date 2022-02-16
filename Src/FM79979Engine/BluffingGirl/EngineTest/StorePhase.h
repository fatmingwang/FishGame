#ifndef	_IAB_MENU_H_
#define	_IAB_MENU_H_

	class	cIABSetup;
	//
	class	cStorePhase:public ISAXCallback,public cSimplePhase
	{
		cPuzzleImage*	m_pIABPI;
		cScroller*		m_pScroller;
		void			ProcessIABMenuData(TiXmlElement*e_pTiXmlElement);
		void			ProcessScrollerData(TiXmlElement*e_pTiXmlElement);
		virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement);
		//
		cIABSetup*		m_pIABSetup;
		bool			m_bEnable;
	public:
		cStorePhase();
		~cStorePhase();
        //set data to correct
        void			Init();
		void			SetEnable(bool e_bEnable);
		bool			IsEnable(){ return m_bEnable; }
        void			Update(float e_fElpaseTime);
        void			Render();
        void			DebugRender();
		//
        void			MouseMove(int e_iPosX,int e_iPosY);
        void			MouseDown(int e_iPosX,int e_iPosY);
        void			MouseUp(int e_iPosX,int e_iPosY);
		bool			IsPurchaseItem();
		const			WCHAR*GetPurchaseProductName();
		cIABSetup*		GetIABSetup(){return m_pIABSetup;}
		virtual	void	FetchData(const WCHAR*e_strPhaseName,void*e_pData);
		virtual	const   WCHAR*	GetNextPhaseName();
		std::wstring	m_strLastPhaseName;
	};
#endif