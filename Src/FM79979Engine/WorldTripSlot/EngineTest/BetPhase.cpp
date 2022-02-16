#include "stdafx.h"
#include "BetPhase.h"
#include "SlotObject.h"
#include "SlotGamePhase.h"
#include "SlotRule.h"
#include "WinMoneyLine.h"
#include "GameMessage.h"
#include "GameNetwork.h"
#include "WorldTripSlotApp.h"
#include "SlotUI.h"
cBetPhase::cBetPhase()
{
	this->SetName(BET_PHASE_NAME);
	m_pConstantUI = 0;
	m_pBetDataButtonManaget = 0;
	m_bAutoSpin = false;
	m_pMenu = 0;
	m_iReadMeProgress = 0;
	m_ExchangeRateVector.push_back(1);
	m_iCurrentFocusExchangeRate = 0;
}

cBetPhase::~cBetPhase()
{
	SAFE_DELETE(m_pConstantUI);
	SAFE_DELETE(m_pBetDataButtonManaget);
}

bool	cBetPhase::ChecMoneyEncoughToBet(bool e_bMinusPlayerMoney)
{
	if( cWorldTripSlotApp::m_spPlayerData->m_i64Money >= g_pSlotGamePhase->m_pBetBehavior->m_iTotalBet )
	{
		//if( e_bMinusPlayerMoney )
		//	cWorldTripSlotApp::m_spPlayerData->m_i64Money -= g_pSlotGamePhase->m_pBetBehavior->m_iTotalBet;
		return true;
	}
	//play error sound.
	return false;
}

void	cBetPhase::Init()
{
	if( cWorldTripSlotApp::m_spPlayerData->m_i64Money < 9999 )
		cWorldTripSlotApp::m_spPlayerData->m_i64Money = 9999;
	if( !m_pConstantUI )
	{
		cMPDIList*l_pMPDIList = cGameApp::m_spAnimationParser->GetMPDIListByFileName(UI_MPDILIST_FILE_NAME);
		cMPDI*l_pMPDI = l_pMPDIList->GetObject(UPPER_UI);
		if( !l_pMPDI )
		{
			UT::ErrorMsg(L"UI"	,L"fetch  MPDI error");
		}
		m_pConstantUI = new cMPDI(l_pMPDI);
		m_pConstantUI->RefreshTotalPlayTime();
		m_pConstantUI->Init();
		m_pConstantUI->Update(EPSIONAL);
		m_pBetDataButtonManaget = new cBetDataButtonManaget(this);
		cNodeISAX	l_NodeISAX;
		bool	l_b = l_NodeISAX.ParseDataIntoXMLNode(BET_PHASE_DATA);
		if( l_b )
		{
			TiXmlElement*l_pElement = l_NodeISAX.GetRootElement();
			const WCHAR*l_strShowLineTime = l_pElement->Attribute(L"ShowLineTime");
			const WCHAR*l_strCoinExchangeRate = l_pElement->Attribute(L"CoinExchangeRate");
			if( l_strShowLineTime )
			{
				float	l_fTime = (float)_wtof(l_strShowLineTime);
				g_pSlotGamePhase->m_pSlotUI->m_pWinmoneyLines->ChangeShowLineTime(l_fTime);
			}
			if( l_strCoinExchangeRate )
			{
				m_ExchangeRateVector.clear();
				//l_strName = wcstok((WCHAR*)l_strCoinExchangeRate,L",");
				//while(l_strName)
				//{
				//	float	l_fValue =  (float)_wtof(l_strName);
				//	m_ExchangeRateVector.push_back(l_fValue);
				//	l_strName = wcstok(0,L",");
				//}
				m_ExchangeRateVector = GetValueListByCommaDivide<int>(l_strCoinExchangeRate);
			}
		}
	}
	g_pSlotGamePhase->m_pSlotLineObjectManagerList->Init();
	if( g_pSlotGamePhase->m_iFreeGame > 0 )
	{
		int	l_iValue = (*g_pSlotGamePhase->m_pRestFreeGameTimes)[0];
		l_iValue = (*g_pSlotGamePhase->m_pRestFreeGameTimes)[0]-1;
		if( l_iValue>0 )
		{
			(*g_pSlotGamePhase->m_pRestFreeGameTimes)[0] = l_iValue;
		}
		else
			(*g_pSlotGamePhase->m_pRestFreeGameTimes).erase(g_pSlotGamePhase->m_pRestFreeGameTimes->begin());
		this->m_bSatisfiedCondition = true;
		--g_pSlotGamePhase->m_iFreeGame;
		g_pSlotGamePhase->m_bFreeToPlay = true;

	}
	else
	{
		g_pSlotGamePhase->m_pBetBehavior->m_iBonusMoneyRate = 1;
		if( !cGameApp::m_spSoundParser->GetObject(NORMAL_BGM_SOUND_NAME)->IsPlay() )
		{
			cGameApp::m_spSoundParser->GetObject(FREE_GAME_BGM_SOUND_NAME)->Play(false);
			cGameApp::m_spSoundParser->GetObject(NORMAL_BGM_SOUND_NAME)->Play(true);
		}
		g_pSlotGamePhase->m_bFreeToPlay = false;
		if( !m_bAutoSpin )
		{
		}
		if(cWorldTripSlotApp::m_spPlayerData->m_i64Money == 0)
		{

		}
	}
	if( m_bAutoSpin )
	{
		if(ChecMoneyEncoughToBet(true))
			this->m_bSatisfiedCondition = true;
		else
			m_bAutoSpin = false;
	}
	if( m_pBetDataButtonManaget )
		m_pBetDataButtonManaget->Init();
	g_pSlotGamePhase->m_pBetBehavior->m_iWinMoney = 0;
	g_pSlotGamePhase->m_pSlotUI->m_pWinmoneyLines->Stop();
}

