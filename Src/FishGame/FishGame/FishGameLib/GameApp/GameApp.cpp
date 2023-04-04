#include "stdafx.h"
#include "GameApp.h"
#include "SceneChange.h"
#include "../MiniGame/MiniGameManager.h"
#include "../Monster/MonsterManager.h"
#include "../PlayerBehavior/PlayerData.h"
#include "../GameBullet/BulletManager.h"
#include "../ProbabilityFish/FishProbability.h"
#include "../GameEffect/GameEffectBase.h"
#include "../ControlPanel/ControlPanel.h"
#include "../PlayerBehavior/WinMoneyEffect.h"
#ifdef WIN32
#include "winioctl.h"
#include <intrin.h>
#endif
//
UINT					g_uiFrame = 0;
//
float					cFishApp::m_sfVersion = 1.0f;
cMiniGameManager*		cFishApp::m_spMiniGameManager = 0;
cMonsterManager*		cFishApp::m_spMonsterManager = 0;
cPlayerManager*			cFishApp::m_spPlayerManager = 0;
cBulletManager*			cFishApp::m_pBulletManager = 0;
cControlPanel*			cFishApp::m_spControlPanel = 0;
cProbabilityFish*		cFishApp::m_spProbabilityFish = 0;
cSceneChange*			cFishApp::m_spSceneChange = 0;
bool					cFishApp::m_sbSceneChange = false;
float					cFishApp::m_sfMonsterUpdateSpeed = 1.f;
UINT64					cFishApp::m_sui64CurrentStep = 0;
float					cFishApp::m_MiniGameShakeTm = 0.f;
bool					cFishApp::m_sbIsUsingMasterLeeProbability = false;
cWinMoneyEffectManager* cFishApp::m_spWinMoneyEffectManager = 0;
cGameEffectManager*		cFishApp::m_spGameEffectManager = 0;

bool*					g_pbIsUsingMasterLeeProbability = &cFishApp::m_sbIsUsingMasterLeeProbability;
extern bool	g_bLeave;
bool		g_bInitOk = false;
extern bool		g_bProtected;
bool g_bDoBatchRendering = false;
void		WritePlayerMoney(float e_fElpaseTime)
{
	if( g_bInitOk )
	{
		if( cFishApp::m_spProbabilityFish )
			cFishApp::m_spProbabilityFish->WrtiePlayerMoney(e_fElpaseTime);
	}
}
int	g_iLoadingStep = 1;
cBaseImage*g_pLoadingImage = 0;
void	LoadingProgressInfo()
{	
	cGameApp::m_sTimeAndFPS.Update();
	if( cGameApp::m_sTimeAndFPS.fElpaseTime >0.032f )
	{
		cGameApp::m_spOpenGLRender->Render();
		const wchar_t*l_strLoadingInfo = L"Loading...";
		int	l_iLength = (int)wcslen(l_strLoadingInfo);
		wchar_t	l_str[MAX_PATH];
		memcpy(l_str,l_strLoadingInfo,sizeof(wchar_t)*g_iLoadingStep);
		l_str[g_iLoadingStep] = L'\0';
		//UseShaderProgram();
		auto l_vVeiewRect = cGameApp::m_spOpenGLRender->m_vViewPortSize;
		auto l_vGameResolution = cGameApp::m_spOpenGLRender->m_vGameResolution;
		//glViewport(0,0,(int)cGameApp::m_spOpenGLRender->m_vViewPortSize.Width(), (int)cGameApp::m_spOpenGLRender->m_vViewPortSize.Height());
		//glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
		//glClearColor( 0,0.5,0.5,1 );
		//glClearDepth(1.0f);	
		//glEnable(GL_TEXTURE_2D);
		//glEnable2D(cGameApp::m_spOpenGLRender->m_vGameResolution.x, cGameApp::m_spOpenGLRender->m_vGameResolution.y);
		if( g_pLoadingImage )
		{
			g_pLoadingImage->Render();
			//g_pLoadingImage->Render(cMatrix44::TranslationMatrix(Vector3(*g_pLoadingImage->GetImageShowWidth()/2,*g_pLoadingImage->GetImageShowHeight()/2,0))*cMatrix44::ScaleMatrix(Vector3(frand(0.999,1.001),1,1)));
		}
		//cGameApp::m_spGlyphFontRender->RenderFont(cGameApp::m_spOpenGLRender->m_vViewPortSize.x/2-100.f,100.f,l_str);
#ifdef WIN32
		SwapBuffers(cGameApp::m_spOpenGLRender->m_Hdc);
#endif
		++g_iLoadingStep;
		if( g_iLoadingStep >= l_iLength )
			g_iLoadingStep = 1;
	}
}
//






