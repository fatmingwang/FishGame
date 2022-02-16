#ifndef GAME_APP_H
#define GAME_APP_H

#include "PlayerData.h"
#include "SlotBetBehavior.h"

enum	eLanguage
{
	e_TCN = 0,
	e_SCN,
	e_EN
};

class	cWorldTripSlotApp:public cGameApp
{
	cPhaseManager*m_pPhaseManager;
	void	ControlPanelSetup();
	void	ReleaseGame();
protected:
	void	Update(float e_fElpaseTime);
	void	Render();
#ifdef WIN32
	virtual	void	OpenglInit(HWND e_Hwnd){}
#endif
public:
#if defined(ANDROID)
	cWorldTripSlotApp(ANativeActivity* e_pActivity,JNIEnv*e_pThreadEnv,jobject*e_pAppThreadThis,Vector2 e_vGameResolution,Vector2 e_vViewportSize );
#elif defined(WIN32)
	cWorldTripSlotApp(HWND e_Hwnd,Vector2 e_vGameResolution = Vector2(1920,1080),Vector2 e_vViewportSize = Vector2(1920,1080));
#else
	cWorldTripSlotApp(Vector2 e_vGameResolution = Vector2(IPHONE_RESOLUTION_WIDTH,IPHONE_RESOLUTION_HEIGHT),Vector2 e_vViewportSize = Vector2(IPHONE_RESOLUTION_WIDTH,IPHONE_RESOLUTION_HEIGHT));	
#endif
	~cWorldTripSlotApp();
	void	Init();
	virtual	void	MouseDown(int e_iPosX,int e_iPosY);
	virtual	void	MouseMove(int e_iPosX,int e_iPosY);
	virtual	void	MouseUp(int e_iPosX,int e_iPosY);
	//void	Destroy();

	//void	Run();

	void	KeyDown(char e_char);
	void	KeyUp(char e_char);
	//if true game exit
	bool	m_bLeave;
	//
	cOrthogonalCamera									m_OrthogonalCamera;
	static cPlayerData*									m_spPlayerData;
	static eLanguage									m_seLanguage;
};
#endif