#include "stdafx.h"
#include "FreeGameForPickGemsGame.h"
#include "SlotGamePhase.h"
#include "SlotObject.h"
#include "SlotRule.h"
WCHAR*	FREE_GAME_NUMERIAL_MPDILIST_FILE_NAME	= L"WorldTripSlot/ImageData/ImageData_CH/Freegametimes.mpdi";
//FREE_GAME_NUMERIAL_MPDILIST_FILE_NAME			= L"WorldTripSlot/ImageData/ImageData_CH/Freegametimes.mpdi";
//FREE_GAME_NUMERIAL_MPDILIST_FILE_NAME			= L"WorldTripSlot/ImageData/ImageData_EN/Freegametimes.mpdi";
sFreeGameData::sFreeGameData()
{
	pPrizeMPDI = 0;
	pPrizeAwardMPDI = 0;
	cMPDIList*	l_pFreegametimesMPDIList = cGameApp::GetMPDIListByFileName(FREE_GAME_NUMERIAL_MPDILIST_FILE_NAME);
	pPrizeAward_NumerialMPDI = l_pFreegametimesMPDIList->GetObject(L"FreeTimeNum");
	//m_pPrizeAwardMPDI = l_pFreegametimesMPDIList->GetObject(L"freegame");
	pPrizeAwardMPDI = l_pFreegametimesMPDIList->GetObject(L"FreeTimeDiamond");
	iTimeForPickupGems = 0;
}
sFreeGameData::~sFreeGameData()
{
	SAFE_DELETE(pPrizeMPDI);
}


void	sFreeGameData::SetupRestFreeSpinTimes(int e_iPrize)
{
	//SAFE_DELETE(pPrizeMPDI);
	//pPrizeMPDI = new cMPDI(pPrizeAwardMPDI);
	//cMPDIList*	l_pFreegametimesMPDIList = cGameApp::GetMPDIListByFileName(FREE_GAME_NUMERIAL_MPDILIST_FILE_NAME);
	//int	l_iNum = 1;
	//if( e_iPrize>99 )
	//{
	//	pPrizeAward_NumerialMPDI = l_pFreegametimesMPDIList->GetObject(L"NunIn3");
	//	l_iNum = 3;
	//}
	//else
	//if( e_iPrize>9 )
	//{
	//	pPrizeAward_NumerialMPDI = l_pFreegametimesMPDIList->GetObject(L"NunIn2");
	//	l_iNum = 2;
	//}
	//else
	//{
	//	pPrizeAward_NumerialMPDI = l_pFreegametimesMPDIList->GetObject(L"NunIn1");
	//	l_iNum = 1;
	//}
	//int	l_iValue[3];
	//l_iValue[0] = e_iPrize/100;
	//l_iValue[1] = (e_iPrize-l_iValue[0]*100)/10;
	//l_iValue[2] = (e_iPrize-l_iValue[0]*100-l_iValue[1]*10);
	//int*l_pStartValue = &l_iValue[0];
	//if( l_iNum == 3 )
	//	l_pStartValue = &l_iValue[0];
	//else
	//if( l_iNum == 2 )
	//	l_pStartValue = &l_iValue[1];
	//if( l_iNum == 1 )
	//	l_pStartValue = &l_iValue[2];

	//for( int i=0;i<l_iNum;++i )
	//{
	//	std::wstring	l_strNumName = NUMERIC_IMAGE_NAME;
	//	cSubMPDI*l_pSubMPDI = pPrizeAward_NumerialMPDI->GetObject(i);
	//	{
	//		l_strNumName += ValueToStringW(*l_pStartValue);
	//		int	l_iImageIndex = l_pSubMPDI->GetPuzzleImage()->GetObjectIndexByName(l_strNumName.c_str());
	//		l_pSubMPDI->ChangeAllImageByImageIndex(l_iImageIndex);
	//		cSubMPDI*l_pSubMPDIClone = new cSubMPDI(l_pSubMPDI);
	//		pPrizeMPDI->AddObjectNeglectExist(l_pSubMPDIClone);
	//		++l_pStartValue;
	//	}
	//}
	//pPrizeMPDI->Start();
}