extern std::string	GetHardDiskSN();
extern std::string	GetEncryptHip(bool e_bCheckVailed);
void CheckData( bool bFirst )
{
}

#ifdef WIN32


cFishApp::cFishApp(HWND e_Hwnd,Vector2 e_vGameResolution,Vector2 e_vViewportSize)
:cGameApp(e_Hwnd,e_vGameResolution,e_vViewportSize)
#else
cFishApp::cFishApp(Vector2 e_vGameResolution,Vector2 e_vViewportSize)
:cGameApp(e_vGameResolution,e_vViewportSize)
#endif
{
	g_sbCollisionRender = true;
	cGameApp::Init();
	FMLog::Log("constructor Start",false);
	if( 1 )
	{
		//cCurveWithTime	l_cCurveWithTime;
		//std::vector<cCurveWithTime>	ll;
		//ll.push_back( l_cCurveWithTime );
		//cFMAnimationRule	l_cFMAnimationRule;
		//std::vector<cFMAnimationRule>	aa;
		//aa.push_back(l_cFMAnimationRule);
	}
	//m_sbSpeedControl = true;
	m_bLeave = false;

	if(!cGameApp::m_spSoundParser->Parse("Fish/Sound/Sound.xml"))
	{
		UT::ErrorMsg(L"Fish/Sound.xml",L"parse sound error");
	}
	FMLog::Log("constructor End",false);
	//cGameApp::m_spSoundParser->GetObject(L"3")->SetVolume(0.1f);
}








cFishApp::~cFishApp()
{
	g_bInitOk = false;
	Sleep(10);
	GameDataDestroy();
	SAFE_DELETE( m_spControlPanel );
	Destroy();
}

