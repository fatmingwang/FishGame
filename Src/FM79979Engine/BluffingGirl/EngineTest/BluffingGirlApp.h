#ifndef GAME_APP_H
#define GAME_APP_H

	//class	cIABSetup;
	//class	cStorePhase;
	class	cBluffingGirlPhaseManager;
	class	cBluffingGirlApp:public cGameApp
	{
		cBluffingGirlPhaseManager*	m_pPhaseManager;
		//cIABSetup*		m_pIABSetup;
		//cStorePhase*		m_pIABMenu;
	protected:
		void	Update(float e_fElpaseTime);
		void	Render();
#ifdef WIN32
		virtual	void	OpenglInit(HWND e_Hwnd){}
#endif
	public:
#if defined(ANDROID)
		cBluffingGirlApp(ANativeActivity* e_pActivity,JNIEnv*e_pThreadEnv,jobject*e_pAppThreadThis,Vector2 e_vGameResolution,Vector2 e_vViewportSize );
#elif defined(WIN32)
		cBluffingGirlApp(HWND e_Hwnd,Vector2 e_vGameResolution = Vector2(1920,1080),Vector2 e_vViewportSize = Vector2(1920,1080));
#else
		cBluffingGirlApp(Vector2 e_vGameResolution = Vector2(IPHONE_RESOLUTION_WIDTH,IPHONE_RESOLUTION_HEIGHT),Vector2 e_vViewportSize = Vector2(IPHONE_RESOLUTION_WIDTH,IPHONE_RESOLUTION_HEIGHT));
#endif
		~cBluffingGirlApp();

		void	Init();
		virtual	void	MouseDown(int e_iPosX,int e_iPosY);
		virtual	void	MouseMove(int e_iPosX,int e_iPosY);
		virtual	void	MouseUp(int e_iPosX,int e_iPosY);		
		//void	Destory();

		//void	Run();

		void	KeyDown(char e_char);
		void	KeyUp(char e_char);
		//if true game exit
		bool	m_bLeave;
	};
#endif