void	sFreeGameData::Update(float e_fElpaseTime)
{
	if( pPrizeMPDI )
	{
		pPrizeMPDI->Update(e_fElpaseTime);
		if(pPrizeMPDI->IsAnimationDone())
			SAFE_DELETE(pPrizeMPDI);
	}		
}
void	sFreeGameData::Render()
{
	if( pPrizeMPDI )
	{
		if( iTimeForPickupGems >0 )
			pPrizeMPDI->Render();
	}
}

void	sFreeGameData::RestIntoFreeGameTimes()
{
	SAFE_DELETE(pPrizeMPDI);
	if( iTimeForPickupGems == 0 )
		return;
	pPrizeMPDI = new cMPDI(pPrizeAwardMPDI);
	int	l_iIndex = iTimeForPickupGems;
	if( l_iIndex>9 )
		l_iIndex = 9;
	cSubMPDI*l_pSubMPDI = new cSubMPDI(pPrizeAward_NumerialMPDI->GetObject(l_iIndex));
	pPrizeMPDI->AddObjectNeglectExist(l_pSubMPDI);
	pPrizeMPDI->SetAnimationLoop(true);
	pPrizeMPDI->Init();
}


cPickupObjectForFreeGameAndExtraBonusRate::cPickupObjectForFreeGameAndExtraBonusRate(cSubMPDI*e_pClickButton,cSubMPDI*e_pPickupGemMPDI,int e_iMoney,sGemsBonusData e_sGemsBonusData)
:cImageButton(e_pClickButton)
{
	m_GemsBonusData = e_sGemsBonusData;
	m_iMoney = e_iMoney;
	m_pPickUpGemMPDI = e_pPickupGemMPDI;
	m_pPickUpGemMPDI->Init();
	//m_pPickUpGemMPDI->SetPos(e_vPos);
	//m_pPickUpGemMPDI->SetPos(e_vPos);
	m_pPickUpGemMPDI->SetAnimationLoop(false);
}

cPickupObjectForFreeGameAndExtraBonusRate::~cPickupObjectForFreeGameAndExtraBonusRate()
{
	//SAFE_DELETE(m_pPickUpGemMPDI);
}

bool	cPickupObjectForFreeGameAndExtraBonusRate::InternalCollide(int e_iPosX,int e_iPosY)
{
	if(this->m_TC.bTragetTimrReached)
		return false;
	return cImageButton::InternalCollide(e_iPosX,e_iPosY);
}

bool    cPickupObjectForFreeGameAndExtraBonusRate::IsSatisfiedCondition()
{
	return m_pPickUpGemMPDI->IsAnimationDone();
}

void	cPickupObjectForFreeGameAndExtraBonusRate::Update(float e_fElpaseTime)
{
	if(!cImageButton::IsSatisfiedCondition())
	{
		cImageButton::Update(e_fElpaseTime);
		if(cImageButton::IsSatisfiedCondition())
		{
			DoPickupGem();
		}
	}
	else
	{
		m_pPickUpGemMPDI->Update(e_fElpaseTime);
	}
}

void	cPickupObjectForFreeGameAndExtraBonusRate::DoPickupGem()
{
	g_pSlotGamePhase->m_iFreeGame += m_GemsBonusData.iFreeGameTime;
	g_pSlotGamePhase->m_pRestFreeGameTimes->push_back(m_GemsBonusData.iFreeGameTime);
	cDoAllMouseBehaviorList<cPickupObjectForFreeGameAndExtraBonusRate>*l_pImageButtonList = dynamic_cast<cDoAllMouseBehaviorList<cPickupObjectForFreeGameAndExtraBonusRate>*>(this->GetOwner());
	int	l_iIndex = l_pImageButtonList->GetObjectIndexByPointer(this);
	WRITELOGFILE("!!Into FreeGame!!");
	for( int i=0;i<m_GemsBonusData.iFreeGameTime;++i )
	{
		if( m_GemsBonusData.BonusRateList.size() )
		{
			int	l_iTarget = rand()%m_GemsBonusData.BonusRateList.size();
			g_pSlotGamePhase->m_pExtraBonusMoneyRate->push_back( (int)m_GemsBonusData.BonusRateList[l_iTarget] );
		}
	}
	cGameApp::m_spSoundParser->GetObject(FREE_GAME_WIN_MONEY_SOUND_NAME)->Play(true);
}

