#include "stdafx.h"
#include "SlotObject.h"
#include "WinMoneyLine.h"
#include "FreeMiningGame.h"
#include "FreeExplosionGame.h"
#include "SlotGamePhase.h"
#include "WorldTripSlotApp.h"


int	g_iExplosionCount = 0;

const wchar_t*         cFreeExplosionGame::TypeID( L"cFreeExplosionGame" );

cFreeExplosionGame::cFreeExplosionGame(const WCHAR*e_strConditionName,int e_iConditionCount,float e_fSpeed,cMPDI*e_pConstantUI,int e_TenComboExtraMoneyMin,int e_TenComboExtraMoneyMax,float e_fGameSpeed)
:cBaseExtraGame(e_strConditionName,e_iConditionCount)
{
	m_fGameSpeed = e_fGameSpeed;
	m_iCurrentWinMoney = 0;
	m_iAddWinMoney = 0;
	m_pConstantUI = e_pConstantUI;
	m_pNumerialImage = 0;
	m_eFreeGameList = eFGL_EXPLOSION;
	m_pResourceMPDIList = 0;
	int	l_iTotalSpinObject = g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x*g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y;
	cPuzzleImage*l_pPI = g_pSlotGamePhase->m_pSlotLineObjectManagerList->GetObject(0)->GetAllSpinObjectPI();
	float	l_fImageHeight = (float)*l_pPI->GetObject(0)->GetWidth();
	for( int i=0;i<g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x;++i )
	{
		Vector3	l_vEndPos = g_pSlotGamePhase->m_pSlotLineObjectManagerList->GetObject(i)->GetLoopMovingObject(g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y-1)->vCurrentPos;
		cFallDownNewSpinObject*l_pFallDownNewSpinObject = new cFallDownNewSpinObject(g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y,l_vEndPos,e_fSpeed,l_fImageHeight);
		m_FallDownNewSpinObject.AddObjectNeglectExist(l_pFallDownNewSpinObject);
	}
	m_pWinmoneyLines = new cWinmoneyLines(BET_LINES_MPDI_FILE_NAME);
	m_pExploreMPDI = 0;
	m_pWinMoneySpinObjectMPDI = 0;
	//m_ExplosionCountPosition = 0;
	m_iExplosionCount = 1;
	m_iMaxAllowExplosionCount = MAXIMUM_EXPLOSION_COUNT;
	m_pJPEffect = 0;
	m_iExtraBonusForReachedMaxAllowExplosionCountMin = e_TenComboExtraMoneyMin;
	m_iExtraBonusForReachedMaxAllowExplosionCountMax = e_TenComboExtraMoneyMax;
}

cFreeExplosionGame::~cFreeExplosionGame()
{
	SAFE_DELETE(m_pJPEffect);
	SAFE_DELETE(m_pNumerialImage);
	SAFE_DELETE(m_pWinMoneySpinObjectMPDI);
	SAFE_DELETE(m_pWinmoneyLines);
}

