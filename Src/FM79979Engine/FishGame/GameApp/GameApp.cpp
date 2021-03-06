#include "stdafx.h"
#include "winioctl.h"
#include "GameApp.h"
#include "QucikInfo.h"
#include "DeviceRefundCoin.h"
#include "SceneChange.h"
#include "TestLogSetup.h"
#include "../MiniGame/MiniGameManager.h"
#include "../Monster/MonsterManager.h"
#include "../PlayerBehavior/PlayerData.h"
#include "../Bullet/BulletManager.h"
#include "../ProbabilityFish/FishProbability.h"

#include "../GameEffect/GameEffectBase.h"

#include <intrin.h>
#include "../PlayerBehavior/WinMoneyEffect.h"
bool	g_bGoToControleTest = false;
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
cTestLogFile*			cFishApp::m_spTestLogFile = 0;
bool					cFishApp::m_sbSceneChange = false;
float					cFishApp::m_sfMonsterUpdateSpeed = 1.f;
UINT64					cFishApp::m_sui64CurrentStep = 0;
float					cFishApp::m_MiniGameShakeTm = 0.f;
bool					cFishApp::m_sbIsUsingMasterLeeProbability = false;
cWinMoneyEffectManager* cFishApp::m_spWinMoneyEffectManager = 0;
cGameEffectManager*		cFishApp::m_spGameEffectManager = 0;

bool*					g_pbIsUsingMasterLeeProbability = &cFishApp::m_sbIsUsingMasterLeeProbability;
cQuickInfo*				g_pQuickInfo = 0;
extern bool	g_bLeave;
bool		g_bInitOk = false;
extern bool		g_bProtected;
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
{	cGameApp::m_sTimeAndFPS.Update();
	if( cGameApp::m_sTimeAndFPS.fElpaseTime >0.032f )
	{
		WCHAR*l_strLoadingInfo = L"Loading...";
		int	l_iLength = wcslen(l_strLoadingInfo);
		WCHAR	l_str[MAX_PATH];
		memcpy(l_str,l_strLoadingInfo,sizeof(WCHAR)*g_iLoadingStep);
		l_str[g_iLoadingStep] = L'\0';
		UseShaderProgram();
		glViewport(0,0,(int)cGameApp::m_svViewPortSize.x,(int)cGameApp::m_svViewPortSize.y);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
		glClearColor( 0,0.5,0.5,1 );
		glClearDepth(1.0f);	
		glEnable(GL_TEXTURE_2D);
		glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);
		if( g_pLoadingImage )
		{
			g_pLoadingImage->Render();
			//g_pLoadingImage->Render(cMatrix44::TranslationMatrix(Vector3(*g_pLoadingImage->GetImageShowWidth()/2,*g_pLoadingImage->GetImageShowHeight()/2,0))*cMatrix44::ScaleMatrix(Vector3(frand(0.999,1.001),1,1)));
		}
		//cGameApp::m_spGlyphFontRender->RenderFont(cGameApp::m_svGameResolution.x/2-100.f,100.f,l_str);
		SwapBuffers(cGameApp::m_sHdc);
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
	cGameApp::Init();
	cGameApp::OutputDebugInfoString(L"\nStart\n");
	if( 1 )
	{
		cCurveWithTime	l_cCurveWithTime;
		std::vector<cCurveWithTime>	ll;
		ll.push_back( l_cCurveWithTime );
		//cFMAnimationRule	l_cFMAnimationRule;
		//std::vector<cFMAnimationRule>	aa;
		//aa.push_back(l_cFMAnimationRule);
	}
	cGameApp::OutputDebugInfoString(L"\nEnd");
	//m_sbSpeedControl = true;
	m_bLeave = false;
	m_pRefoundCoinDeviceManager = 0;
	if(!g_pIOSMDriverInterface)
	{
		this->m_spLogFile->WriteToFileImmediatelyWithLine(L"start init 8051 IO");
		g_pIOSMDriverInterface = new cIOSMDriverInterface();
		g_pIOSMDriverInterface->Init();
		this->m_spLogFile->WriteToFileImmediatelyWithLine(L"init 8051 IO ok");
		if(g_pIOSMDriverInterface->Parse("Fish/IO/IOData.xml"))
			this->m_spLogFile->WriteToFileImmediatelyWithLine(L"Fish/IO/IOData-file.xml ok");
		else
			this->m_spLogFile->WriteToFileImmediatelyWithLine(L"Fish/IO/IOData-file.xml falied");
	}
	if(!cGameApp::m_spSoundParser->Parse("Fish/Sound/Sound.xml"))
	{
		UT::ErrorMsg(L"Fish/Sound.xml",L"parse sound error");
	}
	g_pQuickInfo = new cQuickInfo(Vector2(0,0),38);
	//cGameApp::m_spSoundParser->GetObject(L"3")->SetVolume(0.1f);
}