void	cBetPhase::SetupResult()
{
	if(g_pSlotGamePhase->m_pExtraBonusMoneyRate->size())
	{
		g_pSlotGamePhase->m_pBetBehavior->m_iBonusMoneyRate = (*g_pSlotGamePhase->m_pExtraBonusMoneyRate)[0];
		g_pSlotGamePhase->m_pExtraBonusMoneyRate->erase(g_pSlotGamePhase->m_pExtraBonusMoneyRate->begin());
	}
	g_pSlotGamePhase->m_pBetBehavior->CalculateScoreWithRound();
	cPuzzleImage*l_pPI = g_pSlotGamePhase->m_pSlotLineObjectManagerList->GetObject(0)->GetAllSpinObjectPI();
	int	l_iCount = g_pSlotGamePhase->m_pSlotLineObjectManagerList->Count();
	for( int i=0;i<l_iCount;++i )
	{
		cSlotObjectManager*l_pSlotObjectManager = g_pSlotGamePhase->m_pSlotLineObjectManagerList->GetObject(i);
		int	l_iCount = g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y;
		std::vector<std::wstring>l_strResult;
		for( int j=0;j<g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y;++j )
		{
			std::wstring	l_str;
			int	l_iIndex = i+g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x*j;
			int	l_iResult;
			l_iResult = g_pSlotGamePhase->m_pBetBehaviorResult->m_TableResultList[0]->SpinObjectResultList[l_iIndex];
			l_str = g_pSlotGamePhase->m_pSlotRuleData->m_SpinObjectDataManager.GetObject(l_iResult)->GetName();
			//while( !IsVaildSpinObjectForResult(l_str) )
			//{
			//	assert(0&&"while( !IsVaildSpinObjectForResult(l_str) )");
			//	l_iResult = rand()%g_pSlotGamePhase->m_pSlotLineObjectManagerList->GetObject(0)->GetAllSpinObjectPI()->Count();
			//	l_str = l_pPI->GetObject(l_iResult)->GetName();			
			//}
			l_strResult.push_back(l_str);
		}
		l_pSlotObjectManager->SetupResult(&l_strResult);
	}
	if( !g_pSlotGamePhase->m_bFreeToPlay )
		cWorldTripSlotApp::m_spPlayerData->m_i64Money -= g_pSlotGamePhase->m_pBetBehavior->m_iTotalBet;

	float	l_fJPShareScorePercentage = 0.1f;
	//assign bet money
	g_pSlotGamePhase->m_pBetBehaviorResult->AssignTableData(g_pSlotGamePhase->m_bFreeToPlay?0:g_pSlotGamePhase->m_pBetBehavior->m_iTotalBet);
}