void	cFreeExplosionGame::CheckResultAndSetNewSpinObject(std::vector<std::wstring> e_NewResult)
{
	if( e_NewResult.size() == 0 )
		return;
	//fin out last index where explosion,and count
	int				l_iTargetIndex = 0;
	int				l_iNumSpinObject = g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y;
	cPuzzleImage*	l_pPI = g_pSlotGamePhase->m_pSlotLineObjectManagerList->GetObject(0)->GetAllSpinObjectPI();
	float	l_fImageHeight = (float)*l_pPI->GetObject(0)->GetHeight();
	sSlotBetTableResult*l_pTableResult = g_pSlotGamePhase->m_pBetBehaviorResult->m_TableResultList[this->m_iExplosionCount-1];
	for( int i=0;i<g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x;++i )
	{
		//first assume all objects are explosion
		cSlotObjectManager*l_pSlotObjectManager = g_pSlotGamePhase->m_pSlotLineObjectManagerList->GetObject(i);
		cFallDownNewSpinObject*l_pFallDownNewSpinObject = m_FallDownNewSpinObject[i];
		Vector3	l_vExplosionStartPos = l_pSlotObjectManager->GetLoopMovingObject(0)->vCurrentPos;
		l_vExplosionStartPos.y -= l_fImageHeight;
		int	l_iNonExplosionIndex = g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y-1;
		int	l_iImageIndex = -1;
		for( int j=l_iNumSpinObject-1;j>-1;--j )
		{
			l_iTargetIndex = j*g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x+i;
			//start index from unexplosion 
			if(!l_pTableResult->pbWinMoneySpinObject[l_iTargetIndex])
			{
				Vector3	l_vStartPos = l_pSlotObjectManager->GetLoopMovingObject(j)->vCurrentPos;
				int	l_iFinalResultIndex = (l_iNonExplosionIndex*g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x)+i;
				l_iImageIndex = l_pPI->GetObjectIndexByName(e_NewResult[l_iFinalResultIndex].c_str());
				l_pSlotObjectManager->GetLoopMovingObject(l_iNonExplosionIndex)->iImageIndex = l_iImageIndex;
				l_pFallDownNewSpinObject->SetupData(l_iNonExplosionIndex,l_vStartPos,l_pPI->GetObject(l_iImageIndex));
				--l_iNonExplosionIndex;
			}
		}
		for( int j=l_iNumSpinObject-1;j>-1;--j )
		{
			l_iTargetIndex = j*g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x+i;
			if(l_pTableResult->pbWinMoneySpinObject[l_iTargetIndex])
			{
				int	l_iFinalResultIndex = (l_iNonExplosionIndex*g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x)+i;
				l_iImageIndex = l_pPI->GetObjectIndexByName(e_NewResult[l_iFinalResultIndex].c_str());
				l_pSlotObjectManager->GetLoopMovingObject(l_iNonExplosionIndex)->iImageIndex = l_iImageIndex;
				l_pFallDownNewSpinObject->SetupData(l_iNonExplosionIndex,l_vExplosionStartPos,l_pPI->GetObject(l_iImageIndex));
				l_vExplosionStartPos.y -= l_fImageHeight;
				--l_iNonExplosionIndex;
			}
		}
	}
	for( int i=0;i<g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x;++i )
	{
		cFallDownNewSpinObject*l_pFallDownNewSpinObject = m_FallDownNewSpinObject[i];
		int a=0;
	}
}

std::vector<std::wstring>	cFreeExplosionGame::GetNextExplosionResult()
{
	std::vector<std::wstring>	l_NewResult;
	int	l_iTableSize = (int)g_pSlotGamePhase->m_pBetBehaviorResult->m_TableResultList.size();
	if(m_iExplosionCount >= l_iTableSize)
	//if(g_pSlotGamePhase->m_bFreeToPlay||m_iExplosionCount >= l_iTableSize)
	{
		return l_NewResult;
	}
	int	l_iTotalSpinObject = g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x*g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y;
	//should call probability function
	std::vector<std::wstring>	l_Result;
	std::wstring	l_strName;
	int	l_iResult;
	cPuzzleImage*l_pPI = g_pSlotGamePhase->m_pSlotLineObjectManagerList->GetObject(0)->GetAllSpinObjectPI();
	//OutputDebugString(L"ResultStart\n");
	for( int i=0;i<l_iTotalSpinObject;++i )
	{
		l_iResult = g_pSlotGamePhase->m_pBetBehaviorResult->m_TableResultList[m_iExplosionCount]->SpinObjectResultList[i];
		l_strName = g_pSlotGamePhase->m_pSlotRuleData->m_SpinObjectDataManager.GetObject(l_iResult)->GetName();
		l_Result.push_back(l_strName);
	}
	//OutputDebugString(L"\nEnd\n");
	return l_Result;
}

bool	cFreeExplosionGame::InternalConditionCheck()
{
	return g_pSlotGamePhase->m_pBetBehaviorResult->GetCurrentTablesWinMoney()>0;
}