cFishApp::~cFishApp()
{
	g_bInitOk = false;
	Sleep(10);
	GameDataDestroy();
	SAFE_DELETE( m_spControlPanel );
	SAFE_DELETE( m_pRefoundCoinDeviceManager );
	Destroy();
	SAFE_DELETE( g_pQuickInfo );
	SAFE_DELETE( g_pIOSMDriverInterface );
}

void	cFishApp::Init()
{
	glEnable(GL_ALPHA_TEST);
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
	if( UT::IsFileExists("Fish/Image/Loading.png") )
	{
		UseShaderProgram();
		glViewport(0,0,(int)m_svViewPortSize.Width(),(int)m_svViewPortSize.Height());
		//glScissor(0,0,(int)m_svViewPortSize.x,(int)m_svViewPortSize.y);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
		glClearColor( 0,0.5,0.5,1 );
		glClearDepth(1.0f);	
		glEnable(GL_TEXTURE_2D);
		glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);
		cBaseImage*l_pLoadingImage = new cBaseImage("Fish/Image/Loading.png");
		l_pLoadingImage->Render();
		SwapBuffers(cGameApp::m_sHdc);
		SAFE_DELETE(l_pLoadingImage);
	} 
	//
	GameDataDestroy();
	//
	if ( !m_spControlPanel )
		m_spControlPanel = new cControlPanel( "Fish/ControlPanel/WithMachineNumberEventPachages.xml", 1 );
	if( m_spControlPanel )
	{
		cGameApp::m_spSoundParser->SetVolume(this->m_spControlPanel->m_iSound/100.f);
	} 

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
	this->m_svGameScale.x = this->m_svGameResolution.x/GAME_RESOLUTION_WIDTH;
	this->m_svGameScale.y = this->m_svGameResolution.y/GAME_RESOLUTION_HEIGHT;
	if( !g_bGoToControleTest )
	{
		m_spSceneChange = new cSceneChange();
		m_pBulletManager = new cBulletManager();
		m_spMiniGameManager = new cMiniGameManager();
		m_spMonsterManager = new cMonsterManager();
		m_spPlayerManager = new cPlayerManager();
		m_spProbabilityFish = new cProbabilityFish();
		m_spTestLogFile = new cTestLogFile();
		m_pRefoundCoinDeviceManager = new cRefoundCoinDeviceManager();
		m_spWinMoneyEffectManager = new cWinMoneyEffectManager();
		m_spGameEffectManager = new cGameEffectManager();
	}
	else
	{
		m_spControlPanel->IntoControlPanel();
	}
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
	if(m_pRefoundCoinDeviceManager)
		m_pRefoundCoinDeviceManager->Init(IO8051_DATA_Refound);
	//PrintMemoryInfo();
	if( m_spTestLogFile )
		m_spTestLogFile->Init();
	if ( m_spWinMoneyEffectManager )
		m_spWinMoneyEffectManager->Init();
	if( m_spGameEffectManager )
		m_spGameEffectManager->Init();
	//PrintMemoryInfo();
	this->m_sTimeAndFPS.Update();
	g_sbCollisionRender = false;
	g_bInitOk = true;
	SAFE_DELETE(g_pLoadingImage);
}