void	cPickupObjectForFreeGameAndExtraBonusRate::Render()
{
	cImageButton::Render();
	if(cImageButton::IsSatisfiedCondition())
	{
		m_pPickUpGemMPDI->Render();
	}
	//this->DebugRender();
}

bool	cPickupObjectForFreeGameAndExtraBonusRate::IsPrizeGrabed()
{
	return cImageButton::IsSatisfiedCondition();
}

const wchar_t*         cFreeGameForPickGemsGame::TypeID( L"cFreeGameForPickGemsGame" );

cFreeGameForPickGemsGame::cFreeGameForPickGemsGame(const WCHAR*e_strConditionName,int e_iConditionCount,TiXmlElement*e_pGemsBonusData,cMPDI*e_pConstantUI)
:cBaseExtraGame(e_strConditionName,e_iConditionCount)
{
	m_FreeGameData.RestIntoFreeGameTimes();
	g_pSlotGamePhase->m_pSlotRuleData->m_iNumFreeGameToAchevied = e_iConditionCount;
	m_pFreeGameSpinObjectShow = 0;
	m_pUpperScreenUI = 0;
	m_pConstantUI = e_pConstantUI;
	m_pConstantUI->Init();
	m_pConstantUI->Update(0.0001f);
	TiXmlElement*l_pTiXmlElement = e_pGemsBonusData->FirstChildElement();
	while( l_pTiXmlElement )
	{
		sGemsBonusData	l_sGemsBonusData;
		l_sGemsBonusData.iFreeGameTime = _wtoi(l_pTiXmlElement->Attribute(L"FreeTimes"));
		const WCHAR*l_strBonusRate = l_pTiXmlElement->Attribute(L"BonusRate");
		l_sGemsBonusData.BonusRateList = GetValueListByCommaDivide<float>(l_strBonusRate);
		m_GemsBonusDataList.push_back(l_sGemsBonusData);
		l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
	}
	m_TimeForNoActionToLeave.SetTargetTime(50.f);
	m_eFreeGameList = eFGL_PICK_ROCK_BONUS;
	m_strImageName = L"Free";
	m_bPickRockToWinMoneyDone = false;
	m_iRockShowRowIndex = 1;
	m_pEnteringMPDI = 0;
	m_pLeavingMPDI = 0;
	m_pMPDIList = 0;
	m_pSelectedObject = 0;
}

cFreeGameForPickGemsGame::~cFreeGameForPickGemsGame()
{

}

void	cFreeGameForPickGemsGame::MouseMove(int e_iPosX,int e_iPosY)
{
	if( m_pEnteringMPDI->IsAnimationDone() )
	{
		if(!IsObjectPickUpDone() && !m_ImageButtonList.GetCurrentMouseDownCollidedObject() )
			m_ImageButtonList.MouseMove(e_iPosX,e_iPosY);
	}
}

void	cFreeGameForPickGemsGame::MouseDown(int e_iPosX,int e_iPosY)
{
	if( m_pEnteringMPDI->IsAnimationDone() )
	{
		if(!IsObjectPickUpDone() && !m_ImageButtonList.GetCurrentMouseDownCollidedObject() )
			m_ImageButtonList.MouseDown(e_iPosX,e_iPosY);
	}
}

void    cFreeGameForPickGemsGame::MouseUp(int e_iPosX,int e_iPosY)
{
	if( m_pEnteringMPDI->IsAnimationDone() )
	{
		if(!IsObjectPickUpDone() && !m_ImageButtonList.GetCurrentMouseDownCollidedObject() )
			m_ImageButtonList.MouseUp(e_iPosX,e_iPosY);
	}
}
//return true if the condition of bonus is satisfied.
//bool	cFreeGameForPickGemsGame::InternalConditionCheck()
//{
//	if( cBaseExtraGame::IsConditionOk() )
//		return true;
//	return false;
//}