void	cFreeExplosionGame::LoadResource()
{
	if( !m_pResourceMPDIList )
	{
		m_pResourceMPDIList = cGameApp::GetMPDIListByFileName(WIN_MONEY_SPIN_OBJECT_MPDI_MPDILIST_FILE_NAME);
		m_pExploreMPDI = m_pResourceMPDIList->GetObject(EXPLORE_MPDI_NAME);
		cPuzzleImage*l_pPI = cGameApp::m_spAnimationParser->GetAllBaseImageList()->GetPuzzleImage(NUM_PI_NAME);
		m_pNumerialImage = l_pPI->GetNumerialImageByName(L"line_0",L"line_9");
		m_pJPEffect = new cJPEffect(2);
	}
	m_pExploreMPDI->Init();
	//for spinobject's animnation
	//if(g_pSlotGamePhase->m_pBetBehavior->m_iWinMoney>2000)
	//{
	//
	//}
	sSlotBetTableResult*l_pTableResult = g_pSlotGamePhase->m_pBetBehaviorResult->m_TableResultList[this->m_iExplosionCount];
	m_pWinMoneySpinObjectMPDI->Init(l_pTableResult);
	//GenerateExploreMPDI();
	m_pWinmoneyLines->Init(l_pTableResult);
	m_FallDownNewSpinObject.Init();

	int	l_iSize = g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x*g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y;
	for( int i=0;i<l_iSize;++i )
	{
		GetLoopMovingObjectByIndex(i)->bRender = true;
	}
	m_iAddWinMoney = l_pTableResult->iWinMoney;
	m_iCurrentWinMoney = g_pSlotGamePhase->m_pBetBehavior->m_iWinMoney;
	size_t	l_uiSize = l_pTableResult->WinMoneyLineList.size();
	//if 1 skip show lines by step
	if(m_pWinmoneyLines->m_iWinMoneyLines.size() != l_uiSize&& l_uiSize !=1)
	{
		int a=0;
	}
	m_iCurrentLineWinMoneyList.clear();
	for( size_t i = 0;i<l_uiSize;++i )
	{
		m_iCurrentLineWinMoneyList.push_back(l_pTableResult->WinMoneyLineList[i].iWinMoney);
	}
	g_iExplosionCount = m_iExplosionCount;
	if( this->m_iExplosionCount >= m_iMaxAllowExplosionCount )
	{
		m_bWinJP = true;
		m_iStartJPWinMoney = g_pSlotGamePhase->m_pBetBehaviorResult->GetCurrentTablesWinMoney();
		m_iFinalJPWinMoney = *cWorldTripSlotApp::m_spPlayerData->GetJP(eJPT_LOCAL)+m_iStartJPWinMoney;
		m_pJPEffect->Init(*cWorldTripSlotApp::m_spPlayerData->GetJP(eJPT_LOCAL));
	}
	m_pWinmoneyLines->SetDone();
}

void	cFreeExplosionGame::WinLocalJP()
{
	int*l_piJP = cWorldTripSlotApp::m_spPlayerData->GetJP(eJPT_LOCAL);
	g_pSlotGamePhase->m_pBetBehaviorResult->ExtraGameWinExtraMoney(*l_piJP);
	*l_piJP = 0;
	int*l_piCount = cWorldTripSlotApp::m_spPlayerData->GetJPOpenCount(eJPT_LOCAL);
	if( l_piCount )
		*l_piCount += 1;

	g_pSlotGamePhase->m_pBetBehavior->m_iWinMoney = m_iFinalJPWinMoney;
	WRITELOGFILE("Local	JP OPEN!!!!!");
}

void	cFreeExplosionGame::Init()
{
	if( !m_pWinMoneySpinObjectMPDI )
		m_pWinMoneySpinObjectMPDI = new cWinMoneySpinObjectMPDI;
	cBaseExtraGame::Init();
	m_iExplosionCount = 0;
	g_iExplosionCount = 0;
	this->m_iCurrentWinMoney = 0;
	m_iCurrentLineWinMoneyList.clear();
	m_iAddWinMoney = 0;
	cWorldTripSlotApp::m_spPlayerData->SetJP(eJPT_LOCAL,0);

	int	l_AddedValue = rand(0,m_iExtraBonusForReachedMaxAllowExplosionCountMax-m_iExtraBonusForReachedMaxAllowExplosionCountMin)+m_iExtraBonusForReachedMaxAllowExplosionCountMin;
	int*l_pJP = cWorldTripSlotApp::m_spPlayerData->GetJP(eJPT_LOCAL);
	*l_pJP = l_AddedValue;
	m_bWinJP = false;
	m_iJPMoney = *l_pJP;
}

void	cFreeExplosionGame::Destroy()
{
	if( m_pResourceMPDIList )
	{
		//cGameApp::m_spAnimationParser->RemoveObject(m_pResourceMPDIList);
	}
	int	l_iCount = g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x*g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y;
	for( int i=0;i<l_iCount;++i )
		GetLoopMovingObjectByIndex(i)->bRender = true;
#ifdef _DEBUG
//	OutputDebugString(L"leave cFreeExplosionGame\n");
#endif
}

extern bool	g_bRenderScreen;