void	cBetPhase::Update(float e_fElpaseTime)
{
	if( cWorldTripSlotApp::m_spPlayerData->m_i64Money != 0 )
	{
		if( m_pBetDataButtonManaget )
		{
			m_pBetDataButtonManaget->SingnalProcess();
			m_pBetDataButtonManaget->Update(e_fElpaseTime);
			if( m_pBetDataButtonManaget->GetCurrentWorkingObject() )
				m_pBetDataButtonManaget->Init();
		}
	}
	//if( m_iReadMeProgress != 0 )
	//	return;
	//g_pSlotGamePhase->m_pSlotLineObjectManagerList->Update(e_fElpaseTime);
	if( this->m_bSatisfiedCondition == true )
	{
		//if(!this->m_bAutoSpin)
		//	cGameApp::SoundPlay(SPIN_SOUND_NAME,true);
		SetupResult();
	}
	g_pSlotGamePhase->m_pSlotUI->UpdateSelectedLines(e_fElpaseTime);
	
}

void	cBetPhase::Render()
{
	m_pConstantUI->Render();
	if( m_pBetDataButtonManaget )
		m_pBetDataButtonManaget->Render();
	g_pSlotGamePhase->m_pBetBehavior->Render();
	g_pSlotGamePhase->m_pSlotLineObjectManagerList->Render();
	g_pSlotGamePhase->m_pSlotUI->RenderBetLines();
}

void	cBetPhase::DebugRender()
{
	//m_pBetDataButtonManaget->DebugRender();
}

void	cBetPhase::MouseDown(int e_iX,int e_iY)
{
	if( m_pBetDataButtonManaget )
		m_pBetDataButtonManaget->MouseDown(e_iX,e_iY);
}

void	cBetPhase::MouseUp(int e_iX,int e_iY)
{
	if( m_pBetDataButtonManaget )
	{
		m_pBetDataButtonManaget->MouseUp(e_iX,e_iY);
		g_pSlotGamePhase->m_pBetBehavior->BetMax();
		if(ChecMoneyEncoughToBet(true))
			m_bSatisfiedCondition = true;
	}
}

void	cBetPhase::MouseMove(int e_iX,int e_iY)
{
	if( m_pBetDataButtonManaget )
		m_pBetDataButtonManaget->MouseMove(e_iX,e_iY);
}

void	cBetPhase::KeyDown(char e_cKey)
{
	//m_pBetDataButtonManaget->SingnalProcess(e_cKey);
}

void	cBetPhase::KeyPress(char e_cKey)
{
	//m_pBetDataButtonManaget->SingnalProcess(e_cKey);
}

void	cBetPhase::KeyUp(char e_cKey)
{
	//m_pBetDataButtonManaget->SingnalProcess(e_cKey);
}

void	cBetPhase::RenderReadMeMenu()
{
	if( m_pMenu )
		m_pMenu->Render();
}

const WCHAR*	cBetPhase::GetNextPhaseName()
{
#ifdef _DEBUG
//	OutputDebugString(L"leave cBetPhase\n");
#endif
	return SPIN_PHASE_NAME;
}

