#include "stdafx.h"
#include "SceneChange.h"
//
#include "GameApp.h"
//
#include "../ControlPanel/ControlPanel.h"
#include "../Monster/MonsterManager.h"
#include "../Monster/FishGroup.h"
#include "../MiniGame/MiniGameManager.h"
#include "../FileNameDefine.h"
//
#include "../ProbabilityFish/FishProbability.h"


sSpecialSceneData::sSpecialSceneData()
{
	fTime = 0.f;pFishGroup = 0;MovingMethod = eMM_MAX;
}

sSpecialSceneData::~sSpecialSceneData()
{
	SAFE_DELETE(pFishGroup); 
}

eMovingMethod	GetMovingMethod(const WCHAR*e_strName)
{
	if( !wcscmp(L"Vertical",e_strName) )
	{
		return eMM_VERTICAL;
	}
	else
	if( !wcscmp(L"Horizontal",e_strName) )
	{
		return eMM_HORIZONTAL;
	}
	else
	if( !wcscmp(L"Code",e_strName) )
	{
		return eMM_CODE;
	}
	else
	if( !wcscmp(L"Path",e_strName) )
	{
		return eMM_PATH;
	}
	return eMM_MAX;
}

cSceneChange::cSceneChange()
{
	m_fFishBackTimer = 0.f;
	m_pBGAnimation = 0;
	m_pBGAnimationOld = 0;
	m_iCurrentNormalBGIndex = 0;
	m_iCurrentSpecialBGIndex = 0;
	m_bStopChangeScene = false;
	m_bSceneIsSpecial = false;
	m_fFishRunAwaySpeed = 2.f;
	m_iTotalProbability = 0;
	m_pBGImage = 0;
	m_CurrentBGName = "";
	m_pPrtEmitter = 0;
	m_pbSceneChange = &cFishApp::m_sbSceneChange;
	m_pWaterMPDI = 0;
	m_pWaterList = 0;
	m_pLastBGImage = 0;
	m_pShowChangeSceneMPDI = 0;
	m_BubbleClamDownTC.SetTargetTime(2.f);
	m_StopGenerateFishTime.SetTargetTime(15.f);
	m_pFishGroupList = new cNamedTypedObjectVector<cFishGroup>;
	m_TimeCircleRestrict = Vector2(50,300);
}

cSceneChange::~cSceneChange()
{
	SAFE_DELETE(m_pFishGroupList);
	//SAFE_DELETE( m_pBGImage );
	//SAFE_DELETE( m_pLastBGImage );
	DELETE_VECTOR(m_SpecialSceneList)
}

