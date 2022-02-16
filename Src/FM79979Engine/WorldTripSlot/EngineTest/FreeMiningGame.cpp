#include "stdafx.h"
#include "FreeMiningGame.h"
#include "SlotGamePhase.h"
#include "SlotObject.h"


cTrackFalldownWithRock::cTrackFalldownWithRock(int e_iNumObject,Vector3 e_vEndPos,float	e_fSpeed,float	e_fObjectGap,cBaseImage*e_pWrongDirectionImage)
:cFallDownNewSpinObject(e_iNumObject,e_vEndPos,e_fSpeed,e_fObjectGap)
{
	m_pWrongDirectionImage = e_pWrongDirectionImage;
	for( int i = 0;i<eD_MAX;++i)
		//Direction[i] = eD_MAX;
		Direction[i] = (eDirection)i;
}

void	cTrackFalldownWithRock::Render()
{
	cFallDownNewSpinObject::Render();
	float	l_fHalfWidth = *this->m_pMovingObjects[0].pImage->GetWidth()/2.f;
	float	l_fHalfHeight = *this->m_pMovingObjects[0].pImage->GetHeight()/2.f;
	float	l_fPositionOffset = l_fHalfHeight;
	for(int i=0;i<this->m_iCount;++i)
	{
		Vector3	l_vPos = m_pMovingObjects[i].vPos;
		l_vPos.x += l_fHalfWidth;
		l_vPos.y += l_fHalfHeight;
		for( int j=0;j<4;++j )
		{
			Vector3	l_vFinalPos = l_vPos;
			if( Direction[j] != eD_MAX )
			{
				switch( Direction[j] )
				{
					case eD_LEFT:
						l_vFinalPos.x = l_vPos.x - l_fPositionOffset;
						break;
					case eD_UP:
						l_vFinalPos.y = l_vPos.y -l_fPositionOffset;
						break;
					case eD_RIGHT:
						l_vFinalPos.x = l_vPos.x +l_fPositionOffset;
						break;
					case eD_DOWN:
						l_vFinalPos.y = l_vPos.y +l_fPositionOffset;
						break;
				}
				m_pWrongDirectionImage->SetPos(l_vFinalPos);
				m_pWrongDirectionImage->Render();
			}
		}
	}
}



const wchar_t*         cFreeMiningGame_Mining::TypeID( L"cFreeMiningGame_Mining" );

cFreeMiningGame_Mining::cFreeMiningGame_Mining(const WCHAR*e_strConditionName,int e_iConditionCount,float e_fTrackFallDownSpeed,float e_fTrainToDestinationTime)
:cBaseExtraGame(e_strConditionName,e_iConditionCount)
{
	m_eFreeGameList = eFGL_MINING;
	m_fTrackFallDownSpeed = e_fTrackFallDownSpeed;
	m_fTrainToDestinationTime = e_fTrainToDestinationTime;
	e_fTrackFallDownSpeed;
	e_fTrainToDestinationTime;
	m_pEnteringMPDI = 0;
	m_pLeavingMPDI1 = 0;
	m_pLeavingMPDI2 = 0;
	m_pMPDIList = 0;
	m_iRestTimeToPlay = 0;
	m_pTrainMovingTrack = 0;
}

cFreeMiningGame_Mining::~cFreeMiningGame_Mining()
{
	SAFE_DELETE(m_pTrainMovingTrack);
}

//return true if the condition of bonus is satisfied.
bool	cFreeMiningGame_Mining::InternalConditionCheck()
{
	if( g_pSlotGamePhase->m_bFreeToPlay  )
	{
		if( cBaseExtraGame::InternalConditionCheck() )
			return true;
	}
	return false;
}

void	cFreeMiningGame_Mining::SetupResult()
{
	int	l_iCount = g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.x/2;
	std::vector<std::vector<eDirection> >	l_eDirectionTree;
	for( int i=0;i<l_iCount;++i )
	{
		for( int j=0;j<g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y;++j )
		{
			std::vector<eDirection>	l_DirectionList;
			for( int k = 0;k<eD_MAX;++k)
			{
				eDirection	l_eDirection = (eDirection)(rand()%(eD_MAX));
				if( i == 0 && l_eDirection == eD_UP )
					l_eDirection = eD_MAX;
				if( i == g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y-1 && l_eDirection == eD_DOWN )
					l_eDirection = eD_MAX;
				l_DirectionList.push_back(l_eDirection);
			}
			//l_DirectionList.push_back(eD_LEFT);
			//l_DirectionList.push_back(eD_UP);
			//l_DirectionList.push_back(eD_RIGHT);
			//l_DirectionList.push_back(eD_DOWN);
			l_eDirectionTree.push_back(l_DirectionList);
		}
	}
	WCHAR*	l_strTrackName[] = {L"rail_01",L"rail_02",L"rail_03"};
	cPuzzleImage*l_pPI = m_pMPDIList->GetPuzzleImage(L"trolley");
	cPuzzleImageUnit*l_pTrackImage = l_pPI->GetObject(l_strTrackName[0]);
	float	l_fImageHeight = (float)*l_pTrackImage->GetHeight();
	//float	l_fImageHeight = (float)*l_pPI->GetObject(0)->GetHeight();
	cPuzzleImageUnit*l_pPIUnitRock = l_pPI->GetObject(L"breakpoint");
	int	l_iIndex = 1;
	int	l_iDirectionIndex = 0;
	for( int i=0;i<l_iCount;++i )
	{
		Vector3	l_vEndPos = g_pSlotGamePhase->m_pSlotLineObjectManagerList->GetObject(l_iIndex)->GetLoopMovingObject(g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y-1)->vCurrentPos;
		cTrackFalldownWithRock*l_pTrackFalldownWithRock = new cTrackFalldownWithRock(g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y,l_vEndPos,m_fTrackFallDownSpeed,l_fImageHeight,l_pPIUnitRock);
		m_FallDownNewSpinObjectManager.AddObjectNeglectExist(l_pTrackFalldownWithRock);
		cSlotObjectManager*l_pSlotObjectManager = g_pSlotGamePhase->m_pSlotLineObjectManagerList->GetObject(l_iIndex);
		Vector3	l_vStartPos = l_pSlotObjectManager->GetLoopMovingObject(0)->vCurrentPos;
		l_vStartPos.y -= g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y*l_fImageHeight;
		for( int j=0;j<g_pSlotGamePhase->m_pSlotRuleData->m_TableCount.y;++j )
		{
			l_pTrackImage = l_pPI->GetObject(l_strTrackName[j]);
			std::vector<eDirection> l_DirectionList = l_eDirectionTree[l_iDirectionIndex];
			assert( l_DirectionList.size() == 4 );
			l_pTrackFalldownWithRock->SetupData(j,l_vStartPos,l_pTrackImage);
			l_pTrackFalldownWithRock->SetDirection( &l_DirectionList[0] );
			l_vStartPos.y += l_fImageHeight;
			++l_iDirectionIndex;
		}
		l_iIndex += 2;
	}
}
WCHAR*	FREE_GAME_MINING_MPDILIST_FILE_NAME				= L"WorldTripSlot/ImageData/ImageData_EN/freegame2.mpdi";
//FREE_GAME_MINING_MPDILIST_FILE_NAME				= L"WorldTripSlot/ImageData/freegame2.mpdi";
//extern WCHAR*	FREE_GAME_MINING_MPDILIST_FILE_NAME;