void	cFishApp::Init()
{
	this->m_spSoundParser->SetVolume(0);
	FMLOG("cFishApp::Init 1");
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	g_bInitOk = false;
	g_pLoadingImage = new cBaseImage("Fish/Image/Loading.png");
	LoadingProgressInfo();
	m_sfMonsterUpdateSpeed = 1.f;
	m_sbSceneChange = false;
	cGameApp::SoundPlay(L"bg",false);
	cGameApp::SoundPlay(L"bg02",false);
	cGameApp::SoundPlay(L"gold",false);
#ifdef DEBUG
	cGameApp::SoundPlay(L"logo",false);
#else
	cGameApp::SoundPlay(L"logo",true);
#endif
	cGameApp::SoundPlay(L"6",false);
	FMLOG("cFishApp::Init 2");
	if( UT::IsFileExists("Fish/Image/Loading.png") )
	{
		//UseShaderProgram();
		cGameApp::m_spOpenGLRender->Render();
		//glViewport(0,0,(int)cGameApp::m_spOpenGLRender->m_vViewPortSize.Width(),(int)cGameApp::m_spOpenGLRender->m_vViewPortSize.Height());
		//glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
		//glClearColor( 0,0.5,0.5,1 );
		//glClearDepth(1.0f);	
		//glEnable(GL_TEXTURE_2D);
		//glEnable2D(cGameApp::m_spOpenGLRender->m_vGameResolution.x, cGameApp::m_spOpenGLRender->m_vGameResolution.y);
		cBaseImage*l_pLoadingImage = new cBaseImage("Fish/Image/Loading.png");
		l_pLoadingImage->Render();
#ifdef WIN32
		SwapBuffers(cGameApp::m_spOpenGLRender->m_Hdc);
#endif
		SAFE_DELETE(l_pLoadingImage);
	} 
	//
	FMLOG("cFishApp::Init 3");
	GameDataDestroy();
	FMLOG("cFishApp::Init 4");
	//
	if ( !m_spControlPanel )
		m_spControlPanel = new cControlPanel( "Fish/ControlPanel/WithMachineNumberEventPachages.xml", 1 );
	FMLOG("cFishApp::Init 5");
	if( m_spControlPanel )
	{
		cGameApp::m_spSoundParser->SetVolume(this->m_spControlPanel->m_iSound/100.f);
	} 
	FMLOG("cFishApp::Init 6");
#define	GAME_RESOLUTION_WIDTH	1280.f
#define	GAME_RESOLUTION_HEIGHT	720.f
	//if( this->m_spControlPanel->m_iPlayerCount > 10 )
	//{
	//	this->m_svGameResolution.x = GAME_RESOLUTION_WIDTH*2;
	//	this->m_svGameResolution.y = GAME_RESOLUTION_HEIGHT;
	//}
	//else
	//{
	//	this->m_svGameResolution.x = GAME_RESOLUTION_WIDTH;
	//	this->m_svGameResolution.y = GAME_RESOLUTION_HEIGHT;	
	//}
	cGameApp::m_spOpenGLRender->m_vGameScale.x = cGameApp::m_spOpenGLRender->m_vGameResolution.x/GAME_RESOLUTION_WIDTH;
	cGameApp::m_spOpenGLRender->m_vGameScale.y = cGameApp::m_spOpenGLRender->m_vGameResolution.y/GAME_RESOLUTION_HEIGHT;
	m_spSceneChange = new cSceneChange();
	FMLOG("cFishApp::Init 7");
	m_pBulletManager = new cBulletManager();
	FMLOG("cFishApp::Init 8");
	m_spMiniGameManager = new cMiniGameManager();
	FMLOG("cFishApp::Init 9");
	m_spMonsterManager = new cMonsterManager();
	FMLOG("cFishApp::Init 10");
	m_spPlayerManager = new cPlayerManager();
	FMLOG("cFishApp::Init 11");
	m_spProbabilityFish = new cProbabilityFish();
	FMLOG("cFishApp::Init 12");
	m_spWinMoneyEffectManager = new cWinMoneyEffectManager();
	FMLOG("cFishApp::Init 14");
	m_spGameEffectManager = new cGameEffectManager();
	FMLOG("cFishApp::Init 15");
	//PrintMemoryInfo();
	if( m_spMiniGameManager )
		m_spMiniGameManager->Init();
	//PrintMemoryInfo();
	if( m_spMonsterManager )
		m_spMonsterManager->Init();
	//PrintMemoryInfo();
	if( m_spPlayerManager )
		m_spPlayerManager->Init();
	//PrintMemoryInfo();
	if( m_pBulletManager )
		m_pBulletManager->Init();
	//PrintMemoryInfo();
	if( m_spProbabilityFish )
		m_spProbabilityFish->Init();
	//PrintMemoryInfo();
	if( m_spSceneChange )
		m_spSceneChange->Init();
	//PrintMemoryInfo();
	if ( m_spWinMoneyEffectManager )
		m_spWinMoneyEffectManager->Init();
	if( m_spGameEffectManager )
		m_spGameEffectManager->Init();
	//PrintMemoryInfo();
	this->m_sTimeAndFPS.Update();
	//g_sbCollisionRender = false;
	g_bInitOk = true;
	SAFE_DELETE(g_pLoadingImage);
	this->m_spSoundParser->SetVolume(0);
	FMLOG("cFishApp::Init all done");
}