void	cFreeGameForPickGemsGame::SetupResult()
{
	WCHAR*l_strClickMPDI = L"diamain";
	WCHAR*l_strPickupMPDI = L"diamain2";
	int	l_iMoneyToGive[] = {500,0,1000,0,0};
	cSlotObjectManager*l_pSlotObjectManager = g_pSlotGamePhase->m_pSlotLineObjectManagerList->GetObject(0);
	cPuzzleImage*l_pPI = l_pSlotObjectManager->GetAllSpinObjectPI();
	cPuzzleImageUnit*l_pImage = l_pPI->GetObject(m_strImageName.c_str());		
	vector<int>	l_uiAllMagicIDVector1 = UT::GenerateRandomTable(5,5);
	//for( int i=0;i<g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x;++i )
	char	l_KeyData[] = {'C','V','B','N','M'};
	for( int i=0;i<g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x;++i )
	{
		cSlotObjectManager*l_pSlotObjectManager = g_pSlotGamePhase->m_pSlotLineObjectManagerList->GetObject(i);
		Vector3	l_vPos = l_pSlotObjectManager->IndexToResultShowPos(m_iRockShowRowIndex);
		l_vPos.y -= 683.f;
		int	l_iMoneyToWin = l_iMoneyToGive[l_uiAllMagicIDVector1[i]];
		cMPDI*l_pPickupMPDI = m_pMPDIList->GetObject(l_strPickupMPDI);
		cMPDI*l_pClickMPDI = m_pMPDIList->GetObject(l_strClickMPDI);

		cPickupObjectForFreeGameAndExtraBonusRate*l_pPickupObjectToWinMoney = new cPickupObjectForFreeGameAndExtraBonusRate(l_pClickMPDI->GetObject(i),l_pPickupMPDI->GetObject(i),l_iMoneyToWin,this->m_GemsBonusDataList[i]);
		l_pPickupObjectToWinMoney->SetKeyData(l_KeyData[i]);
		m_ImageButtonList.AddObjectNeglectExist(l_pPickupObjectToWinMoney);
	}
	m_ImageButtonList.Init();
}

bool	cFreeGameForPickGemsGame::InternalConditionCheck()
{
	int	l_iTotalCount = g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x*g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y;
	int	l_iSame = 0;
	for( int i = 0;i<l_iTotalCount;++i)
	{
		std::wstring	l_strTargetName = GetSpinObjectNameByIndex(i);
		if(!wcscmp(this->m_strConditionName.c_str(),l_strTargetName.c_str()))
		{
			++l_iSame;
			if(this->m_iConditionCount<=l_iSame)
			{
				return true;
			}
		}
	}

	//int	l_iCount = g_pSlotGamePhase->m_pSlotLineObjectManagerList->Count();
	//int	l_iSize = (int)g_pSlotGamePhase->m_pSlotRuleData->m_LineRuleList.size();
	//for( int i = 0;i<l_iSize;++i)
	//{
	//	sLineRule*l_pLineRule = g_pSlotGamePhase->m_pSlotRuleData->m_LineRuleList[i];
	//	char	l_cCount = l_pLineRule->cOneLineCount;
	//	int	l_iSame = 0;
	//	for( char j = 0;j<l_cCount;++j)
	//	{
	//		std::wstring	l_strTargetName = GetSpinObjectNameByIndex(l_pLineRule->pcLineObjectIndex[j]);
	//		if(!wcscmp(this->m_strConditionName.c_str(),l_strTargetName.c_str()))
	//		{
	//			++l_iSame;
	//		}
	//		else
	//			break;
	//	}
	//	if(this->m_iConditionCount<=l_iSame)
	//	{
	//		cGameApp::m_sfDebugValue = 1.f;
	//		return true;
	//	}
	//}
	return false;
}

