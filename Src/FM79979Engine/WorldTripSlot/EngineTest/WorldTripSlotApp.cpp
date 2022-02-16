#include "stdafx.h"
#include "WorldTripSlotApp.h"
#include "TestPhase.h"
#include "SlotGamePhase.h"
#include "PlayerData.h"
#include "MainMenuPhase.h"
#include "MapViewPhase.h"
bool	g_bPause = false;
bool	g_bRenderScreen = true;

cPlayerData*								cWorldTripSlotApp::m_spPlayerData = 0;
eLanguage									cWorldTripSlotApp::m_seLanguage = e_TCN;

cFrameBuffer*								g_pFrameBuffer = 0;
//for limimt viewport
Vector4										g_vViewRect;
POINT										g_TouchPoint = {0,0};
POINT										g_RealMousePosition = {0,0};

cMPDI*										g_pMPDITest = 0;
#if defined(ANDROID)
cWorldTripSlotApp::cWorldTripSlotApp(ANativeActivity* e_pActivity,JNIEnv*e_pThreadEnv,jobject*e_pAppThreadThis,Vector2 e_vGameResolution,Vector2 e_vViewportSize ):cGameApp(e_pActivity,e_pThreadEnv,e_pAppThreadThis,e_vGameResolution,e_vViewportSize)
#elif defined(WIN32)
cWorldTripSlotApp::cWorldTripSlotApp(HWND e_Hwnd,Vector2 e_vGameResolution,Vector2 e_vViewportSize):cGameApp(e_Hwnd,e_vGameResolution,e_vViewportSize)
#else
cWorldTripSlotApp::cWorldTripSlotApp(Vector2 e_vGameResolution,Vector2 e_vViewportSize)
:cGameApp(e_vGameResolution,e_vViewportSize)
#endif
{
	cGameApp::OutputDebugInfoString(L"App Name is WorldTripSlot");
	m_sstrGameAppName = L"WorldTripSlot";
	m_bLeave = false;
	this->m_sbSpeedControl = true;
	m_pPhaseManager = new cPhaseManager();
	m_dbGamePlayTime = 0;
	m_spPlayerData = new cPlayerData();
}

void	cWorldTripSlotApp::ReleaseGame()
{
	WRITELOGFILE("clean resource");
	SAFE_DELETE(m_spPlayerData);
	//
	SAFE_DELETE(g_pFrameBuffer);
	this->m_spAnimationParser->Destroy();

}

cWorldTripSlotApp::~cWorldTripSlotApp()
{
	SAFE_DELETE(m_pPhaseManager);
	ReleaseGame();
}

void	cWorldTripSlotApp::Init()
{
	cGameApp::Init();

	if(0)
	{
		cMPDIList*l_pMPDIList = cGameApp::GetMPDIListByFileName(L"C:/Users/wh-n1/Desktop/Project/Media/MyFMBook/iPhone_Monster/MPDI/Page3.mpdi");
		g_pMPDITest = l_pMPDIList->GetObject(L"2");
		g_pMPDITest->Init();
	}
	else
	{
		cSlotGamePhase*l_pSlotGamePhase = new cSlotGamePhase();
		this->m_pPhaseManager->AddObjectNeglectExist(l_pSlotGamePhase);

		//cMainMenuPhase*l_pMainMenuPhase = new cMainMenuPhase("WorldTripSlot/PhaseData/MainMenuPhase.xml");
		//this->m_pPhaseManager->AddObjectNeglectExist(l_pMainMenuPhase);
		//cMapViewPhase*l_pMapViewPhase = new cMapViewPhase("WorldTripSlot/PhaseData/MapViewPhase.xml");
		//this->m_pPhaseManager->AddObjectNeglectExist(l_pMapViewPhase);
		//m_pPhaseManager->SetCurrentCurrentPhase(l_pMainMenuPhase->GetName());

		m_pPhaseManager->SetCurrentCurrentPhase(l_pSlotGamePhase->GetName());
	}
	this->m_sTimeAndFPS.Update();
	this->m_sTimeAndFPS.Update();
}

void	cWorldTripSlotApp::Update(float e_fElpaseTime)
{
	m_dbGamePlayTime += e_fElpaseTime;
	if( m_sucKeyData['P'] )
		this->m_spLogFile->WriteToFileImmediatelyWithLine("GamePaused!!",true);
	if( g_pMPDITest )
		g_pMPDITest->Update(e_fElpaseTime);
	if( m_pPhaseManager )
		this->m_pPhaseManager->Update(e_fElpaseTime);
	if( g_bPause )
		e_fElpaseTime = 0.f;
	//MyUpdate(e_fElpaseTime);	
}

extern POINT g_MousePosition;