void	cFishApp::GameDataDestroy()
{
	SAFE_DELETE(m_spProbabilityFish);
	SAFE_DELETE(m_spMiniGameManager);
	SAFE_DELETE(m_spMonsterManager);
	SAFE_DELETE(m_spPlayerManager);
	SAFE_DELETE(m_pBulletManager);
	SAFE_DELETE(m_spSceneChange);
	SAFE_DELETE(m_spWinMoneyEffectManager);
	SAFE_DELETE(m_spGameEffectManager);
}

//void	cFishApp::Destroy()
//{
//#ifdef WIN32
//	Delete2DShader(L"NoTexture");
//	Delete2DShader();
//#endif
//}

enum	eDebugInfo
{
	eDI_IO = 0,
	eDI_GAMEAPP,
	eDI_SCENE_CHANGE,
	eDI_MINI_GAME,
	eDI_MONSTER,
	eDI_PLAYER,
	eDI_BULLET,
	eDI_PROBABILITY,
	eDI_REFOUND_DEVICE,
	eDI_WINMONEYEFFECT,
};

const int	g_iAllDebugInfo = 10;
float	g_fUpdateUsingTime[g_iAllDebugInfo];
float	g_fRenderTime[g_iAllDebugInfo];

const wchar_t*g_strDebugInfo[g_iAllDebugInfo] = {L"IO",
							L"GameApp",
							L"SceneChange",
							L"MiniGameManager",
							L"MonsterManager",
							L"PlayerManager",
							L"BulletManager",
							L"FishProbability",
							L"RefoundCoinDeviceManager",
							L"WinMoneyEffectManager"};
UT::sTimeAndFPS	g_TimeStamp;

void	TimsStamp(eDebugInfo e_eDebugInfo,bool e_bUpdate,bool e_bTimeAdded = false)
{
	g_TimeStamp.Update();
	if( e_bUpdate )
	{
		cFishApp::m_sui64CurrentStep = e_eDebugInfo;
		if( e_bTimeAdded )
			g_fUpdateUsingTime[e_eDebugInfo] += g_TimeStamp.fElpaseTime;
		else
			g_fUpdateUsingTime[e_eDebugInfo] = g_TimeStamp.fElpaseTime;
		//OutputDebugString(g_strDebugInfo[e_eDebugInfo]);
		//OutputDebugString(L",");
	}
	else
	{
		cFishApp::m_sui64CurrentStep = e_eDebugInfo+100;
		if( e_bTimeAdded )
			g_fRenderTime[e_eDebugInfo] += g_TimeStamp.fElpaseTime;
		else
			g_fRenderTime[e_eDebugInfo] = g_TimeStamp.fElpaseTime;
	}
}