void	cFreeExplosionGame::Update(float e_fElpaseTime)
{
	e_fElpaseTime *= m_fGameSpeed;
	if( cGameApp::m_sucKeyData['S'] )
	{
		//e_fElpaseTime *= 2.5f;
	}

	if( !m_pWinmoneyLines->IsDone() )
	{
		if( cGameApp::m_sucKeyData['S'] || !g_bRenderScreen )
		{
			m_pWinmoneyLines->SetDone();
		}
		if(m_pWinmoneyLines->UpdateWinMoneyLinesByStep(e_fElpaseTime))
		{
			g_pSlotGamePhase->m_pBetBehavior->AddWinMoney(m_iCurrentLineWinMoneyList[0]);
			m_iCurrentLineWinMoneyList.erase(m_iCurrentLineWinMoneyList.begin());
		}
		//m_pWinmoneyLines->Update(e_fElpaseTime);
		if( m_pWinmoneyLines->IsDone() )
		{
		}
	}
	else
	{
		if( m_pWinMoneySpinObjectMPDI->IsAnimationDone() )
		{
			if( m_bWinJP )
			{
				m_pJPEffect->Update(e_fElpaseTime);
				float	l_fProgress = m_pJPEffect->GetCurrentProgress();
				if( l_fProgress >1.f )
					l_fProgress = 1.f;
				g_pSlotGamePhase->m_pBetBehavior->m_iWinMoney = UT::LERP(m_iStartJPWinMoney,m_iFinalJPWinMoney,l_fProgress);
				int*l_pJP = cWorldTripSlotApp::m_spPlayerData->GetJP(eJPT_LOCAL);
				*l_pJP = (int)((1-l_fProgress)*m_iJPMoney);
				m_pJPEffect->SetNumerial(*l_pJP);
				if( m_pJPEffect->IsDone() )
				{
					WinLocalJP();
					this->m_bDone = true;
				}
				return;
			}
			//if( g_pSlotGamePhase->m_bFreeToPlay )
			//{//game designer ask do not explosion again to avoid
			//	this->m_bDone = true;
			//}
			//else
			{
				if( !m_pExploreMPDI->IsAnimationDone() )
				{
					m_pExploreMPDI->Update(e_fElpaseTime);
					if( m_pExploreMPDI->IsAnimationDone() )
					{
						++m_iExplosionCount;
						//m_pWinMoneySpinObjectMPDI->Init();
						int	l_iWinMoney = 0;
						for( int i=0;i<m_iExplosionCount;++i )
						{
							l_iWinMoney += g_pSlotGamePhase->m_pBetBehaviorResult->m_TableResultList[i]->iWinMoney;
						}
						if(this->m_bWinJP)
						{
							l_iWinMoney += m_iFinalJPWinMoney-m_iStartJPWinMoney;
						}
						bool	l_bShowBigWin = false;
						if( (int)g_pSlotGamePhase->m_pBetBehaviorResult->m_TableResultList.size() <= m_iMaxAllowExplosionCount)
						{
							if((int)g_pSlotGamePhase->m_pBetBehaviorResult->m_TableResultList.size() == this->m_iExplosionCount+1 )
								l_bShowBigWin = true;
						}
						if( (int)g_pSlotGamePhase->m_pBetBehaviorResult->m_TableResultList.size() >= m_iExplosionCount +1 )
						{

						}
						std::vector<std::wstring>	l_NewResult = GetNextExplosionResult();
						CheckResultAndSetNewSpinObject(l_NewResult);
					}
				}
				else
				{
					m_FallDownNewSpinObject.Update(e_fElpaseTime);
					if(m_FallDownNewSpinObject.IsDone())
					{
						if((int)g_pSlotGamePhase->m_pBetBehaviorResult->m_TableResultList.size() > this->m_iExplosionCount  )
						{//win money again
							LoadResource();
						}
						else
						{//check free game 3 and bonus game
								this->m_bDone = true;
						}
					}
				}
			}
		}
		else
		{
			m_pWinMoneySpinObjectMPDI->Update(e_fElpaseTime);
			float	l_fProgress = m_pWinMoneySpinObjectMPDI->GetCurrentProgress();
			if( m_pWinMoneySpinObjectMPDI->IsAnimationDone() )
			{//ask king to do something
				sSlotBetTableResult*l_pTableResult = g_pSlotGamePhase->m_pBetBehaviorResult->m_TableResultList[this->m_iExplosionCount];
				std::wstring	l_str = SPIN_OBJECT_EXPLOSION_SOUND_NAME;
				l_str += ValueToStringW(m_iExplosionCount+1);
				cGameApp::SoundPlay(l_str.c_str(),true);
				int	l_iSize = g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x*g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y;
				if( !m_bWinJP )
				{
					for( int i=0;i<l_iSize;++i )
					{
						if(l_pTableResult->pbWinMoneySpinObject[i])
							GetLoopMovingObjectByIndex(i)->bRender = false;
					}
				}
				int	l_iCurrentMoney = g_pSlotGamePhase->m_pBetBehavior->m_iWinMoney;
				g_pSlotGamePhase->m_pBetBehavior->m_iWinMoney = 0;
				for( int i=0;i<this->m_iExplosionCount+1;++i )
				{
					g_pSlotGamePhase->m_pBetBehavior->m_iWinMoney += g_pSlotGamePhase->m_pBetBehaviorResult->m_TableResultList[i]->iWinMoney;
				}
				if( l_iCurrentMoney>g_pSlotGamePhase->m_pBetBehavior->m_iWinMoney )
				{
					int a=0;
				}
			}
			else
			{
				//g_pSlotGamePhase->m_pBetBehavior->m_iWinMoney = (int)(m_iCurrentWinMoney+(l_fProgress*m_iAddWinMoney));
				//m_pWinMoneySpinObjectMPDI
			}
		}
	}
}