void	cSceneChange::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	COMPARE_VALUE("NormaleBG")
	{
		ProcessNormalBG(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("SpecialBG")
	{
		ProcessSpecialBG(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("BG")
	{
		ProcessBG(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("SpecialScene")
	{
		ProcessSpecialScene(e_pTiXmlElement);
	}
	else
	{

	}
}

void	cSceneChange::ProcessSpecialBG(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strBGName = e_pTiXmlElement->Attribute(L"Name");
	const WCHAR*l_strProbability = e_pTiXmlElement->Attribute(L"Probability");
	m_SpecialBG.ImageNameList = GetStringListByCommaDivide(l_strBGName);
	m_SpecialBG.iProbability = _wtoi(l_strProbability);
	m_iTotalProbability += m_SpecialBG.iProbability;
	int l_iIndex;
	for( int i=0; i<(int)m_SpecialBG.ImageNameList.size(); ++i )
	{
		cBaseImage*l_SpecialBG = dynamic_cast<cBaseImage*>(
			cGameApp::m_spImageParser->GetObject(
			UT::CharToWchar(UT::GetFileNameWithoutFullPath(m_SpecialBG.ImageNameList[i]))
			)
			);
		if( !l_SpecialBG )
		{
			l_SpecialBG = new cBaseImage( m_SpecialBG.ImageNameList[i].c_str() );
			if(!cGameApp::m_spImageParser->AddObject(l_SpecialBG))
			{
				UT::ErrorMsg(m_SpecialBG.ImageNameList[i].c_str(),"exist!?");
			}
		}
		const WCHAR *l_str = &UT::CharToWchar(UT::GetFileNameWithoutFullPath(m_SpecialBG.ImageNameList[i].c_str()))[2];
		l_iIndex = GetInt(l_str);
		std::wstring	l_strMPDIListName = UT::ComposeMsgByFormat( L"%s%d.mpdi", SCENE_CHANGE_BG_ANIMATION, l_iIndex );
		cMPDIList *l_pMPDIList = cGameApp::GetMPDIListByFileName( l_strMPDIListName.c_str() );
		if( !l_pMPDIList )
		{
			UT::ErrorMsg( l_strMPDIListName.c_str(), L"not exit" );
		}
	}
}

//<BG ChangeCircle="300,500">
//	<NormaleBG Name="BG1.png,BG2.png,BG3.png" Probability="50" />
//	<SpecialBG Name="BG1.png,BG2.png,BG3.png" Probability="50" />
//</BG>
void	cSceneChange::ProcessNormalBG(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strBGName = e_pTiXmlElement->Attribute(L"Name");
	const WCHAR*l_strProbability = e_pTiXmlElement->Attribute(L"Probability");
	m_NormalBG.ImageNameList = GetStringListByCommaDivide(l_strBGName);
	m_NormalBG.iProbability = _wtoi(l_strProbability);
	m_iTotalProbability += m_NormalBG.iProbability;
	int l_iIndex;
	for( int i=0; i<(int)m_NormalBG.ImageNameList.size(); ++i )
	{
		cBaseImage*l_NormalBG = dynamic_cast<cBaseImage*>(cGameApp::m_spImageParser->GetObject(UT::CharToWchar(GetFileNameWithoutFullPath(m_NormalBG.ImageNameList[i].c_str()))));
		if( !l_NormalBG )
		{
			l_NormalBG = new cBaseImage( m_NormalBG.ImageNameList[i].c_str() );
			if(!cGameApp::m_spImageParser->AddObject(l_NormalBG))
			{
				UT::ErrorMsg(m_NormalBG.ImageNameList[i].c_str(),"exist!?");
			}
		}
		auto l_str = GetFileNameWithoutFullPath(l_NormalBG->GetName(),true);
		l_iIndex = GetInt(&l_str.c_str()[2]);
		std::wstring l_strMPDIListName = UT::ComposeMsgByFormat( L"%s%d.mpdi", SCENE_CHANGE_BG_ANIMATION, l_iIndex );
		cMPDIList* l_pMPDIList = cGameApp::GetMPDIListByFileName( l_strMPDIListName.c_str() );
		
		if( !l_pMPDIList )
		{
			UT::ErrorMsg( l_strMPDIListName.c_str(), L"not exit" );
		}
	}
}

void	cSceneChange::ProcessSpecialScene(TiXmlElement*e_pTiXmlElement)
{
	sSpecialSceneData	*l_pSpecialSceneData = new sSpecialSceneData;
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("Time")
		{
			l_pSpecialSceneData->fTime = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("FileName")
		{
			if( l_pSpecialSceneData->MovingMethod == eMM_PATH )
				l_pSpecialSceneData->pFishGroup = new cFishGroup(2,UT::WcharToChar(l_strValue).c_str());
			else if( l_pSpecialSceneData->MovingMethod == eMM_CODE )
				l_pSpecialSceneData->pFishGroup = new cFishGroup();
			else
			{
				l_pSpecialSceneData->pFishGroup = new cFishGroup();
				l_pSpecialSceneData->pFishGroup->ProcessData(UT::WcharToChar(l_strValue).c_str());
			}
		}
		else
		COMPARE_NAME("MovingMethod")
		{
			l_pSpecialSceneData->MovingMethod = GetMovingMethod(l_strValue);
		}
	PARSE_NAME_VALUE_END
	m_SpecialSceneList.push_back(l_pSpecialSceneData);
}

void	cSceneChange::ProcessBG(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strChangeCircle = e_pTiXmlElement->Attribute(L"ChangeCircle");
	m_TimeCircleRestrict = GetVector2(l_strChangeCircle);
	const WCHAR*l_strBubbleMoving = e_pTiXmlElement->Attribute(L"BubbleMoving");
	m_BubbleMovingTC.SetTargetTime((float)_wtof(l_strBubbleMoving));
	const WCHAR*l_strFishRunAwaySpeed = e_pTiXmlElement->Attribute(L"FishRunAwaySpeed");
	if( l_strFishRunAwaySpeed )
	{
		m_fFishRunAwaySpeed = (float)_wtof(l_strFishRunAwaySpeed);
	}
	const WCHAR*l_strFishGroupMovingTime = e_pTiXmlElement->Attribute(L"FishGroupMovingTime");
	if( l_strFishGroupMovingTime )
	{
		m_StopGenerateFishTime.SetTargetTime((float)_wtof(l_strFishRunAwaySpeed));
	}
}

void	cSceneChange::Init()
{
	m_bStopChangeScene = false;
	m_eSceneChangeStatus = eSCS_NONE;
	if(!this->Parse(SCENE_CHANGE_SETUP_FILE))
	{
		UT::ErrorMsg(SCENE_CHANGE_SETUP_FILE,"parse failed!");
	}
	ChangeScene();
	m_ChangeSceneCircle.SetTargetTime(m_TimeCircleRestrict.Rand());
	m_pWaterList = cGameApp::GetMPDIListByFileName(BG_WATER_MPDI);
	if( m_pWaterList )
	{
		m_pWaterMPDI = m_pWaterList->GetObject(0);
		m_pWaterMPDI->Init();
	}
	m_pPrtEmitter = cGameApp::GetPrtEmitter(SCENE_CHANGE_PRT,L"Bobble");
	if( !m_pPrtEmitter )
	{
		UT::ErrorMsg(SCENE_CHANGE_PRT,"parse failed");
	}
	cMPDIList*l_pMPDIList = cGameApp::GetMPDIListByFileName(SCENE_CHANGE_ALERT);
	m_pShowChangeSceneMPDI = l_pMPDIList->GetObject(L"FishTimeShow");
	m_pShowChangeSceneMPDI->Init();
	//
	if(cFishApp::m_spMonsterManager)
		cFishApp::m_spMonsterManager->m_FishShowProbabilityList.bPauseGenerateMonster = false;
}


void	cSceneChange::UpdateeSCS_NONE(float e_fElpaseTime)
{
	if ( cGameApp::m_spSoundParser->GetObject( L"logo" )->IsPlay() )
	{
		cGameApp::SoundPlay( L"logo", false );
		cGameApp::SoundPlay( L"bg",   true  );
	}

	m_ChangeSceneCircle.Update(e_fElpaseTime);
	if( m_ChangeSceneCircle.bTragetTimrReached )
	{
		int	l_iValue = rand(0,this->m_iTotalProbability);
		if( this->m_NormalBG.iProbability> l_iValue )
		{
			m_bSceneIsSpecial = false;
			ChangeScene();
		}
		else
		{
			m_bSceneIsSpecial = true;
			ChangeSpecialScene();
		}
		m_ChangeSceneCircle.SetTargetTime(m_TimeCircleRestrict.Rand());
		m_pShowChangeSceneMPDI->Init();
		m_eSceneChangeStatus = eSCS_PLAY_SCENE_CHANGE_MPDI;
		*m_pbSceneChange = true;
	}
}

void	cSceneChange::UpdateeeSCS_PLAY_SCENE_CHANGE_MPDI(float e_fElpaseTime)
{
	m_pBGAnimationOld->Update(e_fElpaseTime);
	if( !m_pShowChangeSceneMPDI->IsAnimationDone() )
	{
		m_pShowChangeSceneMPDI->Update(e_fElpaseTime);
		if(m_pShowChangeSceneMPDI->IsAnimationDone())
		{
			cFishApp::m_sfMonsterUpdateSpeed = this->m_fFishRunAwaySpeed;
			m_eSceneChangeStatus = eSCS_WAIT_FISH_LEAVE;
			cFishApp::m_spMonsterManager->MonsterLeaveScene();
		}
	}
}

void	cSceneChange::UpdateeeSCS_WAIT_FISH_LEAVE(float e_fElpaseTime)
{
	m_pBGAnimationOld->Update(e_fElpaseTime);
	if( cFishApp::m_spMonsterManager->GetCountMonsterAlive() == 0)
	{
		m_BubbleMovingTC.Start();
		m_BubbleClamDownTC.Start();
		m_pPrtEmitter->Emit(Vector3(cGameApp::m_spOpenGLRender->m_vGameResolution.x, cGameApp::m_spOpenGLRender->m_vGameResolution.y/2.f,0.f));
		m_pPrtEmitter->SetLoop(true);
		m_eSceneChangeStatus = eSCS_BUBBLE_GO;
		cGameApp::SoundPlay(L"30",true);
	}
}

void	cSceneChange::UpdateeeSCS_BUBBLE_GO(float e_fElpaseTime)
{
	if( m_pBGAnimationOld )
		m_pBGAnimationOld->Update(e_fElpaseTime);
	m_pPrtEmitter->Update(e_fElpaseTime);
	m_BubbleMovingTC.Update(e_fElpaseTime);
	if( m_BubbleMovingTC.bTragetTimrReached )
	{
		//m_pBGImage->SetUV(l_fUV);
		//m_pBGImage->SetImageShowWidth((int)cGameApp::m_svGameResolution.x);
		m_BubbleClamDownTC.Update(e_fElpaseTime);
		m_pPrtEmitter->SetLoop(false);
		m_eSceneChangeStatus = eSCS_WAIT_BUBBLE_DISSAPEAR;
		m_pWaterMPDI->SetColor(Vector4(1,1,1,1));
	}
	else
	{
		m_pWaterMPDI->SetColor(Vector4(0.5,0.5,0.5,0.5));
		m_pPrtEmitter->SetPos(Vector3(cGameApp::m_spOpenGLRender->m_vGameResolution.x/2.f, cGameApp::m_spOpenGLRender->m_vGameResolution.y/2.f,0.f));
		this->m_pBGImage->SetColor(Vector4(1,1,1,m_BubbleMovingTC.GetLERP()));
		if(m_pBGAnimation)
			m_pBGAnimation->SetColor(Vector4(1,1,1,m_BubbleMovingTC.GetLERP()));
		if(m_pBGAnimationOld)
			m_pBGAnimationOld->SetColor(Vector4(1,1,1,1-m_BubbleMovingTC.GetLERP()));
		if( m_pLastBGImage )
		{
			m_pLastBGImage->SetColor(Vector4(1,1,1,1-m_BubbleMovingTC.GetLERP()));
		}
	}
}

void	cSceneChange::UpdateeeSCS_WAIT_BUBBLE_DISSAPEAR(float e_fElpaseTime)
{
	m_pPrtEmitter->Update(e_fElpaseTime);
	m_BubbleClamDownTC.Update(e_fElpaseTime);
	if( m_BubbleClamDownTC.bTragetTimrReached )
	{
		this->m_pBGImage->SetColor(Vector4(1,1,1,1));
		this->m_pLastBGImage->SetColor(Vector4(1,1,1,1));
		*m_pbSceneChange = false;
		m_eSceneChangeStatus = eSCS_NONE;
		cFishApp::m_sfMonsterUpdateSpeed = 1.f;
		if( m_bSceneIsSpecial )
		{
			cGameApp::SoundPlay(L"bg",false);
			cGameApp::SoundPlay(L"bg02",false);
			cGameApp::SoundPlay(L"gold",true);
			cGameApp::SoundPlay(L"30",false);
			if( m_SpecialSceneList.size() )
			{
				static	int	l_siSPIndex = -1;
				++l_siSPIndex;
				if( l_siSPIndex >= (int)m_SpecialSceneList.size() )
				{
					l_siSPIndex = 0;
				}
				sSpecialSceneData*l_pSpecialSceneData = m_SpecialSceneList[l_siSPIndex];
				//check for lucky roulette
				if( l_pSpecialSceneData->MovingMethod == eMM_CODE )
				{					
					++l_siSPIndex;
					if( l_siSPIndex >= (int)m_SpecialSceneList.size() )
					{
						l_siSPIndex = 0;
					}
					l_pSpecialSceneData = m_SpecialSceneList[l_siSPIndex];
				} //end

				m_StopGenerateFishTime.SetTargetTime(l_pSpecialSceneData->fTime);
				if(l_pSpecialSceneData->MovingMethod == eMM_HORIZONTAL)
				{
					l_pSpecialSceneData->pFishGroup->FishGroupGoLeftToRight(0,m_StopGenerateFishTime.fTargetTime);
					l_pSpecialSceneData->pFishGroup->FishGroupGRightTooLeft(1,m_StopGenerateFishTime.fTargetTime);
				}
				else
				if(l_pSpecialSceneData->MovingMethod == eMM_VERTICAL)
				{
					l_pSpecialSceneData->pFishGroup->FishGroupGoUpToDown(0,m_StopGenerateFishTime.fTargetTime);
					l_pSpecialSceneData->pFishGroup->FishGroupGoDownToUp(1,m_StopGenerateFishTime.fTargetTime);
				}
				else
				if(l_pSpecialSceneData->MovingMethod == eMM_PATH)
				{
					l_pSpecialSceneData->pFishGroup->f_PrizeII_Call( m_StopGenerateFishTime.fTargetTime );					
				}

				if(cFishApp::m_spMonsterManager)
					cFishApp::m_spMonsterManager->m_FishShowProbabilityList.bPauseGenerateMonster = true;
				m_StopGenerateFishTime.Start();
			}
		}
		else
		{
			if(cGameApp::m_spSoundParser->GetObject(L"bg")->IsPlay())
			{
				cGameApp::SoundPlay( L"bg", false );
				cGameApp::SoundPlay( L"bg02", true );
			}
			else
			if( cGameApp::m_spSoundParser->GetObject(L"bg02")->IsPlay() )
			{
				cGameApp::SoundPlay( L"bg", true );
				cGameApp::SoundPlay( L"bg02", false );
			}
			else
			{
				cGameApp::SoundPlay( L"bg", true );
			}
		}
	}
}


void	cSceneChange::Update(float e_fElpaseTime)
{
	if( m_pBGAnimation )
		m_pBGAnimation->Update(e_fElpaseTime);

	if(m_pWaterMPDI)
		m_pWaterMPDI->Update(e_fElpaseTime);
	if( m_bStopChangeScene )
		return;
	if( cFishApp::m_spProbabilityFish )
	{
		if( cFishApp::m_spMonsterManager->m_FishShowProbabilityList.bPauseGenerateMonster )
		{
			if(!m_StopGenerateFishTime.bTragetTimrReached)
			{
				m_StopGenerateFishTime.Update(e_fElpaseTime);

				int l_iPlayerCount  = cFishApp::m_spControlPanel->m_iPlayerCount;
				int	l_iFishCount = cFishApp::m_spMonsterManager->GetCountMonsterVisible();
				if(l_iFishCount <= (l_iPlayerCount*2))
					m_fFishBackTimer += e_fElpaseTime;
				else
					m_fFishBackTimer = 0;
				if(m_StopGenerateFishTime.bTragetTimrReached||cFishApp::m_spMonsterManager->GetCountMonsterAlive() == 0   ||  (l_iFishCount <= (l_iPlayerCount*2) && m_fFishBackTimer >20.0 ) )
				{
					m_fFishBackTimer = 0.f;
					if( cFishApp::m_spMonsterManager )
						cFishApp::m_spMonsterManager->m_FishShowProbabilityList.bPauseGenerateMonster = false;
					cGameApp::SoundPlay(L"gold",false);
					if(rand()%2)
					{
						cGameApp::SoundPlay(L"bg",true);
					}
					else
						cGameApp::SoundPlay(L"bg02",true);
				}
			}
		}
	}
	switch(m_eSceneChangeStatus)
	{
		case 	eSCS_NONE:
				UpdateeSCS_NONE(e_fElpaseTime);
			break;
		case 	eSCS_PLAY_SCENE_CHANGE_MPDI:
				UpdateeeSCS_PLAY_SCENE_CHANGE_MPDI(e_fElpaseTime);
			break;
		case 	eSCS_WAIT_FISH_LEAVE:
				UpdateeeSCS_WAIT_FISH_LEAVE(e_fElpaseTime);
			break;
		case 	eSCS_BUBBLE_GO:
				UpdateeeSCS_BUBBLE_GO(e_fElpaseTime);
			break;
		case 	eSCS_WAIT_BUBBLE_DISSAPEAR:
				UpdateeeSCS_WAIT_BUBBLE_DISSAPEAR(e_fElpaseTime);
			break;
	}
}

void	cSceneChange::Render()
{
	if( eSCS_NONE == m_eSceneChangeStatus )
	{
		if( m_pBGImage )
			m_pBGImage->Render();
		if(m_pBGAnimation)
			m_pBGAnimation->Render();
	}
	else
	{
		if( eSCS_WAIT_FISH_LEAVE == m_eSceneChangeStatus || eSCS_PLAY_SCENE_CHANGE_MPDI == m_eSceneChangeStatus )
		{
			m_pLastBGImage->Render();
			if(m_pBGAnimationOld)
				m_pBGAnimationOld->Render();
		}
		else
		if( eSCS_BUBBLE_GO == m_eSceneChangeStatus || eSCS_WAIT_BUBBLE_DISSAPEAR == m_eSceneChangeStatus )
		{
			m_pBGImage->Render();
			if(m_pBGAnimation)
				m_pBGAnimation->Render();
			this->m_pLastBGImage->Render();
			if(m_pBGAnimationOld)
				m_pBGAnimationOld->Render();
			m_pPrtEmitter->Render();
		}
	}
}

void	cSceneChange::LastRender()
{
	if( eSCS_PLAY_SCENE_CHANGE_MPDI == m_eSceneChangeStatus )
		m_pShowChangeSceneMPDI->Render();
	//if( eSCS_NONE == m_eSceneChangeStatus )
	{
		if( m_pWaterMPDI )
			m_pWaterMPDI->Render();
	}
}

void	cSceneChange::DebugRender()
{
	if( !*m_pbSceneChange )
	{
		std::wstring	l_str = UT::ComposeMsgByFormat(L"ToChangeScene%.2f",m_ChangeSceneCircle.fRestTime);
		cGameApp::m_spGlyphFontRender->RenderFont(0,600,l_str.c_str());
	}
}

int		IndexOfVector(const char*e_strName,std::vector<std::string>*e_pList)
{
	size_t	l_uiSize = e_pList->size();
	for( size_t i=0;i<l_uiSize;++i )
	{
		if(!strcmp((*e_pList)[i].c_str(),e_strName))
		{
			return (int)i;
		}
	}
	return -1;
}
void	cSceneChange::ChangeScene()
{
	m_iCurrentNormalBGIndex++;
	assert(m_NormalBG.ImageNameList.size()>0);
	if( m_iCurrentNormalBGIndex >= (int)m_NormalBG.ImageNameList.size() )
	{
		m_iCurrentNormalBGIndex = 0;
	}
	int	l_iIndex = m_iCurrentNormalBGIndex;
	if(IndexOfVector(m_CurrentBGName.c_str(),&m_NormalBG.ImageNameList) == l_iIndex)
	{
		l_iIndex = UT::GetLoopIndex(l_iIndex+1,(int)m_NormalBG.ImageNameList.size());
	}
	m_pLastBGImage = m_pBGImage;
	if( m_NormalBG.ImageNameList.size() > 0 )
	{
		const char *l_strImageName = m_NormalBG.ImageNameList[l_iIndex].c_str();
		//m_pBGImage = new cBaseImage(l_strImageName);
		m_pBGImage = dynamic_cast<cBaseImage*>(cGameApp::m_spImageParser->GetObject( UT::CharToWchar(UT::GetFileNameWithoutFullPath(l_strImageName)) ));
		//m_pBGImage->SetImageShowWidth( (int)cGameApp::m_svGameResolution.x );
		//m_pBGImage->SetImageShowHeight( (int)cGameApp::m_svGameResolution.y );
		m_CurrentBGName = m_NormalBG.ImageNameList[l_iIndex];
		auto l_str = GetFileNameWithoutFullPath(m_pBGImage->GetName(), true);
		l_iIndex = GetInt(&l_str.c_str()[2]);
		std::wstring	l_strMPDIListName = UT::ComposeMsgByFormat(L"%s%d.mpdi",SCENE_CHANGE_BG_ANIMATION,l_iIndex);
		cMPDIList*l_pMPDIList = cGameApp::GetMPDIListByFileName(l_strMPDIListName.c_str());
		m_pBGAnimationOld = m_pBGAnimation;
		if( l_pMPDIList )
		{
			m_pBGAnimation = l_pMPDIList->GetObject(0);
			m_pBGAnimation->Init();
		}
		else
			m_pBGAnimation = 0;
	}
}

void	cSceneChange::ChangeSpecialScene()
{
	m_iCurrentSpecialBGIndex++;
	if( m_iCurrentSpecialBGIndex >= (int)m_SpecialBG.ImageNameList.size() )
	{
		m_iCurrentSpecialBGIndex = 0;
	}
	int	l_iIndex = m_iCurrentSpecialBGIndex;
	assert(m_SpecialBG.ImageNameList.size()>0);
	if(IndexOfVector(m_CurrentBGName.c_str(),&m_SpecialBG.ImageNameList) == l_iIndex)
	{
		l_iIndex = UT::GetLoopIndex(l_iIndex+1,(int)m_SpecialBG.ImageNameList.size());
	}
	m_pLastBGImage = m_pBGImage;
	const char *l_strImageName = m_SpecialBG.ImageNameList[l_iIndex].c_str();
	//m_pBGImage = new cBaseImage(l_strImageName);
	m_pBGImage = dynamic_cast<cBaseImage*>(cGameApp::m_spImageParser->GetObject( UT::CharToWchar(UT::GetFileNameWithoutFullPath(l_strImageName)) ));
	m_pBGImage->SetWidth((int)cGameApp::m_spOpenGLRender->m_vGameResolution.x);
	m_pBGImage->SetHeight((int)cGameApp::m_spOpenGLRender->m_vGameResolution.y);
	m_CurrentBGName = m_SpecialBG.ImageNameList[l_iIndex];

	auto l_str = GetFileNameWithoutFullPath(m_pBGImage->GetName(), true);
	l_iIndex = GetInt(&l_str.c_str()[2]);
	std::wstring	l_strMPDIListName = UT::ComposeMsgByFormat(L"%s%d.mpdi",SCENE_CHANGE_BG_ANIMATION,l_iIndex);
	cMPDIList*l_pMPDIList = cGameApp::GetMPDIListByFileName(l_strMPDIListName.c_str());
	m_pBGAnimationOld = m_pBGAnimation;
	if( l_pMPDIList )
	{
		m_pBGAnimation = l_pMPDIList->GetObject(0);
		m_pBGAnimation->Init();
	}
	else
		m_pBGAnimation = 0;
}

bool	cSceneChange::IsChangeScene()
{
	return m_eSceneChangeStatus == eSCS_NONE?false:true;
}

bool	cSceneChange::IsTimeReachSceneChange(float e_fRestTimeToReachSceneChange)
{
	if( !IsChangeScene() )
	{
		if( m_ChangeSceneCircle.fRestTime >= e_fRestTimeToReachSceneChange )
		{
			return false;
		}
	}
	return true;
}