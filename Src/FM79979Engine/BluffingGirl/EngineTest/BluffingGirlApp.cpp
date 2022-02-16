#include "stdafx.h"
#include "BluffingGirlApp.h"
#include "AllPhaseDefine.h"
#include "MainMenuPhase.h"
#include "IABSetup.h"
#include "StorePhase.h"
#include "FBPhase.h"
#include "ChoiceGirlPhase.h"
#include "BluffingGirlPhaseManager.h"
#include "SetupPhase.h"
#include "ChoiceGirlPhase.h"
#include "ChoiceGirlStagePhase.h"
#include "BluffingDiceGame.h"
POINT	g_RealMousePos = {0,0};
#if defined(ANDROID)
cBluffingGirlApp::cBluffingGirlApp(ANativeActivity* e_pActivity,JNIEnv*e_pThreadEnv,jobject*e_pAppThreadThis,Vector2 e_vGameResolution,Vector2 e_vViewportSize ):cGameApp(e_pActivity,e_pThreadEnv,e_pAppThreadThis,e_vGameResolution,e_vViewportSize)
#elif defined(WIN32)
cBluffingGirlApp::cBluffingGirlApp(HWND e_Hwnd,Vector2 e_vGameResolution,Vector2 e_vViewportSize):cGameApp(e_Hwnd,e_vGameResolution,e_vViewportSize)
#else
cBluffingGirlApp::cBluffingGirlApp(Vector2 e_vGameResolution,Vector2 e_vViewportSize):cGameApp(e_vGameResolution,e_vViewportSize)
#endif
{
	m_bLeave = false;
	m_pPhaseManager = new cBluffingGirlPhaseManager();
	//m_pIABSetup = new cIABSetup("IAB/IABSetup.xml");
#ifdef DEBUG
	this->m_sbSpeedControl = true;
#endif
}

cBluffingGirlApp::~cBluffingGirlApp()
{
	SAFE_DELETE(m_pPhaseManager);
	Destroy();
}

void	cBluffingGirlApp::Init()
{
	cGameApp::Init();
	//common file parse
	cMPDIList*l_pCommonMPDIList = cGameApp::GetMPDIListByFileName(L"Image/Common.mpdi");
	//
	cMainMenuPhase*l_pMainMenuPhase = new cMainMenuPhase("PhaseData/MainMenuPhase.xml");
	this->m_pPhaseManager->AddObjectNeglectExist(l_pMainMenuPhase);
	//
	cStorePhase*l_pIABMenu = new cStorePhase();	l_pIABMenu->Parse("IAB/IABMenu.xml");
	this->m_pPhaseManager->AddObjectNeglectExist(l_pIABMenu);
	//
	cSetupPhase*l_pSetupPhase = new cSetupPhase();
	this->m_pPhaseManager->AddObjectNeglectExist(l_pSetupPhase);
	//
	cFBPhase*l_pFBPhase = new cFBPhase();
	this->m_pPhaseManager->AddObjectNeglectExist(l_pFBPhase);
	//
	cChoiceGirlPhase*l_pChoiceGirlPhase = new cChoiceGirlPhase("PhaseData/ChoiceGirlPhase.xml");
	this->m_pPhaseManager->AddObjectNeglectExist(l_pChoiceGirlPhase);
	//
	cChoiceGirlStagePhase*l_pChoiceGirlStagePhase = new cChoiceGirlStagePhase();
	this->m_pPhaseManager->AddObjectNeglectExist(l_pChoiceGirlStagePhase);
	//
	cBluffingDiceGame*l_pBluffingDiceGame = new cBluffingDiceGame("PhaseData/GirlStage/GameSetup/EasyMode.xml");
	this->m_pPhaseManager->AddObjectNeglectExist(l_pBluffingDiceGame);
	//l_pBluffingDiceGame->Init();
	//
	//m_pPhaseManager->SetCurrentCurrentPhase(l_pMainMenuPhase->GetName());
	m_pPhaseManager->SetCurrentCurrentPhase(l_pBluffingDiceGame->GetName());
	//let first update is not too big
	this->m_sTimeAndFPS.Update();
}

void	cBluffingGirlApp::Update(float e_fElpaseTime)
{
    cGameApp::Update(e_fElpaseTime);
	this->m_pPhaseManager->Update(e_fElpaseTime);
}

void	cBluffingGirlApp::Render()
{
	glViewport(0,0,(int)m_svViewPortSize.Width(),(int)m_svViewPortSize.Height());
	//glScissor(0,0,(int)m_svViewPortSize.x,(int)m_svViewPortSize.y);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glClearColor( 0,0.5,0.5,1 );
	glClearDepth(1.0f);	
	glEnable(GL_TEXTURE_2D);
	float	l_fVP[16];
	glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y,l_fVP);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	//glAlphaFunc(GL_GREATER,0.001f);	
	cGameApp::Render();
	this->m_pPhaseManager->Render();
	glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y,l_fVP);
	if( cGameApp::m_spGlyphFontRender )
	{
	    WCHAR   l_str[MAX_PATH];
	    swprintf(l_str,MAX_PATH,L"MousePos:%d,%d",cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	    cGameApp::m_spGlyphFontRender->RenderFont(0,0,l_str);
	    cGameApp::m_spGlyphFontRender->RenderFont(0,40,UT::CharToWchar(cGameApp::m_sTimeAndFPS.GetFPS()));
	    swprintf(l_str,MAX_PATH,L"RealMousePos:%d,%d",g_RealMousePos.x,g_RealMousePos.y);
	    cGameApp::m_spGlyphFontRender->RenderFont(0,20,l_str);
	}
	glDisable(GL_TEXTURE_2D);
	this->m_pPhaseManager->DebugRender();
	glDisable2D();
#ifdef WIN32
	SwapBuffers(cGameApp::m_sHdc);
#endif
}

void	cBluffingGirlApp::MouseDown(int e_iPosX,int e_iPosY)
{
	g_RealMousePos.x = e_iPosX;
	g_RealMousePos.y = e_iPosY;
    cGameApp::MouseDown(e_iPosX,e_iPosY);
	this->m_pPhaseManager->MouseDown(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
}

void	cBluffingGirlApp::MouseMove(int e_iPosX,int e_iPosY)
{
	g_RealMousePos.x = e_iPosX;
	g_RealMousePos.y = e_iPosY;
    cGameApp::MouseMove(e_iPosX,e_iPosY);
	this->m_pPhaseManager->MouseMove(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
}

void	cBluffingGirlApp::MouseUp(int e_iPosX,int e_iPosY)
{
	g_RealMousePos.x = e_iPosX;
	g_RealMousePos.y = e_iPosY;
    cGameApp::MouseUp(e_iPosX,e_iPosY);
	this->m_pPhaseManager->MouseUp(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
}

void	cBluffingGirlApp::KeyDown(char e_char)
{
	 cGameApp::KeyDown(e_char);
}
void	cBluffingGirlApp::KeyUp(char e_char)
{
	cGameApp::KeyUp(e_char);
}