void	cWorldTripSlotApp::Render()
{
	if( !g_bRenderScreen )
		return;
	glViewport(0,0,(int)m_svViewPortSize.Width(),(int)m_svViewPortSize.Height());
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glClearColor( 0,0,0,1 );
	glClearDepth(1.0f);	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable2D(4048,2024);
	if( g_pMPDITest )
		g_pMPDITest->Render();
	if( m_pPhaseManager )
		m_pPhaseManager->Render();
#ifdef _DEBUG
	if( cGameApp::m_spGlyphFontRender )
	{

		WCHAR   l_str[MAX_PATH];
		swprintf(l_str,MAX_PATH,L"MousePos:%d,%d",cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
		cGameApp::m_spGlyphFontRender->RenderFont(0,0,l_str);
		swprintf(l_str,MAX_PATH,L"RealMousePos:%d,%d",g_RealMousePosition.x,g_RealMousePosition.y);
		cGameApp::m_spGlyphFontRender->RenderFont(0,200,l_str);
		swprintf(l_str,MAX_PATH,L"ViewPortMousePos:%d,%d",g_MousePosition.x,g_MousePosition.y);
		//cGameApp::m_spGlyphFontRender->RenderFont(200,300,l_str);
		m_spGlyphFontRender->SetScale(1.f);
		cGameApp::m_spGlyphFontRender->RenderFont(0,50,UT::CharToWchar(cGameApp::m_sTimeAndFPS.GetFPS()));
		swprintf(l_str,MAX_PATH,L"TouchPoint:%d,%d",g_TouchPoint.x,g_TouchPoint.y);
		//cGameApp::m_spGlyphFontRender->RenderFont(0,100,l_str);
		//cGameApp::m_spGlyphFontRender->RenderFont(200,20,ValueToStringW(this->m_siFreeGame));
		swprintf(l_str,MAX_PATH,L"GamePlayTime:%.3f",m_dbGamePlayTime);
		cGameApp::m_spGlyphFontRender->RenderFont(0,500,l_str);

	}
#endif
	if( g_pFrameBuffer )
	{
		g_pFrameBuffer->EndDraw();
		glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);
		glViewport((int)g_vViewRect.x,(int)g_vViewRect.y,(int)g_vViewRect.z,(int)g_vViewRect.w);
		POINT	l_vPos = {0,0};
		POINT	l_vSize = {(int)cGameApp::m_svGameResolution.x,(int)cGameApp::m_svGameResolution.y};
		g_pFrameBuffer->DrawBuffer(l_vPos,l_vSize);
	}

	glDisable(GL_TEXTURE_2D);
	if( m_pPhaseManager )
		this->m_pPhaseManager->DebugRender();
	glDisable2D();
#ifdef WIN32
	SwapBuffers(cGameApp::m_sHdc);
#endif
}

float	g_fOriginalViewPort = 0;
void	TouchPointConvert(int *e_piPosX,int* e_piPosY)
{
	if(g_pFrameBuffer)
	{
		g_fOriginalViewPort = cGameApp::m_svViewPortSize.x;
		if( *e_piPosX>g_vViewRect.x&&*e_piPosX<(g_vViewRect.z+g_vViewRect.x) )
		{
			float	l_fWidth = g_vViewRect.z;
			cGameApp::m_svViewPortSize.x = l_fWidth;
			*e_piPosX -= (int)g_vViewRect.x;
			g_TouchPoint.x = *e_piPosX;
			g_TouchPoint.y = *e_piPosY;
		}
		else
		{
			*e_piPosX= 0;
			*e_piPosY = 0;
		}
	}
}

void	TouchEnd()
{
	if(g_pFrameBuffer)
	{
		cGameApp::m_svViewPortSize.x = g_fOriginalViewPort;
	}
}

void	cWorldTripSlotApp::MouseDown(int e_iPosX,int e_iPosY)
{
	TouchPointConvert(&e_iPosX,&e_iPosY);
	cGameApp::MouseDown(e_iPosX,e_iPosY);
	if( m_pPhaseManager )
		m_pPhaseManager->MouseDown(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	TouchEnd();
}

void	cWorldTripSlotApp::MouseMove(int e_iPosX,int e_iPosY)
{
	g_RealMousePosition.x = e_iPosX;
	g_RealMousePosition.y = e_iPosY;
	TouchPointConvert(&e_iPosX,&e_iPosY);
	cGameApp::MouseMove(e_iPosX,e_iPosY);
	if( m_pPhaseManager )
		m_pPhaseManager->MouseMove(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	TouchEnd();
}

void	cWorldTripSlotApp::MouseUp(int e_iPosX,int e_iPosY)
{
	TouchPointConvert(&e_iPosX,&e_iPosY);
	cGameApp::MouseUp(e_iPosX,e_iPosY);
	if( m_pPhaseManager )
		m_pPhaseManager->MouseUp(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	TouchEnd();
}


void	cWorldTripSlotApp::KeyUp(char e_char)
{
	cGameApp::KeyUp(e_char);
	if( e_char == 'P' )
	{
		g_bPause = !g_bPause;
	}
	if( e_char == 13 )
	{
		g_bRenderScreen = !g_bRenderScreen;
	}
	else
	{

	}
	m_pPhaseManager->KeyUp(e_char);
	if( cGameApp::m_sucKeyData[17] )//alt
	{
		switch(e_char)
		{
			case 38://up
				//cGameApp::m_svGameResolution.x = ;
				this->m_seDeviceDirection = eDD_PORTRAIT;
			break;
			case 37://left
				this->m_seDeviceDirection = eDD_LANDSCAPE_LEFT;
			break;
			case 39://right
				this->m_seDeviceDirection = eDD_LANDSCAPE_RIGHT;
			break;
			case 40://down
				this->m_seDeviceDirection = eDD_UPSIDE_DOWN;
			break;
		}
	}
}

void	cWorldTripSlotApp::KeyDown(char e_char)
{
	cGameApp::KeyDown(e_char);
	m_pPhaseManager->KeyDown(e_char);
}