cImageButton*g_pAutoSpinButton = 0;
cClickMouseBehavior*g_pExchangeRateButton = 0;
cBetDataButtonManaget::cBetDataButtonManaget(cBetPhase*e_pBetPhase)
{
	cMPDIList*l_pMPDIList = cGameApp::m_spAnimationParser->GetMPDIListByFileName(UI_MPDILIST_FILE_NAME);
	cMPDI*l_pButtonClickMPDI = l_pMPDIList->GetObject(L"but1");
	cMPDI*l_pButtonPressedMPDI = l_pMPDIList->GetObject(L"but2");
	unsigned char	l_ucKeyDataList[] = { 'G','A','L','J','I','K','S','R','E' };
	m_pBetPhase = e_pBetPhase;
	WCHAR*l_strButtonName[] = {	BET_MAX_BUTTON_NAME,
								AUTO_SPIN_BUTTON_NAME,
								LINE_ADD_BUTTON_NAME,
								LINE_MNUS_BUTTON_NAME,
								ADD_BET_BUTTON_NAME,
								MINES_BET_BUTTON_NAME,
								SPIN_BUTTON_NAME,
								MENU_BUTTON_NAME,
								EXCHANGE_BUTTON_NAME};
	for( int i=0;i<eBT_MAX;++i )
	{
		cSubMPDI*l_pSubMPDI = l_pButtonClickMPDI->GetObject(l_strButtonName[i]);
		cSubMPDI*l_pPressedSubMPDI = l_pButtonPressedMPDI->GetObject(l_strButtonName[i]);
		cImageButton*l_pButton = new cImageButton(l_pSubMPDI);
		l_pButton->SetPressedImage(l_pPressedSubMPDI);
		if( i == eBT_AutoSpinButton )
		{
			m_pAutoSpinYesImageButton = new cImageButton(l_pPressedSubMPDI);
			m_pAutoSpinYesImageButton->SetPressedImage(l_pSubMPDI);
			m_pAutoSpinYesImageButton->SetKeyData(l_ucKeyDataList[i]);
		}

		l_pButton->SetKeyData(l_ucKeyDataList[i]);
		if( i != eBT_SpinButton && i != eBT_MenuButton && i != eBT_ExchangeButton && i != eBT_BetMaxButton && i != eBT_AutoSpinButton )
		{
			l_pButton->SetPressedRepeatMouseUp(true);
		}
		this->AddObjectNeglectExist(l_pButton);
	}
	//g_pAutoSpinButton = this->GetObject(eBT_AutoSpinButton);
	g_pAutoSpinButton = m_pAutoSpinYesImageButton;
	g_pExchangeRateButton = this->GetObject(eBT_ExchangeButton);
}

cBetDataButtonManaget::~cBetDataButtonManaget()
{
	SAFE_DELETE(m_pAutoSpinYesImageButton);
}

