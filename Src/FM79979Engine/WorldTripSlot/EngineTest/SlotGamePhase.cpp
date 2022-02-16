#include "stdafx.h"
#include "SlotGamePhase.h"
#include "SlotRule.h"
#include "SpinPhase.h"
#include "BetPhase.h"
#include "CalculateScorePhase.h"
#include "BaseExtraGame.h"
#include "ExtraGamePhase.h"
#include "GameNetwork.h"
#include "StageData.h"
#include "WorldTripSlotApp.h"
#include "SlotUI.h"
#include "SlotBetResult.h"
//for limimt viewport
//en 
WCHAR*	BET_LINES_MPDI_FILE_NAME					= L"WorldTripSlot/ImageData/line25.mpdi";
WCHAR*	UI_MPDILIST_FILE_NAME						= L"WorldTripSlot/ImageData/ImageData_EN/UI.mpdi";
WCHAR*	FREE_GAME_BONUS_MPDILIST_FILE_NAME			= L"WorldTripSlot/ImageData/ImageData_EN/FG.mpdi";
extern cImageButton*	g_pAutoSpinButton;
cSlotGamePhase*			g_pSlotGamePhase = 0;
cSlotGamePhase::cSlotGamePhase()
{
	this->SetName(SLOT_GAME_PHASE);
	g_pSlotGamePhase = this;
	m_pSlotLineObjectManagerList = 0;
	m_pSlotRuleData = 0;
	m_pBetBehavior = 0;
	m_iFreeGame = 0;
	m_bFreeToPlay = false;
	m_bOpenNetWork = false;
	m_pbForceIntoFreeGame = 0;
	m_pExtraBonusMoneyRate = 0;
	m_pRestFreeGameTimes = 0;
	m_pBetBehaviorResult = 0;
	m_iNumLineBetable = 25;
	m_pstrFreeGameTest = 0;
	m_iExchangeRate = 1;
	m_pStageData = 0;
	m_pSlotUI = 0;
	m_pstrFreeGameTest = new std::wstring;
	m_pStageData = new cStageData();
	m_pStageData->m_strStageName = L"Test";
}

void	cSlotGamePhase::ReleaseGame()
{
	//WRITELOGFILE("clean resource",true);
	SAFE_DELETE(m_pExtraBonusMoneyRate);
	SAFE_DELETE(m_pRestFreeGameTimes);
	SAFE_DELETE(m_pSlotLineObjectManagerList);
	SAFE_DELETE(m_pSlotRuleData);
	SAFE_DELETE(m_pBetBehavior);
	SAFE_DELETE(m_pSlotUI);
	SAFE_DELETE(m_pbForceIntoFreeGame);
	SAFE_DELETE(m_pBetBehaviorResult);
	m_iExchangeRate = 1;
	m_iFreeGame = 0;
	m_bFreeToPlay = false;

}

cSlotGamePhase::~cSlotGamePhase()
{
	SAFE_DELETE(m_pStageData);
	SAFE_DELETE(m_pstrFreeGameTest);
	ReleaseGame();

}

void	cSlotGamePhase::Init()
{
	WRITELOGFILE("GameInit start");
	LoadGame();	
	WRITELOGFILE("Load Game processed...");
	cGameApp::m_sTimeAndFPS.Update();
}