void	cFishApp::GameDataDestroy()
{
	SAFE_DELETE(m_spProbabilityFish);
	SAFE_DELETE(m_spMiniGameManager);
	SAFE_DELETE(m_spMonsterManager);
	SAFE_DELETE(m_spPlayerManager);
	SAFE_DELETE(m_pBulletManager);
	SAFE_DELETE(m_pRefoundCoinDeviceManager);
	SAFE_DELETE(m_spSceneChange);
	SAFE_DELETE(m_spTestLogFile);
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

WCHAR*g_strDebugInfo[g_iAllDebugInfo] = {L"IO",
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
		const WCHAR*l_strFullscreen = l_pTiXmlElement->Attribute(L"FullScreen");
		if( l_strFullscreen )
			m_sbFullScreen = _wtoi(l_strFullscreen)?true:false;
		const WCHAR*l_strResolution = l_pTiXmlElement->Attribute(L"Resolution");
		const WCHAR*l_strViewPort = l_pTiXmlElement->Attribute(L"ViewPort");
		const WCHAR*l_strDeviceOrietation = l_pTiXmlElement->Attribute(L"DeviceOrietation");
		const WCHAR*l_strHideMouseCursor = l_pTiXmlElement->Attribute(L"HideMouseCursor");
		const WCHAR*l_strTexturePowerOfTwo = l_pTiXmlElement->Attribute(L"TexturePowerOfTwo");
		const WCHAR*l_strDevice = l_pTiXmlElement->Attribute(L"Device");
		const WCHAR*l_strDebugFunctionWorking = l_pTiXmlElement->Attribute(L"DebugFunction");

		if( l_strDebugFunctionWorking )
		{
			m_sbDebugFunctionWorking = _wtoi(l_strDebugFunctionWorking)?true:false;
		}		

		if( l_strDevice )
			m_sbDeviceExist = _wtoi(l_strDevice)?true:false;
		if( l_strResolution )
		{
			POINT	l_Resolution = GetPoint(l_strResolution);
			if( !l_strViewPort )
			{
				cGameApp::m_svViewPortSize.x = (float)l_Resolution.x;
				cGameApp::m_svViewPortSize.y = (float)l_Resolution.y;
			}
			cGameApp::m_svGameResolution.x = (float)l_Resolution.x;
			cGameApp::m_svGameResolution.y = (float)l_Resolution.y;
			//if(!m_spClickMouseBehavior)
			//	m_spClickMouseBehavior = new cClickMouseBehavior();
			//m_spClickMouseBehavior->SetCollisionRange(Vector4(0,0,m_svGameResolution.x,m_svGameResolution.y));
		}
		if( l_strViewPort )
		{
			POINT	l_Resolution = GetPoint(l_strViewPort);
			cGameApp::m_svViewPortSize.z = (float)l_Resolution.x;
			cGameApp::m_svViewPortSize.w = (float)l_Resolution.y;
		}
		if( l_strDeviceOrietation )
		{
			int	l_iIndex = _wtoi(l_strDeviceOrietation);
			if( l_iIndex <= eDD_LANDSCAPE_RIGHT )
				cGameApp::m_seDeviceDirection = (eDeviceDirection)l_iIndex;
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
	if( g_pIOSMDriverInterface )
		g_pIOSMDriverInterface->Update(e_fElpaseTime);
	TimsStamp(eDI_IO,true);
	bool	l_bControlPanel = false;
	if( m_spControlPanel )
	{
		l_bControlPanel = m_spControlPanel->IsInControlPanelMode();
		m_spControlPanel->Update(e_fElpaseTime);
	}
	if( l_bControlPanel )
	{
		if( g_pQuickInfo )
			g_pQuickInfo->DisableShowInfo();
		if ( !m_spControlPanel->IsInControlPanelMode() )
			Init();
	}
	else
	{
		if( g_pQuickInfo )
			g_pQuickInfo->Update();

		if ( m_MiniGameShakeTm > 0.f )
		{
			m_MiniGameShakeTm -= e_fElpaseTime;

			if ( m_MiniGameShakeTm < 0.f )
				m_MiniGameShakeTm = 0.f;
		}

		cGameApp::Update(e_fElpaseTime);
		if( m_spTestLogFile )
		{
			m_spTestLogFile->Update(e_fElpaseTime);
		}
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
		//MyUpdate(e_fElpaseTime);
		if(m_pRefoundCoinDeviceManager)
		{
			m_pRefoundCoinDeviceManager->Update(e_fElpaseTime);
		}
		TimsStamp(eDI_REFOUND_DEVICE,true);
	}
	++g_uiFrame;
}

void	cFishApp::Render()
{
	glViewport((int)m_svViewPortSize.x,(int)m_svViewPortSize.y,(int)m_svViewPortSize.z,(int)m_svViewPortSize.w);
	//glScissor(0,0,(int)m_svViewPortSize.x,(int)m_svViewPortSize.y);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glClearColor( 0,0.5,0.5,1 );
	glClearDepth(1.0f);	
	glEnable(GL_TEXTURE_2D);
	glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);

	cMatrix44 l_matProjection;
	glhOrthof2((float*)l_matProjection.m, -300, cGameApp::m_svGameResolution.x+300, cGameApp::m_svGameResolution.y+300, -300, -10000, 10000);
	FATMING_CORE::SetupShaderViewProjectionMatrix((float*)&l_matProjection,false);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
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
		TimsStamp(eDI_MONSTER,false);
		if( m_pBulletManager )
			m_pBulletManager->Render();
		if( m_spMonsterManager )
			m_spMonsterManager->LastRender();
		TimsStamp(eDI_BULLET,false);
		if(m_spProbabilityFish)
			m_spProbabilityFish->Render();
		TimsStamp(eDI_PROBABILITY,false);
		if(m_pRefoundCoinDeviceManager)
			m_pRefoundCoinDeviceManager->Render();
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
		glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);
////#ifdef DEBUG
//		if( cGameApp::m_spGlyphFontRender)
//		{
//			cGameApp::m_spGlyphFontRender->RenderFont( 750, 30, UT::CharToWchar(cGameApp::m_sTimeAndFPS.GetFPS()) );
//		} 
////#endif
		if ( m_spControlPanel )
			m_spControlPanel->RenderOverReportTime();
		if( g_pQuickInfo )
			g_pQuickInfo->Render();
		if( g_sbCollisionRender && m_sbDebugFunctionWorking )
		{
			cGameApp::m_spGlyphFontRender->RenderFont( 750, 30, UT::CharToWchar(cGameApp::m_sTimeAndFPS.GetFPS()) );
			if(g_pIOSMDriverInterface)
			{
				//g_pIOSMDriverInterface->Render();
				//TimsStamp(eDI_IO,false);
			}
			if( cGameApp::m_spGlyphFontRender)
			{
				WCHAR   l_str[MAX_PATH];
				swprintf( l_str,MAX_PATH, L"MousePos:%d,%d", cGameApp::m_sMousePosition.x, cGameApp::m_sMousePosition.y );
				cGameApp::m_spGlyphFontRender->RenderFont( 750, 0, l_str );
				cGameApp::m_spGlyphFontRender->RenderFont( 750, 30, UT::CharToWchar(cGameApp::m_sTimeAndFPS.GetFPS()) );
			}
			if( m_spMonsterManager  )
				m_spMonsterManager->DebugRender();
			//if( m_pBulletManager  )
			//	m_pBulletManager->DebugRender();

			if(m_spProbabilityFish)
				m_spProbabilityFish->DebugRender();

			//if(m_spSceneChange)
			//	m_spSceneChange->DebugRender();
			//for( int i=0;i<g_iAllDebugInfo;++i )
			//{
			//	std::wstring	l_str = UT::ComposeMsgByFormat(L"%s:%.4f,%.4f",g_strDebugInfo[i],g_fUpdateUsingTime[i],g_fRenderTime[i]);
			//	cGameApp::m_spGlyphFontRender->RenderFont(150,30*i,l_str.c_str());
			//}
			//std::wstring	l_str = UT::ComposeMsgByFormat( L"Frame:%d", g_uiFrame );
			//cGameApp::m_spGlyphFontRender->RenderFont( 0, 650, l_str.c_str() );
		}
	}
	glDisable(GL_TEXTURE_2D);
	glDisable2D();