void	cBetDataButtonManaget::ProcessButtEvent(eButtonList e_eButtonList)
{
	if( m_pBetPhase->m_iReadMeProgress  != 0  && e_eButtonList != eBT_MenuButton )
	{
		return;
	}
	switch(e_eButtonList)
	{
		case eBT_MenuButton:
			{
				cMPDIList*l_pMPDIList = cGameApp::m_spAnimationParser->GetMPDIListByFileName(UI_MPDILIST_FILE_NAME);
				m_pBetPhase->m_iReadMeProgress++;
				if( m_pBetPhase->m_iReadMeProgress>3 )
					m_pBetPhase->m_iReadMeProgress = 0;
				if( m_pBetPhase->m_iReadMeProgress == 1 )
					m_pBetPhase->m_pMenu = l_pMPDIList->GetObject(L"Menu1");
				else
				if( m_pBetPhase->m_iReadMeProgress == 2 )
					m_pBetPhase->m_pMenu = l_pMPDIList->GetObject(L"Menu2");
				else
				if( m_pBetPhase->m_iReadMeProgress == 3 )
					m_pBetPhase->m_pMenu = l_pMPDIList->GetObject(L"Menu3");
				else
					m_pBetPhase->m_pMenu = 0;
				if( m_pBetPhase->m_pMenu )
					m_pBetPhase->m_pMenu->Init();
			}
			break;
	
		case eBT_BetMaxButton:
			g_pSlotGamePhase->m_pBetBehavior->BetMax();
			break;
		case eBT_AutoSpinButton:
			if(m_pBetPhase->ChecMoneyEncoughToBet(true))
			{
				m_pBetPhase->m_bAutoSpin = true;
				m_pBetPhase->m_bSatisfiedCondition = true;
				//cGameApp::SoundPlay(AUTO_SPIN_SOUND_NAME,true);
			}
			break;
		case eBT_AddLineBetButton:
			g_pSlotGamePhase->m_pBetBehavior->AddLineBet();
			break;
		case eBT_MinusLineBetButton:
			g_pSlotGamePhase->m_pBetBehavior->MinusLineBet();
			break;
		case eBT_AddBetMoneyButton:
			g_pSlotGamePhase->m_pBetBehavior->AddBetMoney();
			break;
		case eBT_MinusBetMoneyButton:
			g_pSlotGamePhase->m_pBetBehavior->MinusBetMoney();
			break;
		case eBT_SpinButton:
			if(m_pBetPhase->ChecMoneyEncoughToBet(true))
				this->m_pBetPhase->m_bSatisfiedCondition = true;
		break;
		case eBT_ExchangeButton:
		{
			int	l_iOriginalRate = m_pBetPhase->m_ExchangeRateVector[m_pBetPhase->m_iCurrentFocusExchangeRate];
			cMPDIList*l_pMPDIList = cGameApp::m_spAnimationParser->GetMPDIListByFileName(UI_MPDILIST_FILE_NAME);
			cMPDI*l_pButtonPressedMPDI = l_pMPDIList->GetObject(L"but2");
			cSubMPDI*l_pPressedSubMPDI = l_pButtonPressedMPDI->GetObject(EXCHANGE_BUTTON_NAME);
			
			++this->m_pBetPhase->m_iCurrentFocusExchangeRate;
			if( this->m_pBetPhase->m_iCurrentFocusExchangeRate >= (int)this->m_pBetPhase->m_ExchangeRateVector.size() )
			{
				this->m_pBetPhase->m_iCurrentFocusExchangeRate = 0;
			}
			int	l_iNewRate = m_pBetPhase->m_ExchangeRateVector[m_pBetPhase->m_iCurrentFocusExchangeRate];
			sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = l_pPressedSubMPDI->GetPointData(this->m_pBetPhase->m_iCurrentFocusExchangeRate);
			float	*l_pfUV = l_pTexBehaviorDataWithImageIndexData->pPI->GetObject(l_pTexBehaviorDataWithImageIndexData->iImageIndex)->GetUV();
			cImageButton*l_pImageButton = (cImageButton*)this->GetObject(eBT_ExchangeButton);
			l_pImageButton->SetUV(l_pfUV);
			l_pPressedSubMPDI = dynamic_cast<cSubMPDI*>(l_pImageButton->GetPressedImage());
			l_pPressedSubMPDI->Init();
			l_pPressedSubMPDI->Update(l_pPressedSubMPDI->GetEndTime()/l_pPressedSubMPDI->GetPointDataList()->size()*this->m_pBetPhase->m_iCurrentFocusExchangeRate);
			
			g_pSlotGamePhase->m_pBetBehavior->ExchangeRateChange(l_iOriginalRate,l_iNewRate);
		}
			break;
	}
}

void	cBetDataButtonManaget::Init()
{
	g_pAutoSpinButton->Init();
	cClickMouseBehaviorList<cClickMouseBehavior>::Init();
}

void    cBetDataButtonManaget::Update(float e_fElpaseTime)
{
    int l_iCount = this->Count();
    for( int i=0;i<l_iCount;++i )
    {
        this->m_ObjectList[i]->Update(e_fElpaseTime);
        if( this->m_ObjectList[i]->IsSatisfiedCondition() )
        {
			ProcessButtEvent((eButtonList)i);
			cImageButton*l_pImageButton = (cImageButton*)m_ObjectList[i];
			l_pImageButton->Init();
			if( l_pImageButton->IsPressedRepeatMouseUp() )
				l_pImageButton->InitRepeat();
        }
    }
}