void	cSlotGamePhase::Update(float e_fElpaseTime)
{
	cPhaseManager::Update(e_fElpaseTime);
	const float	l_fAllowMaxmimElpaseTime = 1/30.f;
	//make sure game working smoothly...
	if( e_fElpaseTime > l_fAllowMaxmimElpaseTime )
	{
		//e_fElpaseTime = l_fAllowMaxmimElpaseTime;
	}
	if( this->GetCurrentPhaseName() )
	{
		if(wcscmp(GetCurrentPhaseName(),BET_PHASE_NAME))
		{
			if( g_pAutoSpinButton )
			{
				g_pAutoSpinButton->SingnalProcess();
				g_pAutoSpinButton->Update(e_fElpaseTime);
				if( g_pAutoSpinButton->GetMouseBehavior() != eOMB_NONE )
				{
					cBetPhase*l_pBetPhase = (cBetPhase*)GetObject(BET_PHASE_NAME);
					l_pBetPhase->SetAutoSpin( false );
					g_pAutoSpinButton->Init();
				}
			}
		}
	}
	if(wcscmp(GetCurrentPhaseName(),EXTRA_GAME_PHASE_NAME))
	{
		if( cGameApp::m_sucKeyData['1'] )
		{
			*m_pstrFreeGameTest = L"JP1";
		}
		if( cGameApp::m_sucKeyData['2'] )
		{
			*m_pstrFreeGameTest = L"JP2";
		}
		else
		if( cGameApp::m_sucKeyData['3'] )
		{
			*m_pstrFreeGameTest = L"Free";
		}
		else
		if( cGameApp::m_sucKeyData['4'] )
		{
			//this->m_pPlayerData->m_i64Money += 1000;
		}
	}
}

void	cSlotGamePhase::Render()
{
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glClearColor( 0,0,0,1 );
	glClearDepth(1.0f);	
	glEnable(GL_TEXTURE_2D);
	glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);
	//cMatrix44	l_matProjection;
	//glhOrthof2((float*)l_matProjection.m, -720, 1720, 1366, -1366, -10000, 10000);
	//FATMING_CORE::SetupShaderViewProjectionMatrix(l_matProjection,true);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	//glAlphaFunc(GL_GREATER,0.001f);
	//m_OrthogonalCamera.Render();
	cPhaseManager::Render();
	m_pSlotUI->Render();
	//if( m_pSlotLineObjectManagerList )
		//this->m_pSlotLineObjectManagerList->DoScissor();
	//if( m_pSlotLineObjectManagerList )
		//this->m_pSlotLineObjectManagerList->UnDoScissor();
	const WCHAR*l_strPhaseName = GetCurrentPhaseName();
	if( l_strPhaseName&&!wcscmp(l_strPhaseName,BET_PHASE_NAME) )
	{
		cBetPhase*l_pBetPhase = (cBetPhase*)GetObject(BET_PHASE_NAME);
		l_pBetPhase->RenderReadMeMenu();
	}

	if( GetCurrentPhase() != -1 && wcscmp(this->GetCurrentPhaseName(),BET_PHASE_NAME))
	{
		if( g_pAutoSpinButton )
		{
			cBetPhase*l_pBetPhase = (cBetPhase*)GetObject(BET_PHASE_NAME);
			g_pAutoSpinButton->Render();
			if(l_pBetPhase->IsAutoSpin() && g_pAutoSpinButton->GetMouseBehavior() == eOMB_NONE )
			{
				g_pAutoSpinButton->SetColor(Vector4::One);
				g_pAutoSpinButton->Render();
			}
			else
			{
				cSubMPDI*l_pPressedImage = dynamic_cast<cSubMPDI*>(g_pAutoSpinButton->GetPressedImage());
				l_pPressedImage->GetPointData(0)->vColor = Vector4::One;
				l_pPressedImage->Render();
			}
		}
	}

	glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);
	glDisable(GL_TEXTURE_2D);
	cPhaseManager::DebugRender();
	glDisable2D();
}

//void	cSlotGamePhase::MouseDown(int e_iPosX,int e_iPosY)
//{
//
//}
//
//void	cSlotGamePhase::MouseMove(int e_iPosX,int e_iPosY)
//{
//
//}
//
//void	cSlotGamePhase::MouseUp(int e_iPosX,int e_iPosY)
//{
//
//}


//void	cSlotGamePhase::KeyUp(char e_char)
//{
//
//}
//
//void	cSlotGamePhase::KeyDown(char e_char)
//{
//
//}
//
//void	cSlotGamePhase::KeyPress(char e_cKey)
//{
//
//}

void	cSlotGamePhase::DebugRender()
{

}

const	WCHAR*	cSlotGamePhase::GetNextPhaseName()
{
	return MAP_VIEW_PHASE;
}