void	cFreeMiningGame_Mining::Init()
{
	m_iRestTimeToPlay = g_pSlotGamePhase->m_pSlotLineObjectManagerList->GetNameCount(this->m_strConditionName.c_str());
	if( !m_pMPDIList )
	{
		m_pMPDIList = cGameApp::GetMPDIListByFileName(FREE_GAME_MINING_MPDILIST_FILE_NAME);
		m_pEnteringMPDI = m_pMPDIList->GetObject(L"freegame_mov");
		m_pLeavingMPDI1 = m_pMPDIList->GetObject(L"black");
		m_pLeavingMPDI2 = m_pMPDIList->GetObject(L"light");
		m_pTrackIdelMPDI = m_pMPDIList->GetObject(L"rail_x3");
		m_pEnteringMPDI->SetAnimationLoop(false);
		m_pLeavingMPDI1->SetAnimationLoop(false);
		m_pLeavingMPDI2->SetAnimationLoop(false);
	}

	Vector3	l_vStartPos(0,0,0);
	Vector3	l_vEndPos(960,640,0);
	cCurveWithTime	l_CurveWithTime;
	l_CurveWithTime.AddPoint(l_vStartPos,0.f);
	l_CurveWithTime.AddPoint(l_vEndPos,3.f);
	m_pTrainMovingTrack = new cTrainMovingTrack(l_CurveWithTime,0);
	//======================================
	m_pTrackIdelMPDI->Init();
	m_pTrackIdelMPDI->Update(0.0001f);
	m_pEnteringMPDI->Init();
	m_pLeavingMPDI1->Init();
	m_pLeavingMPDI2->Init();
	SetupResult();
	m_FallDownNewSpinObjectManager.Init();
}

void	cFreeMiningGame_Mining::Destroy()
{
	if( m_pMPDIList )
	{
		cGameApp::m_spAnimationParser->RemoveObject(m_pMPDIList->GetName());
		m_pMPDIList = 0;
	}
	SAFE_DELETE(m_pTrainMovingTrack);
	m_FallDownNewSpinObjectManager.Destroy();
}

void	cFreeMiningGame_Mining::Update(float e_fElpaseTime)
{
	if( m_pEnteringMPDI->IsAnimationDone() )
	{
		if( m_FallDownNewSpinObjectManager.IsDone() )
		{
			if( m_pTrainMovingTrack->IsDone() )
			{
				if( m_pLeavingMPDI1->IsAnimationDone() )
				{
					if( m_pLeavingMPDI2->IsAnimationDone() )
					{
						this->m_bDone = true;
					}
					else
						m_pLeavingMPDI2->Update(e_fElpaseTime);
				}
				else
					m_pLeavingMPDI1->Update(e_fElpaseTime);
			}
			else
				m_pTrainMovingTrack->Update(e_fElpaseTime);
		}
		else
			m_FallDownNewSpinObjectManager.Update(e_fElpaseTime);
	}
	else
		m_pEnteringMPDI->Update(e_fElpaseTime);
}

void	cFreeMiningGame_Mining::Render()
{
	m_pTrackIdelMPDI->Render();
	if( m_pEnteringMPDI->IsAnimationDone() )
	{
		if( m_FallDownNewSpinObjectManager.IsDone() )
		{
			m_FallDownNewSpinObjectManager.Render();
			if( m_pTrainMovingTrack->IsDone() )
			{
				if( m_pLeavingMPDI1->IsAnimationDone() )
				{
					g_pSlotGamePhase->m_pSlotLineObjectManagerList->Render();
					m_pLeavingMPDI2->Render();
				}
				else
					m_pLeavingMPDI1->Render();
			}
			else
				m_pTrainMovingTrack->Render();
		}
		else
			m_FallDownNewSpinObjectManager.Render();
	}
	else
	{
		m_pEnteringMPDI->Render();
	}
}