void	cFreeGameForPickGemsGame::LoadResource()
{
	cGameApp::m_spSoundParser->GetObject(FREE_GAME_CONDITION_OK)->Play(true);
	if( !m_pMPDIList )
	{
		m_pMPDIList = cGameApp::GetMPDIListByFileName(FREE_GAME_BONUS_MPDILIST_FILE_NAME);
		m_pEnteringMPDI = m_pMPDIList->GetObject(L"INTO");
		m_pLeavingMPDI = m_pMPDIList->GetObject(L"LEAVE");
		m_pAwardListMPDI = m_pMPDIList->GetObject(L"plate");
		//m_pUpperScreenUI = m_pMPDIList->GetObject(L"UIsb");
		m_pEnteringMPDI->SetAnimationLoop(false);
		m_pLeavingMPDI->SetAnimationLoop(false);
		m_pAwardListMPDI->SetAnimationLoop(false);
		m_pAwardListMPDI->Init();
	}
	if( !m_pFreeGameSpinObjectShow )
	{
		cMPDIList*l_pMPDIList = cGameApp::GetMPDIListByFileName(WIN_MONEY_SPIN_OBJECT_MPDI_MPDILIST_FILE_NAME);
		m_pFreeGameSpinObjectShow = l_pMPDIList->GetObject(L"Free");
	}
	m_pFreeGameSpinObjectShow->Init();
	if( m_pUpperScreenUI )
		m_pUpperScreenUI->Init();
	m_bPickRockToWinMoneyDone = false;
	m_pEnteringMPDI->Init();
	m_pLeavingMPDI->Init();
	SetupResult();
	m_TimeForNoActionToLeave.Start();
	cGameApp::m_spSoundParser->GetObject(NORMAL_BGM_SOUND_NAME)->Play(false);
	cGameApp::m_spSoundParser->GetObject(FREE_GAME_BGM_SOUND_NAME)->Play(true);

	m_pSelectedObject = 0;
}

void	cFreeGameForPickGemsGame::Destroy()
{
	if( m_pMPDIList )
	{
		//cGameApp::m_spAnimationParser->RemoveObject(m_pMPDIList->GetName());
		//m_pMPDIList = 0;
	}
	m_ImageButtonList.Destroy();
	cGameApp::m_spSoundParser->GetObject(FREE_GAME_WIN_MONEY_SOUND_NAME)->Play(false);
	m_ImageButtonList.SetCurrentMouseDownCollidedObject(0);
#ifdef _DEBUG
//	OutputDebugString(L"leave cFreeGameForPickGemsGame\n");
#endif
}

bool	cFreeGameForPickGemsGame::IsObjectPickUpDone()
{
	int	l_iCount = m_ImageButtonList.Count();
	for( int i=0;i<l_iCount;++i )
	{
		if( m_ImageButtonList[i]->IsPrizeGrabed() )
		{
			return true;
		}
	}
	return false;
}

void	cFreeGameForPickGemsGame::DoSelected()
{
	m_bPickRockToWinMoneyDone = true;
	m_TimeForNoActionToLeave.Start();
}