void	cSlotGamePhase::FetchData(const WCHAR*e_strPhaseName,void*e_pData)
{
	if(cGameApp::m_spstrErrorMsgString)
		cGameApp::m_spstrErrorMsgString->clear();
	std::wstring*	l_pstrStageName = (std::wstring*)e_pData;
	this->m_pStageData->m_strStageName = l_pstrStageName->c_str();
}

void	cSlotGamePhase::LoadGameSetup()
{
	//get language setup from device
	std::wstring			l_strLanguage = L"CH";

	BET_LINES_MPDI_FILE_NAME	= L"WorldTripSlot/ImageData/line25.mpdi";

	if( !wcscmp(L"CH",l_strLanguage.c_str()) )
	{
		FREE_GAME_BONUS_MPDILIST_FILE_NAME				= L"WorldTripSlot/ImageData/ImageData_CH/FG.mpdi";
			//case 25:
				UI_MPDILIST_FILE_NAME							= L"WorldTripSlot/ImageData/ImageData_CH/UI25/UI.mpdi";
			//	break;
			//case 15:
			//	UI_MPDILIST_FILE_NAME							= L"WorldTripSlot/ImageData/ImageData_CH/UI15/UI.mpdi";
			//	break;
			//case 9:
			//	UI_MPDILIST_FILE_NAME							= L"WorldTripSlot/ImageData/ImageData_CH/UI9/UI.mpdi";
			//	break;
	}
	else
	if( !wcscmp(L"EN",l_strLanguage.c_str()) )
	{
		FREE_GAME_BONUS_MPDILIST_FILE_NAME				= L"WorldTripSlot/ImageData/ImageData_EN/FG.mpdi";
		//switch(*l_pEventVariable->GetInt())
		{
			//case 25:
				UI_MPDILIST_FILE_NAME							= L"WorldTripSlot/ImageData/ImageData_EN/UI25/UI.mpdi";
			//	break;
			//case 15:
			//	UI_MPDILIST_FILE_NAME							= L"WorldTripSlot/ImageData/ImageData_EN/UI15/UI.mpdi";
			//	break;
			//case 9:
			//	UI_MPDILIST_FILE_NAME							= L"WorldTripSlot/ImageData/ImageData_EN/UI9/UI.mpdi";
			//	break;
		}
	}
}

void	cSlotGamePhase::SetupSpinData(const WCHAR*e_strStageName)
{
	m_pSlotLineObjectManagerList = new cSlotObjectManagerList;
	m_pSlotRuleData = new cSlotRuleData;
	m_pBetBehavior = new cSlotBetBehavior();
	std::string	l_strSlotRuleData = UT::WcharToChar(cGameApp::m_sstrGameAppName).c_str();
	l_strSlotRuleData	+= "/";
	l_strSlotRuleData	+= UT::WcharToChar(e_strStageName).c_str();
	l_strSlotRuleData	+= "_BetRuleData.xml";
	//
	l_strSlotRuleData = "WorldTripSlot/BetRule_25.xml";
	bool	l_b = m_pSlotRuleData->ParseWithMyParse(l_strSlotRuleData.c_str());
	if( !l_b )
	{
		UT::ErrorMsg(l_strSlotRuleData.c_str(),"parse file failed");
	}
	m_pSlotUI = new cSlotUI();
	cNodeISAX	l_NodeISAX;
	l_b = l_NodeISAX.ParseDataIntoXMLNode(SPIN_OBJECT_FALL_DOWN_DATA);
	if( l_b )
	{
		TiXmlElement*l_pElement = l_NodeISAX.GetRootElement();
		cMPDIList*l_pMPDIList = cGameApp::m_spAnimationParser->GetMPDIListByFileName(UI_MPDILIST_FILE_NAME);
		Vector4	l_vFrameRect = m_pSlotRuleData->m_vShowRect;
		cMPDI*l_pSpinObject = l_pMPDIList->GetObject(SPIN_OBJECT);
		cSubMPDI*l_pSubMPDISpinObject = l_pSpinObject->GetObject(0);//SPIN_OBJECT
		int	l_iCount = (int)l_pSubMPDISpinObject->GetOriginalPointList().size();
		int	l_iChildElementCount = l_pElement->ChildElementCount();
		assert(l_iChildElementCount == l_iCount && "daya count is not match");
		assert(l_iCount == m_pSlotRuleData->m_TableCount.x&&"line data count is not match!!BetRule.xml");
		//avoid data is not match
		TiXmlElement*l_pChildElement = l_pElement->FirstChildElement();
		cPuzzleImage*l_pPI = cGameApp::GetPuzzleImageByFileName(L"WorldTripSlot/ImageData/SpinObject.pi");
		for( int i=0;i<m_pSlotRuleData->m_TableCount.x;++i )
		{
			assert(!wcscmp(l_pChildElement->Value(),L"SpinData"));
			float	l_fTime = 2.f;
			float	l_fDistance = 3950.f;
			const WCHAR*l_strSpinTime = l_pChildElement->Attribute(L"Time");
			const WCHAR*l_strSpinDistance = l_pChildElement->Attribute(L"SpinDistance");
			if( l_strSpinTime )
				l_fTime = (float)_wtof(l_strSpinTime);
			if( l_strSpinDistance )
				l_fDistance = (float)_wtof(l_strSpinDistance);
			Vector3	l_vPos = Vector3::Zero;
			if( (int)l_pSubMPDISpinObject->GetOriginalPointList().size() > i )
				l_vPos = l_pSubMPDISpinObject->GetOriginalPointList()[i];
			l_vFrameRect.x = l_vPos.x;
			l_vFrameRect.y = l_vPos.y;
			cSlotObjectManager*l_pSlotLineObjectManager = new cSlotObjectManager(l_vFrameRect,l_fTime,l_fDistance,l_pPI,m_pSlotRuleData->m_TableCount.y,l_pPI->Count());
			m_pSlotLineObjectManagerList->AddObjectNeglectExist(l_pSlotLineObjectManager);
			l_pChildElement = l_pChildElement->NextSiblingElement();
		}
	}
	m_pBetBehavior->Init();
}
//namespace IOSM
//{
//	extern unsigned char g_ucOutCount;
//}