void	cFishApp::ResoluctionParse2(char*e_strFileName)
{
	cNodeISAX	l_NodeISAX;
	bool	l_bFullScreen = false;
	bool	l_b = l_NodeISAX.ParseDataIntoXMLNode(e_strFileName);
	if(l_b)
	{
		TiXmlElement*l_pTiXmlElement = l_NodeISAX.GetRootElement();
		const wchar_t*l_strFullscreen = l_pTiXmlElement->Attribute(L"FullScreen");
		if( l_strFullscreen )
			m_sbFullScreen = _wtoi(l_strFullscreen)?true:false;
		const wchar_t*l_strResolution = l_pTiXmlElement->Attribute(L"Resolution");
		const wchar_t*l_strViewPort = l_pTiXmlElement->Attribute(L"ViewPort");
		const wchar_t*l_strDeviceOrietation = l_pTiXmlElement->Attribute(L"DeviceOrietation");
		const wchar_t*l_strHideMouseCursor = l_pTiXmlElement->Attribute(L"HideMouseCursor");
		const wchar_t*l_strTexturePowerOfTwo = l_pTiXmlElement->Attribute(L"TexturePowerOfTwo");
		const wchar_t*l_strDebugFunctionWorking = l_pTiXmlElement->Attribute(L"DebugFunction");

		if( l_strDebugFunctionWorking )
		{
			m_sbDebugFunctionWorking = _wtoi(l_strDebugFunctionWorking)?true:false;
		}		

		if( l_strResolution )
		{
			POINT	l_Resolution = GetPoint(l_strResolution);
			if( !l_strViewPort )
			{
				cGameApp::m_spOpenGLRender->m_vGameResolution.x = (float)l_Resolution.x;
				cGameApp::m_spOpenGLRender->m_vGameResolution.y = (float)l_Resolution.y;
			}
			//if(!m_spClickMouseBehavior)
			//	m_spClickMouseBehavior = new cClickMouseBehavior();
			//m_spClickMouseBehavior->SetCollisionRange(Vector4(0,0,m_svGameResolution.x,m_svGameResolution.y));
		}
		if( l_strViewPort )
		{
			POINT	l_Resolution = GetPoint(l_strViewPort);
			cGameApp::m_spOpenGLRender->m_vViewPortSize.z = 0;
			cGameApp::m_spOpenGLRender->m_vViewPortSize.w = 0;
			cGameApp::m_spOpenGLRender->m_vViewPortSize.z = (float)l_Resolution.x;
			cGameApp::m_spOpenGLRender->m_vViewPortSize.w = (float)l_Resolution.y;
		}
		if( l_strDeviceOrietation )
		{
			int	l_iIndex = _wtoi(l_strDeviceOrietation);
			if( l_iIndex <= eDD_LANDSCAPE_RIGHT )
				cGameApp::m_spOpenGLRender->m_eDeviceDirection = (eDeviceDirection)l_iIndex;
		}
		if( l_strHideMouseCursor )
		{
#ifdef WIN32
			ShowCursor(_wtoi(l_strHideMouseCursor)==0?false:true);
#endif
		}
		if( l_strTexturePowerOfTwo )
		{
			g_bSupportNonPowerOfTwoTexture = _wtoi(l_strTexturePowerOfTwo)?true:false;
		}
	}
}

void	cFishApp::Update(float e_fElpaseTime)
{
	//e_fElpaseTime = 0.016f;
//#ifndef DEBUG//avoid frame skip.
//	if( e_fElpaseTime>1/30.f )
//		e_fElpaseTime = 1/30.f;
//#endif
	g_TimeStamp.Update();
	TimsStamp(eDI_IO,true);
	bool	l_bControlPanel = false;
	if( m_spControlPanel )
	{
		l_bControlPanel = m_spControlPanel->IsInControlPanelMode();
		m_spControlPanel->Update(e_fElpaseTime);
	}
	if( l_bControlPanel )
	{
		if ( !m_spControlPanel->IsInControlPanelMode() )
			Init();
	}
	else
	{

		if ( m_MiniGameShakeTm > 0.f )
		{
			m_MiniGameShakeTm -= e_fElpaseTime;

			if ( m_MiniGameShakeTm < 0.f )
				m_MiniGameShakeTm = 0.f;
		}

		cGameApp::Update(e_fElpaseTime);
		TimsStamp(eDI_GAMEAPP,true);
		if( m_spSceneChange )
			m_spSceneChange->Update(e_fElpaseTime);
		TimsStamp(eDI_SCENE_CHANGE,true);
		if( m_spMiniGameManager )
			m_spMiniGameManager->Update(e_fElpaseTime);
		TimsStamp(eDI_MINI_GAME,true);
		if( m_spMonsterManager )
			m_spMonsterManager->Update(e_fElpaseTime*m_sfMonsterUpdateSpeed);
		TimsStamp(eDI_MONSTER,true);
		if( m_spPlayerManager )
			m_spPlayerManager->Update(e_fElpaseTime);
		TimsStamp(eDI_PLAYER,true);
		if(m_spGameEffectManager)
			m_spGameEffectManager->Update( e_fElpaseTime );
		if ( m_spWinMoneyEffectManager )
		{
			m_spWinMoneyEffectManager->Update( e_fElpaseTime );
		}
		TimsStamp( eDI_WINMONEYEFFECT, true );
		if( m_pBulletManager )
		{
			m_pBulletManager->Update(e_fElpaseTime);
			if( m_spMonsterManager )
			{
				m_pBulletManager->Collide(m_spMonsterManager);
			}
		}
		TimsStamp(eDI_BULLET,true);
		//must update at last.
		if(m_spProbabilityFish)
		{
			//m_spProbabilityFish->Update(e_fElpaseTime>1/30.f?1/30.f:e_fElpaseTime);
			m_spProbabilityFish->Update(e_fElpaseTime);
		}
		TimsStamp(eDI_PROBABILITY,true);
		TimsStamp(eDI_REFOUND_DEVICE,true);
	}
	++g_uiFrame;
}