void	cFreeGameForPickGemsGame::Update(float e_fElpaseTime)
{
	m_FreeGameData.Update(e_fElpaseTime);
	if( m_pUpperScreenUI )
		m_pUpperScreenUI->Update(e_fElpaseTime);

	if( !m_pFreeGameSpinObjectShow->IsAnimationDone() )
	{
		m_pFreeGameSpinObjectShow->Update(e_fElpaseTime);
		return;
	}
	if( !m_TimeForNoActionToLeave.bTragetTimrReached )
	{
		m_TimeForNoActionToLeave.Update(e_fElpaseTime);
		if(m_TimeForNoActionToLeave.bTragetTimrReached )
		{
			DoSelected();
			cPickupObjectForFreeGameAndExtraBonusRate*l_pPickupObjectToWinMoney = (cPickupObjectForFreeGameAndExtraBonusRate*)m_ImageButtonList[0];
			l_pPickupObjectToWinMoney->DoPickupGem();
		}
	}
	if( m_bPickRockToWinMoneyDone )
	{
		if( !m_pLeavingMPDI->IsAnimationDone() )
		{
			m_pLeavingMPDI->Update(e_fElpaseTime);
			if( m_pLeavingMPDI->IsAnimationDone() )
			{
				this->m_bDone = true;
			}
		}
	}
	else
	{
		if( m_pEnteringMPDI->IsAnimationDone() )
		{
			if( !m_bPickRockToWinMoneyDone )
			{
				if( !m_pSelectedObject )
				{
					m_ImageButtonList.Update(e_fElpaseTime);
					if(!IsObjectPickUpDone())
						m_ImageButtonList.SingnalProcess();
					int	l_iCount = m_ImageButtonList.Count();
					if( !m_pSelectedObject )
					{
						for( int i=0;i<l_iCount;++i )
						{
							if(m_ImageButtonList[i]->GetMouseBehavior() != eOMB_NONE  && !m_pSelectedObject)
							{
								m_pSelectedObject = m_ImageButtonList[i];
								break;
							}
						}
					}
				}
				else
				{
					if( !m_pSelectedObject->IsSatisfiedCondition() )
					{
						m_pSelectedObject->Update(e_fElpaseTime);
						m_pSelectedObject->SingnalProcess();
						m_pSelectedObject->ButtonUp();
					}
					else
					{
						DoSelected();
					}
				}
			}
		}
		else
		{
			m_pEnteringMPDI->Update(e_fElpaseTime);
			if( m_pEnteringMPDI->IsAnimationDone() )
			{

			}
		}
	}
}

void	cFreeGameForPickGemsGame::Render()
{
	m_FreeGameData.Render();
	if( m_pFreeGameSpinObjectShow->IsAnimationDone() )
	{
		if( m_bPickRockToWinMoneyDone )
		{
			if( m_pLeavingMPDI->IsAnimationDone() )
			{
				
			}
			else
			{
			}
			m_pLeavingMPDI->Render();
			if(m_pUpperScreenUI)
				m_pUpperScreenUI->Render();

		}
		else
		{
			m_pEnteringMPDI->Render();
			if(m_pUpperScreenUI)
				m_pUpperScreenUI->Render();

			if( m_pEnteringMPDI->IsAnimationDone() )
			{
				m_ImageButtonList.Render();
				//m_pAwardListMPDI->Render();
			}
			else
			{
				//if( m_pEnteringMPDI->GetCurrentProgress() >0.5f )
				{
					//m_ImageButtonList.Render();
				}
			}
		}
		m_pConstantUI->Render();
		if( m_pEnteringMPDI->IsAnimationDone() )
		{
			m_pAwardListMPDI->Render();
		}
		g_pSlotGamePhase->m_pSlotLineObjectManagerList->Render();
	}
	else
	{
		m_pEnteringMPDI->Render();
		m_pConstantUI->Render();
		g_pSlotGamePhase->m_pSlotLineObjectManagerList->Render();	
		cPuzzleImage*l_pPI = cGameApp::m_spAnimationParser->GetAllBaseImageList()->GetPuzzleImage(SPIN_OBJECT_PI_NAME);
		cPuzzleImageUnit*l_pPIUnit = l_pPI->GetObject(0);
		float	l_fImageHalfWidth = *l_pPIUnit->GetWidth()/2.f;
		float	l_fImageHalfHeight = *l_pPIUnit->GetHeight()/2.f;
		int	l_iTotalCount = g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x*g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y;
		for( int i = 0;i<l_iTotalCount;++i)
		{
			std::wstring	l_strTargetName = GetSpinObjectNameByIndex(i);
			if(!wcscmp(this->m_strConditionName.c_str(),l_strTargetName.c_str()))
			{
				sLoopMovingObject*l_pLoopMovingObject = GetLoopMovingObjectByIndex(i);
				Vector3	l_vPos = l_pLoopMovingObject->vCurrentPos;
				l_vPos.x += l_fImageHalfWidth;
				l_vPos.y += l_fImageHalfHeight;
				m_pFreeGameSpinObjectShow->SetPos(l_vPos);
				m_pFreeGameSpinObjectShow->Render();
			}
		}
	}
}