#ifdef WIN32
	SwapBuffers(cGameApp::m_sHdc);
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
	if( m_spControlPanel&&m_spControlPanel->IsInControlPanelMode() )
	{
		m_spControlPanel->MouseDown(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
		return;
	}
}

void	cFishApp::MouseMove(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseMove(e_iPosX,e_iPosY);
	if( m_spControlPanel&&m_spControlPanel->IsInControlPanelMode() )
	{
		m_spControlPanel->MouseMove(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
		return;
	}
}

void	cFishApp::MouseUp(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseUp(e_iPosX,e_iPosY);
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
	if( m_sbDebugFunctionWorking && !this->m_sbDeviceExist )
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
	//if( e_char == 8 )
	//{
	//	g_sbCollisionRender = !g_sbCollisionRender;
	//}
	if ( e_char == 46 )
		m_spControlPanel->m_bPressOpenControlPanel = false;

	if( cGameApp::m_sbDebugFunctionWorking )
	{
		if ( cGameApp::m_sucKeyData[ 17 ] && e_char == 38 )
		{
			switch ( cGameApp::m_seDeviceDirection )
			{
				case eDD_PORTRAIT:        cGameApp::m_seDeviceDirection = eDD_UPSIDE_DOWN;     break;
				case eDD_UPSIDE_DOWN:     cGameApp::m_seDeviceDirection = eDD_LANDSCAPE_LEFT;  break;
				case eDD_LANDSCAPE_LEFT:  cGameApp::m_seDeviceDirection = eDD_LANDSCAPE_RIGHT; break;
				case eDD_LANDSCAPE_RIGHT: cGameApp::m_seDeviceDirection = eDD_PORTRAIT;        break;
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
	if( m_sbDebugFunctionWorking && !this->m_sbDeviceExist && m_spPlayerManager )
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