void	cFreeExplosionGame::Render()
{
	m_pConstantUI->Render();
	if(!m_pExploreMPDI->IsAnimationDone())
	{
		//if(m_pWinmoneyLines->IsDone())
		//if( !m_FallDownNewSpinObject.IsDone() )
			g_pSlotGamePhase->m_pSlotLineObjectManagerList->Render();
	}
	//for( int i=0;i<this->m_iExplosionCount;++i )
	//{
	//	if( i>9 )
	//		break;
	//	Vector3	l_vPos = m_ExplosionCountPosition->GetOriginalPointList()[9-i];
	//	m_pNumerialImage->Draw(i+1,(int)l_vPos.x,(int)l_vPos.y);
	//}
	if( !m_pWinmoneyLines->IsDone() )
	{
		sSlotBetTableResult*l_pTableResult = g_pSlotGamePhase->m_pBetBehaviorResult->m_TableResultList[this->m_iExplosionCount];
		m_pWinmoneyLines->Render(l_pTableResult->iWinMoney);
	}
	else
	{
		if( m_pWinMoneySpinObjectMPDI->IsAnimationDone() )
		{
			if( this->m_bWinJP  )
			{
				m_pJPEffect->Render();
			}
			else
			{
				if( !m_pExploreMPDI->IsAnimationDone() )
				{
					cPuzzleImageUnit*l_pPIUnit = g_pSlotGamePhase->m_pSlotLineObjectManagerList->GetObject(0)->GetAllSpinObjectPI()->GetObject(0);
					int	l_iTotalSpinObject = g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x*g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y;
					Vector2	l_vSize((float)*l_pPIUnit->GetWidth(),(float)*l_pPIUnit->GetHeight());
					sSlotBetTableResult*l_pTableResult = g_pSlotGamePhase->m_pBetBehaviorResult->m_TableResultList[this->m_iExplosionCount];
					for( int i=0;i<l_iTotalSpinObject;++i )
					{
						if(l_pTableResult->pbWinMoneySpinObject[i])
						{
							sLoopMovingObject*l_pLoopMovingObject = GetLoopMovingObjectByIndex(i);
							Vector3	l_vPos = l_pLoopMovingObject->vCurrentPos;
							//l_vPos.x += l_vSize.x/2.f;
							//l_vPos.y += l_vSize.y/2.f;
							m_pExploreMPDI->SetPos(l_vPos);
							m_pExploreMPDI->Render();
						}
					}
				}
				else
				{
					g_pSlotGamePhase->m_pSlotLineObjectManagerList->DoScissor();
					m_FallDownNewSpinObject.Render();
					g_pSlotGamePhase->m_pSlotLineObjectManagerList->UnDoScissor();
					if(m_FallDownNewSpinObject.IsDone())
					{
					}
				}
			}
		}
		else
		{
			m_pWinMoneySpinObjectMPDI->Render();
		}		
	}
}