void	cFishApp::Render()
{
	cGameApp::Render();
	//glAlphaFunc(GL_GREATER,0.001f);	
	g_TimeStamp.Update();
	if( m_spControlPanel && m_spControlPanel->IsInControlPanelMode() )
	{
		glEnable2D( 1280, 720 );
		m_spControlPanel->Render();
	}
	else
	{
		cGameApp::Render();
		TimsStamp(eDI_GAMEAPP,false);
		if( m_spSceneChange )
			m_spSceneChange->Render();
		TimsStamp(eDI_SCENE_CHANGE,false);
		if( m_spMiniGameManager )
			m_spMiniGameManager->RenderTrigger();
		TimsStamp(eDI_MINI_GAME,false);
		if( m_spMonsterManager )
			m_spMonsterManager->Render();
		TimsStamp(eDI_BULLET,false);
		if( m_pBulletManager )
			m_pBulletManager->Render();
		TimsStamp(eDI_MONSTER, false);
		if( m_spMonsterManager )
			m_spMonsterManager->LastRender();
		TimsStamp(eDI_BULLET,false);
		if(m_spProbabilityFish)
			m_spProbabilityFish->Render();
		TimsStamp(eDI_PROBABILITY,false);
		TimsStamp(eDI_REFOUND_DEVICE,false);
		if(m_spSceneChange)
			m_spSceneChange->LastRender();
		if( m_spPlayerManager )
			m_spPlayerManager->Render();
		TimsStamp(eDI_PLAYER,false);
		if( m_spMiniGameManager )
			m_spMiniGameManager->RenderActiver();
		TimsStamp(eDI_MINI_GAME,false,true);
		if( m_spWinMoneyEffectManager )
			m_spWinMoneyEffectManager->Render();
		if(m_spGameEffectManager)
			m_spGameEffectManager->Render();
		TimsStamp( eDI_WINMONEYEFFECT, false );
		TimsStamp(eDI_SCENE_CHANGE,false,true);
		glEnable2D(cGameApp::m_spOpenGLRender->m_vGameResolution.x, cGameApp::m_spOpenGLRender->m_vGameResolution.y);
////#ifdef DEBUG
//		if( cGameApp::m_spGlyphFontRender)
//		{
//			cGameApp::m_spGlyphFontRender->RenderFont( 750, 30, UT::CharToWchar(cGameApp::m_sTimeAndFPS.GetFPS()) );
//		} 
////#endif
		//if ( m_spControlPanel )
//			m_spControlPanel->RenderOverReportTime();
		//if( g_sbCollisionRender && m_sbDebugFunctionWorking )
#ifdef DEBUG
		if (m_sbDebugFunctionWorking)
		{
			cGameApp::m_spGlyphFontRender->RenderFont( 750, 30, UT::CharToWchar(cGameApp::m_sTimeAndFPS.GetFPS()) );
			if( cGameApp::m_spGlyphFontRender)
			{
				WCHAR   l_str[MAX_PATH];
				swprintf( l_str,MAX_PATH, L"MousePos:%d,%d", cGameApp::m_sMousePosition.x, cGameApp::m_sMousePosition.y );
				cGameApp::m_spGlyphFontRender->RenderFont( 750, 0, l_str );
				cGameApp::m_spGlyphFontRender->RenderFont( 750, 30, UT::CharToWchar(cGameApp::m_sTimeAndFPS.GetFPS()) );
			}
			if( m_spMonsterManager  )
				m_spMonsterManager->DebugRender();
			if( m_pBulletManager  )
				m_pBulletManager->DebugRender();

			if(m_spProbabilityFish)
				m_spProbabilityFish->DebugRender();

			//if(m_spSceneChange)
			//	m_spSceneChange->DebugRender();
			for( int i=0;i<g_iAllDebugInfo;++i )
			{
				std::wstring	l_str = UT::ComposeMsgByFormat(L"%s:%.4f,%.4f",g_strDebugInfo[i],g_fUpdateUsingTime[i],g_fRenderTime[i]);
				cGameApp::m_spGlyphFontRender->RenderFont(150,30*i,l_str.c_str());
			}
			//std::wstring	l_str = UT::ComposeMsgByFormat( L"Frame:%d", g_uiFrame );
			//cGameApp::m_spGlyphFontRender->RenderFont( 0, 650, l_str.c_str() );
		}
#endif
	}
	int	l_iStaryPosY = 0;
	std::wstring	l_strInfo = ComposeMsgByFormat(L"ElpaseTime:%.3f", (float)m_dbGamePlayTime);
	cGameApp::m_spGlyphFontRender->RenderFont(50, l_iStaryPosY, UT::CharToWchar(cGameApp::m_sTimeAndFPS.GetFPS()).c_str());
	l_iStaryPosY += 30;
	cGameApp::RenderFont(Vector2(50, l_iStaryPosY), g_bDoBatchRendering ? L"BatchRendering:On" : L"BatchRendering:Off");
	glDisable2D();
#ifdef WIN32
	SwapBuffers(cGameApp::m_spOpenGLRender->m_Hdc);
#endif
}