void	cSlotGamePhase::LoadSound(char*e_strFileName)
{
	bool	l_b = cGameApp::m_spSoundParser->Parse(e_strFileName);
	//cGameApp::m_spSoundParser->GetObject(NORMAL_BGM_SOUND_NAME)->Play(true);
	//cGameApp::m_spSoundParser->SetVolume(0.01f);//>GetObject(NORMAL_BGM_SOUND_NAME)->SetVolume(0.1f);
}

void	cSlotGamePhase::PhaseSetup()
{
	Destroy();
	cSpinPhase*l_pSpinPhase = new cSpinPhase();
	cBetPhase*l_pBetPhase = new cBetPhase();
	cCalculateScorePhase*l_pCalculateScorePhase = new cCalculateScorePhase();
	cExtraGamePhase*l_pFreeGamePhase = new cExtraGamePhase();
	//do not call init to avoid into free game and else
	//l_pFreeGamePhase->Init();
	this->AddObjectNeglectExist(l_pBetPhase);
	this->AddObjectNeglectExist(l_pSpinPhase);
	this->AddObjectNeglectExist(l_pFreeGamePhase);
	this->AddObjectNeglectExist(l_pCalculateScorePhase);
	SetCurrentCurrentPhase(l_pBetPhase->GetName());
}

void	cSlotGamePhase::LoadGame()
{
	ReleaseGame();
	//
	if( !m_pRestFreeGameTimes )
		m_pRestFreeGameTimes = new std::vector<int>;
	//m_pBetBehaviorResult = 0;
	m_pBetBehaviorResult = new cSlotBetResult(99.f);
	LoadGameSetup();
	SetupSpinData(m_pStageData->m_strStageName.c_str());
	LoadSound(SOUND_FIL_ENAME);
	m_pExtraBonusMoneyRate = new std::vector<int>;
	//================================================	
	//debug test
	m_pbForceIntoFreeGame = new bool[eFGL_MAX+1];
	memset(m_pbForceIntoFreeGame,0,sizeof(bool)*eFGL_MAX);
	//================================================
	PhaseSetup();
	WRITELOGFILE("load game OK!");
	cGameApp::m_sTimeAndFPS.Update();
}