//void	cFishApp::Run()
//{
//	Update(g_TimeAndFPS.fElpaseTime);
//	Render();
//}

void	cFishApp::MouseDown(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseDown(e_iPosX,e_iPosY);
#ifdef WASM
	m_sMousePosition.y += EMSDK::EMSDK_GetCanvasPosY();
#endif
	if( m_spControlPanel&&m_spControlPanel->IsInControlPanelMode() )
	{
		m_spControlPanel->MouseDown(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
		return;
	}
}

void	cFishApp::MouseMove(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseMove(e_iPosX,e_iPosY);
#ifdef WASM
	m_sMousePosition.y += EMSDK::EMSDK_GetCanvasPosY();
#endif
	if( m_spControlPanel&&m_spControlPanel->IsInControlPanelMode() )
	{
		m_spControlPanel->MouseMove(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
		return;
	}
}

void	cFishApp::MouseUp(int e_iPosX,int e_iPosY)
{
	g_bDoBatchRendering = !g_bDoBatchRendering;
    cGameApp::MouseUp(e_iPosX,e_iPosY);
#ifdef WASM
	m_sMousePosition.y += EMSDK::EMSDK_GetCanvasPosY();
#endif
	if( m_spControlPanel&&m_spControlPanel->IsInControlPanelMode() )
	{
		m_spControlPanel->MouseUp(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
		return;
	}
}

void	cFishApp::KeyDown(char e_char)
{
	 cGameApp::KeyDown(e_char);
	 if ( e_char == 46 )
		 m_spControlPanel->m_bPressOpenControlPanel = true;
	if( m_sbDebugFunctionWorking )
	{
		int	l_iPlayerIndex = e_char-96;//right keyboard 0
		if( l_iPlayerIndex >-1 && l_iPlayerIndex <= cFishApp::m_spControlPanel->m_iPlayerCount )
		{
			cPlayerData*l_pPlayerData = (cPlayerData*)this->m_spPlayerManager->GetObject(l_iPlayerIndex);
			if( cGameApp::m_sucKeyData[17] )
			{
				//cGameApp::m_sucKeyData[l_pPlayerData->m_szInput_Level] = true;
			}
		}
	}
}
void	cFishApp::KeyUp(char e_char)
{
	//SHORT	l_ = GetAsyncKeyState('A');
	cGameApp::KeyUp(e_char);
	//std::string	l_sss = UT::ComposeMsgByFormat("KeyValue:%s,%d",&e_char,e_char);
	//std::wstring l_ss2 = UT::CharToWchar(l_sss.c_str());
	//OutputDebugString(l_ss2.c_str());
	//OutputDebugString(L"\n");
	if( e_char == 8 )
	{
		g_sbCollisionRender = !g_sbCollisionRender;
	}
	if ( e_char == 46 )
		m_spControlPanel->m_bPressOpenControlPanel = false;

	if( cGameApp::m_sbDebugFunctionWorking )
	{
		if ( cGameApp::m_sucKeyData[ 17 ] && e_char == 38 )
		{
			switch (cGameApp::m_spOpenGLRender->m_eDeviceDirection )
			{
				case eDD_PORTRAIT:        cGameApp::m_spOpenGLRender->m_eDeviceDirection = eDD_UPSIDE_DOWN;     break;
				case eDD_UPSIDE_DOWN:     cGameApp::m_spOpenGLRender->m_eDeviceDirection = eDD_LANDSCAPE_LEFT;  break;
				case eDD_LANDSCAPE_LEFT:  cGameApp::m_spOpenGLRender->m_eDeviceDirection = eDD_LANDSCAPE_RIGHT; break;
				case eDD_LANDSCAPE_RIGHT: cGameApp::m_spOpenGLRender->m_eDeviceDirection = eDD_PORTRAIT;        break;
				default:					break;
			}
		}
		if ( e_char == 110 )
			m_spControlPanel->AutoReport();

		if( e_char == 8 )
		{
			g_sbCollisionRender = !g_sbCollisionRender;
		}
		//if( e_char == -36 )//107'+'
		//{
		//	m_sfDebugValue *= 2.f;
		//	if( m_sfDebugValue >1073741824 )
		//		m_sfDebugValue  = 1073741824;
		//}
		//else
		//if( e_char == 16 )//109'-'
		//{
		//	m_sfDebugValue /= 2.f;	
		//	if( m_sfDebugValue <= 0.00000000001f )
		//		m_sfDebugValue = 0.00000000001f;
		//}
	}
	/*if( e_char == 'W' )
	{
		static float	l_fAngle = 5.f;
		l_fAngle += 5.f;
		cBullet*l_pBullet = m_pBulletManager->Require(0,0);
		if( l_pBullet )
		{
			l_pBullet->Fire(0,Vector3(400,400,0),l_fAngle);
		}
	}*/
	if( m_sbDebugFunctionWorking && m_spPlayerManager )
	{
		int	l_iPlayerIndex = e_char-96;//right keyboard 0
		if( l_iPlayerIndex >-1 && l_iPlayerIndex < cFishApp::m_spControlPanel->m_iPlayerCount )
		{
			cPlayerData*l_pPlayerData = (cPlayerData*)this->m_spPlayerManager->GetObject(l_iPlayerIndex);
			if( cGameApp::m_sucKeyData[17] )
			{
				//cGameApp::m_sucKeyData[l_pPlayerData->m_szInput_Level] = false;
			}
		}
	}
}